#include <common.h>

void CAM_StartOfRace(struct CameraDC* cDC)
{
  struct GameTracker* gGT = sdata->gGT;
  struct Level* level1 = gGT->level1;
  

	if(cDC != &gGT->cameraDC[0])
		return;

  cDC->cameraMode = 0;

  // remove bit & 0x4 (battle end of race) and 0x1000 (idk)
  cDC->flags &= 0xFFFFEFFB;
  return;
}
