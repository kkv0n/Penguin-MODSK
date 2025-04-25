#include <common.h>

int force;

// force parameter is a personal optimization
void MainInit_PrimMem(u_int* param_1)
{
	struct GameTracker* gGT = param_1;

	int GetOriginalSize(struct GameTracker* gGT);
	int size = GetOriginalSize(gGT);

	// optimization,
	// use all remaining heap for primMem,
	// LOAD_TenStages:Stage8
	if(force == 0)
	{
		// gGT->levelID is set cause Stage8
		// is past all the level load+callback
		if (gGT->levelID <= CITADEL_CITY)
		{
			
			// store the memory here, past the level
			sdata->PtrMempack->firstFreeByte = 0x80400000;
			
				
			int newSize = ( MEMPACK_GetFreeBytes()/2);
			
			// adjust for alignment
			// that can inflate size later
			newSize &= 0xffffffE0;
		
				
			// if remaining size is less than OG requirement,
			// allocate way too much memory and crash the game
			if(newSize < size) 
			{
				printf("Missed by: %08x\n", (size-newSize)*2);
				 MEMPACK_AllocMem(0x900000);
			}
			
			printf("BonusPrim: %08x\n", newSize-size);
			size = newSize;
		}
	}
	
	// optimization,
	// steal OT mem during loading screen,
	// LOAD_TenStages:Stage0
	else
		size = force/2;

	
	 MainDB_PrimMem(&gGT->db[0].primMem, size);
	 MainDB_PrimMem(&gGT->db[1].primMem, size);
}

int GetOriginalSize(struct GameTracker* gGT)
{
	int levelID = gGT->levelID;
	
	// adv garage
	if(levelID == ADVENTURE_GARAGE)
		return 0x1b800;
	
	// main menu
	if(levelID == MAIN_MENU_LEVEL)
		return 0x17c00;
	
	if(gGT->numPlyrCurrGame == 1)
	{
		// any% end, 101% end, credits
		if(levelID >= OXIDE_ENDING)
			return 0x17c00;
		
		// intro cutscene
		if(levelID >= INTRO_RACE_TODAY)
			return 0x1e000;
		
		// adv hub
		if(levelID >= GEM_STONE_VALLEY)
			return 0x1c000;
		
		// ordinary tracks

		// all are 0x67 or 0x5F, adv hub was 0x5F too
		return data.primMem_SizePerLEV_1P[levelID] << 10;
	}
	
	if(gGT->numPlyrCurrGame == 2)
	{
		// assume only levID 0-24
		return data.primMem_SizePerLEV_2P[levelID] << 10;
	}
	
	// 3P 4P
	// assume only levID 0-24
	return data.primMem_SizePerLEV_4P[levelID] << 10;
}


