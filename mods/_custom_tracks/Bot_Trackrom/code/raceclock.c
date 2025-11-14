#include <common.h>
#include "header/bot.h"

// used for both finished lap time and current race time
void UI_DrawRaceClock(u_short paramX, u_short paramY, u_int flags, struct Driver* driver)
{
	// flag parameter bits
	// despite being a u_int only 6 bits are ever used
	// 0: Display in-race time (if 0) / Display time trial results (if 1)
	// 1: Enable or disable flashing
	// 2: Only used to check if you're in a relic race
	// 3: Only used to check if lap 1 should flash (which is when it's the fastest lap)
	// 4: Only used to check if lap 2 should flash (which is when it's the fastest lap)
	// 5: Only used to check if lap 3 should flash (which is when it's the fastest lap)
	struct GameTracker* gGT;
	short sVar1;
	int stringColor;
	u_int lapIndex;
	u_int uVar3;
	u_char *totalTimeString;
	int iVar5;
	int numParamY;
	int iVar7;
	u_int fontType;

	int msElapsed;
	int str;
	int posX;
	int numLaps;
	int levID;

	int strOffset;

	char msOnes;
	u_short uVar11;
	short *lapTextHeight;
	char msTens;
	char secondsOnes;
	int lapFontType;
	char secondsTens;
	short strFlags_but_its_also_posY;
	u_short stringColor_but_its_also_relicColor;
	char minutesOnes;
	char minutesTens;
	char acStack80 [8];

	u_short textPosX;
	u_short textPosY;

	char *local_38;
	int unbitshiftTextPosX;
	int bitshiftTextPosX;

	gGT = sdata->gGT;

	minutesTens = '\0';

	// if pointer is nullptr
	if (driver == NULL || driver != NULL && driver->driverID != 0)
	{
		// quit the function
		return;
	}


	minutesTens = 9;
	minutesOnes = 9;
	secondsTens = 5;
	secondsOnes = 9;
	msTens = 9;
	msOnes = 9;


	// milliseconds elapsed in race
	msElapsed = driver->timeElapsedInRace;



	// OG game was "== 7"
	// but now expand for Online
	if (gGT->numLaps >= 7)
	{
		// less than 99:59:99
		if ((char)(msElapsed / 0x8ca00) < 10)
		{
			minutesTens = (char)(msElapsed / 0x8ca00) % 10;

			goto setRestOfTime;
		}
	}

	// if numLaps is not 7
	else
	{
		// less than 9:59:99
		if ((char)(msElapsed / 0xe100) < 10)
		{
			setRestOfTime:
			minutesOnes = (char)(msElapsed / 0xe100) % 10;
			secondsTens = (char)((msElapsed / 0x2580) % 6);
			secondsOnes = (char)((msElapsed / 0x3c0) % 10);
			msTens = (char)(((msElapsed * 10) / 0x3c0) % 10);
			msOnes = (char)(((msElapsed * 100) / 0x3c0) % 10);
		}
	}


	if ((flags & 1) == 0)
	{
		// TIME
		str = (ghostMode) ? 0x4d : 0x12;

		// Draw small string
		fontType = FONT_SMALL;

		strFlags_but_its_also_posY = 0;
	}

	else
	{
		// TOTAL
		str = 0xc4;

		// Draw big string
		fontType = FONT_BIG;

		// this particular snippet is for drawing the TOTAL text in the time trial lap results screen before it flashes, at which point it moves from the left to the center of the screen
		// if global game timer is on an odd number (in which case this condition doesn't execute) then set string to end at posX of next DrawLine and use data.ptrColor[WHITE]
		if (((flags & 4) == 0) || (strFlags_but_its_also_posY = (JUSTIFY_RIGHT | WHITE), (gGT->timer & FPS_DOUBLE(2)) != 0))
		{
			strFlags_but_its_also_posY = (JUSTIFY_RIGHT | ORANGE);
		}
	}

	textPosX = paramX;
	textPosY = paramY;

	// str = 0x12: TIME
	// str = 0x4d: TIME TRIAL
	// str = 0xc4: TOTAL
	// str = 0xc5: YOUR TIME
	 DecalFont_DrawLine(sdata->lngStrings[str], (int)(short)paramX, (int)(short)paramY, fontType, (int)strFlags_but_its_also_posY);

	// set string to use data.ptrColor[1], which is the periwinkle gradient seen in the LAP text on the HUD
	// particularly used for relic race when the time is frozen
	strFlags_but_its_also_posY = PERIWINKLE;

	if
	(
		// if time isn't frozen or we're not in relic race
		(gGT->frozenTimeRemaining == 0) &&
		(
			// set color to orange if the string shouldn't flash
			strFlags_but_its_also_posY = ORANGE,

			// if the string (which is probably the total time text in the time trial lap results screen) should flash
			(flags & 4) != 0
		)
	)
	{
		// use timer to change color on even and odd frames
		// strFlags_but_its_also_posY equals either 4 (white) or 0 (orange)
		strFlags_but_its_also_posY = (u_short)((gGT->timer & FPS_DOUBLE(2)) == 0) << 2;
	}


	// OG game was "== 7"
	// but now expand for Online
	if (gGT->numLaps >= 7)
	{
		// String for amount of time in total race
		totalTimeString = &rdata.s_timeString_empty[0];

		// Convert each number from the binary version of the number to the ascii version of the number by adding ascii value of '0'
		totalTimeString[0] = minutesTens + '0';

		strOffset = 1;
	}

	// if number of laps is not 7
	else
	{
		// String for amount of time in lap
		totalTimeString = &sdata->raceClockStr[0];

		strOffset = 0;
	}

	// Convert each number from the binary version of the number to the ascii version of the number by adding ascii value of '0'
	totalTimeString[strOffset+0] = minutesOnes + '0';
	totalTimeString[strOffset+2] = secondsTens + '0';
	totalTimeString[strOffset+3] = secondsOnes + '0';
	totalTimeString[strOffset+5] = msTens + '0';
	totalTimeString[strOffset+6] = msOnes + '0';


	// If in-race, reuse X and Y positions used for the TIME/TIME TRIAL text in the top left corner of the HUD for the actual time itself
	// Except set the Y position to be lower
	if ((flags & 1) == 0)
	{
		posX = (int)(short)textPosX;
		numParamY = ((u_int)textPosY + 8) * 0x10000;
	}

	// If in the time trial results screen, reuse X and Y positions used for the rightmost margin of the "TOTAL" text used for displaying the total time, and then adjust them accordingly
	else
	{
		posX = (int)(((u_int)textPosX + 0x11) * 0x10000) >> 0x10;
		numParamY = (u_int)textPosY << 0x10;
	}

	// Draw String	
	 DecalFont_DrawLine(totalTimeString, posX, numParamY >> 0x10, FONT_BIG, (int)strFlags_but_its_also_posY);



	if
	(
		// If you're not in a Relic Race
		((gGT->gameMode1 & RELIC_RACE) == 0) ||
		((flags & 2) != 0)
	)
	{
		// If you're not in Arcade mode,
		// nor Time Trial, nor adventure mode
		if (gGT->levelID >= GEM_STONE_VALLEY)
		{
			return;
		}

		lapIndex = (u_int)driver->lapIndex;
		numLaps = 0;

		if (lapIndex == -1)
		{
			return;
		}

		local_38 = acStack80;
		numParamY = 1;
		bitshiftTextPosX = (u_int)textPosX << 0x10;
		unbitshiftTextPosX = bitshiftTextPosX >> 0x10;
		do
		{
			if ((numLaps <= (int)lapIndex) && (numLaps < (char)gGT->numLaps))
			{
				 UI_SaveLapTime(
					lapIndex % 4,
					gGT->elapsedEventTime - driver->lapTime,
					(u_int)driver->driverID);

				// custom code for optimization using this unrelated variable
				iVar5 = (u_int)driver->driverID * 7 + (numLaps % 4);

				// set slot for the tens place of a minute to nothing
				rdata.s_timeString_empty[0] = ' ';

				// Convert each number from the binary version of the number to the ascii version of the number by adding ascii value of '0'
				// This is dynamically programmed to handle time for more than one player, see UI_SaveLapTime

				rdata.s_timeString_empty[1] = sdata->LapTimes.p1_Min1s[iVar5] + '0';
				rdata.s_timeString_empty[3] = sdata->LapTimes.p1_Sec10s[iVar5] + '0';
				rdata.s_timeString_empty[4] = sdata->LapTimes.p1_Sec1s[iVar5] + '0';
				rdata.s_timeString_empty[6] = sdata->LapTimes.p1_Ms10s[iVar5] + '0';
				rdata.s_timeString_empty[7] = sdata->LapTimes.p1_Ms1s[iVar5] + '0';

				// default
				stringColor_but_its_also_relicColor = PERIWINKLE;

				if
				(
					// if this is lap 1, and if lap 1 should flash
					((numLaps == 0) && ((flags & 8) != 0)) ||

					// if this is lap 2, and if lap 2 should flash
					((numLaps == 1) && ((flags & 0x10) != 0)) ||

					// if this is lap 3, and if lap 3 should flash
					((numLaps == 2) && ((flags & 0x20) != 0))
				)
				{
					// Change color based on frame counter
					stringColor_but_its_also_relicColor = ((u_short)(gGT->timer >> FPS_DOUBLE(1)) ^ 1) & 1;
				}

				// Otherwise, color is white by default, you can see that in "stringColor_but_its_also_relicColor = 1" near lap 3 check

				if ((flags & 1) == 0)
				{

					// Set lap number in "Ln" string
					sdata->s_Ln[1] = (char)numLaps + '1';

					iVar7 = (int)(((u_int)textPosY + numParamY * 8 + 0x10) * 0x10000) >> 0x10;

					// draw "Ln" string
					 DecalFont_DrawLine(&sdata->s_Ln[0], bitshiftTextPosX >> 0x10, iVar7, FONT_SMALL, RED);

					lapFontType = FONT_SMALL;
					stringColor = PERIWINKLE;
					iVar5 = (int)(((u_int)textPosX + 0x1a) * 0x10000) >> 0x10;
				}
				else
				{
					// draw big text for time in each lap
					lapFontType = FONT_BIG;

					// if number of laps is more than 3
					if (gGT->numLaps > 3)
					{
						// draw small text for time in each lap
						lapFontType = FONT_SMALL;
					}

					// DAT_8008d510
					// %d

					sprintf(local_38, &sdata->s_int[0], numParamY);
					lapTextHeight = (short *)(&data.font_charPixHeight[lapFontType]);

					// draw string
					 DecalFont_DrawLine(local_38, unbitshiftTextPosX, (int)(((u_int)textPosY - ((char)gGT->numLaps - numLaps) * (int)*lapTextHeight) * 0x10000) >> 0x10, lapFontType, (JUSTIFY_RIGHT | RED));

					// LAP
					 DecalFont_DrawLine(sdata->lngStrings[0x18], (int)(((u_int)textPosX - (u_int)data.font_charPixWidth[lapFontType])), (int)(((u_int)textPosY - ((char)gGT->numLaps - numLaps) * (int)*lapTextHeight) * 0x10000) >> 0x10, lapFontType, (JUSTIFY_RIGHT | RED));

					stringColor = (int)(short)stringColor_but_its_also_relicColor;
					iVar7 = (int)(((u_int)textPosY - ((char)gGT->numLaps - numLaps) * (int)*lapTextHeight) * 0x10000) >> 0x10;
					iVar5 = unbitshiftTextPosX;
				}

				// draw string for total amount of time in race
				 DecalFont_DrawLine(&rdata.s_timeString_empty[0], iVar5, iVar7, lapFontType, stringColor);
			}

			// lap counter
			lapIndex = (u_int)driver->lapIndex;

			numLaps++;
			numParamY = numParamY + 1;
			if ((int)(lapIndex + 1) <= numLaps)
			{
				return;
			}
		} while( 1 );
	}



	return;
}