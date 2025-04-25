#include <common.h>
#include "../../tracks.h"

void CTR_Box_DrawWireBox(RECT* r, int* unk, u_long* ot, struct PrimMem* primMem);
extern u_char hudptr;

//apparently if you use [0] or just [] idk why but it gives you white fire
u_long redFlames_clut[8] __attribute__((section(".data"))) = { 0x80DA809A, 0x805A80BA, 0x811A803A, 0x819A81DA, 0x813A817A, 0x829A821A, 0x835A839A, 0x82DA831A };
u_long blueFlames_clut[8] __attribute__((section(".data"))) = { 0xFF25FF65, 0xFFA5FF45, 0xFEE5FFC5, 0xFE65FE25, 0xFEC5FE85, 0xFD65FDE5, 0xFCA5FC65, 0xFD25FCE5 };
u_long redPlumes_clut[8] __attribute__((section(".data"))) = { 0x000083FF, 0x821B833E, 0x829D81B8, 0x80F6800D, 0x819A8054, 0x80D98011, 0x80078019, 0x84008015 };
u_long bluePlumes_clut[8] __attribute__((section(".data"))) = { 0x7FFFF260, 0xFC20F980, 0xF400FC20, 0xF980AC00, 0xF260F980, 0xB400FCA0, 0x8800FC20, 0x8800FC20 };


RECT flames_pos __attribute__((section(".data"))) = {
	.x = 176,
	.y = 256,
	.w = 16,
	.h = 1
};

RECT plumes_pos __attribute__((section(".data"))) = {
	.x = 464,
	.y = 257,
	.w = 16,
	.h = 1
};

char blueFire = 0;


void DrawReservesMeter(short posX, short posY, struct Driver* d)
{
	u_int colorAndCode;
	short reservesMeter;
	struct DB* backDB;
	u_long* primmemCurr;
	POLY_F4* p;
	short bottomY;
	RECT box2;
	u_int auStack48[2];
	int reservesMeterWidth;

	reservesMeter = posY - 0x2d;

	backDB = sdata->gGT->backBuffer;
	primmemCurr = backDB->primMem.curr;
	p = 0;

	// if there is room left for more
	if (primmemCurr <= (u_long*)backDB->primMem.endMin100)
	{
		// add primitives
		backDB->primMem.curr = &primmemCurr[6];
		p = (POLY_F4*)primmemCurr;
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
		if (d->reserves > 1600)
		{
			if (d->reserves < 3840)
			{
				colorAndCode = 0x2800ffff; // yellow
			}
			else
			{
				colorAndCode = 0x2800ff00; // green
			}
		}
		*(u_int*)&p->r0 = colorAndCode;
		reservesMeterWidth = d->reserves * 0xE; //Meter maximum is 8000 reserves units
		bottomY = posY - 3;
		p->y0 = posY;
		p->y1 = posY;
		p->x1 = posX;
		p->y2 = bottomY;
		p->x3 = posX;
		p->y3 = bottomY;

		reservesMeter = posX - ((short)(reservesMeterWidth / 0x960));
		if (reservesMeter < (posX - 0x31))
		{
			reservesMeter = posX - 0x31;
			*(u_int*)&p->r0 = 0x28ff0000; // blue
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
		if (primmemCurr <= (u_long*)backDB->primMem.endMin100)
		{
			// Add more primitives
			backDB->primMem.curr = &primmemCurr[6];
			p = (POLY_F4*)primmemCurr;
		}

		if (p != 0)
		{
			*(u_int*)&p->r0 = 0x28808080;
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

void InjectBlueFire()
{
	if (!blueFire)
	{
		LoadImage(&flames_pos, blueFlames_clut);
		LoadImage(&plumes_pos, bluePlumes_clut);
		blueFire = 1;
	}
}

void InjectRedFire()
{
	if (blueFire)
	{
		LoadImage(&flames_pos, redFlames_clut);
		LoadImage(&plumes_pos, redPlumes_clut);
		blueFire = 0;
	}
}

void Update_Retro() {

	struct GameTracker* gGT;
	struct Driver* d;
	gGT = sdata->gGT;

#ifdef DEV_MODE
	if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0) && (selected_mod == RETRO_FUELED) && (hudptr != 0))
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
#else
	if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0) && (selected_mod == RETRO_FUELED))
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
#endif

	for (int i = 0; i < gGT->numPlyrCurrGame; i++)
	{



		d = gGT->drivers[i];

		if (selected_mod != RETRO_FUELED) {
			InjectRedFire();
			return;
		}

		if (d->reserves)
		{
			if (d->fireSpeedCap == 0x4800)
				InjectBlueFire();
			else
				InjectRedFire();
		}
	}
}