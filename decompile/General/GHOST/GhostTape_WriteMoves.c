#include <common.h>
#include "../../General/AltMods/OnlineCTR/global.h"
#include "../../General/AltMods/OnlineCTR/spectator_icons.c""

//saffi available in room 15
void saffi_fire(struct Driver * driver, int reserves) {
	int oldOTT;
	
	oldOTT = driver->turbo_outsideTimer;
	
	if (oldOTT < reserves)
		{
			if (octr->serverRoom == 15) {
				driver->reserves += 			(reserves - oldOTT);
			}
			else
			{
				FixReservesIncrement(driver, reserves - oldOTT);
			}
			driver->turbo_outsideTimer += 	(reserves - oldOTT);
}
}