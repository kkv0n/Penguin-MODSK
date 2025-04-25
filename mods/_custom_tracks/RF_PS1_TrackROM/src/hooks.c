#include <common.h>
#include "utils.h"
#include "retro.c"

extern char* track_name;
unsigned char CUSTOM_LEVEL_ID;
u_char showstars;

struct MenuRow NewRowsPAUSE[5] =
{
	[0] =
	{
		.stringIndex = 2, // resume
		.rowOnPressUp = 0,
		.rowOnPressDown = 1,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},

	[1] =
	{
		.stringIndex = 1, // restart
		.rowOnPressUp = 0,
		.rowOnPressDown = 2,
		.rowOnPressLeft = 1,
		.rowOnPressRight = 1,
	},

	[2] =
	{
		.stringIndex = 3, // quit
		.rowOnPressUp = 1,
		.rowOnPressDown = 3,
		.rowOnPressLeft = 2,
		.rowOnPressRight = 2,
	},

	[3] =
	{
		.stringIndex = 0xE, // options
		.rowOnPressUp = 2,
		.rowOnPressDown = 3,
		.rowOnPressLeft = 3,
		.rowOnPressRight = 3,
	},

	[4] =
	{
		.stringIndex = 0xFFFF,
	}
};

static int once = 1;

void RunInitHook()
{
	struct GameTracker* gGT = sdata->gGT;
	
	// Extend bit range of quadblockID,
	// upper bits are never used, but still
	// need AND for alignment with LW instruction
	*(unsigned short*)0x800a0f18 = 0xFFFC;
	*(unsigned short*)0x800a1e80 = 0xFFFC;
	*(unsigned short*)0x800a36d8 = 0xFFFC;
	*(unsigned short*)0x800a4fd0 = 0xFFFC;
	*(unsigned short*)0x800a6f70 = 0xFFFC;
	*(unsigned short*)0x800a8b90 = 0xFFFC;

	// set arcade/TT menu to use adv cup rows
	*(unsigned int*)(0x80084510 + 0xC) = NewRowsPAUSE;

	// required for AI Nav, cause I dont have
	// offsets [0xA] or [0xC] and it gets stuck
	// in a loop, so this breaks the loop
	*(int*)0x800150c0 = 0;

	// wont clear itself?
	sdata->ptrLoadSaveObj = 0;
	
		if (once)
	{
		once = 0;

		char* currDriver = 0x80290000;
		for (int i = 0; i < 15; i++) // load every character except oxide. oxide will come with time trial pack
		{
			int fileSize;
			LOAD_ReadFile(sdata->ptrBigfile1, 2, BI_RACERMODELHI + i, currDriver, &fileSize, 0);
			int* pMap = (int*) (currDriver + 4 + (*(int*)currDriver));
			LOAD_RunPtrMap(currDriver + 4, pMap + 1, *pMap >> 2);
			struct Model** g_charModelPtrs = CHAR_MODEL_PTRS;
			g_charModelPtrs[i] = (struct Model*) (currDriver + 4);
			currDriver += fileSize;
		}
	}


	if(gGT->levelID != CUSTOM_LEVEL_ID) return;

	sdata->ptrActiveMenu = 0;
}

struct MenuRow NewRowsMM[2] =
{
	[0] =
	{
		.stringIndex = 379, // custom name string
		.rowOnPressUp = 0,
		.rowOnPressDown = 0,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	},

	[1] =
	{
		.stringIndex = 0xFFFF,
	}
};

struct MenuRow NewRowsEND[] =
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

void RunUpdateHook()
{
	struct GameTracker* gGT = sdata->gGT;
	
	//add retrofueled + reserves meter
	void reserves_plusbluefire();
	reserves_plusbluefire();
	
if (sdata->gGT->levelID == CUSTOM_LEVEL_ID && showstars != 0)
{
sdata->gGT->stars.numStars = 768;
sdata->gGT->stars.spread = 1;
sdata->gGT->stars.seed = 65535;
sdata->gGT->stars.distance = 1022;
}


	// main menu
	if (sdata->ptrActiveMenu == &D230.menuMainMenu)
	{
		sdata->ptrActiveMenu->rows = &NewRowsMM[0];
		sdata->lngStrings[379] = track_name;
	}

	// time trial end of race
	if ((sdata->ptrActiveMenu == 0x800a0458) || (sdata->ptrActiveMenu == 0x800A04A4))
	{
		sdata->ptrActiveMenu->rows = &NewRowsEND[0];
	}

	if (gGT->levelID != CUSTOM_LEVEL_ID) return;
}
