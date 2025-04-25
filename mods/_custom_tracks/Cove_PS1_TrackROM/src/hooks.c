#include <common.h>
#include "utils.h"


void CTR_Box_DrawWireBox(RECT* r, int* unk, u_long* ot, struct PrimMem* primMem);

extern char* track_name;
u_char reserves_bar;
u_char showstars;
unsigned char CUSTOM_LEVEL_ID;

void DrawReservesMeter(short posX, short posY, struct Driver* d)
{
	if (reserves_bar == 0) return;
	
	u_int colorAndCode;
	short reservesMeter;
	struct DB* backDB;
	u_long *primmemCurr;
	POLY_F4 *p;
	short bottomY;
	RECT box2;
	u_int auStack48 [2];
	int reservesMeterWidth;

	reservesMeter = posY - 0x2d;

	backDB = sdata->gGT->backBuffer;
	primmemCurr = backDB->primMem.curr;
	p = 0;

	// if there is room left for more
	if (primmemCurr <= (u_long *)backDB->primMem.endMin100)
	{
		// add primitives
		backDB->primMem.curr = &primmemCurr[6];
		p = (POLY_F4 *)primmemCurr;
	}

	if (p != 0)
	{
		box2.y = posY - 3;
		box2.w = 0x31;
		box2.h = 3;
		box2.x = posX - 0x31;
		memset(auStack48, 0, 4);

		CTR_Box_DrawWireBox(&box2, auStack48, sdata->gGT->pushBuffer_UI.ptrOT, &sdata->gGT->backBuffer->primMem);

		colorAndCode = 0x280000ff; // red
		
		
		if (d->reserves >= 32766 || d->reserves < 0)
		{
			d->reserves = 32766;
		}
		
		if (d->reserves > 3600)
		{
			if (d->reserves < 6840)
			{
				colorAndCode = 0x2800ffff; // yellow
			}
			else
			{
				colorAndCode = 0x2800ff00; // green
			}
		}
		*(u_int *)&p->r0 = colorAndCode;
		reservesMeterWidth = d->reserves * 0x4; //Meter maximum is 8000 reserves units
		bottomY = posY - 3;
		p->y0 = posY;
		p->y1 = posY;
		p->x1 = posX;
		p->y2 = bottomY;
		p->x3 = posX;
		p->y3 = bottomY;
		
		reservesMeter = posX - ((short)(reservesMeterWidth / 0x960));
		if(reservesMeter < (posX - 0x31))
		{
			reservesMeter = posX - 0x31;
			*(u_int *)&p->r0 = 0x28ff0000; // blue
		}
		p->x0 = reservesMeter;
		p->x2 = reservesMeter;

		primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

		*(int*)p = *primmemCurr | 0x5000000;
		*primmemCurr = (u_int)p & 0xffffff;

		backDB = sdata->gGT->backBuffer;
		primmemCurr = backDB->primMem.curr;
		p = 0;

		// If there is room to add more
		if (primmemCurr <= (u_long *)backDB->primMem.endMin100)
		{
			// Add more primitives
			backDB->primMem.curr = &primmemCurr[6];
			p = (POLY_F4 *)primmemCurr;
		}

		if (p != 0)
		{
			*(u_int *)&p->r0 = 0x28808080;
			p->y0 = posY;
			p->y1 = posY;
			p->x1 = posX;
			p->y2 = bottomY;
			p->x3 = posX;
			p->y3 = bottomY;
			p->x0 = posX - 0x31;
			p->x2 = posX - 0x31;

			// pointer to OT memory
			primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

			*(int*)p = *primmemCurr | 0x5000000;
			*primmemCurr = (u_int)p & 0xffffff;
		}
	}

	return;
}

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
		.stringIndex = 379, // time trial
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
		.rowOnPressDown = 2,
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
	
		if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0))
		{
	DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
		}



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
