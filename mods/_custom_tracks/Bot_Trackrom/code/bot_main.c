#include <common.h>
#include "header/bot.h"

bool ghostMode = true;

unsigned char numCustomBots = 1;

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




void CustomTrack_Init()
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

}


struct MenuRow CMAIN_MENU[] =
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
		// all of them uses time trial lng string
		// but the stringIndex changes later
		.stringIndex = 0x4D,
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




void RB_Fruit_GetScreenCoords(struct PushBuffer* pb, struct Instance* inst, short* output)
{
	MATRIX* m;
	short posWorld[4];
	
	// load camera matrix
	m = &pb->matrix_ViewProj;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
	
	// load input vector, each int casts to short
	posWorld[0] = (short)inst->matrix.t[0];
	posWorld[1] = (short)inst->matrix.t[1];
	posWorld[2] = (short)inst->matrix.t[2];
	posWorld[3] = 0;
	gte_ldv0(&posWorld[0]);

	// perspective projection
	gte_rtps();
	
	// get result
	gte_stsxy(&output[0]);
}


void weapon_roulette(unsigned char driverID)
{

	struct GameTracker* gGT = sdata->gGT;
	
	// if driver already has a weapon, quit
	if (
		(gGT->drivers[driverID]->heldItemID != 0xf) &&
		(gGT->drivers[driverID]->noItemTimer == 0)
		)
	{
		return;
	}

	// held item count
	if (gGT->drivers[driverID]->numHeldItems != 0)
	{
		return;
	}

	// if driver is firing weapon, quit
	if ((gGT->drivers[driverID]->actionsFlagSet & 0x8000) != 0)
	{
		return;
	}

	// if driver has raincloud and weapon is shuffling, quit
	if (gGT->drivers[driverID]->thCloud != 0)
	{
		if (
			(
				(struct RainCloud*)gGT->drivers[driverID]->thCloud->object
				)->boolScrollItem == 1
			)
		{
			return;
		}
	}


	// if driver is influenced by clock weapon, quit
	if (gGT->drivers[driverID]->clockReceive != 0)
	{
		return;
	}


	// set weapon to roulette
	gGT->drivers[driverID]->heldItemID = 0x10;


	// incrememt
	gGT->drivers[driverID]->numTimesHitWeaponBox++;

	// timer for weapon roulette
	gGT->drivers[driverID]->itemRollTimer = FPS_DOUBLE(90);

	// if no roulette
	if ((sdata->gGT->gameMode1 & ROLLING_ITEM) == 0)
	{
		// start loop
		OtherFX_Play(0x5d, 0);

		sdata->gGT->gameMode1 |= ROLLING_ITEM;
	}

	//gGT->drivers[driverID]->PickupTimeboxHUD.cooldown = FPS_DOUBLE(5);
	gGT->drivers[driverID]->noItemTimer = 0;

	if (gGT->drivers[driverID]->heldItemID == 0x10)
	{

		if (gGT->drivers[driverID]->itemRollTimer == 0)
		{
			VehPhysGeneral_SetHeldItem(gGT->drivers[driverID]);
		}
		//if Item roll is not done
		else gGT->drivers[driverID]->itemRollTimer--;
	}
}

void Item_Crate(struct GameTracker* gGT)
{
	struct Driver* player = gGT->drivers[0];
	
	if (gGT->level1 == NULL || player == NULL) return;
	
	static int timer = 0;
	

	
	if ((gGT->timer + FPS_DOUBLE(96)) < (timer + FPS_DOUBLE(96)) || player->heldItemID != 0xF)
		return;

	else
		timer = gGT->timer;

	
	unsigned int trackLength = gGT->level1->ptr_restart_points[0].distToFinish * 10;
	unsigned int progress = player->distanceToFinish_curr;
	
	for (unsigned char i = 1; i < 10; i++)
	{
		
		if (progress >= ((trackLength / 10) * i) && progress <= ((trackLength / 10) * i) + 125)
		{
			weapon_roulette((unsigned char)player->driverID);
			break;
		}
	}
	
		
}



void BotMod_Main()
{
	
	struct GameTracker* gGT = sdata->gGT;
	
		if (gGT->levelID < GEM_STONE_VALLEY)
		{
			if (gGT->cameraDC->unk8E != 0)
				gGT->cameraDC->unk8E = 0;
				
			if ((gGT->gameMode1 & PAUSE_ALL) == 0)
				ShowBotPathInfo(gGT->drivers[1]);
			
			
			if (sdata->Loading.stage != -1 && gGT->level1 != NULL)
			{

				//set custom spawn since not all custom tracks will have 2 spawns		
				for (unsigned char b = 0; b < (gGT->numPlyrCurrGame + numCustomBots); b++)
				{
					sdata->kartSpawnOrderArray[b] = b;
					
					if (b == 1) continue;
					
					memcpy(&gGT->level1->DriverSpawn[b], &gGT->level1->DriverSpawn[1], 0xC);
				}
		
				//move our player to the left
				gGT->level1->DriverSpawn[0].pos[2] =  gGT->level1->DriverSpawn[0].pos[2] - 200;

			}
			
			
			
		}



if (!ghostMode && RaceFlag_IsFullyOffScreen())
{
      // Item modifiers
	if (gGT->levelID < INTRO_RACE_TODAY)
	{
		  
	  if (gGT->trafficLightsTimer < 1)
		Item_Crate(gGT);

	}
}



	// changes main menu to new custom menu
	if (sdata->ptrActiveMenu == &D230.menuMainMenu)
	{
		sdata->ptrActiveMenu->rows = CMAIN_MENU; //custom menu




		CMAIN_MENU[0].stringIndex = 133;
		CMAIN_MENU[1].stringIndex = 134;

		sdata->lngStrings[133] = main_menu_options[0]; //rename the main menu with custom names
		sdata->lngStrings[134] = main_menu_options[1];

	}

}
