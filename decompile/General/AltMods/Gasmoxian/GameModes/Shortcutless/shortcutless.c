// Shortcutless by Anfrost

#include <common.h>
#include "../../utils.h"

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

// Helper function for absolute value
int abs_val(int value) { return value < 0 ? -value : value; }

// Track previous quadblocks for each player
struct QuadBlock* lastValid_prev[4] = {NULL, NULL, NULL, NULL};
struct QuadBlock* prevTouchedBlock[4] = {NULL, NULL, NULL, NULL};

// Timer for showing "NO SHORTCUTS!" message (in frames)
int noShortcutMsgTimer[4] = {0, 0, 0, 0};

void MaskGrab(struct Thread* t, struct Driver* d) {

    //if mask grab its already going then quit
    if (d->kartState == KS_MASK_GRABBED) return;

    // Play sound effect for mask grab
    OtherFX_Play(fx_menu_locked, 0);

    // Store original position values
    int origX = d->posCurr.x;
    int origY = d->posCurr.y;
    int origZ = d->posCurr.z;
    
    // Call original mask grab
    VehStuckProc_MaskGrab_Init(t, d);
    
    // Restore original position
    d->posCurr.x = origX;
    d->posCurr.y = origY;
    d->posCurr.z = origZ;
    
    // Fix previous position too (prevents teleporting on next frame)
    d->posPrev.x = origX;
    d->posPrev.y = origY;
    d->posPrev.z = origZ;
    
    // Position mask to follow player
    struct MaskHeadWeapon* mask = d->KartStates.MaskGrab.maskObj;
    if (mask != NULL) {
        mask->pos[0] = origX >> 8;
        mask->pos[1] = (origY >> 8) + 0x140; // Position mask above player
        mask->pos[2] = origZ >> 8;
    }

    // Get player index from the driver pointer
    int playerIndex = -1;
    for (int i = 0; i < 4; i++) {
        if (sdata->gGT->drivers[i] == d) {
            playerIndex = i;
            break;
        }
    }
    
    // Set timer to show message (3 seconds at 30 FPS = 90 frames)
    if (playerIndex >= 0) {
        shortcutAttempts[playerIndex]++;
        noShortcutMsgTimer[playerIndex] = FPS_DOUBLE(30) * 3; // 3 seconds
    }
}

// Prevent any shortcut
void PreventShortcut(struct Driver* driver, int driverIndex) {
    struct GameTracker *gGT = sdata->gGT;
    
    // Continue only if driver has a valid quadblock
    if (driver != NULL && driver->lastValid != NULL) {
        int maxCheckpoint = gGT->level1->cnt_restart_points - 1;
        int currentCheckpoint = (int)(unsigned char) driver->lastValid->checkpointIndex;
        
        // 13% for all the tracks except for:
        // 7% for PAPU_PYRAMID and OXIDE_STATION
        int maxCheckpointSkip = 
            (gGT->levelID == PAPU_PYRAMID || gGT->levelID == OXIDE_STATION) ? (maxCheckpoint * 7) / 100 : (maxCheckpoint * 13) / 100;

        //25% for sewer speedway
        if (gGT->levelID == SEWER_SPEEDWAY) {
            maxCheckpointSkip = (maxCheckpoint * 25) / 100;
        }

        //9% for HOT_AIR_SKYWAY
        if (gGT->levelID == HOT_AIR_SKYWAY) {
            maxCheckpointSkip = (maxCheckpoint * 9) / 100;
        }

        //17% for polar pass
        if (gGT->levelID == POLAR_PASS) {
            maxCheckpointSkip = (maxCheckpoint * 17) / 100;
        }
        
        // Only check if we have a previous valid checkpoint
        if (lastValid_prev[driverIndex] != NULL && currentCheckpoint != lastValid_prev[driverIndex]->checkpointIndex) {
            int prevCheckpoint = (int)(unsigned char)lastValid_prev[driverIndex]->checkpointIndex;

            //9% for polar pass if previous checkpoint is ahead 56
            if (gGT->levelID == POLAR_PASS && prevCheckpoint > 56) {
                maxCheckpointSkip = (maxCheckpoint * 9) / 100;
            }

            //4% for polar pass if previous checkpoint is ahead 86
            if (gGT->levelID == POLAR_PASS && prevCheckpoint > 86) {
                maxCheckpointSkip = (maxCheckpoint * 4) / 100;
            }

            //6% for polar pass if previous checkpoint is ahead 130
            if (gGT->levelID == POLAR_PASS && prevCheckpoint > 130) {
                maxCheckpointSkip = (maxCheckpoint * 6) / 100;
            }

            //3% for HOT_AIR_SKYWAY if previous checkpoint is ahead 145
            if (gGT->levelID == HOT_AIR_SKYWAY && prevCheckpoint > 145) {
                maxCheckpointSkip = (maxCheckpoint * 3) / 100;
            }

            // Detect lap skip (NMZ abuse)
            if (
                currentCheckpoint == maxCheckpoint 							//If player lands on NMZ
                && prevCheckpoint > 5										//If player was a bit ahead of the start
                && abs_val(currentCheckpoint - prevCheckpoint) > 15			//If it was doing a skip
            ) { 
                // Reset to previous valid quadblock
                driver->lastValid = lastValid_prev[driverIndex];

                //Mask grab the player
                MaskGrab(driver->instSelf->thread, driver);

                // sprintf(decalText, "LAP SKIP BLOCKED! P%d", driverIndex+1);
                // DecalFont_DrawLine(decalText, 0x100, 0xc8 + 10, FONT_SMALL, (JUSTIFY_CENTER | RED));
            }
            // Detect other skips
            else if (
                prevCheckpoint != 0xFF
                && currentCheckpoint != 0xFF
                && currentCheckpoint != 0
                && prevCheckpoint < maxCheckpoint - 1
                && prevCheckpoint > 5
                && abs_val(currentCheckpoint - prevCheckpoint) > maxCheckpointSkip
            ) {
                driver->lastValid = lastValid_prev[driverIndex];
                MaskGrab(driver->instSelf->thread, driver);        
            }
            // Detect TA nitro lap skip (Awfull solution)
            else if (
                gGT->levelID == TINY_ARENA
                && (prevCheckpoint == maxCheckpoint || prevCheckpoint == maxCheckpoint - 1)
                && currentCheckpoint == 141
            ) {
                driver->lastValid = lastValid_prev[driverIndex];
                MaskGrab(driver->instSelf->thread, driver);
            }
        }

        // Mask grab if touching a quadblock with Q_NV_USED flag
        if(
            driver->currBlockTouching->quadFlags & Q_NV_USED
            && driver->currBlockTouching->checkpointIndex == -1
            && driver->currBlockTouching != prevTouchedBlock[driverIndex]  // Only if it's a new block
        ){
            MaskGrab(driver->instSelf->thread, driver);
        }
        // Detect Labs cut (block id 694 or 685)
        else if (
            gGT->levelID == N_GIN_LABS
            && driver->lastValid->checkpointIndex > 100
            && (driver->currBlockTouching->blockID == 694 || driver->currBlockTouching->blockID == 685)
            && driver->currBlockTouching != prevTouchedBlock[driverIndex]  // Only if it's a new block
        ) {
            driver->lastValid = lastValid_prev[driverIndex];
            MaskGrab(driver->instSelf->thread, driver);
        }

        prevTouchedBlock[driverIndex] = driver->currBlockTouching;
        
        // Update previous valid quadblock for this driver
        lastValid_prev[driverIndex] = driver->lastValid;
    }
}

// Initialize or reset skip prevention system
void InitSkipPrevention() {
    for (unsigned char i = 0; i < 4; i++) {
        lastValid_prev[i] = NULL;
        prevTouchedBlock[i] = NULL;
    }
}

void InitShortcutless(bool enabled) {
    if (!enabled) return;

    InitSkipPrevention();
    
    // Reset message timers
    for (int i = 0; i < 4; i++) {
        noShortcutMsgTimer[i] = 0;
        shortcutAttempts[i] = 0;
    }
}

void HandleShortcutless(bool enabled) {
    if (!enabled) return;

    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
        struct Driver* driver = gGT->drivers[i];

        if (driver == NULL) continue;
        if (gGT->levelID > TURBO_TRACK) continue;
        
        PreventShortcut(driver, i);

        // Display "NO SHORTCUTS!" message if timer is active
        if (noShortcutMsgTimer[i] > 0) {
            if (gGT->numPlyrCurrGame == 1) {
                int msgIndex = (shortcutAttempts[i] - 1) % NUM_SHORTCUT_MESSAGES;
                sprintf(decalText, "%s", shortcutMessages[msgIndex]);
                DecalFont_DrawLine(decalText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | RED));
            }
            noShortcutMsgTimer[i]--;
        }
    }
}

//check if inside of marked quadblock
bool NeedsMaskGrab(short blockID, short LevelID) {
    short CRASH_COVE_M[] = {1538, 1531, 1215, 1530};
    short CORTEX_CASTLE_M[] = {2572};
    short HOT_AIR_SKYWAY_M[] = {466, 467};
    short SLIDE_COLISEUM_M[] = {1402, 1469, 1468, 1492, 1491, 805, 806, 848, 637, 636, 582, 421, 420, 419, 388};
    short PAPU_PYRAMID_M[] = {180, 540, 457, 456, 496};
    short SEWER_SPEEDWAY_M[] = {1651, 1650};
    short POLAR_PASS_M[] = {802, 814};

    short* blockIDs = NULL;
    int numBlocks = 0;

    switch (LevelID) {
        case CRASH_COVE:
            blockIDs = CRASH_COVE_M;
            numBlocks = sizeof(CRASH_COVE_M) / sizeof(short);
            break;
        case CORTEX_CASTLE:
            blockIDs = CORTEX_CASTLE_M;
            numBlocks = sizeof(CORTEX_CASTLE_M) / sizeof(short);
            break;
        case HOT_AIR_SKYWAY:
            blockIDs = HOT_AIR_SKYWAY_M;
            numBlocks = sizeof(HOT_AIR_SKYWAY_M) / sizeof(short);
            break;
        case SLIDE_COLISEUM:
            blockIDs = SLIDE_COLISEUM_M;
            numBlocks = sizeof(SLIDE_COLISEUM_M) / sizeof(short);
            break;
        case PAPU_PYRAMID:
            blockIDs = PAPU_PYRAMID_M;
            numBlocks = sizeof(PAPU_PYRAMID_M) / sizeof(short);
            break;
        case SEWER_SPEEDWAY:
            blockIDs = SEWER_SPEEDWAY_M;
            numBlocks = sizeof(SEWER_SPEEDWAY_M) / sizeof(short);
            break;
        case POLAR_PASS:
            blockIDs = POLAR_PASS_M;
            numBlocks = sizeof(POLAR_PASS_M) / sizeof(short);
            break;
        default:
            return false;
    }

    for (int i = 0; i < numBlocks; i++) {
        if (blockID == blockIDs[i]) {
            return true; 
        }
    }

    return false;
}

// Marks with Q_NV_USED flag all the quadblocks that will be hardcoded as custom mask grabs
void RemoveOffRoadCHK(struct Level *level) {

    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;
    
    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];

        // MARK AS MASK GRAB ALL THE OFF ROAD
        if (
            qb->terrain_type == TERRAIN_GRASS
            || qb->terrain_type == TERRAIN_DIRT
            || qb->terrain_type == TERRAIN_SNOW
            || qb->terrain_type == TERRAIN_SLOWGRASS
            || qb->terrain_type == TERRAIN_SLOWDIRT
        ) {
            qb->checkpointIndex = -1;
            // qb->quadFlags |= Q_MASK_GRAB;
            qb->quadFlags |= Q_NV_USED;
        }

        // MASK GRAB ON TERRAIN_WATER if level is DRAGON_MINES
        if (
            qb->terrain_type == TERRAIN_WATER
            && (sdata->gGT->levelID == DRAGON_MINES)
        ) {
            qb->checkpointIndex = -1;
            // qb->quadFlags |= Q_MASK_GRAB;
            qb->quadFlags |= Q_NV_USED;

        }

        // MARK AS MASK GRAB THE MARKED QUADBLOCKS;
        if (NeedsMaskGrab(qb->blockID, sdata->gGT->levelID)) {
            qb->checkpointIndex = -1;
            // qb->quadFlags |= Q_MASK_GRAB;
            qb->quadFlags |= Q_NV_USED;
        }

    }
}

