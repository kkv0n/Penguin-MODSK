#include <common.h>
#include "utils.h"

// Helper function for absolute value
int abs_val(int value) { return value < 0 ? -value : value; }

// Track previous quadblocks for each player
struct QuadBlock* lastValid_prev[4] = {NULL, NULL, NULL, NULL};

void MaskGrab(struct Thread* t, struct Driver* d) {
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
}

// Prevent lap skips (NMZ Abuse)
void PreventLapSkip(struct Driver* driver, int driverIndex) {
    struct GameTracker *gGT = sdata->gGT;
    
    // Continue only if driver has a valid quadblock
    if (driver != NULL && driver->lastValid != NULL) {
        int maxCheckpoint = gGT->level1->cnt_restart_points - 1;
        int currentCheckpoint = (int)(unsigned char) driver->lastValid->checkpointIndex;
        
        // int maxCheckpointSkip = (maxCheckpoint + 1) / 5; // 20% of the track length
        //Max checkpoint skip will be 20% if CORTEX_CASTLE and 26% for HOT_AIR_SKYWAY, else no limit
        int maxCheckpointSkip = (gGT->levelID == CORTEX_CASTLE) ? (maxCheckpoint / 5) : (gGT->levelID == HOT_AIR_SKYWAY) ? ((maxCheckpoint * 13) / 50) : maxCheckpoint + 1;
        
        // Only check if we have a previous valid checkpoint
        if (lastValid_prev[driverIndex] != NULL && currentCheckpoint != lastValid_prev[driverIndex]->checkpointIndex) {
            int prevCheckpoint = (int)(unsigned char)lastValid_prev[driverIndex]->checkpointIndex;

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

                // sprintf(debugText, "LAP SKIP BLOCKED! P%d", driverIndex+1);
                // DecalFont_DrawLine(debugText, 0x100, 0xc8 + 10, FONT_SMALL, (JUSTIFY_CENTER | RED));
            }
            //This conditional can be removed but will allow too broken SCs on Castle and HAS
            else if (
                prevCheckpoint != 0xFF
                && currentCheckpoint != 0xFF
                && prevCheckpoint < maxCheckpoint - 1
                && prevCheckpoint > 5
                && abs_val(currentCheckpoint - prevCheckpoint) > maxCheckpointSkip
            ) {
                driver->lastValid = lastValid_prev[driverIndex];
                MaskGrab(driver->instSelf->thread, driver);
            }
        }
        
        // Update previous valid quadblock for this driver
        lastValid_prev[driverIndex] = driver->lastValid;
    }
}

// Initialize or reset lap skip prevention system
void initLapSkipPrevention() {
    for (unsigned char i = 0; i < 4; i++) {
        lastValid_prev[i] = NULL;
    }
}