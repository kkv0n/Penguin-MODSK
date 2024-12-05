#include <common.h>

//draw clock icon instead of clockflash/blur
void iconclock()
{
	struct GameTracker* gGT;
	
	gGT = sdata->gGT;
	int iconID = 13; 


int posX = 183;
int posY = 80;


int scale = 0x3000; 


DECOMP_DecalHUD_DrawWeapon(

    gGT->ptrIcons[iconID],


    (int)posX, (int)posY,


    &gGT->backBuffer->primMem,


    gGT->pushBuffer_UI.ptrOT,


    TRANS_50_DECAL,


    (int)scale,


    1
	);
}