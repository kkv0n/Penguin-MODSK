#include <common.h>
#include "global.h"

// forward declaration
extern int currCam;
void SetNextCamera(bool increase);
extern void spec_text();
extern int shouldExecuteSpecText;

void EndOfRace_Camera()
{
    const char s_switchCam[] = "Press R1 or L1 to change the camera";

	DECOMP_DecalFont_DrawLine(s_switchCam, 0x100, 5, FONT_SMALL, JUSTIFY_CENTER | ORANGE);
	shouldExecuteSpecText = 1;
    spec_text();

	DECOMP_DecalFont_DrawLine(octr->nameBuffer[currCam], 252, 195, FONT_BIG, JUSTIFY_CENTER | TINY_GREEN);
	struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[0];
	if (pad->buttonsTapped & BTN_R1) { SetNextCamera(true); }
	if (pad->buttonsTapped & BTN_L1) { SetNextCamera(false); }
}
