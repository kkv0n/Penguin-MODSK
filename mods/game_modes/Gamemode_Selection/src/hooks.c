#include <common.h>
#include "utils.h"

//MOD MENU / UI / HUD
#include "ModMenu.c"
#include "custom_hud.c"

//Retro Fueled
// #include "old_retro_fueled.c"
#include "GameModes/RetroFueled/retro_fueled_bluefire.c"

//Shortcutless
#include "GameModes/Shortcutless/shortcutless.c"

//N_Verted
#include "GameModes/N-Verted/n_verted.c"
#include "GameModes/N-Verted/n_verted_driver_state.c"
#include "GameModes/N-Verted/n_verted_lap_skip_prevention.c"

//Mirror
#include "GameModes/MirrorMode/mirror.c"
#include "GameModes/MirrorMode/mirror_pad_remap.c"

//Stats Modifiers (Gravity, Speed, etc...)
#include "GameModes/stats_modifiers.c"

//MK Fueled
//#include "mk_fueled.c"

bool USE_RETRO_FUELED = false;
bool USE_SHORTCUTLESS = false;
bool USE_N_VERTED = false;
bool USE_MIRROR = false;
bool USE_MOON_GRAVITY = false;

// Default gravity value
short gravity = 900;

char* debugText = (char*)0x1F800000;

void RunUpdateHook() {
    struct GameTracker *gGT = sdata->gGT;
    struct Driver* driver = sdata->gGT->drivers[0];

    // Initialize gamemode settings once at the start of the race
    bool initialized = false;
    if (gGT->trafficLightsTimer > 0) {
        if (USE_SHORTCUTLESS) {
            initSkipPrevention();
            // Reset message timers
            for (int i = 0; i < 4; i++) {
                noShortcutMsgTimer[i] = 0;
                shortcutAttempts[i] = 0;
            }
        }

        if(USE_N_VERTED){
            initDriverStates();
			initLapSkipPrevention();
        }

        if(USE_MOON_GRAVITY){
            gravity = 369;
        } else {
            gravity = 900;
        }

        //Stats modifiers
        SetGravity(gravity);
        ApplyModifiers();

        initialized = true;
    }

    // Update retro fueled
    Update_Retro();

    // Remap pad if mirror
    Remap_Mirror();
    
    // Process each driver
    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
        struct Driver* driver = gGT->drivers[i];

        if (driver == NULL) continue;
        if (gGT->levelID > TURBO_TRACK) continue;
        
        if (USE_SHORTCUTLESS) {
            PreventSkip(driver, i);

            // Display "NO SHORTCUTS!" message if timer is active
            if (noShortcutMsgTimer[i] > 0) {
                if (gGT->numPlyrCurrGame == 1) {
                    int msgIndex = (shortcutAttempts[i] - 1) % NUM_SHORTCUT_MESSAGES;
                    sprintf(debugText, "%s", shortcutMessages[msgIndex]);
                    DecalFont_DrawLine(debugText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | RED));
                }
                noShortcutMsgTimer[i]--;
            }
        }

        if(USE_N_VERTED){
            // Update driver float state
            if (updateDriverFloatState(i, driver)) {
                // Float the driver
                driver->forcedJump_trampoline = 2;
                driver->jump_unknown = 0x180;
                driver->jump_InitialVelY = driver->const_JumpForce * 3;
                
                // Change tire color while floating
                driver->tireColor = 0x2e0c0cc2;
            } else {
                // Reset tire color when not floating
                driver->tireColor = 0x2e808080;
            }

			// Check for lap skips, if it happens, mask grab the player
			PreventLapSkip(driver, i);
        }
    }

    // Draw reserver metter for 1P
    if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0))
	{
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
	}

    // Handle mod menu input and rendering
    struct GamepadBuffer* controller = &sdata->gGamepads->gamepad[0];
    HandleMenuInput(controller);
    RenderMenu();

    // Draw version info on main menu
    if (D230.MM_State == 1) {
        DecalFont_DrawLine("MOD MASHUP v1.0.0", 5, 197, FONT_SMALL, LIME_GREEN);
        DecalFont_DrawLine(__DATE__, 5, 206, FONT_SMALL, ORANGE);
        DecalFont_DrawLine(__TIME__, 170, 206, FONT_SMALL, ORANGE);
    }

    // DrawDebugString();
}





