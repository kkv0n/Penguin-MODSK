#include <common.h>

//just proccess player 1 (human)
void GAMEPAD_ProcessTapRelease(struct GamepadSystem *gGamepads)
{
	void GAMEPAD_CUSTOM_INPUT(struct GamepadBuffer *pad, bool isBot);
	GAMEPAD_CUSTOM_INPUT(&gGamepads->gamepad[0], false);
	return;
}