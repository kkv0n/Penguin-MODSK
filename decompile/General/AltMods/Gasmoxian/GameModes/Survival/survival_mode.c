#include <common.h>
#include "../../utils.h"

bool RaceShouldEnd(void);
void ForceRaceEnd(void);
void FreezeDriver(struct Driver* driver);
bool DriverIsEliminated(struct Driver* driver);
int GetDriverEliminationPosition(int driverID);

bool eliminatedDrivers[8] = {false, false, false, false, false, false, false, false};
int eliminationOrder[8] = {-1, -1, -1, -1, -1, -1, -1, -1}; // -1 means not eliminated yet
int eliminationCount = 0;  // Number of drivers eliminated so far
int previousLapCheck = 0;
bool checkForElimination = false;
int survivalActiveDriversCount; // Counts active players that are not eliminated 

u_char hudFlagsBackup;

void InitSurvivalMode(bool enabled){
    if(!enabled) return;

    for(int i = 0; i < 8; i++) {
        eliminatedDrivers[i] = false;
        eliminationOrder[i] = -1;  // Reset elimination order
    }
    eliminationCount = 0;
    previousLapCheck = 0;
    checkForElimination = false;

    survivalActiveDriversCount = raceInitActiveDriversCount;
}

void HandleSurvivalMode(bool enabled){
    if(!enabled) return;

    // Check if there are at least 2 players
    if(raceInitActiveDriversCount < 2)
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
    if(lastDriver == NULL || survivalActiveDriversCount <= 1) 
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
        // Record elimination order
        eliminationOrder[eliminationCount++] = lastDriver->driverID;
        survivalActiveDriversCount--;
        
        // If this is the local player, freeze
        if(lastDriver->driverID == localDriver->driverID) {
            FreezeDriver(lastDriver);
        }
    }

    // If local player its on last place show a warning message
    if(lastDriver->driverID == localDriver->driverID) {
        int textColor = gGT->timer & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        
        sprintf(
            decalText, 
            #ifdef GASMOX_ENG
            "LAST PLACE!!!"
            #elif defined(GASMOX_ES)
            "ULTIMO LUGAR!!!"
            #elif defined(GASMOX_BR)
            "ULTIMO LUGAR!!!"
            #endif
        );
            DecalFont_DrawLine(decalText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | textColor));
    }

    // If local player was eliminated, display the message
    if(DriverIsEliminated(localDriver)) {
        int textColor = gGT->timer & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(
            decalText, 
            #ifdef GASMOX_ENG
            "ELIMINATED! POSITION: %d",
            #elif defined(GASMOX_ES)
            "ELIMINADO! POSICION: %d",
            #elif defined(GASMOX_BR)
            "ELIMINADO! POSIÇÃO: %d",
            #endif
            localDriver->driverRank + 1
        );
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

void EliminateAllPlayersExceptFirst(void){
    // Find the actual winner - either someone who finished or the first in race order
    struct Driver* winnerDriver = NULL;
    
    // First, check if any driver has finished the race legitimately
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL && (driver->actionsFlagSet & ACTION_RACE_FINISHED) != 0) {
            winnerDriver = driver;
            break; // Found a driver who finished
        }
    }
    
    // If no driver finished, the first non-eliminated driver in race order is the winner
    if(winnerDriver == NULL) {
        for(int i = 0; i < 8; i++) {
            struct Driver* driver = gGT->driversInRaceOrder[i];
            if(driver != NULL && !DriverIsEliminated(driver)) {
                winnerDriver = driver;
                break; // Found the leader
            }
        }
    }
    
    // If still no winner found (unlikely), just exit
    if(winnerDriver == NULL) return;
    
    // Now mark all drivers except the winner as eliminated
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL && driver != winnerDriver && !DriverIsEliminated(driver)) {
            // Mark the driver as eliminated
            eliminatedDrivers[driver->driverID] = true;
            // Record elimination order
            eliminationOrder[eliminationCount++] = driver->driverID;
            survivalActiveDriversCount--;
        }
    }
}

bool RaceShouldEnd()
{
    // Race should end if any driver has finished
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL && (driver->actionsFlagSet & ACTION_RACE_FINISHED) != 0) {
            return true;
        }
    }
    
    // Race also should end if a player if there is only eliminated and dead players
    // dead players is a player with this condition == octr->nameBuffer[i][0] == 0
    for(int i = 0; i < 8; i++) {
        struct Driver* driver = gGT->drivers[i];
        if(driver != NULL && !DriverIsEliminated(driver) && octr->nameBuffer[i][0] != 0) {
            // Found a player who is not eliminated and not dead
            return false; 
        }
    }

    return true;
}

void ForceRaceEnd()
{   
    // Restore HUD flags
    // gGT->hudFlags = hudFlagsBackup;

    EliminateAllPlayersExceptFirst();

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
    FPS_DOUBLE(32 * 60), // 1:00
    FPS_DOUBLE(32 * 45), // 0:45
    FPS_DOUBLE(32 * 30), // 0:30
    FPS_DOUBLE(32 * 30), // 0:30
    FPS_DOUBLE(32 * 20), // 0:20
    FPS_DOUBLE(32 * 20), // 0:20
    FPS_DOUBLE(32 * 20), // 0:20
    FPS_DOUBLE(32 * 20), // 0:20
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
}

// Main function for time-based survival mode
void HandleTimeBasedSurvivalMode(bool enabled) {
    if(!enabled) return;
    
    // Check if there are at least 2 players
    if(raceInitActiveDriversCount < 2)
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
        //Play sound
        DECOMP_OtherFX_Play(fx_semaphor2, 1);

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
        int textColor = gGT->timer & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(
            decalText, 
            #ifdef GASMOX_ENG
            "LAST PLACE!!!"
            #elif defined(GASMOX_ES)
            "ULTIMO LUGAR!!!"
            #elif defined(GASMOX_BR)
            "ULTIMO LUGAR!!!"
            #endif
        );
        DecalFont_DrawLine(decalText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | textColor));
    }
    
    // If local player was eliminated, display the message
    if(DriverIsEliminated(localDriver)) {
        int textColor = gGT->timer & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
        sprintf(
            decalText, 
            #ifdef GASMOX_ENG
            "ELIMINATED! POSITION: %d",
            #elif defined(GASMOX_ES)
            "ELIMINADO! POSICION: %d",
            #elif defined(GASMOX_BR)
            "ELIMINADO! POSIÇÃO: %d",
            #endif
            localDriver->driverRank + 1
        );
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
        textColor = gGT->timer & FPS_DOUBLE(5) ? CORTEX_RED : PENTA_WHITE;
    }

    // If 5 seconds or less play a warning sound each second
    if(totalSeconds <= 5 && totalSeconds >= 1 && frames == 0) {
        DECOMP_OtherFX_Play(fx_semaphor, 1);
    }

    sprintf(
        decalText,
        #ifdef GASMOX_ENG
        "NEXT ELIMINATION: %01d:%02d:%02d",
        #elif defined(GASMOX_ES)
        "PROXIMA ELIMINACION: %01d:%02d:%02d",
        #elif defined(GASMOX_BR)
        "PRÓXIMA ELIMINAÇÃO: %01d:%02d:%02d",
        #endif
        minutes, seconds, frames
    );
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
    // Record elimination order
    eliminationOrder[eliminationCount++] = lastDriver->driverID;
    survivalActiveDriversCount--;
    
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

int GetDriverEliminationPosition(int driverID) {
    // Check if driver is eliminated
    if (!eliminatedDrivers[driverID]) {
        // Not eliminated yet, should be ranked first
        return -1; // Special value for non-eliminated drivers
    }

    // Find position in elimination order
    for (int i = 0; i < 8; i++) {
        if (eliminationOrder[i] == driverID) {
            return i;
        }
    }
    
    // Should never reach here
    return -1;
}