#include <common.h>
#include "quad.h"
#include "../adventure_settings.c"




unsigned char hazard_id[4];
unsigned short hazard_fx[4];
unsigned char terrain_effect[4];
unsigned char quad_effect[4];
unsigned char d_terrain[4];
uint16_t q_flag[4];
unsigned char ELEVATOR_TIMER[4];
unsigned char LAP_COUNT_TIMER;
unsigned char desired_driver;
unsigned char delaytimer[16];
unsigned char single_item[4];
unsigned char prev_levID;

void CTR_Box_DrawWireBox(RECT* r, int* unk, u_long* ot, struct PrimMem* primMem);

struct GameTracker* GT;
struct Driver* driver[4]; //should delete the array later
struct QuadBlock* quadblock[4]; //should delete the array later
unsigned char task = 0;




void DrawReservesMeter(short posX, short posY, struct Driver* d)
{
	
	u_int colorAndCode;
	short reservesMeter;
	struct DB* backDB;
	u_long* primmemCurr;
	POLY_F4* p;
	short bottomY;
	RECT box2;
	u_int auStack48[2];
	int reservesMeterWidth;

	reservesMeter = posY - 0x2d;

	backDB = sdata->gGT->backBuffer;
	primmemCurr = backDB->primMem.curr;
	p = 0;

	// if there is room left for more
	if (primmemCurr <= (u_long*)backDB->primMem.endMin100)
	{
		// add primitives
		backDB->primMem.curr = &primmemCurr[6];
		p = (POLY_F4*)primmemCurr;
	}

	if (p != 0)
	{
		box2.y = posY - 3;
		box2.w = 0x31;
		box2.h = 3;
		box2.x = posX - 0x31;
		memset(auStack48, 0, 4);

		CTR_Box_DrawWireBox(&box2, auStack48, sdata->gGT->pushBuffer_UI.ptrOT, &sdata->gGT->backBuffer->primMem);

		colorAndCode = 0x280000ff; // red

		//saffi fix by anfrost
		if (d->reserves >= 32766 || d->reserves < 0)
		{
			d->reserves = 32766;
		}

		if (d->reserves > 3600)
		{
			if (d->reserves < 6840)
			{
				colorAndCode = 0x2800ffff; // yellow
			}
			else
			{
				colorAndCode = 0x2800ff00; // green
			}
		}
		*(u_int*)&p->r0 = colorAndCode;
		reservesMeterWidth = d->reserves * 0x4; //Meter maximum is 8000 reserves units
		bottomY = posY - 3;
		p->y0 = posY;
		p->y1 = posY;
		p->x1 = posX;
		p->y2 = bottomY;
		p->x3 = posX;
		p->y3 = bottomY;

		reservesMeter = posX - ((short)(reservesMeterWidth / 0x960));
		if (reservesMeter < (posX - 0x31))
		{
			reservesMeter = posX - 0x31;
			*(u_int*)&p->r0 = 0x28ff0000; // blue
		}
		p->x0 = reservesMeter;
		p->x2 = reservesMeter;

		primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

		*(int*)p = *primmemCurr | 0x5000000;
		*primmemCurr = (u_int)p & 0xffffff;

		backDB = sdata->gGT->backBuffer;
		primmemCurr = backDB->primMem.curr;
		p = 0;

		// If there is room to add more
		if (primmemCurr <= (u_long*)backDB->primMem.endMin100)
		{
			// Add more primitives
			backDB->primMem.curr = &primmemCurr[6];
			p = (POLY_F4*)primmemCurr;
		}

		if (p != 0)
		{
			*(u_int*)&p->r0 = 0x28808080;
			p->y0 = posY;
			p->y1 = posY;
			p->x1 = posX;
			p->y2 = bottomY;
			p->x3 = posX;
			p->y3 = bottomY;
			p->x0 = posX - 0x31;
			p->x2 = posX - 0x31;

			// pointer to OT memory
			primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

			*(int*)p = *primmemCurr | 0x5000000;
			*primmemCurr = (u_int)p & 0xffffff;
		}
	}

	return;
}

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

void quad_main()
{
	int i;
	int j;
	GT = sdata->gGT;

	timer();
	
	if (GT->levelID >= GEM_STONE_VALLEY)
		return;

        
	if ((GT->numPlyrCurrGame == 1) && ((GT->gameMode1 & END_OF_RACE) == 0) && (sdata->RaceFlag_Position != 0))
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);

	for (i = 0; i < GT->numPlyrCurrGame; i++)
	{
		if (GT->drivers[i] != NULL)
		{
			driver[i] = GT->drivers[i];
			quadblock[i] = driver[i]->currBlockTouching;
		}
		else
		{
			continue;
		}


		if (driver[i]->currBlockTouching) //this just checks if is not null
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
					quadblock[desired_driver] = driver[desired_driver]->currBlockTouching;

					check_effect(task); //call this if the player is driving in the specified terrain
				}
			}
		}
	}





}



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
				trampoline_fx = (GT->levelID == MYSTERY_CAVES) ? fx_turtle_bounce : fx_bounce;

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


//load driver models in race tracks or adv hubs

void LOAD_Custom_LOD_Driver(struct BigHeader* bigfile, unsigned char levelLOD, void* callback)
{
	unsigned char i;
	int gameMode1;
    short MODEL_QUALITY;
	short MPK_QUALITY;
	
    struct GameTracker* gGT = sdata->gGT;
	
	
	unsigned char lastIndex = gGT->numPlyrCurrGame - 1; //number of human players
	
	gameMode1 = gGT->gameMode1;
	
	
	//Decides which model quality should be used
	switch(levelLOD)
	{
		default:
		case 1:
		{
            MODEL_QUALITY = BI_RACERMODELHI;
	        MPK_QUALITY = BI_TIMETRIALPACK;
			break;
		}
		case 2:
		{
			MODEL_QUALITY = BI_RACERMODELMED;
	        MPK_QUALITY = BI_2PARCADEPACK;
			break;
		}
		case 3:
		case 4:
		{
			MODEL_QUALITY = BI_RACERMODELLOW;
	        MPK_QUALITY = BI_4PARCADEPACK;
			break;
		}
		
	}




	if(
			// If you are in Adventure cup
			((gameMode1 & ADVENTURE_CUP) != 0) &&

			// purple gem cup
			(gGT->cup.cupID == 4)
		)
	{
		data.characterIDs[1] = 0xA;
		data.characterIDs[2] = 0x9;
		data.characterIDs[3] = 0xB;
		data.characterIDs[4] = 0x8;
		
		lastIndex = 4; // 5 characters, bosses + player

	}
	else if ((gameMode1 & TIME_TRIAL) != 0) 
	{
		lastIndex = 1; //2, ghost + player ???
	}
	
	//if single player arcade mode
	if(((gameMode1 & (ADVENTURE_MODE | ARCADE_MODE)) != 0) && (gGT->numPlyrCurrGame == 1))
	{
		 LOAD_Robots1P(data.characterIDs[0]);
		 
		 lastIndex = 7; // 8 players
	}
	 
	//if multiplayer arcade mode
	else if(((gameMode1 & ARCADE_MODE) != 0) && (gGT->numPlyrCurrGame == 2))
		lastIndex = 5; // 5 players


            //loop throught all players + bots
            for(i = 0; i < lastIndex; i++)
		   {
			// CTR model
			 LOAD_AppendQueue(bigfile, 2,
				MODEL_QUALITY + data.characterIDs[i],
				&data.driverModelExtras[i],0xfffffffe);
		   }
		   

		   	if(((gameMode1 & ARCADE_MODE) != 0) && (gGT->numPlyrCurrGame == 2))
		   {
			    //load bot IDs
			   	LOAD_Robots2P(bigfile, data.characterIDs[0], data.characterIDs[1], callback);
				return;
		   }
			
     //mpk
	 LOAD_AppendQueue(
		bigfile, 2,
		MPK_QUALITY + data.characterIDs[i],
		NULL, callback);
	
}