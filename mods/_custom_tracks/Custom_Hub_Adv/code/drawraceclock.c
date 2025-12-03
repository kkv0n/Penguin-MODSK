#include <common.h>
#include "adventure.h"

extern unsigned int PB_lap;
extern unsigned int Worst_lap;

RECT timesRect =
{
	.x = 130,
	.y = 66,
	.w = 233,
	.h = 68
};

// used for both finished lap time and current race time
void UI_DrawRaceClock(u_short paramX, u_short paramY, u_int flags, struct Driver* d)
{
	int numParamY;
	int posX;
	u_short textPosX;
	u_short textPosY;


	// if pointer is nullptr or we are in a custom hub
	if (d == 0)
	{
		// quit the function
		return;
	}





	textPosX = paramX;
	textPosY = paramY;
	posX = (int)(short)textPosX;
	numParamY = ((u_int)textPosY + 8) * 0x10000;

	/*
	78 = "arcade"
	77 = "time trial"
	18 = "time"
	both were replaced for custom cup tittle anyways
	i just chose "arcade" text bc i dont like TT in any form :D
	*/
	DecalFont_DrawLine(sdata->lngStrings[18], (int)(short)paramX, (int)(short)paramY, FONT_SMALL, ORANGE);





	time_helper(0, false);





	return;
}

void time_helper(u_char showthis, bool win)
{
	struct Driver* d = sdata->gGT->drivers[0];


	char RaceTime[9];
	// milliseconds elapsed in race
	int msElapsed = d->timeElapsedInRace;
	
	// Draw String
	unsigned int mseconds;
	unsigned int seconds;
	unsigned int minutes;
	mseconds = (((msElapsed * (MILLISECOND * 10)) / MILLISECONDS(100)) % 100);
	seconds = (msElapsed / SECONDS(1)) % 60;
	minutes = (msElapsed / MINUTES(1)) % 101;
	sprintf(RaceTime, "%02d:%02d:%02d", minutes, seconds, mseconds);
	if (msElapsed >= MINUTES(100)) strcpy(RaceTime, "99:59:99");



	/*
	this was probably made by a math addicted lmao stop pls, just set a coordinate manually
	fortunately copy paste > maths B)
	*/
	//DecalFont_DrawLine(RaceTime, (int)(short)0x14, (int)(((u_int)8 + 8) * 0x10000) >> 0x10, FONT_BIG, ORANGE); //i just used the calculator and third parameter is just 0x10 lol


       


	unsigned short posxy[2];
	unsigned char color;
	posxy[0] = (showthis == 1) ? 0xD8 - 35 : 0x14; // x
	posxy[1] = (showthis == 1) ? 0x4C + 20 : 0x18; // y
	
	
	    //if times condition to win is enabled
	    if (hardcore)
		{
			unsigned short coordY = (showthis == 1) ? posxy[1] - 36 : 206;
			unsigned short coordX = (showthis == 1) ? 0x100 : 60;
			
			char* text = (showthis == 1) ? (win) ? "YOU WIN!" : "YOU FAILED!" : timeToWin;
			
			unsigned short colr = (showthis == 1) ? (win) ? JUSTIFY_CENTER | TINY_GREEN : JUSTIFY_CENTER | CORTEX_RED :
            ((sdata->gGT->timer & FPS_DOUBLE(2)) == 0) ? JUSTIFY_CENTER | SILVER : JUSTIFY_CENTER | PAPU_YELLOW ; 
			
		  DecalFont_DrawLine(text, coordX, coordY, FONT_SMALL, colr);
		}
		
		
		

	color = (showthis == 1) ? ((sdata->gGT->timer & FPS_DOUBLE(2)) == 0) ? //decides what color the text should use
		JUSTIFY_CENTER | WHITE : JUSTIFY_CENTER | ORANGE : ORANGE;

	DecalFont_DrawLine(RaceTime, posxy[0], posxy[1] - 8, FONT_BIG, color);


	color = (showthis == 1) ? JUSTIFY_CENTER | ORANGE : ORANGE;

	
	if (showthis == 1) DecalFont_DrawLine("TOTAL TIME", posxy[0], posxy[1] - 24, FONT_BIG, color);

	if (sdata->gGT->numPlyrCurrGame == 1) //idk if this is called in multiplayer
	{
		if (showthis == 0)
		{
			unsigned int LapTime = sdata->gGT->elapsedEventTime - d->lapTime;
			//current lap time (live counter)
			mseconds = (((LapTime * (MILLISECOND * 10)) / MILLISECONDS(100)) % 100);
			seconds = (LapTime / SECONDS(1)) % 60;
			minutes = (LapTime / MINUTES(1)) % 10;
			sprintf(RaceTime, "%02d:%02d:%02d", minutes, seconds, mseconds);
			DecalFont_DrawLine("CURR:", 0x14, 8 + posxy[1], FONT_SMALL, RED);
			DecalFont_DrawLine(RaceTime, 0x14 + 63, 8 + 0x18, FONT_SMALL, PERIWINKLE);
		}


		//worst and best time
		if (d->lapIndex > 0)
		{

			//best
			mseconds = (((PB_lap * (MILLISECOND * 10)) / MILLISECONDS(100)) % 100);
			seconds = (PB_lap / SECONDS(1)) % 60;
			minutes = (PB_lap / MINUTES(1)) % 11;
			sprintf(RaceTime, "%02d:%02d:%02d", minutes, seconds, mseconds);
			if (PB_lap >= MINUTES(10)) strcpy("09:59:99", RaceTime);
			color = (showthis == 1) ? JUSTIFY_CENTER | RED : RED;
			DecalFont_DrawLine("PB:", posxy[0], posxy[1] + 16, FONT_SMALL, color);
			color = (showthis == 1) ? JUSTIFY_CENTER | ORANGE : ORANGE;
			DecalFont_DrawLine(RaceTime, posxy[0] + 37, posxy[1] + 16, FONT_SMALL, PERIWINKLE);

			//worst
			mseconds = (((Worst_lap * (MILLISECOND * 10)) / MILLISECONDS(100)) % 100);
			seconds = (Worst_lap / SECONDS(1)) % 60;
			minutes = (Worst_lap / MINUTES(1)) % 11;
			sprintf(RaceTime, "%02d:%02d:%02d", minutes, seconds, mseconds);
			if (Worst_lap >= MINUTES(10)) strcpy("09:59:99", RaceTime);
			color = (showthis == 1) ? JUSTIFY_CENTER | RED : RED;
			DecalFont_DrawLine("WORST:", posxy[0], posxy[1] + 24, FONT_SMALL, RED);
			color = (showthis == 1) ? JUSTIFY_CENTER | ORANGE : ORANGE;
			DecalFont_DrawLine(RaceTime, posxy[0] + 76, posxy[1] + 24, FONT_SMALL, PERIWINKLE);

		}

	}
	if (showthis == 1)
	{
		
		UI_DrawDriverIcon(sdata->gGT->ptrIcons[data.MetaDataCharacters[data.characterIDs[0]].iconID],
		posxy[0] - 44, posxy[1] - 20,
		&sdata->gGT->backBuffer->primMem,
		sdata->gGT->pushBuffer_UI.ptrOT,
        TRANS_50_DECAL, 0x1000, 0x808080);
		
		struct RectMenu* m = sdata->ptrActiveMenu;
		

		// draw menu now because it is drawn
		// later, which puts it behind our background
		if (m != 0)
		{
			// clear width, then get width
			int width = 0;
			RECTMENU_GetWidth(m, &width, 1);

			// draw
			RECTMENU_DrawSelf(m, 0, 0, (int)width);
		}

		RECTMENU_DrawInnerRect(
			&timesRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
	}
}
