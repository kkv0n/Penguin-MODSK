#include <common.h>
#include "header/cove_rom.h"

void MM_TrackSelect_MenuProc(struct RectMenu* menu)
{
	//dont show track menu, load the track directly
    MainRaceTrack_RequestLoad(CUSTOM_LEVEL_ID);
	sdata->gGT->currLEV = CUSTOM_LEVEL_ID;
	sdata->gGT->prevLEV = CUSTOM_LEVEL_ID;
}