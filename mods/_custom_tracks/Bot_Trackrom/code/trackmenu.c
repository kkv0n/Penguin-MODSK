#include <common.h>
#include "header/bot.h"

void MM_TrackSelect_MenuProc(struct RectMenu* menu)
{
	//dont show track menu, load the track directly
	sdata->gGT->currLEV = CUSTOM_TRACK_ID;
    MainRaceTrack_RequestLoad(CUSTOM_TRACK_ID);
	sdata->gGT->prevLEV = CUSTOM_TRACK_ID;
	
	//close menu or the game crashes
	RECTMENU_Hide(menu);
}