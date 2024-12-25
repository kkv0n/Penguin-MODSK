#include <common.h>
#include "global.h"
#include "namesdata.c"


int GetOverheadLen(struct Driver* d)
{
	struct MyData* ptrDest = (struct MyData*)0x1f800108;
	ptrDest->World_posX = d->posCurr.x >> 8;
	ptrDest->World_posY = (d->posCurr.y >> 8) + 25;
	ptrDest->World_posZ = d->posCurr.z >> 8;
	ptrDest->World_posW = 0;

	gte_ldv0(&ptrDest->World_posX);

	gte_rtps();
	gte_stsxy(&ptrDest->Screen_posX);
	gte_stsz(&ptrDest->Screen_posZ);

	int posZ = ptrDest->Screen_posZ;

	if(posZ < 150) return 0;
	if(posZ < 190) return 6;
	if(posZ < 234) return 5;
	if(posZ < 331) return 4;
	if(posZ < 475) return 3;
	if(posZ < 906) return 2;
	return 0;
}