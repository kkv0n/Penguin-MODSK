#include <common.h>


void LOAD_DriverMPK(struct BigHeader* bigfile, int levelLOD, void* callback)
{
	int i;
	int gameMode1;
	
	unsigned int param_1 = bigfile;	
	void (*param_3)(struct LoadQueueSlot*) = callback;	
	
	struct GameTracker* gGT = sdata->gGT;
	gameMode1 = gGT->gameMode1;
	
	int lastFileIndexMPK;

	// 3P/4P
	if(levelLOD - 3U < 2)
	{
		for(i = 0; i < 3; i++)
		{
			// low lod CTR model
			 LOAD_AppendQueue(param_1,2,
				BI_RACERMODELLOW + data.characterIDs[i],
				&data.driverModelExtras[i],(void(*)(struct LoadQueueSlot*))0xfffffffe);
		}

		// load 4P MPK of fourth player
		lastFileIndexMPK = BI_4PARCADEPACK + data.characterIDs[i];
	}

// This fails ONLY on DuckStation for any MPK
// other than BI_ADVENTUREPACK + data.characterIDs[0],
// VehBirth_GetModelByString will return nullptr and
// nullptr-dereference "somewhere" will explode, but 
// PC port works fine, so how is model used?


	else if(
		// adv mpk when we just need text from MPK
		((gameMode1 & (GAME_CUTSCENE | ADVENTURE_ARENA | MAIN_MENU)) != 0)
		||

		// credits
		((gGT->gameMode2 & CREDITS) != 0)
	  )
	{
		/*
		// need oxide model for character select
		if(gGT->levelID == MAIN_MENU_LEVEL)
		{
			// high lod model (temporary workaround)
			 LOAD_AppendQueue(param_1,2,
				BI_RACERMODELHI + 0xF,
				&data.driverModelExtras[0],(void(*)(struct LoadQueueSlot*))0xfffffffe);
		}
		
		// get rid of oxide cause MPK is too big
		if(gGT->levelID == ADVENTURE_CHARACTER_SELECT)
		{
			data.characterIDs[0] = 0;
		}
		*/
		
		lastFileIndexMPK = BI_ADVENTUREPACK + data.characterIDs[0];
	}


	else if((gameMode1 & (ADVENTURE_BOSS | RELIC_RACE | TIME_TRIAL)) != 0)
	{
		// high lod model
		 LOAD_AppendQueue(param_1,2,
			BI_RACERMODELHI + data.characterIDs[0],
			&data.driverModelExtras[0],(void(*)(struct LoadQueueSlot*))0xfffffffe);

		// time trial mpk
		lastFileIndexMPK = BI_TIMETRIALPACK + data.characterIDs[1];
	}

	else if(
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

		// high lod model
		 LOAD_AppendQueue(param_1,2,
			BI_RACERMODELHI + data.characterIDs[0],
			&data.driverModelExtras[0],(void(*)(struct LoadQueueSlot*))0xfffffffe);

		// pack of four AIs with bosses
		lastFileIndexMPK = BI_2PARCADEPACK + 7;
	}

	// any 1P mode,
	// not adv, not time trial, not gem cup, not credits
	else if(levelLOD == 1)
	{
ForceOnlineLoad8:
		 LOAD_Robots1P(data.characterIDs[0]);

		// arcade mpk
		lastFileIndexMPK = BI_1PARCADEPACK + data.characterIDs[0];
		
		

		int i;
		for(i = 0; i < 7; i++)
		{
			// high lod CTR model
			 LOAD_AppendQueue(param_1,2,
				BI_RACERMODELHI + data.characterIDs[i],
				&data.driverModelExtras[i],(void(*)(struct LoadQueueSlot*))0xfffffffe);
		}
		
		// time trial mpk
		lastFileIndexMPK = BI_TIMETRIALPACK + data.characterIDs[7];
	}

	//else if(levelLOD == 2)
	else
	{
		// med models
		for(i = 0; i < 2; i++)
		{
			// med lod CTR model
			 LOAD_AppendQueue(param_1,2,
				BI_RACERMODELMED + data.characterIDs[i],
				&data.driverModelExtras[i],(void(*)(struct LoadQueueSlot*))0xfffffffe);
		}

		// this also loads MPK
		 LOAD_Robots2P(param_1, data.characterIDs[0], data.characterIDs[1], param_3);

		return;
	}

	 LOAD_AppendQueue(
		param_1,2,
		lastFileIndexMPK,
		NULL, param_3);

	return;
}