#include <common.h>
#include "header/cove_rom.h"

void MM_TrackSelect_MenuProc(struct RectMenu* menu)
{
	//move ghost reset/allocation there just like vanilla game does
	//allocate memory for ghosts
    sdata->ptrGhostTapePlaying = MEMPACK_AllocHighMem(0x3e00);
		
	//reset ghost data
    memset(sdata->ptrGhostTapePlaying, 0, sizeof(struct GhostHeader));
    sdata->boolReplayHumanGhost = 0;
	
	static bool firstLoad = false;
	
	//not sure if memory card affects this, so clean all this
	if (!firstLoad)
	{
		
		GAMEPROG_NewProfile_OutsideAdv(&sdata->gameProgress);
		
		sdata->gameProgress.highScoreTracks[CUSTOM_LEVEL_ID].timeTrialFlags = 0;
		
		GAMEPROG_GetPtrHighScoreTrack();
	}
		
	firstLoad = true;
	

		
	//dont show track menu, load the track directly
	sdata->gGT->currLEV = CUSTOM_LEVEL_ID;
    MainRaceTrack_RequestLoad(CUSTOM_LEVEL_ID);
	sdata->gGT->prevLEV = CUSTOM_LEVEL_ID;
	
	//close menu or the game crashes
	RECTMENU_Hide(menu);
	
	return;
}