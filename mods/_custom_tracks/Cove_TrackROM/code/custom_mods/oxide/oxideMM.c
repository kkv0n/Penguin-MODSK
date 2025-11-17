#include <common.h>

bool oxide = false;

void Load_Oxide(struct Model* characterModel, char* ch)
{
	
	
	char currCharacter;
	
    RECT r;
	
	r.x = 350;
	r.y = 175;
	r.w = 0x34;
	r.h = 0x21;
	
	
	
	
	DecalHUD_DrawWeapon(

	sdata->gGT->ptrIcons[data.MetaDataCharacters[NITROS_OXIDE].iconID],

	(int)356, (int)180,


	&sdata->gGT->backBuffer->primMem,


	sdata->gGT->pushBuffer_UI.ptrOT,

	TRANS_50_DECAL, FP(1), 0);
	

	RECTMENU_DrawInnerRect(&r, 0, sdata->gGT->pushBuffer_UI.ptrOT);

	
	char charID = data.characterIDs[0];
	

	bool dontchange = (charID != FAKE_CRASH && charID != PAPU_PAPU && charID != PURA);

		unsigned int check = (charID == FAKE_CRASH) ? BTN_RIGHT : BTN_DOWN;
		
		if ((!dontchange) && ((sdata->gGamepads->gamepad[0].buttonsTapped & check) != 0))
		{
          oxide = true;
		  D230.characterSelect_MoveDir[0] = 0;
		  sdata->buttonTapPerPlayer[0] = 0;
		  OtherFX_Play(0, 1);
		}

    
    else if ((sdata->gGamepads->gamepad[0].buttonsTapped & MM_RESTORE) != 0)
        oxide = false;
        
    if (oxide)
    {
		// draw string
		DecalFont_DrawLine
		(
			sdata->lngStrings[data.MetaDataCharacters[NITROS_OXIDE].name_LNG_long],
			((int)D230.ptrTransitionMeta[FAKE_CRASH].currX + D230.characterSelect_ptrWindowXY[0] + (int)((u_int)D230.characterSelect_sizeX >> 1)),
			((int)D230.ptrTransitionMeta[FAKE_CRASH].currY + D230.characterSelect_ptrWindowXY[0*2+1] + D230.textPos), FONT_CREDITS, (JUSTIFY_CENTER | ORANGE)
		);
			
          currCharacter = NITROS_OXIDE;
          data.characterIDs[0] = currCharacter;
          D230.characterSelect_charIDs_curr[0] = currCharacter;
		  ch[0] = currCharacter;
    }

    

		  
    characterModel->headers[0].ptrAnimations[0] = 
    characterModel->headers[0].ptrAnimations[1];
	
	
};