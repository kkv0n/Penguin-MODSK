//load driver models in race tracks or adv hubs
#include <common.h>

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
		data.characterIDs[1] = RIPPER_ROO;
		data.characterIDs[2] = PAPU_PAPU;
		data.characterIDs[3] = KOMODO_JOE;
		data.characterIDs[4] = PINSTRIPE;
		
		lastIndex = 5; // 5 characters, bosses + player

	}
	else if ((gameMode1 & TIME_TRIAL) != 0) 
	{
		lastIndex = 4; // 3 ghosts + player ???
	}
	
	
	if((gameMode1 & ADVENTURE_BOSS) != 0)
	{
		lastIndex = 2;
		data.characterIDs[1] = data.metaDataLEV[gGT->levelID].characterID_Champion;
	}
	
	//if single player arcade mode
	else if(((gameMode1 & (ADVENTURE_MODE | ARCADE_MODE)) != 0) && (gGT->numPlyrCurrGame == 1))
	{
		 LOAD_Robots1P(data.characterIDs[0]);
		 
		 lastIndex = 8; // 7 players
	}
	 
	//if multiplayer arcade mode
	else if(((gameMode1 & ARCADE_MODE) != 0) && (gGT->numPlyrCurrGame == 2))
		lastIndex = 6; // 5 players


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
