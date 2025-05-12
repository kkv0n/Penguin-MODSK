#include <common.h>
#include "utils.h"
#include "skip_prevention.c"

char* debugText = (char*)0x1F800000;

// Track how many times each player attempts shortcuts
int shortcutAttempts[4] = {0, 0, 0, 0};

// Array of progressively more frustrated messages
const char* shortcutMessages[] = {
    "NO SHORTCUTS!",
    "SERIOUSLY, NO SHORTCUTS!",
    "HEY! DID YOU NOT READ THE RULES?",
    "CHEATERS NEVER PROSPER",
    "I SAW THAT",
    "WHAT PART OF 'NO SHORTCUTS' IS UNCLEAR?",
    "OKAY, NOW YOU'RE JUST MESSING WITH ME",
    "THIS IS GETTING RIDICULOUS...",
    "FINE, KEEP TRYING. IT WON'T WORK",
    "YOU'RE VERY PERSISTENT, AREN'T YOU?",
	"YOU COULD'VE FINISHED THE RACE BY NOW"
};

#define NUM_SHORTCUT_MESSAGES (sizeof(shortcutMessages) / sizeof(shortcutMessages[0]))

void RunUpdateHook(){
	struct GameTracker *gGT = sdata->gGT;

	// struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[0];
	struct Driver* driver = sdata->gGT->drivers[0];

	// Initialize the driver states and lap skip prevention system at the start of the race
    bool initialized = false;
    if (gGT->trafficLightsTimer > 0) {
        if(USE_SHORTCUTLESS){
			initSkipPrevention();
			// Reset message timers
			for (int i = 0; i < 4; i++) {
				noShortcutMsgTimer[i] = 0;
				shortcutAttempts[i] = 0;
			}
        }
        initialized = true;
    }
    
    // Process each driver
    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
        struct Driver* driver = gGT->drivers[i];

        if (driver == NULL) continue;
        if (gGT->levelID > TURBO_TRACK) continue;
        
		if(USE_SHORTCUTLESS){
            PreventSkip(driver, i);

            // Display "NO SHORTCUTS!" message if timer is active
            if (noShortcutMsgTimer[i] > 0) {
                if(gGT->numPlyrCurrGame == 1){
                    int msgIndex = (shortcutAttempts[i] - 1) % NUM_SHORTCUT_MESSAGES;
                    sprintf(debugText, "%s", shortcutMessages[msgIndex]);
                    DecalFont_DrawLine(debugText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | RED));
                }
                noShortcutMsgTimer[i]--;
            }
        }
    }

    //Draw version info on main menu
    if (D230.MM_State == 1){
        DecalFont_DrawLine("SHORTCUTLESS v1.0.0", 5, 197, FONT_SMALL, LIME_GREEN);
        DecalFont_DrawLine(__DATE__, 5, 206, FONT_SMALL, ORANGE);
        DecalFont_DrawLine(__TIME__, 170, 206, FONT_SMALL, ORANGE);
    }

	// if (gGT->levelID != MAIN_MENU_LEVEL && gGT->levelID != ADVENTURE_GARAGE && gGT->levelID != NAUGHTY_DOG_CRATE)
	// {
	// 	sprintf(debugText, "BlockID: %d", driver->currBlockTouching->blockID);
	// 	DecalFont_DrawLine(debugText, 0x100, 0xc8 - 60, FONT_SMALL, (JUSTIFY_CENTER | RED));

	// 	sprintf(debugText, "check id: %d", (unsigned char) driver->underDriver->checkpointIndex);
	// 	DecalFont_DrawLine(debugText, 0x100, 0xc8 -20, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

	// 	sprintf(debugText, "lastValid: %d", driver->lastValid->checkpointIndex);
	// 	DecalFont_DrawLine(debugText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
	// }
}