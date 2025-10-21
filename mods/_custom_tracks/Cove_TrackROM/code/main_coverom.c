#include <common.h>
#include "header/cove_rom.h"
#include "header/cove_files.c"


unsigned char CUSTOM_LEVEL_ID;

struct MenuRow MainMenuOptions[2] =
{
	[0] =
	{
		.stringIndex = 379, // time trial
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




void Cove_Main()
{
	struct GameTracker* gGT = sdata->gGT;
	    	
	//high lod fix
	//DrawLevelOvr1P
	*(unsigned int*)0x800AB460 = 0x800A6F40;
	
	if (gGT->levelID < GEM_STONE_VALLEY && gGT->cameraDC->unk8E != 0)
		gGT->cameraDC->unk8E = 0;
	
	
	bluefire();
			

	//reserves meter
	if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0) && (RaceFlag_IsFullyOffScreen()))
	{
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5,
		(struct Driver*)sdata->gGT->threadBuckets[0].thread->object); // ??? this can easily be gGT->drivers, why looking on threads
	}


	
	// main menu options
	if (sdata->ptrActiveMenu == &D230.menuMainMenu)
	{
		sdata->ptrActiveMenu->rows = &MainMenuOptions[0];
		sdata->lngStrings[379] = track_name;
		
	}
	

}
