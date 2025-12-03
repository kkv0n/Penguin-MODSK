#include <common.h>
#include "../adventure.h"

void MainFreeze_SafeAdvDestroy(void)
{
	// check if Adv Hub is loaded
	if(current_track != CUSTOM_HUB) return;
	
	AH_Pause_Destroy();   
    return;
}