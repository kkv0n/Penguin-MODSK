//load driver models in race tracks or adv hubs
#include <common.h>
#include "../header/cove_rom.h"

void LOAD_Custom_LOD_Driver(struct BigHeader* bigfile, unsigned char levelLOD, void* callback)
{
	unsigned char i;
	int gameMode1;
    short MODEL_QUALITY;
	short MPK_QUALITY;
	unsigned char charToLoad = 0;
	unsigned int* modelPointers;
	
    struct GameTracker* gGT = sdata->gGT;
	
	
	unsigned char lastIndex = 4; // 3 ghost, 1 player
	
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

			
			if (gGT->levelID == MAIN_MENU_LEVEL)
			{
				lastIndex = 16;		
				MPK_QUALITY = BI_ADVENTUREPACK;
			}
			
			
            //loop throught all drivers
            for(i = 0; i < lastIndex; i++)
		   {
			   if (gGT->levelID != MAIN_MENU_LEVEL)
			   {
				   charToLoad = data.characterIDs[i];
				   modelPointers = &data.driverModelExtras[i]; //normal driver model variable(racing)
			   }
			   else
			   {
				   charToLoad = i;
				   modelPointers = &ptrCharacterModels[i]; //save ctr models ptr on our custom variables
			   }
			   
				// CTR model
				LOAD_AppendQueue(bigfile, 2,
				MODEL_QUALITY + charToLoad,
				modelPointers, 0xfffffffe);
		   }
		   

		if (gGT->levelID == MAIN_MENU_LEVEL)
			charToLoad = data.characterIDs[0];
			
		//mpk
		LOAD_AppendQueue(
		bigfile, 2,
		MPK_QUALITY + charToLoad,
		NULL, callback);
		
		return;
	
}
