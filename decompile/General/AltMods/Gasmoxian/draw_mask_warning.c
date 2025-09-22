#include "common.h"
#include "global.h"
#include "utils.h"

extern int SquaredDistanceDrivers(struct Driver* d1, struct Driver* d2);
extern void CrossDotProductsDriversPos(struct Driver* d1, struct Driver* d2, int* crossProduct, int* dotProduct);

void HandleMaskWarning(){
	struct Driver* localDriver = sdata->gGT->drivers[0];

	//If local player is last place, skip
	int lastplaceRank = activeDriversCount - 1;
	if(localDriver->driverRank  == lastplaceRank)
		return;

	// Get driver that its behind local
	struct Driver* behindDriver = NULL;
	if(localDriver->driverRank < lastplaceRank)
		behindDriver = sdata->gGT->drivers[localDriver->driverRank + 1];

	if(behindDriver == NULL)
		return;

	// If driver behind is not using mask, skip 
	bool usingMask = (behindDriver->actionsFlagSet & 0x00800000) != 0 && behindDriver->kartState != KS_MASK_GRABBED && behindDriver->kartState != KS_ENGINE_REVVING;
	if(!usingMask)
		return;

	// Get distance between local and behind driver
	// If cose enough and driver behind using a mask, show warning
	int playerDistance = SquaredDistanceDrivers(localDriver, behindDriver);
	if (playerDistance > 0x90000 * 5)
		return;

	// Determine if the driver is approaching from left or right
	int crossProduct, dotProduct;
	CrossDotProductsDriversPos(localDriver, behindDriver, &crossProduct, &dotProduct);

	// Only continue if behind us
	if (dotProduct >= 0) return;
	
	int screenCenter = 0x100;
	int screenWidth = 0x200;
	
	// This determines how far left/right the indicator appears
	int offset = (crossProduct) / 0x5000;
	int maxOffset = 100;
	if (offset > maxOffset) offset = maxOffset;
	if (offset < -maxOffset) offset = -maxOffset;

	// Map offset to screen position
	int warningX = screenCenter + ((offset * screenWidth) / 300);

	// If mirror mode xor looking backwards, invert left/right
	bool lookingBackwards = gGT->cameraDC[0].flags & 0x10000;
	if (USE_MIRROR ^ lookingBackwards)
		warningX = screenWidth - warningX;

	// Warning text
	int warningColor = (gGT->timer & FPS_DOUBLE(2)) != 0 ? PAPU_YELLOW : CORTEX_RED;
	char* Wtext[3] = {
	"MASK",
	"MASCARA",
	"MÁSCARA"

	};
	

		unsigned char desired_index = (unsigned char)sdata->unused_8008d700;
		
		if (sdata->unused_8008d700 > 1)
		{
			if (sdata->unused_8008d700 == 3)
			{
			    desired_index = 2;
			}
			else
			{
				desired_index = 0;
			}
		}


		char* warningText = Wtext[desired_index];
	
	int posY = 0x90;
	// Draw the warning
	DECOMP_DecalFont_DrawLine(warningText, warningX, posY, FONT_SMALL, JUSTIFY_CENTER | warningColor);
	int iconIndex = DECOMP_VehPickupItem_MaskBoolGoodGuy(behindDriver) ? AKUAKU_ICON : UKAUKA_ICON;
	DECOMP_DecalHUD_DrawWeapon(
		// pointer to icon, from array of icon pointers
		sdata->gGT->ptrIcons[iconIndex],
		warningX - 10, posY - 20,
		// PrimMem
		&sdata->gGT->backBuffer->primMem,
		// OTMem
		sdata->gGT->pushBuffer_UI.ptrOT,
		TRANS_50_DECAL,
		FP(0.6),
		1
	);
}
