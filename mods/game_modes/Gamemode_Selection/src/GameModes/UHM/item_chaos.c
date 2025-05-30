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
        if(warpballInst->model->id == 0x36){
            count++;
        }
    }
    
    return count;
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
    if (gGT->levelID <= TURBO_TRACK && gGT->trafficLightsTimer < 0) {
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