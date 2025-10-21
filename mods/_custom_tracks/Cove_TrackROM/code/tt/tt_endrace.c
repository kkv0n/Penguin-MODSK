#include <common.h>

// this goes to footer
static int str_number = 0x20; // " \0"

extern struct RectMenu menu224;


void TT_End(void)
{
    int elapsedFrames;
    int startX;
    short endX;
    short pos[2];
	char** lngStrings;
	u_int gameModeEnd;
    struct GameTracker *gGT;

	gGT = sdata->gGT;
	lngStrings = sdata->lngStrings;
	gameModeEnd = gGT->gameModeEnd;

    sdata->flags_timeTrialEndOfRace |= 1;
	
	

    // If you just beat N Tropy && N Tropy was beaten on all tracks
    if (
			((gameModeEnd & NTROPY_JUST_BEAT) != 0) &&
			((GAMEPROG_CheckGhostsBeaten(1) & 0xffff) != 0)
		)
    {
        sdata->gameProgress.unlocks[0] |= 0x20; // Unlock N Tropy
    }

    // copy the frame counter variable
    elapsedFrames = sdata->framesSinceRaceEnded;

	// Conditions to increment frame
    if (
			// First 900 frames (30 seconds)
			(elapsedFrames < FPS_DOUBLE(900)) ||
			
			(
				((gameModeEnd & NEW_HIGH_SCORE) == 0) &&
				
				(
					// Transition the high scores on-screen
					(elapsedFrames <= FPS_DOUBLE(1000)) ||
					
					(
						// Wait until press X, then transition off-screen
						((sdata->menuReadyToPass & 0x10) != 0) &&
						(elapsedFrames < FPS_DOUBLE(1018))
					)
				)
			)
		)
    {
        elapsedFrames++;
    }

    // copy "back" the frame counter variable
    sdata->framesSinceRaceEnded = elapsedFrames;

	// First 90 frames (0-3)
	// Return at bottom of IF block
	if (elapsedFrames <= FPS_DOUBLE(90))
    {
		// no lerp, just sit on-screen
        endX = 0x14;

        if (elapsedFrames >= FPS_DOUBLE(65))
        {
            endX = -0x96;
            elapsedFrames -= FPS_DOUBLE(65);
        }

		// draw race clock in top-left corner
         UI_Lerp2D_Linear(
			&pos[0],
			0x14, 8,
			endX, 8,
			elapsedFrames, FPS_DOUBLE(0x14));

		 UI_DrawRaceClock(
			(int)pos[0], (int)pos[1], 0, gGT->drivers[0]);

		return;
    }

	// between 91 and 900 frames (3-30)
	// Return at bottom of IF block
	if (elapsedFrames <= FPS_DOUBLE(900))
	{
		// first transition is race clock
		elapsedFrames -= FPS_DOUBLE(90);

		// race time
		 UI_Lerp2D_Linear(
			&pos[0],
			-0x64, 90,
			0x100, 90,
			elapsedFrames, FPS_DOUBLE(0x14));

		 TT_End_Time(
			(int)pos[0], pos[1], sdata->flags_timeTrialEndOfRace);



		// Blink Orange/White
		int color = 
			(gGT->timer & FPS_DOUBLE(1)) ? 0xffff8000 : 0xffff8004;



		// "new high score" 1 second later
		elapsedFrames -= FPS_DOUBLE(30);

		if ((elapsedFrames > 0) &&

			// if there is a new high score
			gGT->newHighScoreIndex > -1)
		{
			 UI_Lerp2D_Linear(
				&pos[0],
				0x264, 122,
				0x100, 122,
				elapsedFrames, FPS_DOUBLE(0x14));

			// "NEW HIGH SCORE!"
			DecalFont_DrawLine(
				lngStrings[353],
				(int)pos[0], (int)pos[1],
				1, color);

			// Total time should flash
			sdata->flags_timeTrialEndOfRace |= (1<<2);
		}



		// "new best lap" 1 second later
		elapsedFrames -= FPS_DOUBLE(30);

		if ((elapsedFrames > 0) &&

			// if got new best lap
			((gameModeEnd & NEW_BEST_LAP) != 0))
		{
			 UI_Lerp2D_Linear(
				&pos[0],
				-0x64, 142,
				0x100, 142,
				elapsedFrames, FPS_DOUBLE(0x14));

			// NEW BEST LAP!
			DecalFont_DrawLine(
				lngStrings[370],
				(int)pos[0], (int)pos[1],
				1, color);

			// make the best row start flashing
			sdata->flags_timeTrialEndOfRace |= 1 << (3 + gGT->lapIndexNewBest);
		}



		// "n tropy" 1 second later
		elapsedFrames -= FPS_DOUBLE(30);

		int eitherOneTrue = 
			NTROPY_JUST_BEAT | NTROPY_JUST_OPENED;

		if ((elapsedFrames > 0) &&

			// if just open, or beat, n tropy
			((gameModeEnd & eitherOneTrue) != 0))
		{
			 UI_Lerp2D_Linear(
				&pos[0],
				0x264, 162,
				0x100, 162,
				elapsedFrames, FPS_DOUBLE(0x14));

			char *nTropyString;

			// N Tropy Opened, or Beat
			nTropyString = lngStrings[371];
			if ((gameModeEnd & NTROPY_JUST_BEAT) != 0)
				nTropyString = lngStrings[372];

			// Draw the "N Tropy" related string
			DecalFont_DrawLine(
				nTropyString,
				(int)pos[0], (int)pos[1],
				1, color);
		}

		// PRESS * TO CONTINUE
		DecalFont_DrawLine(lngStrings[201], 0x100, 0xbe, 1, 0xffff8000);

		
					// ==== Pause Timer until Press X =======
			// Cross or Circle, or if timer drags on too long
			if (
					((sdata->AnyPlayerTap & 0x50) != 0)
				)
			{
				sdata->framesSinceRaceEnded = FPS_DOUBLE(1001);

				// unpause frame counter,
				// which then counts up to 1018 for transition-out
				sdata->menuReadyToPass |= 0x10;
			}
			return;
		}

		




	if ((sdata->menuReadyToPass & 0x10) != 0)
	{
		// start showing menu
		sdata->menuReadyToPass = (sdata->menuReadyToPass & 0xffffffef) | 1;

		sdata->flags_timeTrialEndOfRace = 0;



		RECTMENU_Show(&menu224);
	}

    return;
}

void TT_End_Time(int paramX, short paramY, u_int UI_DrawRaceClockFlags)
{
	struct GameTracker* gGT;
	struct Driver* d;
	short textWidth;
	short pos[2];
	RECT rectangle;

	gGT = sdata->gGT;
	d = gGT->drivers[0];

	// "TOTAL"
	textWidth =  DecalFont_GetLineWidth(sdata->lngStrings[0xc4], 1);

	// === Naughty Dog Bug ===
	// Start and End is the same
	
	#if 0
	
	 UI_Lerp2D_Linear(
		&pos[0],
		(paramX - (0x88 - textWidth) / 2), paramY,
		(paramX - (0x88 - textWidth) / 2), paramY,
		sdata->framesSinceRaceEnded, FPS_DOUBLE(0x14));
	
	#else
	
	pos[0] = (paramX - (0x88 - textWidth) / 2);
	pos[1] = paramY;
	
	#endif

	// "YOUR TIME"
	DecalFont_DrawLine(sdata->lngStrings[197], paramX, ((u_int)pos[1] - 0x4c), FONT_BIG, (JUSTIFY_CENTER | ORANGE));

	 UI_DrawRaceClock(pos[0], pos[1], UI_DrawRaceClockFlags, d);

	rectangle.x = (pos[0] - textWidth) - 6;
	rectangle.y = pos[1] - 0x50;

	rectangle.w = textWidth + 0x94;
	rectangle.h = 99;

	// Draw 2D Menu rectangle background
	RECTMENU_DrawInnerRect(&rectangle, 4, gGT->backBuffer->otMem.startPlusFour);

	return;
}



struct MenuRow EndRaceMenu[] =
{
	[0] =
	{
		.stringIndex = 4, // retry
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},

	[1] =
	{
		.stringIndex = 3, // quit
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 1,
		.rowOnPressRight = 1,
	},

	[2] =
	{
		.stringIndex = 0xFFFF,
	}
};

struct RectMenu menu224 =
{
	.stringIndexTitle = 0xFFFF,
	.posX_curr = 0x100,
	.posY_curr = 0xA0,

	.unk1 = 0,

	.state = 0xC83,
	.rows = EndRaceMenu,
	.funcPtr =  UI_RaceEnd_MenuProc,
	.drawStyle = 4,

	// rest of variables all default zero
};