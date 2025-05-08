#include <common.h>

void walls_()
{
	struct GameTracker* gGT = sdata->gGT;
	struct Driver* driver;


	if (D230.MM_State == 1)
	{
		DecalFont_DrawLine("WALL DRIVING MOD", 5, 197, FONT_SMALL, PENTA_WHITE);
		DecalFont_DrawLine("BY CUSTOM TEAM RACING", 5, 206, FONT_SMALL, PENTA_WHITE);
	}

	//dont run this if the level is not a race track or traffic lights are running
	if (gGT->levelID > TURBO_TRACK && gGT->trafficLightsTimer > 0) return;
    
	//allow multiplayer
	for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++)
	{
		if (gGT->drivers[i] == NULL) continue; //avoid null driver

		driver = gGT->drivers[i];

		//give turbo in walls	
		if (driver->currBlockTouching->speedImpact == -127 && gGT->levelID != SEWER_SPEEDWAY)
		{
			goto turbo;
		}
		else if (driver->currBlockTouching->speedImpact == -126 && gGT->levelID == SEWER_SPEEDWAY)
		{
		turbo:
			VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), 0x100);
		}
	}

}