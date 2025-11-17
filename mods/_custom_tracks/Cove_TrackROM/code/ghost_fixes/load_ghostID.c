#include <common.h>

char reload_Ghosts(struct GameTracker* gGT)
{
	//ghostIDs already loaded, reload the track and models
	 //if loaded from main menu the gamemode will be arcade
	MainRaceTrack_RequestLoad(CUSTOM_LEVEL_ID);
	gGT->gameMode1 &= ~(ARCADE_MODE);
	gGT->gameMode1 |= TIME_TRIAL;
	return -4;

}

void get_ghostIDs(struct SpawnType1* ptrSpawnType1)
{
	         //if not ghosts on this level
			if (ptrSpawnType1->count < ST1_NTROPY)
			{
				//for some reason a random tiny tiger ghost spawns
				sdata->ptrGhostTape[1] = NULL; 
				return;
			}
			
			//struct that stores data related to ghosts
			void** pointers = ST1_GETPOINTERS(ptrSpawnType1);
		
            //search ghost data inside level file
			struct GhostHeader* ntropy = (struct GhostHeader*)pointers[ST1_NTROPY];
			struct GhostHeader* oxide = (struct GhostHeader*)pointers[ST1_NOXIDE];
		
			//get ntropy
			if(ntropy != NULL)
				data.characterIDs[2] = ntropy->characterID;


		
			//get oxide
			if (oxide != NULL)
				data.characterIDs[3] = oxide->characterID;

		
			
	
}