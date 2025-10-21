#include <common.h>
#include "../header/cove_rom.h"

//avoid ghost crashes when the race ends
void BOTS_Driver_Convert(struct Driver* d)
{
	return;	
}

//store this there, free bytes
#include "load_ghostID.c"
#include "onboot.c"