#include "common.h"
#include "../../utils.h"

int itemTimer;

void ItemModifier_Init(bool enabled) {
    if (!enabled) return;

    // Initialize item timer, wait 5 seconds to start throwing items
    itemTimer = FPS_DOUBLE(160);
}

//TODO: Check kart state for both humans and bots before throwing items
//TODO: Sometimes a bug happens where only driver 0 will shoot items
//TODO: Rename this, this is not a modifier, this is a game mode
// Each second, force a random driver to throw a random item
// All players on last lap will have 99 wumpas
void HandleItemModifiers(bool enabled) {
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
            itemTimer = FPS_DOUBLE(32); // Reset timer to 1 second (32 frames)
            
            // Select a random driver
            int randomDriverIndex = rand() % (gGT->numPlyrCurrGame + gGT->numBotsNextGame);
            struct Driver* randomDriver = gGT->drivers[randomDriverIndex];
            
            if (randomDriver != NULL) {
                // Check if a human player is in first
                bool humanInFirst = false;
                struct Driver* firstPlaceDriver = gGT->driversInRaceOrder[0];
                if (firstPlaceDriver != NULL && ((firstPlaceDriver->actionsFlagSet & 0x100000) == 0)) {
                    humanInFirst = true;
                }
                
                // Check if the random driver is in first place
                bool isDriverInFirst = (randomDriver == firstPlaceDriver);
                
                // Check if it's the last lap for the leader
                bool isLastLap = (firstPlaceDriver != NULL && 
                                  firstPlaceDriver->lapIndex == gGT->numLaps - 1);
                
                // Define item weights
                int bombWeight = 15;
                int missileWeight = (humanInFirst && randomDriver->driverRank == 1) ? 70 : 20;
                int crateWeight = 70;
                int beakerWeight = 40;
                int clockWeight = humanInFirst ? 7 : 2;
                int orbWeight = humanInFirst ? 20 : 7;
                
                // Players in first place can't shoot orb, clock or missile
                if (isDriverInFirst) {
                    missileWeight = 0;
                    clockWeight = 0;
                    orbWeight = 0;
                }
                
                // Calculate total weight
                int totalWeight = bombWeight + missileWeight + crateWeight + beakerWeight + clockWeight + orbWeight;
                if (totalWeight == 0) {
                    // If all weights are 0, use bomb, crate and beaker only
                    bombWeight = 15;
                    crateWeight = 70;
                    beakerWeight = 40;
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
                
                // If human in first, item is orb, and it's last lap, 
                // force orb to shoot from player in rank 1 (second place)
                if (humanInFirst && item == ITEM_WARP_ORB && isLastLap) {
                    randomDriver = gGT->driversInRaceOrder[1];
                }
                
                // Fire the selected item
                VehPickupItem_ShootNow(randomDriver, item, 0);
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