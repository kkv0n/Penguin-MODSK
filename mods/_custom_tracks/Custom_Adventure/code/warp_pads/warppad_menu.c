#include <common.h>

void AH_WarpPad_MenuProc(struct RectMenu* menu)
{
	struct GameTracker* gGT = sdata->gGT;
	
	menu->rowSelected = 0;
	sdata->ptrActiveMenu = 0;
	sdata->ptrDesiredMenu = 0;
	
	
	
	// optimizes to JMP
	 RECTMENU_Hide(menu);
	return;
}