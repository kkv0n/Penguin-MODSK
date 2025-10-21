#include <common.h>
#include "header/cove_rom.h"

// force parameter is a personal optimization(niko)
int force;


void MainInit_PrimMem(u_int* param_1)
{
	struct GameTracker* gGT = param_1;

	int GetOriginalSize(struct GameTracker* gGT);
	int size = GetOriginalSize(gGT);
	
	int backup = 0;
	
	if(force != 0)
	{
		size = force/2;
		
		backup = (int)sdata->mempack[0].firstFreeByte;
		
		sdata->mempack[0].firstFreeByte =
		(void*)((int)sdata->mempack[0].lastFreeByte
		- 0xA000 // primMem needed
		- (0x4400)); // ghost HighMem
	}
	else if (force == 0 && gGT->levelID <= CITADEL_CITY)
	{
		//dont use hardcoded address for mempack thanks.
		
		
		//get free mempack bytes to store primMem
		//this variable is used 2 times to allocate mem
		size = (MEMPACK_GetFreeBytes() / 2);
		
		size &= ~31; //some trick to align number just in case
		
		//dont use the entire mempack space
		if (size > 0x104400)
			size -= 0x104400;
		else //hopefully this else never happens
			size -= 0x2400; //ghost required + some extra bytes
			
		  
	}

	 
	 //allocate +size on the mempack for prim mem
	 MainDB_PrimMem(&gGT->db[0].primMem, size);
	 MainDB_PrimMem(&gGT->db[1].primMem, size);
	 
	// Stage 0:
	// restore firstFreeByte for the rest
	// of the LOAD_TenStages loading system
	if(backup != 0)
		sdata->mempack[0].firstFreeByte = (void*)backup;

	 
	 return;
}

int GetOriginalSize(struct GameTracker* gGT)
{
	int levelID = gGT->levelID;
	
	// main menu
	if(levelID == MAIN_MENU_LEVEL)
		return 0x17c00;
	


	// all are 0x67 or 0x5F, adv hub was 0x5F too
	return data.primMem_SizePerLEV_1P[levelID] << 10;

}

