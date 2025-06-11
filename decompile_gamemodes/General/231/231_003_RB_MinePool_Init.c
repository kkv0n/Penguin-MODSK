#include <common.h>
#include "../../../mods/game_modes/Gamemode_Selection/src/utils.h"

void DECOMP_RB_MinePool_Init(void)
{
	int i;
	int numMines;
	unsigned int addr;
	int gameMode;

	DECOMP_LIST_Clear(&D231.minePoolTaken);
	DECOMP_LIST_Clear(&D231.minePoolFree); 
	
	gameMode = sdata->gGT->gameMode1;
	
	// default
	if(USE_ITEM_CHAOS){
		numMines = 20; 
	}else{
		numMines = 15; // retail default: 10
	}
	
	if ((gameMode & CRYSTAL_CHALLENGE) != 0)
	{
		// naughty dog bug, should be 50,
		// this caused nitro court challenge bug
		numMines = 40;
	}
	
	// boss race
	if ((gameMode & ADVENTURE_BOSS) != 0)
	{
		if(USE_BOSS_CHALLENGE) numMines = 12;
		if (sdata->gGT->levelID == DRAGON_MINES) numMines = 3;
		if (sdata->gGT->levelID == ROO_TUBES) numMines = 7;
	}
		
	// add all mines
	for(i = 0; i < numMines; i++)
	{
		DECOMP_LIST_AddFront(&D231.minePoolFree, (struct Item*)&D231.minePoolItem[i]);
	}
}