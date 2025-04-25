#include <common.h>
#include "../../tracks.h"
#include "../../tracks_settings.c"




unsigned char hazard_id[4];
unsigned short hazard_fx[4];
unsigned char track_id;
unsigned char d_laps;
unsigned int gamemode;
unsigned char terrain_effect[4];
unsigned char quad_effect[4];
unsigned char d_terrain[4];
uint16_t q_flag[4];
char* selected_name;
unsigned char ELEVATOR_TIMER[4];
bool cup_enabled;
unsigned char LAP_COUNT_TIMER;
unsigned char desired_driver;
unsigned char delaytimer[16];
unsigned char single_item[4];


//set crash cove 1p as default value just in case
unsigned char selected_track = 0;
unsigned char d_vram = 24;
unsigned char d_track = 25;

#ifdef USE_MODS
unsigned char selected_mod;
#endif


void weapon_roulette()
{


	// if driver already has a weapon, quit
	if (
		(driver[desired_driver]->heldItemID != 0xf) &&
		(driver[desired_driver]->noItemTimer == 0)
		)
	{
		return;
	}

	// held item count
	if (driver[desired_driver]->numHeldItems != 0)
	{
		return;
	}

	// if driver is firing weapon, quit
	if ((driver[desired_driver]->actionsFlagSet & 0x8000) != 0)
	{
		return;
	}

	// if driver has raincloud and weapon is shuffling, quit
	if (driver[desired_driver]->thCloud != 0)
	{
		if (
			(
				(struct RainCloud*)driver[desired_driver]->thCloud->object
				)->boolScrollItem == 1
			)
		{
			return;
		}
	}


	// if driver is influenced by clock weapon, quit
	if (driver[desired_driver]->clockReceive != 0)
	{
		return;
	}


	// set weapon to roulette
	driver[desired_driver]->heldItemID = 0x10;


	// incrememt
	driver[desired_driver]->numTimesHitWeaponBox++;

	// timer for weapon roulette
	driver[desired_driver]->itemRollTimer = FPS_DOUBLE(90);

	// if no roulette
	if ((sdata->gGT->gameMode1 & ROLLING_ITEM) == 0)
	{
		// start loop
		OtherFX_Play(0x5d, 0);

		sdata->gGT->gameMode1 |= ROLLING_ITEM;
	}

	//driver[desired_driver]->PickupTimeboxHUD.cooldown = FPS_DOUBLE(5);
	driver[desired_driver]->noItemTimer = 0;

	if (driver[desired_driver]->heldItemID == 0x10)
	{

		if (driver[desired_driver]->itemRollTimer == 0)
		{
			VehPhysGeneral_SetHeldItem(driver[desired_driver]);
		}
		//if Item roll is not done
		else driver[desired_driver]->itemRollTimer--;
	}
}


void call_hazards(unsigned char s_hazard)
{


	int RB_Hazard_HurtDriver();
	unsigned char selected_hazard[] = { hazard_id[s_hazard], 0, 0 };
	bool using_trampoline = false;
	short jumpForce;
	unsigned int trampoline_fx;
	bool spawn_clock = false;

	driver[desired_driver]->actionsFlagSet &= ~0x100000;


	if (hazard_id[s_hazard] == K_SPIN)
	{
		selected_hazard[2] = 2;
	}
	else
	{
		if (hazard_id[s_hazard] == CLOCK_EFFECT)
		{
			selected_hazard[0] = 1;
			spawn_clock = true;
		}
		else if (hazard_id[s_hazard] == TRAMPOLINE)
		{

			driver[desired_driver]->forcedJump_trampoline = 2;
			// if first frame (basically)
			if (
				// if not currently airborne from forced jump
				(driver[desired_driver]->jump_ForcedMS == 0) ||

				// if jump_InitialVelY was just now set to const_jump
				(driver[desired_driver]->jump_InitialVelY == driver[desired_driver]->const_JumpForce))
			{
				trampoline_fx = (GT->levelID == CAVES) ? fx_turtle_bounce : fx_bounce;

				OtherFX_Play(trampoline_fx, 0);

			}

			// currently forced airborne
			driver[desired_driver]->jump_ForcedMS = 0xa0;

			jumpForce = driver[desired_driver]->const_JumpForce * 3;

			// if big force jump (turtles)
			if (driver[desired_driver]->forcedJump_trampoline == 2)
			{
				driver[desired_driver]->jump_unknown = 0x180;
				driver[desired_driver]->jump_InitialVelY = jumpForce;
			}

			// if small force jump (turtles)
			else
			{
				driver[desired_driver]->jump_InitialVelY = (jumpForce >> 1);
			}

			// remove force jump (turtles)
			driver[desired_driver]->forcedJump_trampoline = 0;

			using_trampoline = true;


		}
		selected_hazard[2] = 0;
	}

	if (!using_trampoline && RB_Hazard_HurtDriver(driver[desired_driver], selected_hazard[0], selected_hazard[1], selected_hazard[2]) != 0)
	{
		if (spawn_clock) driver[desired_driver]->clockReceive = 0x1e00;
	}

}


#ifdef CUSTOM_CUP
void custom_cup()
{


	selected_track = ((GT->gameMode1 & END_OF_RACE) != 0) ? selected_track + 1 : 0;

	if (selected_track == 4) //go back to the main menu
	{
		sdata->mainMenuState = 0;

		MainRaceTrack_RequestLoad(0x27); // load main menu
		return;
	}



	track_options(); //refresh custom track selection

	GT->currLEV = track_id;


	GT->numLaps = d_laps % 128;
	GT->numLaps = (GT->numLaps == 0) ? 1 : GT->numLaps;
	GT->numPlyrNextGame = 1;
	GT->gameMode1 |= gamemode;

	MainRaceTrack_RequestLoad(track_id); //load the next custom track

}
#endif

#ifdef USE_MODS
void turbo_(struct Driver* d)
{
if (((d->stepFlagSet & 2) != 0) & (selected_mod == RETRO_FUELED))
{
	GT->gameMode2 |= CHEAT_TURBOPAD;
}
else
{
	GT->gameMode2 &= ~(CHEAT_TURBOPAD);
}
}
#endif

void GhostReplay_ThTick(struct Thread* t)
{
	return;

}