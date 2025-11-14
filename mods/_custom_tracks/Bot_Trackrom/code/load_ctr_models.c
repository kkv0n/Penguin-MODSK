#include <common.h>
#include "header/bot.h"

void LOAD_Custom_LOD_Driver(struct BigHeader* bigfile, unsigned char levelLOD, void* callback)
{
	unsigned char i;
	int gameMode1;
    short MODEL_QUALITY;
	short MPK_QUALITY;
	
    struct GameTracker* gGT = sdata->gGT;
	
	unsigned char drivers = gGT->numPlyrCurrGame; 
	
	unsigned char lastIndex = drivers + numCustomBots;
	
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

			// CTR model
			for (i = 0; i < lastIndex; i++)
			{
			 LOAD_AppendQueue(bigfile, 2,
				MODEL_QUALITY + data.characterIDs[i],
				&data.driverModelExtras[i],0xfffffffe);
			}
                
		   
       
			
     //mpk
	 LOAD_AppendQueue(
		bigfile, 2,
		MPK_QUALITY + data.characterIDs[i - 1],
		NULL, callback);
	
}