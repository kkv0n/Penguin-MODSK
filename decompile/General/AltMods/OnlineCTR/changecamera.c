#include <common.h>
#include "global.h"

extern int currCam;
void SetNextCamera(bool increase);
extern void spec_text();
extern int shouldExecuteSpecText;

	extern const char s_switchCam[];
    extern const char s_gg[];
	const char* s_text;
	
void EndOfRace_Camera()
{
//change l1 & r1 text
s_text = shouldExecuteSpecText ? s_switchCam : s_gg;
	DECOMP_DecalFont_DrawLine(s_text, 0x100, 5, FONT_SMALL, JUSTIFY_CENTER | ORANGE);

//change spectator text
	 spec_text();

    

	DECOMP_DecalFont_DrawLine(octr->nameBuffer[currCam], 252, 195, FONT_BIG, JUSTIFY_CENTER | TINY_GREEN);
	struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[0];
	if (pad->buttonsTapped & BTN_R1) { SetNextCamera(true); }
	if (pad->buttonsTapped & BTN_L1) { SetNextCamera(false); }
}