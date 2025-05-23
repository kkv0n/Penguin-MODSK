#include "common.h"
#include "utils.h"

void SetGravity(short gravity) { 
	for (int i = 0; i < 4; i++) {
		data.metaPhys[0].value[i] = gravity; 
	}
}

void ApplyModifiers(){
	struct GameTracker *gGT = sdata->gGT;
	for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
		struct Driver* driver = gGT->drivers[i];

		if (driver == NULL) continue;
		if (gGT->levelID > TURBO_TRACK) continue;

		VehBirth_SetConsts(driver);
	}
}

