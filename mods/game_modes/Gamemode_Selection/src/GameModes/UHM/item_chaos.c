#include "common.h"
#include "../../utils.h"

int itemTimer;
int itemChaosDifficulty = 0; // 0=off, 1=easy, 2=medium, 3=hard

bool CanThrowItems(struct Driver* driver) {
    if (driver == NULL) return false;
    
    // If the driver has a TNT on their head or is affected by a clock, they cannot throw items
    if ((driver->instTntRecv != 0) || (driver->clockReceive != 0)) return false;

    // Check if the driver is in a valid state to throw items
    bool isAI = ((driver->actionsFlagSet & 0x100000) != 0);
    if (isAI && (driver->botData.botFlags & 2) != 0) return false; // Bot is spinning or blasted

    bool isHuman = !isAI;
    if (isHuman && (driver->kartState != KS_NORMAL && driver->kartState != KS_DRIFTING)) return false;

    // If its boss race and is human, don't allow to throw items
    bool isBossRace = (gGT->gameMode1 & ADVENTURE_BOSS) != 0;
    if (isBossRace && isHuman) return false;

    return true;
}

void ItemChaos_Init(bool enabled) {
    if (!enabled) return;

    // Initialize item timer, wait 5 seconds to start throwing items
    itemTimer = FPS_DOUBLE(160);
}

int CountActiveWarpOrbs() {
    int count = 0;
    struct Thread* warpballThread;
    struct Instance* warpballInst;

    // Loop through all threads in the TRACKING bucket
    for (
        warpballThread = gGT->threadBuckets[TRACKING].thread;
        warpballThread != 0;
        warpballThread = warpballThread->siblingThread
    ) {
        warpballInst = warpballThread->inst;

        //If its a warpball       
        if(warpballInst->model->id == DYNAMIC_WARPBALL){
            count++;
        }
    }
    
    return count;
}

int lastCheckpointForWeaponRoulette[8] = {-1, -1, -1, -1, -1, -1, -1, -1}; // Store last checkpoint for each driver

// Check if we need to give a weapon based on checkpoint progress
void HandleWeaponRoulette(bool enabled) {
    if (!enabled) return;

    // Only run if there are no bots and more than one player
    if (gGT->numBotsNextGame > 0 || gGT->numPlyrCurrGame <= 1) {
        return;
    }
    
    int totalCheckpoints = gGT->level1->cnt_restart_points;
    if (totalCheckpoints <= 0) return;
    
    // Calculate checkpoint intervals (20% increments)
    int interval = totalCheckpoints / 5;
    if (interval <= 0) interval = 1; // Ensure minimum interval
    
    // Check each player to see if they crossed an interval boundary
    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
        struct Driver* driver = gGT->drivers[i];
        if (driver == NULL || driver->underDriver == NULL) continue;
        
        int currentCheckpoint = driver->underDriver->checkpointIndex;

        // skip if checkpoint is last checkpoint
        if (currentCheckpoint == totalCheckpoints - 1) continue;
        
        // Skip if the checkpoint hasn't changed
        if (currentCheckpoint == lastCheckpointForWeaponRoulette[i]) continue;
        
        // Check if player crossed an interval boundary
        for (int j = 1; j <= 4; j++) { // 20%, 40%, 60%, 80%
            int checkpointThreshold = j * interval;
            
            // If the previous checkpoint was before the threshold and 
            // the current checkpoint is at or past the threshold
            if (lastCheckpointForWeaponRoulette[i] < checkpointThreshold && 
                currentCheckpoint >= checkpointThreshold) {
                // Give player a weapon
                WeaponRoulette(driver);
                break;
            }
        }
        
        // Update the last checkpoint
        lastCheckpointForWeaponRoulette[i] = currentCheckpoint;
    }
}


// Give a weapon to a driver at checkpoint intervals
void WeaponRoulette(struct Driver* driver) {
    // If driver already has a weapon, quit
    if ((driver->heldItemID != 0xF) && (driver->noItemTimer == 0)) {
        return;
    }

    // Held item count
    if (driver->numHeldItems != 0) {
        return;
    }

    // If driver is firing weapon, quit
    if ((driver->actionsFlagSet & 0x8000) != 0) {
        return;
    }

    // If driver has raincloud and weapon is shuffling, quit
    if (driver->thCloud != 0) {
        struct RainCloud* rainCloud = (struct RainCloud*)driver->thCloud->object;
        if (rainCloud->boolScrollItem == 1) {
            return;
        }
    }

    // If driver is influenced by clock weapon, quit
    if (driver->clockReceive != 0) {
        return;
    }

    // Set weapon to roulette
    driver->heldItemID = 0x10;

    // Increment
    driver->numTimesHitWeaponBox++;

    // Timer for weapon roulette
    driver->itemRollTimer = FPS_DOUBLE(90);

    // If no roulette is active
    if ((gGT->gameMode1 & ROLLING_ITEM) == 0) {
        // Start sound
        OtherFX_Play(0x5D, 0);
        
        // Set rolling item flag
        gGT->gameMode1 |= ROLLING_ITEM;
    }

    driver->noItemTimer = 0;
}

//TODO: Sometimes a bug happens where only driver 0 will shoot items
// Each second, force a random driver to throw a random item
// All players on last lap will have 99 wumpas
void HandleItemChaos(bool enabled) {
    if (!enabled) return;

    // Give all players that are in last lap 99 wumpas
    for (unsigned char i = 0; i < gGT->numPlyrCurrGame + gGT->numBotsNextGame; i++) {
        struct Driver* driver = gGT->drivers[i];
        if (driver != NULL && driver->lapIndex == gGT->numLaps - 1) {
            driver->numWumpas = 99;
        }
    }

    // Throw items when race is active (after traffic lights)
    if (gGT->levelID <= LAB_BASEMENT && gGT->trafficLightsTimer < 0) {
        if (itemTimer <= 0) {
            // Set timer based on difficulty level and lap status
            bool isLastLap = false;
            struct Driver* firstPlaceDriver = gGT->driversInRaceOrder[0];
            if (firstPlaceDriver != NULL) {
                // Check if it's the last lap for the leader
                isLastLap = (firstPlaceDriver->lapIndex == gGT->numLaps - 1);
            }
            
            // Set timer based on difficulty level
            switch (itemChaosDifficulty) {
                case 1: // Easy
                    itemTimer = FPS_DOUBLE(60);
                    break;
                case 2: // Medium
                    itemTimer = FPS_DOUBLE(45);
                    break;
                case 3: // Hard
                    itemTimer = FPS_DOUBLE(30);
                    break;
                default: // Default to medium if something goes wrong
                    itemTimer = FPS_DOUBLE(45);
                    break;
            }

            // If is last lap divide the timer by 2
            if (isLastLap) {
                itemTimer /= 2;
            }

            // Select a random driver
            int randomDriverIndex = rand() % (gGT->numPlyrCurrGame + gGT->numBotsNextGame);
            struct Driver* randomDriver = gGT->drivers[randomDriverIndex];

            // Ensure the random driver can throw items
            if (!CanThrowItems(randomDriver)) return;
            
            if (randomDriver != NULL) {
                // Check if a human player is in first
                bool humanInFirst = false;

                if (((firstPlaceDriver->actionsFlagSet & 0x100000) == 0)) {
                    humanInFirst = true;
                }
                
                // Check if the random driver is in first place
                bool isDriverInFirst = randomDriver->driverRank == 0;
                
                // Define item weights
                int bombWeight = 15;
                int missileWeight = 30;
                int crateWeight = 60;
                int beakerWeight = 35;
                int clockWeight = humanInFirst ? 4 : 2;
                int orbWeight = humanInFirst ? 14 : 7;

                // Players in first place can't shoot orb, clock or missile
                // Human player can't shoot orb, clock or missile
                if (isDriverInFirst || (randomDriver->actionsFlagSet & 0x100000) == 0) {
                    missileWeight = 0;
                    clockWeight = 0;
                    orbWeight = 0;
                }

                // Calculate total weight
                int totalWeight = bombWeight + missileWeight + crateWeight + beakerWeight + clockWeight + orbWeight;
                if (totalWeight == 0) {
                    // If all weights are 0, use bomb, crate and beaker only
                    bombWeight = 10;
                    crateWeight = 60;
                    beakerWeight = 35;
                    totalWeight = bombWeight + crateWeight + beakerWeight;
                }
                
                // Generate a random number in the range [0, totalWeight)
                int randWeight = rand() % totalWeight;
                
                // Select item based on weights
                int item;
                if (randWeight < bombWeight) {
                    item = ITEM_BOWLING_BOMB;
                } else if (randWeight < bombWeight + missileWeight) {
                    item = ITEM_TRACKING_MISSILE;
                } else if (randWeight < bombWeight + missileWeight + crateWeight) {
                    item = ITEM_EXPLOSIVE_CRATE;
                } else if (randWeight < bombWeight + missileWeight + crateWeight + beakerWeight) {
                    item = ITEM_N_BRIO_BEAKER;
                } else if (randWeight < bombWeight + missileWeight + crateWeight + beakerWeight + clockWeight) {
                    item = ITEM_N_TROPY_CLOCK;
                } else {
                    item = ITEM_WARP_ORB;
                }

                // Check if there are already 5 or more warp orbs active
                if (item == ITEM_WARP_ORB && CountActiveWarpOrbs() >= 5) {
                    return; // Don't shoot more orbs if there are already 5 active
                }
                
                // If human in first, item is orb, and it's last lap, 
                // force orb to shoot from player in rank 1 (second place)
                if (humanInFirst && item == ITEM_WARP_ORB && isLastLap) {
                    randomDriver = gGT->driversInRaceOrder[1];
                }

                int flag = 0;

                //if item is a crate or a breaker give a probability of 25% to trow in the air
                if (item == ITEM_EXPLOSIVE_CRATE || item == ITEM_N_BRIO_BEAKER) {
                    if (rand() % 4 == 0) {
                        flag = 4;
                    }
                }
                
                // Fire the selected item
                VehPickupItem_ShootNow(randomDriver, item, flag);
            }
        } else {
            itemTimer--;
        }
    }
}

//In theory this should expand the mine pool, but it doesn't work

// LIST_Clear(&D231.minePoolTaken);
// LIST_Clear(&D231.minePoolFree);    
// int numMines = 40;
// for(int i = 0; i < numMines; i++){
//     LIST_AddFront(&D231.minePoolFree, (struct Item*)&D231.minePoolItem[i]);
// }

//Make all drivers to throw an item
// for (unsigned char i = 0; i < gGT->numPlyrCurrGame + gGT->numBotsNextGame; i++) {
//         struct Driver* driver = gGT->drivers[i];
//         if (driver != NULL && (driver->kartState == KS_NORMAL || driver->kartState == KS_DRIFTING)) {
//         VehPickupItem_ShootNow(driver, ITEM_EXPLOSIVE_CRATE, 0);
//         }
// }