#include <common.h>
#include "../../General/AltMods/OnlineCTR/bluefire.c"
#include "../../General/AltMods/OnlineCTR/global.h"



#ifdef USE_ONLINE

extern void uibb_entryHook();

//if boss race
bool bossrace = 0;

//shootoncircle moved here 
void DECOMP_VehPickupItem_ShootOnCirclePress(struct Driver *d)
{
    u_char weapon;

    if (d->ChangeState_param2 != 0)
    {
		#ifndef REBUILD_PS1
        DECOMP_VehPickState_NewState(d, d->ChangeState_param2, d->ChangeState_param3, d->ChangeState_param4);
		#endif
	}

    // If you want to fire a weapon
    if ((d->actionsFlagSet & 0x8000) == 0)
        return;

    // Remove the request to fire a weapon, since we will fire it now
    d->actionsFlagSet &= ~(0x8000);

	weapon = d->heldItemID;
	
u_char flag = 0;

			// Missiles and Bombs share code,
	// Change Bomb1x, Bomb3x, Missile3x, to Missile1x	
	if(
		(weapon == 1) ||
		(weapon == 10) ||
		(weapon == 11)
	  )
	{
		weapon = 2;
	}
    
    
if (bossrace) {
	
        flag = 2;

    DECOMP_VehPickupItem_ShootNow(d, weapon, flag);
}
else {
		DECOMP_VehPickupItem_ShootNow(d, weapon, 0);
}
}

#ifdef USE_16BY9
void ui16by9_ViewProj(struct PushBuffer* pb)
{
	// Y axis is already scaled 512/216 x 0x360/0x600 -> 4x3
	// X axis correction for 16x9 must be 9/16 x 4/3 -> 0.75
	// 16x9 is 0.75, 20x9 is 0.6, etc

	// 600  / 1000 for 20x9
	// 750  / 1000 for 16x9
	// 1000 / 1000 for 4x3

	pb->matrix_ViewProj.t[0] =
	pb->matrix_ViewProj.t[0] * 750 / 1000;

	pb->matrix_ViewProj.m[0][0] =
	pb->matrix_ViewProj.m[0][0] * 750 / 1000;

	pb->matrix_ViewProj.m[0][1] =
	pb->matrix_ViewProj.m[0][1] * 750 / 1000;

	pb->matrix_ViewProj.m[0][2] =
	pb->matrix_ViewProj.m[0][2] * 750 / 1000;
}
#endif
#endif

//saffi available in room 15
void saffi_fire2(struct Driver * driver, int reserves) {
		if (octr->serverRoom == 15) {
		driver->reserves += reserves;
		}
		else
		{
		FixReservesIncrement(driver, reserves);
		}
}
