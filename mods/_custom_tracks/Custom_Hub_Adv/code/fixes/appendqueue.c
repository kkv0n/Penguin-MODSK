#include <common.h>

void LOAD_AppendQueue(int bigfile, int type, int fileIndex, void* destinationPtr, void* callback)
{
	
	struct LoadQueueSlot* lqs;
	

	lqs = &sdata->queueSlots[sdata->queueLength];
	lqs->ptrBigfileCdPos_UNUSED = bigfile;
	lqs->flags = 0;
	lqs->type_UNUSED = type;
	lqs->subfileIndex = fileIndex;
	lqs->ptrDestination = destinationPtr;
	lqs->size_UNUSED = 0;
	lqs->callbackFuncPtr = callback;
	
	sdata->queueLength++;
	

}