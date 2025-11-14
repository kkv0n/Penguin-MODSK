#include <common.h>
#include "../../header/bot.h"

static int str_number = 0x20; // " \0"
extern struct RectMenu menu222;

// 3528

void CBOT_EndEvent_DrawMenu(void)
{
	struct GameTracker *gGT;


	char i;
	char numPlyr;
	char totalPlyr;

	short letterPos[2];

	short t;
	short elapsedFrames;
	short levSpawn;

	short lerpStartX;
	short txtStartX;
	short lerpStartY;
	short lerpEndX;
	short txtEndX;
	short lerpEndY;
	short lerpFrames;
	short currFrame;
	u_int scaleDown;
	u_int txtColor;
	

	gGT = sdata->gGT;
	numPlyr = gGT->numPlyrCurrGame;
	totalPlyr = numPlyr + numCustomBots;
	
	struct Driver *driver = gGT->drivers[0];

	elapsedFrames = sdata->framesSinceRaceEnded;

	// count frames if hasn't been 30 seconds
	if (elapsedFrames < FPS_DOUBLE(900))
		elapsedFrames++;

	sdata->framesSinceRaceEnded = elapsedFrames;

	if (driver->instBigNum->scale[0] != 0x1e00)
	{
		struct Instance* instFruitDisp = driver->instFruitDisp;
		instFruitDisp->scale[0] = 0;
		instFruitDisp->scale[1] = 0;
		instFruitDisp->scale[2] = 0;
	}

	// if not in Token mode, these won't be used until later;
	lerpStartY = 0;
	lerpEndY = 0;
	
	// For trophy race, check 1st place
	int boolWin = (driver->driverRank == 0);

	for (i = 0; i < numPlyr; i++)
	{
		// Draw how much time it took to finish laps and race
		 AA_EndEvent_DisplayTime(i, lerpEndY);
	}
	
	elapsedFrames = sdata->framesSinceRaceEnded;

	// If it hasn't been 1 second from race ended
	if (elapsedFrames < FPS_DOUBLE(30))
		return;

	// If there is one player
	if (numPlyr == 1)
	{
		// start counting time 1 second after race ends
		t = (elapsedFrames & 0xffff) - FPS_DOUBLE(30);

		if (
			// Every 0.5 seconds or so
			((t % FPS_DOUBLE(10) & 0xffff) == 0) &&

			// sdata->numIconsEOR is the number of icons being
			// drawn on the end-of-race menu in 1P mode

			// If you have not drawn all drivers yet
			(sdata->numIconsEOR < totalPlyr))
		{
			// add an icon to draw
			sdata->numIconsEOR++;
		}

		// loop through all the driver icons
		for (i = 0; i < sdata->numIconsEOR; i++)
		{
			int iVar11 = gGT->pushBuffer[0].rect.x +
						 (gGT->pushBuffer[0].rect.w - totalPlyr * 56 + 12) / 2 + (i * 56);

			if (elapsedFrames + lerpEndY > FPS_DOUBLE(300))
			{
				lerpStartX = iVar11;
				lerpEndX = -100;
				currFrame = elapsedFrames + lerpEndY - FPS_DOUBLE(300);
			}
			else
			{
				lerpStartX = 0x218;
				lerpEndX = iVar11;
				currFrame = t;
			}

			t -= FPS_DOUBLE(10);

			// interpolate fly-in
			 UI_Lerp2D_Linear(
				&letterPos[0],
				lerpStartX, 0x60,
				lerpEndX, 0x60,
				currFrame, FPS_DOUBLE(10));

			str_number = (char)i + '1';

			// print a single character, a number 1-8,
			DecalFont_DrawLine(
				(char*)&str_number, letterPos[0] + 0x20, 0x5f, 2, 4);

			// Draw the driver's character icon
			 UI_DrawDriverIcon(

				gGT->ptrIcons[
					data.MetaDataCharacters[
						data.characterIDs[
							gGT->driversInRaceOrder[i]->driverID
						]
					].iconID],

				letterPos[0], 0x60,
				
				&gGT->backBuffer->primMem,

				// pointer to OT mem
				gGT->pushBuffer_UI.ptrOT,

				1, 0x1000, 0x808080);
		}
	}

	// 0x78 + 0x6e = 0xe6 (230) frames waited for Token Race
	if ((elapsedFrames-lerpStartY) < FPS_DOUBLE(110))
		return;

	


	// if the menu is already drawing
	if (sdata->menuReadyToPass & 1)
		return;


		// End of Race based on number of players (1 or more)
		//menu222.posY_curr = (numPlyr == 1) ? 170 : 108;
		
		menu222.posY_curr = 170;
		
		RECTMENU_Show(&menu222);

		// record that the menu is drawing
		sdata->menuReadyToPass |= 1;
	

		return;

}

void AA_EndEvent_DisplayTime(short driverId, short param_2)
{
	struct GameTracker *gGT;
	struct Driver *driver;
	struct UiElement2D *hudArray;
	struct UiElement2D *hud;
	struct Instance *bigNum;
	char numPlyr, tenseconds;
	short framesElapsed;
	short lerpStartY;
	short lerpEndY;
	short lerpStartX;
	short lerpEndX;
	short currFrame;
	short width;
	short endFrame;
	short posXY[2];
	RECT r;

	gGT = sdata->gGT;
	driver = gGT->drivers[0];

	// stop after 12 seconds
	if(driver->framesSinceRaceEnded_forThisDriver > FPS_DOUBLE(360))
		return;

	numPlyr = gGT->numPlyrCurrGame;
	hudArray = data.hudStructPtr[numPlyr - 1];
	hud = &hudArray[driverId * 0x14]; // to-do, use enum where 0x14 is number of hud
	bigNum = driver->instBigNum;


		// default height for 1/3 laps.
		r.h = 0x44;

	// increment counter for number of frames since the player ended the race
	driver->framesSinceRaceEnded_forThisDriver++;
	framesElapsed = driver->framesSinceRaceEnded_forThisDriver;

	if (
		// if player ended race less than 110 frames ago
		(framesElapsed < FPS_DOUBLE(110)) &&

		// If you press Cross or Circle
		((sdata->AnyPlayerTap & 0x50) != 0) &&

		// only one player
		(numPlyr == 1))
	{
		// Assume race ended 110 frames ago
		framesElapsed = FPS_DOUBLE(110);
		sdata->framesSinceRaceEnded = framesElapsed;
		driver->framesSinceRaceEnded_forThisDriver = framesElapsed;

		sdata->numIconsEOR = numPlyr + gGT->numBotsNextGame;

		// clear gamepad input (for menus)
		RECTMENU_ClearInput();
	}

	tenseconds = (framesElapsed + param_2 > FPS_DOUBLE(300));

	// === Draw BigNum ===

	// Player 2
	lerpEndY = 0x41;


		if (driverId == 0)
			lerpEndY = -0x3d;

	// If race ended more than 10 seconds ago.
	if (tenseconds)
	{
		currFrame = framesElapsed + param_2 - FPS_DOUBLE(300);
		endFrame = FPS_DOUBLE(0xf);

		lerpStartX = -0xae;
		lerpEndX =  UI_ConvertX_2(-100, hud[2].z);
		lerpStartY = lerpEndY;


	}

	// If not
	else
	{
		currFrame = framesElapsed;
		endFrame = FPS_DOUBLE(0x1e);

		lerpStartX =  UI_ConvertX_2(hud[2].x, hud[2].z);
		lerpStartY =  UI_ConvertY_2(hud[2].y, hud[2].z);
		lerpEndX = -0xae;


	}

	// interpolate fly-in positionXY
	 UI_Lerp2D_Linear(
		&posXY[0],
		lerpStartX, lerpStartY,
		lerpEndX, lerpEndY,
		currFrame, endFrame);

	bigNum->matrix.t[0] = posXY[0];
	bigNum->matrix.t[1] = posXY[1];

	// interpolate scale to 0x1e00
	 UI_Lerp2D_Linear(
		&posXY[0],
		hud[2].scale, 0,
		0x1e00, 0,
		framesElapsed, FPS_DOUBLE(30));

	bigNum->scale[0] = posXY[0];
	bigNum->scale[1] = posXY[0];
	bigNum->scale[2] = posXY[0];

	// === Draw Suffix ===

	// Player 2
	lerpEndY = 0x89;


		if (driverId == 0)
			lerpEndY = 9;

	if (tenseconds)
	{
		lerpStartX = 0x78;
		lerpStartY = lerpEndY;
		lerpEndX = -0x3c;


	}
	else
	{
		lerpStartX = hud[5].x;
		lerpStartY = hud[5].y;
		lerpEndX = 0x78;


	}

	 UI_Lerp2D_Linear(
		&posXY[0],
		lerpStartX, lerpStartY,
		lerpEndX, lerpEndY,
		currFrame, endFrame);
	
	if (!ghostMode)
		UI_DrawPosSuffix(posXY[0], posXY[1], driver, 0);

	// === DrawRaceClock ===



		lerpEndY = 0xc3;
		if (driverId == 0)
			lerpEndY = 0x3e;

		if (tenseconds)
		{
			lerpStartX = 0x150;
			lerpEndX = 0x27c;
		}

		else
		{
			lerpStartX = 0x218;
			lerpEndX = 0x150;
		}



	 UI_Lerp2D_Linear(
		&posXY[0],
		lerpStartX, lerpEndY,
		lerpEndX, lerpEndY,
		currFrame, endFrame);

	 UI_DrawRaceClock(posXY[0], posXY[1], 1, driver);

	// "TOTAL"
	width =  DecalFont_GetLineWidth(sdata->lngStrings[0xc4], 1);

	r.x = (posXY[0] - width) + -6;
	r.y = (posXY[1] - r.h) + 0xd;
	r.w = width + WIDE_34(0x94);
	r.h += 6;

	// Draw 2D Menu rectangle background
	RECTMENU_DrawInnerRect(&r, 4, gGT->backBuffer->otMem.startPlusFour);
	return;
}

struct MenuRow rows222[] =
{
	// Retry
	{
		.stringIndex = 4,
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},
	// Change Character
	{
		.stringIndex = 5,
		.rowOnPressUp = 0,
		.rowOnPressDown = 2,
		.rowOnPressLeft = 1,
		.rowOnPressRight = 1,
	},
	// Quit
	{
		.stringIndex = 3,
		.rowOnPressUp = 1,
		.rowOnPressDown = 2,
		.rowOnPressLeft = 2,
		.rowOnPressRight = 2,
	},
	// NULL, end of menu
	{
		.stringIndex = 0xFFFF,
		.rowOnPressUp = 0,
		.rowOnPressDown = 0,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	}
};

struct RectMenu menu222 =
{
	.stringIndexTitle = 0xFFFF,
	.posX_curr = 256,
	.posY_curr = 170,
	.unk1 = 0,
	.state = (0x800 | USE_SMALL_FONT | CENTER_ON_COORDS), // 0x883
	.rows = rows222,
	.funcPtr =  UI_RaceEnd_MenuProc,
	.drawStyle = 4,
	// rest of variables all default zero
};