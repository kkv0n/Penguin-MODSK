#include <common.h>
#include "../../utils.h"

// Helper function for absolute value
extern int abs_val(int value);

// Track previous quadblocks for each player
extern struct QuadBlock* lastValid_prev[4];

extern void MaskGrab(struct Thread* t, struct Driver* d);

// Prevent lap skips (NMZ Abuse)
void PreventLapSkip(struct Driver* driver, int driverIndex) {
    struct GameTracker *gGT = sdata->gGT;

    // sprintf(decalText, "CHK Index %d", (int)(unsigned char) driver->lastValid->checkpointIndex);
    // DecalFont_DrawLine(decalText, 0x100, 0xc8 + 10, FONT_SMALL, (JUSTIFY_CENTER | ORANGE));
    
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

                // sprintf(decalText, "LAP SKIP BLOCKED! P%d", driverIndex+1);
                // DecalFont_DrawLine(decalText, 0x100, 0xc8 + 10, FONT_SMALL, (JUSTIFY_CENTER | RED));
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
void InitLapSkipPrevention() {
    for (unsigned char i = 0; i < 4; i++) {
        lastValid_prev[i] = NULL;
    }
}