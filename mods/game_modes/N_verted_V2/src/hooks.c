#include <common.h>
#include "utils.h"
#include "driver_state.c"
#include "n_verted.c" //Allocate n_verted on rdata_free
#include "lap_skip_prevention.c"

// char* debugText = (char*)0x1F800000;

void RunUpdateHook(){
	struct GameTracker *gGT = sdata->gGT;

	// struct GamepadBuffer* pad = &sdata->gGamepads->gamepad[0];
	// struct Driver* driver = sdata->gGT->drivers[0];

	// Initialize the driver states and lap skip prevention system at the start of the race
    bool initialized = false;
    if (gGT->trafficLightsTimer > 0) {
        if(USE_N_VERTED){
            initDriverStates();
			initLapSkipPrevention();
        }
        initialized = true;
    }
    
    // Process each driver
    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
        struct Driver* driver = gGT->drivers[i];

        if (driver == NULL) continue;
        if (gGT->levelID > TURBO_TRACK) continue;
        
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

	//Draw float timer
	// if ((gGT->numPlyrCurrGame > 0) && ((gGT->gameMode1 & END_OF_RACE) == 0) && gGT->levelID != MAIN_MENU_LEVEL)
	// {
	// 	unsigned char maxFrames = 0;
	// 	short currentLevelID = gGT->levelID;
	// 	JumpBlock* blocksArray = NULL;
	// 	short blockCount = 0;
		
	// 	getLevelBlocksData(currentLevelID, &blocksArray, &blockCount);
		
	// 	if (blocksArray != NULL) {
	// 		for (short i = 0; i < blockCount; i++) {
	// 			if (FPS_DOUBLE(blocksArray[i].frameCount) > maxFrames) {
	// 				maxFrames = FPS_DOUBLE(blocksArray[i].frameCount);
	// 			}
	// 		}
	// 	}
		
	// 	// Only draw if there's a float timer active
	// 	//this cant be done with hud1P parameter for all players, it should be changed to work in all players hud
	// 	if (driverState[0].frameTimer > 0 && gGT->numPlyrCurrGame == 1) {
	// 		DrawFloatTimer(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, driverState[0].frameTimer, maxFrames);
	// 	}
	// }

    //Draw version info on main menu
    if (D230.MM_State == 1){
        DecalFont_DrawLine("CTR N-VERTED - V2.0.0", 5, 197, FONT_SMALL, LIME_GREEN);
        DecalFont_DrawLine(__DATE__, 5, 206, FONT_SMALL, ORANGE);
        DecalFont_DrawLine(__TIME__, 170, 206, FONT_SMALL, ORANGE);
    }

	// DEBUG STUFF -------------------------------
		
	// if (gGT->levelID != MAIN_MENU_LEVEL && gGT->levelID != ADVENTURE_GARAGE && gGT->levelID != NAUGHTY_DOG_CRATE)
	// {
		// sprintf(debugText, "BlockID: %d", driver->currBlockTouching->blockID);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 - 60, FONT_SMALL, (JUSTIFY_CENTER | RED));

		// sprintf(debugText, "progress: %d", driver->distanceToFinish_curr);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 - 50, FONT_SMALL, (JUSTIFY_CENTER | RED));
		
		// sprintf(debugText, "checkpoint: %d", driver->distanceToFinish_checkpoint);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 - 40, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
		
		// sprintf(debugText, "reverse: %d", driver->distanceDrivenBackwards);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 -30, FONT_SMALL, (JUSTIFY_CENTER | CRASH_BLUE));

		// sprintf(debugText, "check id: %d", (unsigned char) driver->underDriver->checkpointIndex);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 -20, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

		// sprintf(debugText, "check cnt: %d", gGT->level1->cnt_restart_points);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 -10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

		// sprintf(debugText, "lastValid: %d", driver->lastValid->checkpointIndex);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

		// struct CheckpointNode* nodes = gGT->level1->ptr_restart_points;
		// unsigned int dist = nodes[driver->underDriver->checkpointIndex].distToFinish;
		// unsigned int forward_index = nodes[driver->underDriver->checkpointIndex].nextIndex_forward;
		// unsigned int backward_index = nodes[driver->underDriver->checkpointIndex].nextIndex_backward;

		// sprintf(debugText, "dist: %d", dist);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 - 10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

		// sprintf(debugText, "forward id: %d", forward_index);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

		// sprintf(debugText, "backward id: %d", backward_index);
		// DecalFont_DrawLine(debugText, 0x100, 0xc8 + 10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

		//Force mask Grab on L1 + Triangle
		// if (((pad->buttonsHeldCurrFrame & BTN_L1) != 0) && ((pad->buttonsHeldCurrFrame & BTN_TRIANGLE) != 0)){
		// 	// VehStuckProc_MaskGrab_Update((struct Thread*)driver, driver);
		// 	// VehStuckProc_MaskGrab_Animate((struct Thread*)driver, driver);
		// 	// VehStuckProc_MaskGrab_Init((struct Thread*)driver, driver);

		// 	MaskGrab((struct Thread*)driver, driver);
		// }

		//Fly cheat
		// if (((pad->buttonsHeldCurrFrame & BTN_L1) != 0) && ((pad->buttonsHeldCurrFrame & BTN_TRIANGLE) != 0))
		// {
		// 	pad->buttonsHeldCurrFrame |= BTN_CROSS;
		// 	driver->forcedJump_trampoline = 2;
		// 	driver->jump_unknown = 0x180;
		// 	driver->jump_InitialVelY = driver->const_JumpForce * 3;
		// }
	// }
}