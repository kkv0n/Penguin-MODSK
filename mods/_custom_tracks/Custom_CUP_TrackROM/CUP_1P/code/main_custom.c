#include <common.h>
#include "../../tracks.h"

#ifdef USE_MODS
#include "bluefire.c"

#ifdef DEV_MODE
u_char hudptr;
#endif

#endif

struct GameTracker* GT;
struct Driver* driver[4]; //should delete the array later
struct QuadBlock* quadblock[4]; //should delete the array later
unsigned char task = 0;


bool firstperson = false;


struct MenuRow PAUSE_MENU[5] =
{
	[0] =
	{
		.stringIndex = 2, // resume
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},

	[1] =
	{
		.stringIndex = 1, // restart
		.rowOnPressUp = 0,
		.rowOnPressDown = 2,
		.rowOnPressLeft = 1,
		.rowOnPressRight = 1,
	},

	[2] =
	{
		.stringIndex = 3, // quit
		.rowOnPressUp = 1,
		.rowOnPressDown = 3,
		.rowOnPressLeft = 2,
		.rowOnPressRight = 2,
	},

	[3] =
	{
		.stringIndex = 0xE, // options
		.rowOnPressUp = 2,
		.rowOnPressDown = 3,
		.rowOnPressLeft = 3,
		.rowOnPressRight = 3,
	},

	[4] =
	{
		.stringIndex = 0xFFFF,
	}
};




void custom_init()
{
	
	// Extend bit range of quadblockID,
	// upper bits are never used, but still
	// need AND for alignment with LW instruction
	*(unsigned short*)0x800a0f18 = 0xFFFC;
	*(unsigned short*)0x800a1e80 = 0xFFFC;
	*(unsigned short*)0x800a36d8 = 0xFFFC;
	*(unsigned short*)0x800a4fd0 = 0xFFFC;
	*(unsigned short*)0x800a6f70 = 0xFFFC;
	*(unsigned short*)0x800a8b90 = 0xFFFC;
	
	// set arcade/TT menu to use adv cup rows
	data.menuArcadeRace.rows = &PAUSE_MENU; // pause menu
	data.menuBattle.rows = &PAUSE_MENU; // pause menu
	

	//data.menuQuit.funcPtr = pause_quit; //not needed //unused

	// required for AI Nav, cause I dont have
	// offsets [0xA] or [0xC] and it gets stuck
	// in a loop, so this breaks the loop
	*(int*)0x800150c0 = 0; //bots_thtick_drive ???

	// wont clear itself?
	sdata->ptrLoadSaveObj = 0;


	if (GT->levelID != track_id) return;


	sdata->ptrActiveMenu = 0;
}
#ifdef CUSTOM_CUP
struct MenuRow CMAIN_MENU[6] =
{
	[0] =
	{
		// all of them uses time trial lng string
		// but the stringIndex changes later
		.stringIndex = 0x4D,
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},
	[1] =
	{
		.stringIndex = 0x4D,
		.rowOnPressUp = 0,
		.rowOnPressDown = 2,
		.rowOnPressLeft = 1,
		.rowOnPressRight = 1,
	},

	[2] =
	{
		.stringIndex = 0x4D,
		.rowOnPressUp = 1,
		.rowOnPressDown = 3,
		.rowOnPressLeft = 2,
		.rowOnPressRight = 2,
	},
	[3]
	{
		.stringIndex = 0x4D,
		.rowOnPressUp = 2,
		.rowOnPressDown = 4,
		.rowOnPressLeft = 3,
		.rowOnPressRight = 3,
	},

	[4]
	{
		.stringIndex = 0x4D,
		.rowOnPressUp = 3,
		.rowOnPressDown = 4,
		.rowOnPressLeft = 4,
		.rowOnPressRight = 4,

	},

	[5] =
	{
		.stringIndex = 0xFFFF,
	}
};

#else
struct MenuRow CMAIN_MENU[2] =
{
	[0] =
	{
		// all of them uses time trial lng string
		// but the stringIndex changes later
		.stringIndex = 0x4D,
		.rowOnPressUp = 0,
		.rowOnPressDown = 0,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},

	[1] =
	{
		.stringIndex = 0xFFFF,
	}
};
#endif

struct MenuRow END_MENU[] =
{
	[0] =
	{
		.stringIndex = 4, // retry
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},

	[1] =
	{
		.stringIndex = 3, // quit
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 1,
		.rowOnPressRight = 1,
	},

	[2] =
	{
		.stringIndex = 0xFFFF,
	}
};

void timer()
{
	static unsigned fxms[16];


	for (int i = 0; i < 16; i++)
	{
		if (delaytimer[i] > 0) 	//count down
		{
			fxms[i] += FPS_DOUBLE(30);
			if (fxms[i] >= SECONDS(1))
			{
				fxms[i] -= SECONDS(1);
				delaytimer[i]--;
			}
		}
		else
		{
			fxms[i] = 0;
		}
	}
}


void check_effect(unsigned char run_this)
{

	int i;
	unsigned char desired_hazard;
	unsigned char desired_elevator;
	unsigned char desired_timer;
	unsigned char desired_item;


	//not working
	//int RB_CrateWeapon_LInC();
	//need parameters that i dont know
	//RB_CrateWeapon_LInC();

	if ((GT->gameMode1 & END_OF_RACE) != 0) return;

	for (i = 0; i < MAX_HAZARDS; i++) {
		if (quadblock[desired_driver]->terrain_type == d_terrain[i] ||
			(quadblock[desired_driver]->quadFlags & q_flag[i]) != 0) {
			desired_hazard = i; //checks which hazard should be called
			desired_elevator = i; //checks which elevator timer should be used
			desired_item = i;
			break;
		}
	}



	switch (run_this)
	{
	case ITEMS:
	{
		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (delaytimer[desired_timer] == 0)
		{
			weapon_roulette();
			OtherFX_Play(fx_crate_smash, 0);
			delaytimer[desired_timer] = 3;
		}

		break;
	}

	case HAZARD_:
	{

		call_hazards(desired_hazard);

		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (delaytimer[desired_timer] == 0 && hazard_fx[desired_hazard] != NO_FX_SOUND)
		{
			OtherFX_Play(hazard_fx[desired_hazard], 0);
			delaytimer[desired_timer] = 2;

		}


		break;
	}

	case WUMPA_JUICED:
	{

		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (driver[desired_driver]->numWumpas != 10) driver[desired_driver]->numWumpas = 10;

		if (delaytimer[desired_timer] == 0)
		{
			OtherFX_Play(fx_juiced_up, 0);
			delaytimer[desired_timer] = 3;
		}


		break;
	}

	case LAP_COUNT:
	{
		desired_timer = (desired_driver == 0) ? 1 : desired_driver * 4 + 1;

		if (delaytimer[desired_timer] == 0) //lap increment only happens 1 time every 60 seconds
		{

			OtherFX_Play(fx_lap_cross, 0);
			if (GT->numLaps > driver[desired_driver]->lapIndex) driver[desired_driver]->lapIndex += 1; // just in case
			delaytimer[desired_timer] = LAP_COUNT_TIMER; //timer

			if (GT->numPlyrCurrGame > 1)
			{
				if (driver[desired_driver]->lapIndex == GT->numLaps) driver[desired_driver]->actionsFlagSet |= 0x2000000;
				driver[desired_driver]->heldItemID = 0xf;
				return;
			}

			if (driver[desired_driver]->lapIndex == GT->numLaps)
			{
				driver[desired_driver]->heldItemID = 0xf;
				GT->gameMode1 |= END_OF_RACE;
			}



		}

		break;
	}
	case ELEVATOR:
	{
		desired_timer = (desired_driver == 0) ? 2 : desired_driver * 4 + 2;
		u_char desired_timer2 = (desired_driver == 0) ? 3 : desired_driver * 4 + 3;

		if (delaytimer[desired_timer] == 0 && delaytimer[desired_timer2] == 0) delaytimer[desired_timer] = ELEVATOR_TIMER[desired_elevator]; //sets the timer for the elevator

		if (delaytimer[desired_timer] > 0) //only call this if elevator timer is not 0
		{

			sdata->gGamepads->gamepad[desired_driver].buttonsHeldCurrFrame |= BTN_CROSS; // keep the kart driving

			//fly
			driver[desired_driver]->forcedJump_trampoline = 2;
			driver[desired_driver]->jump_unknown = 0x180;
			driver[desired_driver]->jump_InitialVelY = driver[i]->const_JumpForce * 3;

			if (delaytimer[desired_timer2] == 0) delaytimer[desired_timer2] = ELEVATOR_TIMER[desired_elevator] + 8; //set a cooldown

		}

		break;
	}
	case SINGLE_ITEM:
	{
		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (delaytimer[desired_timer] == 0)
		{
			OtherFX_Play(fx_crate_smash, 0);
			delaytimer[desired_timer] = 3;
		}
        
		driver[desired_driver]->heldItemID = single_item[desired_item];
		break;
	}
	}




}




//this runs in a loop since the game is running
void custom_main()
{
	int i;
	int j;
	GT = sdata->gGT;

	timer();


#ifdef DEV_MODE
	static unsigned char hud = 1;
#ifdef USE_MODS
	hudptr = hud;
#endif
#endif

#ifdef USE_MODS
	void Update_Retro();
	Update_Retro();
#endif



	for (i = 0; i < GT->numPlyrCurrGame; i++)
	{
		driver[i] = GT->drivers[i];
		quadblock[i] = driver[i]->underDriver;

#ifdef DEV_MODE
		struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[i];



		if ((GT->levelID == track_id) & ((GT->gameMode1 & END_OF_RACE) == 0))
		{
			if (((pad->buttonsHeldCurrFrame & BTN_L1) != 0) && ((pad->buttonsHeldCurrFrame & BTN_TRIANGLE) != 0)) //fly
			{
				pad->buttonsHeldCurrFrame |= BTN_CROSS;
				driver[i]->forcedJump_trampoline = 2;
				driver[i]->jump_unknown = 0x180;
				driver[i]->jump_InitialVelY = driver[i]->const_JumpForce * 3;
			}

			if (pad->buttonsHeldCurrFrame & BTN_UP) //trigger turbo to the player
			{
				int fire = (pad->buttonsHeldCurrFrame & BTN_CROSS) ? 0x100 : 0x800;
				VehFire_Increment(driver[i], 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), fire);
			}

			if (((pad->buttonsTapped & BTN_SELECT) != 0) && (GT->numPlyrCurrGame == 1)) //hide or unhide hud
			{

				if (hud > 0)
				{
					GT->hudFlags = 0;
				}
				else
				{
					GT->hudFlags |= 43; //enable hud
				}

				firstperson = (hud == 2) ? true : false;

				data.NearCam4x3.distMin = (!firstperson) ? 0xB3 : 10;
				data.NearCam4x3.distMax = (!firstperson) ? 0xE0 : 10;
				data.NearCam4x3.angle[2] = (!firstperson) ? 0x70 : 0x50;

				hud = (hud + 1) % 3;


			}
		}
#endif


		if (driver[i]->underDriver) //idk i think this is a flag/boolean (?) in any case ill leave it here
		{

			for (j = 0; j < MAX_EFFECTS; j++)
			{
				task = (quadblock[i]->terrain_type == d_terrain[j]) //checks if you are driving in a quad & decides what effect would be applied
					? terrain_effect[j]
					: ((quadblock[i]->quadFlags & q_flag[j]) != 0)
					? quad_effect[j]
					: NO_EFFECT;







				if (task != NO_EFFECT)
				{
					desired_driver = i; //the driver affected

					//these lines can be improved
					driver[desired_driver] = GT->drivers[desired_driver];
					quadblock[desired_driver] = driver[desired_driver]->underDriver;

					check_effect(task); //call this if the player is driving in the specified terrain
				}
			}
		}
	}










	// changes main menu to new custom menu
	if (sdata->ptrActiveMenu == &D230.menuMainMenu)
	{
		sdata->ptrActiveMenu->rows = CMAIN_MENU; //custom menu

		u_char menu_options;

#ifdef CUSTOM_CUP
		menu_options = 5;
#else
		menu_options = 1;
#endif

		for (i = 0; i < menu_options; i++)
		{
			CMAIN_MENU[i].stringIndex = 133 + i;
#ifdef CUSTOM_CUP
			sdata->lngStrings[133 + i] = names[i]; //rename the main menu with custom track names
#else
			sdata->lngStrings[133 + i] = names;  //rename the main menu with custom track names
#endif
		}

#ifdef CUSTOM_CUP
		selected_name = names[selected_track]; //mostly to show the name in race intro
#else
		selected_name = names;
#endif

		for (i = 0; i < 18; i++)
		{
			sdata->lngStrings[110 + i] = selected_name; //rename all track names in the lng to use current custom track name

			if (i < 7) sdata->lngStrings[98 + i] = selected_name; //idr if this is battle tracks lol

			if (i < 5) sdata->lngStrings[76 + i] = cup_tittle; //renames the "TIME TRIAL/ARCADE" text in the race intro

		}
	}


	// end of race menu
	if ((sdata->ptrActiveMenu == 0x800a0458) || (sdata->ptrActiveMenu == 0x800A04A4))
	{
		sdata->ptrActiveMenu->rows = &END_MENU[0];
	}


	if (GT->levelID != track_id) return;
}