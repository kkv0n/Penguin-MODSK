#include <common.h>
#include "../header/cove_rom.h"

void MM_Characters_DrawWindows(int boolShowDrivers)
{
  struct GameTracker* gGT;
  struct Model* characterModel;
  int iVar5;
  unsigned int directionMoving;
  int iVar8;
  short sVar9;
  struct Instance* characterInstance;
  short *psVar11;
  struct PushBuffer* pb;
  char currCharacter;
  unsigned char i;
  struct TransitionMeta* tMeta;
  SVec3 rot;
  unsigned char j;
  
  gGT = sdata->gGT;

  if (boolShowDrivers != 0) {
    // enable drawing wheels
	gGT->renderFlags |= 0x80;
  }

  for (i = 0; i < gGT->numPlyrNextGame; i++)
  {
    psVar11 = &D230.characterSelect_ptrWindowXY[i*2];
    tMeta = &D230.ptrTransitionMeta[i];
  
    pb = &gGT->pushBuffer[i];
    pb->rect.x = psVar11[0] + tMeta[0x10].currX;
    pb->rect.y = psVar11[1] + tMeta[0x10].currY;
    pb->rect.w = D230.characterSelect_sizeX;
    pb->rect.h = D230.characterSelect_sizeY;
  
    // negative StartX
    if ((short)pb->rect.x < 0) {
      pb->rect.w -= pb->rect.x;
      pb->rect.x = 0;
      if ((short)pb->rect.w < 0) {
        pb->rect.w = 0;
      }
    }
  
    // negative StartY
    if ((short)pb->rect.y < 0) {
      pb->rect.h -= pb->rect.y;
      pb->rect.y = 0;
      if ((short)pb->rect.h < 0) {
        pb->rect.h = 0;
      }
    }
  
    // startX + sizeX out of bounds
    if (
            (0x200 < pb->rect.x + pb->rect.w) &&
            (
                pb->rect.w = 0x200 - pb->rect.x,
                pb->rect.w < 0
            )
        )
    {
      pb->rect.x = 0x200;
      pb->rect.w = 0;


    }
  
    // startY + sizeY out of bounds
    if (
            (0xd8 < pb->rect.y + pb->rect.h) &&
            (
                pb->rect.h = 0xd8 - pb->rect.y,
                pb->rect.h < 0
            )
        ) 
    {
      pb->rect.y = 0xd8;
      pb->rect.h = 0;


    }
  
    // distanceToScreen
    pb->distanceToScreen_CURR = 0x100;
    pb->distanceToScreen_PREV = 0x100;
  
    // pushBuffer pos and rot to all zero
    pb->pos[0] = 0;
    pb->pos[1] = 0;
    pb->pos[2] = 0;
    pb->rot[0] = 0;
    pb->rot[1] = 0;
    pb->rot[2] = 0;
  
    // player -> instance
    characterInstance = gGT->drivers[i]->instSelf;
  
    // Make Visible
    characterInstance->flags &= 0xffffff7f;
  
    // if driver is off-screen
    if (
			
			(boolShowDrivers == 0)
		) 
    {
      // invisible
      characterInstance->flags |= 0x80;
    }
  


	struct InstDrawPerPlayer* idpp = INST_GETIDPP(characterInstance);
  
    // clear pushBuffer in every InstDrawPerPlayer
    for( j = 0; j < 4; j++)
    {
        idpp[j].pushBuffer = 0;
    }
  
    // set pushBuffer in InstDrawPerPlayer,
    // so that each camera can only see one driver
    idpp[i].pushBuffer = pb;

    currCharacter = D230.characterSelect_charIDs_curr[i];

    characterInstance->animFrame = 0;
    characterInstance->vertSplit = 0;



	//Custom character fix for main menu
	characterModel = (struct Model*)ptrCharacterModels[currCharacter];
  



	gGT->drivers[i]->wheelSize = 0xccc;
    
    //check oxide
    Load_Oxide(characterModel, &currCharacter);
    
    

    
	if(currCharacter == NITROS_OXIDE)
		gGT->drivers[i]->wheelSize = 0;

  
    // set modelPtr in Instance
    characterInstance->model = characterModel;
    

	
    // CameraDC, freecam mode
    gGT->cameraDC[i].cameraMode = 3;
	
    // Set position of player
    characterInstance->matrix.t[0] = D230.csm_instPos[0];
    characterInstance->matrix.t[1] = D230.csm_instPos[1];
    characterInstance->matrix.t[2] = D230.csm_instPos[2];
  
    psVar11 = &D230.timerPerPlayer[i];
    sVar9 = psVar11[0] - 1;
  
    // If no transition between players
    if (psVar11[0] == 0) {
      // compare to character ID
      if (currCharacter != data.characterIDs[i]) {
        psVar11[0] = D230.moveModels << 1;
        D230.characterSelect_charIDs_desired[i] = data.characterIDs[i];
      }
    }
  
    // if transition between players
    else {
      // get timer
      psVar11[0] = sVar9;
  
      // if timer is before midpoint
      if ((int)sVar9 < (int)D230.moveModels) 
	  {
        // make driver fly off screen
        D230.characterSelect_charIDs_curr[i] = D230.characterSelect_charIDs_desired[i];
        
        currCharacter = D230.characterSelect_charIDs_curr[i];
        
        iVar5 =  RaceFlag_MoveModels((int)sVar9, (int)D230.moveModels);
  
        // direction moving
        directionMoving = -D230.characterSelect_MoveDir[i];
        iVar8 = iVar5 * D230.unkCharacterWindows >> 0xc;
      }
  
      // if timer is after midpoint
      else {
        // make new driver fly on screen
        iVar5 =  RaceFlag_MoveModels((int)sVar9 - (int)D230.moveModels, (int)D230.moveModels);
  
        // direction moving
        iVar8 = D230.characterSelect_MoveDir[i];
        directionMoving = (0x1000 - iVar5) * (int)D230.unkCharacterWindows >> 0xc;
      }

      characterInstance->matrix.t[0] += directionMoving * iVar8;
    }
  
    // driver rotation
    rot.x = D230.csm_instRot[0];
    rot.y = D230.csm_instRot[1] + D230.characterSelect_angle[i];
    rot.z = D230.csm_instRot[2];


    ConvertRotToMatrix(&characterInstance->matrix, &rot);
  }
  return;
}