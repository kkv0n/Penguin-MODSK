#include <common.h>

void MainInit_OTMem(u_int* param_1)
{
	int size;
	struct GameTracker* gGT = param_1;
	int levelID = gGT->levelID;

	size = 0x20000;


	 MainDB_OTMem(&gGT->db[0].otMem, size);
	 MainDB_OTMem(&gGT->db[1].otMem, size);

	// 0x1000 per player, plus 0x18 for linking
	size = ((gGT->numPlyrCurrGame) << 0xC) | 0x18;
	gGT->otSwapchainDB[0] =  MEMPACK_AllocMem(size); // "ot1"
	gGT->otSwapchainDB[1] =  MEMPACK_AllocMem(size); // "ot2"
	
	return;
}