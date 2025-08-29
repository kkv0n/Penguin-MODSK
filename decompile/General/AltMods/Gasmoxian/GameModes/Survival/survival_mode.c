#include <common.h>
#include "../../utils.h"

bool RaceShouldEnd(void);
void ForceRaceEnd(void);
void FreezeDriver(struct Driver* driver);
bool DriverIsEliminated(struct Driver* driver);

bool eliminatedDrivers[8] = {false, false, false, false, false, false, false, false};
int previousLapCheck = 0;
bool checkForElimination = false;

static unsigned frameCounter = 0;
u_char hudFlagsBackup;

void InitSurvivalMode(bool enabled){
    if(!enabled) return;

    for(int i = 0; i < 8; i++) {
        eliminatedDrivers[i] = false;
    }
    previousLapCheck = 0;
    checkForElimination = false;
}

void HandleSurvivalMode(bool enabled){
    if(!enabled) return;

    // Check if there are at least 2 players
    if(activeDriversCount < 2)
        return;

    struct Driver* localDriver = gGT->drivers[0];

    // Check if any player has legitimately finished the race
    // If someone finished, end the race
    if(RaceShouldEnd())
    {
        ForceRaceEnd();
        return; // Skip the rest of the logic
    }

    // Find the last non-eliminated driver
    struct Driver* lastDriver = NULL;
    int lastDriverPos = -1;
    for(int i = 7; i >= 0; i--) {
        struct Driver* driver = gGT->driversInRaceOrder[i];
        if(driver == NULL) continue;
        
        // If found a non-eliminated driver
        if(!DriverIsEliminated(driver)) {
            lastDriver = driver;
            lastDriverPos = i;
            break; // Found the last non-eliminated driver
        }
    }
    
    // If no last driver found (unlikely) or only one driver left, exit
    if(lastDriver == NULL || activeDriversCount <= 1) 
        return;
    
    if(lastDriverPos - 1 < 0) return;
    
    // If no second-to-last driver found, exit
    struct Driver* secondLastDriver = gGT->driversInRaceOrder[lastDriverPos - 1];
    if(secondLastDriver == NULL) 
        return;
        
    // If the second-to-last driver is at least one lap ahead, eliminate the last driver
    if(secondLastDriver->lapIndex > lastDriver->lapIndex) {
        // Mark the driver as eliminated
        eliminatedDrivers[lastDriver->driverID] = true;
        activeDriversCount--;
        
        // If this is the local player, freeze
        if(lastDriver->driverID == localDriver->driverID) {
            FreezeDriver(lastDriver);
        }
    }

    // If local player its on last place show a warning message
    if(lastDriver->driverID == localDriver->driverID) {
        int textColor = frameCounter++ & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(decalText, "LAST PLACE!!!");
        DecalFont_DrawLine(decalText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | textColor));
    }

    // If local player was eliminated, display the message
    if(DriverIsEliminated(localDriver)) {
        int textColor = frameCounter++ & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(decalText, "ELIMINATED! POSITION: %d", localDriver->driverRank + 1);
        DecalFont_DrawLine(decalText, 0x100, 0x84, FONT_SMALL, (JUSTIFY_CENTER | textColor));

        //Disable HUD
        // hudFlagsBackup = gGT->hudFlags;
        // gGT->hudFlags = 0;

        //Enable Spectator
        EndOfRace_Camera();
    }

    //Make sure all eliminated drivers are frozen
    for(int i = 0; i < 8; i++) {
        if(eliminatedDrivers[i]) {
            FreezeDriver(gGT->drivers[i]);
        }
    }
}


bool RaceShouldEnd()
{
    // Check if the race should end
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL && (driver->actionsFlagSet & ACTION_RACE_FINISHED) != 0) {
            return true;
        }
    }
    return false;
}

void ForceRaceEnd()
{   
    // Restore HUD flags
    // gGT->hudFlags = hudFlagsBackup;

    // Force the race to end for all drivers
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL) {
            if((driver->actionsFlagSet & ACTION_RACE_FINISHED) == 0){
                FreezeDriver(driver); //Just in case
            }
            driver->actionsFlagSet |= ACTION_RACE_FINISHED;
        }
    }
}

extern void* PlayerFreezeFuncTable[13];
void FreezeDriver(struct Driver* driver)
{
    if(driver == NULL) return;

    for(int i = 0; i < 13; i++) {
        // driver->funcPtrs[i] = 0;
        driver->funcPtrs[i] = PlayerFreezeFuncTable[i];
    }

    //Remove item
    driver->heldItemID = ITEM_NONE;
    driver->numHeldItems = 0;

    // Burn driver (so it appears black)
    // Since FreezeDriver is called by frame this keeps the timer refreshed
    driver->burnTimer = 0xf00;
}

bool DriverIsEliminated(struct Driver* driver)
{
    if(driver == NULL) return false;
    return eliminatedDrivers[driver->driverID];
}


//////////////////////////

// Global variables for time-based elimination
unsigned int eliminationTimer = 0;
const unsigned int eliminationTimeLimits[] = {
    FPS_DOUBLE(32 * 90), // 1:30
    FPS_DOUBLE(32 * 60), // 1:00
    FPS_DOUBLE(32 * 45), // 0:45
    FPS_DOUBLE(32 * 45), // 0:45
    FPS_DOUBLE(32 * 30), // 0:30
    FPS_DOUBLE(32 * 30), // 0:30
    FPS_DOUBLE(32 * 30), // 0:30
    FPS_DOUBLE(32 * 30), // 0:30
    FPS_DOUBLE(32 * 10), // 0:10
};


// Function declarations for time-based elimination
void InitTimeBasedSurvivalMode(bool enabled);
void HandleTimeBasedSurvivalMode(bool enabled);
void DisplayEliminationTimer(void);
void EliminateLastDriver(void);
bool CheckLastDriverRemaining(void);

// Initialize time-based survival mode
void InitTimeBasedSurvivalMode(bool enabled) {
    if(!enabled) return;
    
    for(int i = 0; i < 8; i++) {
        eliminatedDrivers[i] = false;
    }
    
    // Set initial timer
    eliminationTimer = eliminationTimeLimits[0];

    // Reset counter for text flashing
    frameCounter = 0;
}

// Main function for time-based survival mode
void HandleTimeBasedSurvivalMode(bool enabled) {
    if(!enabled) return;
    
    // Check if there are at least 2 players
    if(activeDriversCount < 2)
        return;
    
    struct Driver* localDriver = gGT->drivers[0];
    
    // Check if any player has legitimately finished the race
    if(RaceShouldEnd()) {
        ForceRaceEnd();
        return; // Skip the rest of the logic
    }
    
    // Decrement timer only when traffic lights run out
    if(gGT->trafficLightsTimer < 1 && eliminationTimer > 0) {
        eliminationTimer--;
    }
    
    // Display timer on screen
    if(gGT->trafficLightsTimer < 1){
        DisplayEliminationTimer();
    }
    
    // Check if time's up
    if(eliminationTimer == 0) {
        // Eliminate the last non-eliminated driver
        EliminateLastDriver();
        
        // If only one driver remains, end the race
        if(CheckLastDriverRemaining()) {
            ForceRaceEnd();
            return;
        }

        // Set timer for next elimination
        eliminationTimer = eliminationTimeLimits[CountEliminatedDrivers()];
    }
    
    // Find the last non-eliminated driver for warning message
    struct Driver* lastDriver = NULL;
    for(int i = 7; i >= 0; i--) {
        struct Driver* driver = gGT->driversInRaceOrder[i];
        if(driver == NULL) continue;
        
        // If found a non-eliminated driver
        if(!DriverIsEliminated(driver)) {
            lastDriver = driver;
            break; // Found the last non-eliminated driver
        }
    }
    
    // If local player is the last non-eliminated driver, show warning
    if(lastDriver != NULL && lastDriver->driverID == localDriver->driverID) {
        int textColor = frameCounter++ & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(decalText, "LAST PLACE!!!");
        DecalFont_DrawLine(decalText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | textColor));
    }
    
    // If local player was eliminated, display the message
    if(DriverIsEliminated(localDriver)) {
        int textColor = frameCounter++ & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(decalText, "ELIMINATED! POSITION: %d", localDriver->driverRank + 1);
        DecalFont_DrawLine(decalText, 0x100, 0x84, FONT_SMALL, (JUSTIFY_CENTER | textColor));
        
        // Enable spectator mode
        EndOfRace_Camera();
    }
    
    // Make sure all eliminated drivers are frozen
    for(int i = 0; i < 8; i++) {
        if(eliminatedDrivers[i]) {
            FreezeDriver(gGT->drivers[i]);
        }
    }
}

// Display the elimination timer on screen
void DisplayEliminationTimer() {
    // Game runs at 32 FPS
    const int FRAMES_PER_SECOND = FPS_DOUBLE(32);
    
    // Calculate minutes, seconds, and frames
    int totalSeconds = eliminationTimer / FRAMES_PER_SECOND;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    int frames = eliminationTimer % FRAMES_PER_SECOND;
    
    // Determine color based on time remaining
    int textColor = PENTA_WHITE;
    
    // Flash red when less than 10 seconds remaining
    if(totalSeconds < 10) {
        textColor = frameCounter & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
    }
    
    sprintf(decalText, "NEXT ELIMINATION: %01d:%02d:%02d", minutes, seconds, frames);
    DecalFont_DrawLine(decalText, 0x100, 0x32, FONT_SMALL, (JUSTIFY_CENTER | textColor));
}

// Function to eliminate the last driver
void EliminateLastDriver() {
    // Find the last non-eliminated driver
    struct Driver* lastDriver = NULL;
    
    for(int i = 7; i >= 0; i--) {
        struct Driver* driver = gGT->driversInRaceOrder[i];
        if(driver == NULL) continue;
        
        // If found a non-eliminated driver
        if(!DriverIsEliminated(driver)) {
            lastDriver = driver;
            break; // Found the last non-eliminated driver
        }
    }
    
    if(lastDriver == NULL) return; // No driver to eliminate
    
    // Mark the driver as eliminated
    eliminatedDrivers[lastDriver->driverID] = true;
    activeDriversCount--;
    
    // Freeze the driver
    FreezeDriver(lastDriver);
}

// Function to check if only one driver remains
bool CheckLastDriverRemaining() {
    int activeDrivers = 0;
    
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL && !DriverIsEliminated(driver)) {
            activeDrivers++;
        }
    }
    
    return (activeDrivers <= 1);
}

int CountEliminatedDrivers() {
    int count = 0;
    for(int i = 0; i < 8; i++) {
        if(eliminatedDrivers[i]) {
            count++;
        }
    }
    return count;
}