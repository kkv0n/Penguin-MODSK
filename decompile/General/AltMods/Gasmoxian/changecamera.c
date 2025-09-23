#include <common.h>
#include "global.h"
#include "utils.h"

extern int currCam;
extern void SetNextCamera(bool increase);
extern void spec_text();
extern int shouldExecuteSpecText;
static bool DemoCameraSpectator = false;

extern char* s_switchCam[];
extern char* s_gg[];
const char* s_text;
	
void EndOfRace_Camera()
{


//change spectator text
	 spec_text();

//change l1 & r1 text
s_text = shouldExecuteSpecText ? s_switchCam[gmoxLngIndex] : s_gg[gmoxLngIndex];
	DECOMP_DecalFont_DrawLine(s_text, 0x100, 5, FONT_SMALL, JUSTIFY_CENTER | ORANGE);
    

	DECOMP_DecalFont_DrawLine(octr->nameBuffer[currCam], 252, 195, FONT_BIG, JUSTIFY_CENTER | TINY_GREEN);
	struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[0];

	if(!USE_FIRST_PERSON && !USE_N_VERTED){
		// Toggle demo camera with triangle
		extern char* decalText;
		
		char* text[3] = {
			"PRESS ^ TO TOGGLE DEMO CAMERA",
			"PRESIONA ^ PARA ALTERNAR CAM DEMO",
			"PRESSIONE ^ PARA ALTERNAR CAM DEMO"			
		};
		



		sprintf(decalText, text[gmoxLngIndex]);
		DECOMP_DecalFont_DrawLine(decalText, 0x100, 13, FONT_SMALL, JUSTIFY_CENTER | ORANGE);
		if (pad->buttonsTapped & BTN_TRIANGLE) {
			DemoCameraSpectator = !DemoCameraSpectator;
		}
	
		if(DemoCameraSpectator){
			gGT->cameraDC[0].flags |= 0x20;
			gGT->cameraDC[0].flags |= 0x1000;
		} else {
			gGT->cameraDC[0].flags &= ~0x20;
			gGT->cameraDC[0].flags &= ~0x1000;
			gGT->cameraDC[0].cameraMode = 0;
		}
	}

	if (pad->buttonsTapped & BTN_R1) { SetNextCamera(true); }
	if (pad->buttonsTapped & BTN_L1) { SetNextCamera(false); }
}