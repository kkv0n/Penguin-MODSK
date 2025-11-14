#include <common.h>

void  GAMEPAD_ProcessAnyoneVars(struct GamepadSystem* gGamepads)
{
  struct GamepadBuffer* pad;

  //process gamepads
  GAMEPAD_ProcessHold(gGamepads);
  GAMEPAD_ProcessSticks(gGamepads);
  GAMEPAD_ProcessTapRelease(gGamepads);
  

  // GAMEPAD_ProcessMotors(gGamepads);

  // These are used to see if any button is pressed by anyone
  // during this frame. Reset them all to zero
  gGamepads->anyoneHeldCurr = 0;
  gGamepads->anyoneTapped 	= 0;
  gGamepads->anyoneReleased = 0;
  gGamepads->anyoneHeldPrev = 0;


    //get gamepad
    pad = &gGamepads->gamepad[0];

    //update global system flag
    gGamepads->anyoneHeldCurr |= pad->buttonsHeldCurrFrame;
    gGamepads->anyoneTapped   |= pad->buttonsTapped;
    gGamepads->anyoneReleased |= pad->buttonsReleased;
    gGamepads->anyoneHeldPrev |= pad->buttonsHeldPrevFrame;

}