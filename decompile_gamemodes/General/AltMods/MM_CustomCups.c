#include <common.h>
#include "../../../mods/game_modes/Gamemode_Selection/src/utils.h"

// Transfer data from Single(SIN) menu to Cup(CUP) menu
#define CUP_TRACK(i, cup) data.ArcadeCups[cup].CupTrack[i].trackID
#define SIN_TRACK(i, cup) D230.arcadeTracks[cc->choiceX[cup][i]].levID
#define CUP_ICON(i, cup) data.ArcadeCups[cup].CupTrack[i].iconID
#define SIN_ICON(i, cup) D230.arcadeTracks[cc->choiceX[cup][i]].videoThumbnail

#define TOTAL_TRACKS 18
#define TOTAL_TRACKS_WITH_CUSTOM (TOTAL_TRACKS + CUSTOM_TRACKS_COUNT)

struct CustomCup
{
	char choiceX[4][4];

	char choiceY;
	char boolOpen;
};

struct CustomCup cc_static;
struct CustomCup* cc = &cc_static;

RECT windowSel[4] = 
{
	[0] = {0x100, 0x25, WIDE_PICK(0xE0, 0xC0), 0x4E}, // top right
	[1] = {0x20,  0x25, WIDE_PICK(0xE0, 0xC0), 0x4E}, // top left
	[2] = {0x100, 0x79, WIDE_PICK(0xE0, 0xC0), 0x4E}, // bottom right
	[3] = {0x20,  0x79, WIDE_PICK(0xE0, 0xC0), 0x4E}  // bottom left
};

RECT windowText = {0x40, 0, 0x180, 0x50};

static char CustomCups_boolRepeat(int numTracksSet, int currTrack, int cupIndex)
{
	int i;

	// check if track is taken
	// previously (<)
	// or by self (=)
	for (i = 0; i <= numTracksSet; i++)
		if (cc->choiceX[cupIndex][i] == currTrack)
			return 1;

	return 0;
}

// Helper function to get track name, handles both standard and custom tracks
const char* GetTrackName(int trackID)
{
	#ifdef USE_CUSTOM_TRACKS
	if (IS_CUSTOM_TRACK_ID(trackID)) {
		return GET_CUSTOM_TRACK_NAME(trackID);
	}
	#endif
	
	return sdata->lngStrings[trackID + 0x6e];
}

void CustomCups_MenuProc(struct RectMenu* menu)
{
	int i;
	unsigned int buttonTap;
	int id;
	
	int CUP_SELECT = D230.menuCupSelect.rowSelected;

	DECOMP_DecalFont_DrawLine(
		"PRESS SELECT FOR CUSTOM CUPS",
		0x100,
		0x4,
		FONT_SMALL,
		(JUSTIFY_CENTER | ORANGE));

	buttonTap = sdata->gGamepads->gamepad[0].buttonsTapped;

	if (buttonTap & BTN_SELECT)
	{
		cc->boolOpen = !cc->boolOpen;
		cc->choiceY = 0;
	}

	if (cc->boolOpen)
	{		
		// prevent cup from changing selection
		// while in the custom cups config menu
		sdata->AnyPlayerTap &= ~(0xF);
		sdata->buttonTapPerPlayer[0] &= ~(0xF);
		
		if (buttonTap & BTN_UP) cc->choiceY--;
		if (buttonTap & BTN_DOWN) cc->choiceY++;
		cc->choiceY &= 3;
		
		if (buttonTap & BTN_LEFT)
			cc->choiceX[CUP_SELECT][cc->choiceY]--;
		if (buttonTap & BTN_RIGHT)
			cc->choiceX[CUP_SELECT][cc->choiceY]++;

		// Update track limits to include custom tracks
		#ifdef USE_CUSTOM_TRACKS
		// pressing Left on Crash Cove goes to the last custom track,
		// pressing Right on the last custom track goes to Crash Cove
		if (cc->choiceX[CUP_SELECT][cc->choiceY] < 0)
			cc->choiceX[CUP_SELECT][cc->choiceY] = TOTAL_TRACKS_WITH_CUSTOM - 1;
		else
			cc->choiceX[CUP_SELECT][cc->choiceY] = 
			cc->choiceX[CUP_SELECT][cc->choiceY] % TOTAL_TRACKS_WITH_CUSTOM;
		#else
		// Original logic for standard tracks only
		if (cc->choiceX[CUP_SELECT][cc->choiceY] < 0)
			cc->choiceX[CUP_SELECT][cc->choiceY] = TOTAL_TRACKS - 1;
		else
			cc->choiceX[CUP_SELECT][cc->choiceY] = 
			cc->choiceX[CUP_SELECT][cc->choiceY] % TOTAL_TRACKS;
		#endif

		if ((buttonTap & BTN_R1) != 0)
		{
			i = 0;

			while (i < 4)
			{
				// random
				DECOMP_MixRNG_Scramble();

				#ifdef USE_CUSTOM_TRACKS
				id = (
						// system clock
						(DECOMP_Timer_GetTime_Total() & 0xf) +
		
						// from RNG
						(sdata->randomNumber >> 8)

					  ) % TOTAL_TRACKS_WITH_CUSTOM; // Include custom tracks
				#else
				id = (
						// system clock
						(DECOMP_Timer_GetTime_Total() & 0xf) +
		
						// from RNG
						(sdata->randomNumber >> 8)

					  ) % TOTAL_TRACKS; // Original tracks only
				#endif

				// avoid repeats
				if (CustomCups_boolRepeat(i, id, CUP_SELECT))
					continue;

				// set value
				cc->choiceX[CUP_SELECT][i] = id;
				i++;
			}
		}

		int startX;
		int startY;

		int flipSide = 
			(CUP_SELECT & ~(1)) |
			(!(CUP_SELECT & 1));

		// copied from CupSelect_MenuProc
		startX = (short)D230.transitionMeta_cupSel[flipSide].currX + (flipSide &1) * 200;
		startY = (short)D230.transitionMeta_cupSel[flipSide].currY + (flipSide>>1) * 0x54;
		
		// copied from CupSelect_MenuProc
        startX = startX + 0x4e -3-3;
        startY = startY + 0x29 -2-2;
		
		windowSel[CUP_SELECT].x = startX;
		windowSel[CUP_SELECT].y = startY;
		
		startX += 8;
		startY += 11;

		// Draw arrow
		DECOMP_DecalFont_DrawLine(
			"-\0",
			startX,
			startY + 0x10 * cc->choiceY,
			FONT_SMALL,
			ORANGE);

		// set values of all cups
		for (i = 0; i < 4; i++)
		{
			// Set trackID
			CUP_TRACK(i, CUP_SELECT) = SIN_TRACK(i, CUP_SELECT);

			// set iconID
			CUP_ICON(i, CUP_SELECT) = SIN_ICON(i, CUP_SELECT);

			#ifdef USE_CUSTOM_TRACKS
			// Draw name of track, handling custom tracks
			int trackID = CUP_TRACK(i, CUP_SELECT);
			DECOMP_DecalFont_DrawLine(
				GetTrackName(trackID),
				startX + 0x10,
				startY + 0x10 * i,
				FONT_SMALL,
				ORANGE);
			#else
			// Original code for standard tracks
			DECOMP_DecalFont_DrawLine(
				sdata->lngStrings[CUP_TRACK(i, CUP_SELECT) + 0x6e],
				startX + 0x10,
				startY + 0x10 * i,
				FONT_SMALL,
				ORANGE);
			#endif
		}

		windowText.y = 0x25;
		if(CUP_SELECT < 2)
			windowText.y = 0x79;

		DECOMP_DecalFont_DrawLine(
			"USE D-PAD TO CUSTOMIZE CUP",
			0x100,
			windowText.y + 0x18,
			FONT_SMALL,
			(JUSTIFY_CENTER | PERIWINKLE));

		DECOMP_DecalFont_DrawLine(
			"PRESS R1 TO RANDOMIZE",
			0x100,
			windowText.y + 0x30,
			FONT_SMALL,
			(JUSTIFY_CENTER | PERIWINKLE));

		int* ot = sdata->gGT->backBuffer->otMem.startPlusFour;

		DECOMP_RECTMENU_DrawInnerRect(
			&windowSel[CUP_SELECT], 1, ot);

		DECOMP_RECTMENU_DrawInnerRect(
			&windowText, 1, ot);
	}
}

void CustomCups_Init()
{
	// Start with default tracks,
	// each choiceX[x][y] is the index of D230.arcadeTracks
	// that matches data.ArcadeCups[x].CupTrack[y].trackID;
	cc->choiceX[0][0] = 0;
	cc->choiceX[0][1] = 2;
	cc->choiceX[0][2] = 5;
	cc->choiceX[0][3] = 3;
	cc->choiceX[1][0] = 1;
	cc->choiceX[1][1] = 7;
	cc->choiceX[1][2] = 9;
	cc->choiceX[1][3] = 6;
	cc->choiceX[2][0] = 4;
	cc->choiceX[2][1] = 8;
	cc->choiceX[2][2] = 11;
	cc->choiceX[2][3] = 12;
	cc->choiceX[3][0] = 10;
	cc->choiceX[3][1] = 14;
	cc->choiceX[3][2] = 13;
	cc->choiceX[3][3] = 16;
	
	// set values of all cups
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// Set trackID
			CUP_TRACK(i, j) = SIN_TRACK(i, j);
	
			// set iconID
			CUP_ICON(i, j) = SIN_ICON(i, j);
		}
	}

	cc->choiceY = 0;
	cc->boolOpen = 0;
}