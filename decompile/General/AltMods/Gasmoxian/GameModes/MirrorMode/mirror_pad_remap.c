#include <common.h>
#include "../../utils.h"

char normal[] = {BTN_LEFT, BTN_RIGHT};
char swap[] = {BTN_RIGHT, BTN_LEFT};

void SwapDirection(u_int toggle)
{
	for (char i = 0; i < 2; i++)
		data.gamepadMapBtn[i + 2].output = (toggle) ? swap[i] : normal[i];

	// Swap analog stick if mirror mode is enabled
    if (toggle)
    {
        struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[0];
        
        // Save the original X value
        short originalX = pad->stickLX;
        
        // Mirror the X axis (0 becomes 255, 255 becomes 0, 128 stays at 128)
        pad->stickLX = 0xFF - originalX;
        pad->stickLX_dontUse1 = 0xFF - pad->stickLX_dontUse1;
        pad->stickRX = 0xFF - pad->stickRX;
    }
}

void HandleMirrorInput()
{
	if ((sdata->gGT->gameMode1 & (PAUSE_1 | END_OF_RACE | MAIN_MENU)) == 0){

		SwapDirection(USE_MIRROR);
	}
	else{
		SwapDirection(false);
	}
}

// Restore original button mapping
void RestoreDpadMapping()
{
	for (char i = 0; i < 2; i++)
		data.gamepadMapBtn[i + 2].output = normal[i];
}