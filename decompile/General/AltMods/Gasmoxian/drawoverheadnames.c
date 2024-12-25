#include <common.h>
#include "global.h"

extern int GetOverheadLen(struct Driver* d);

void DrawOverheadNames()
{
	int i;
	MATRIX* m;

	struct GameTracker* gGT = sdata->gGT;
	struct MyData* ptrDest = (struct MyData*)0x1f800108;

	// pushBuffer offset 0x28
	m = &gGT->pushBuffer[0].matrix_ViewProj;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);

	#if 0
	DrawOverheadCalibration();
	#endif

	// start from P2
	i = 1;
	int color = (JUSTIFY_CENTER | ORANGE);

	if((gGT->gameMode1 & START_OF_RACE) != 0)
	{
		// start from P1
		i = 0;
		color = (JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);

	}

	for(i; i < octr->NumDrivers; i++)
	{
		int len = GetOverheadLen(gGT->drivers[i]);
		if(len == 0) continue;

//mirror mode enabled


		// if mirror mode
		if(octr->special == 1)
		{
			ptrDest->Screen_posX =
			0x200 - ptrDest->Screen_posX;
		}



		DECOMP_DecalFont_DrawLineStrlen(
			octr->nameBuffer[i],
			len,
			ptrDest->Screen_posX,
			ptrDest->Screen_posY-0x4,
			FONT_SMALL, color);

		// all players except P1
		color = (JUSTIFY_CENTER | ORANGE);
	}
}