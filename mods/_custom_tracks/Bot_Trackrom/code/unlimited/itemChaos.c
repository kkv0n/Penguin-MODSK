#include "common.h"
#include "../header/bot.h"

int itemTimer;
int itemChaosDifficulty; // 0=off, 1=easy, 2=medium, 3=hard
int maxWarpOrbs;


struct GameTracker* gGT;


enum ITEMS {
	ITEM_TURBO_BOOST = 0,
	ITEM_BOWLING_BOMB,
	ITEM_TRACKING_MISSILE,
	ITEM_EXPLOSIVE_CRATE,
	ITEM_N_BRIO_BEAKER,
	ITEM_SPRING,
	ITEM_POWER_SHIELD,
	ITEM_MASK,
	ITEM_N_TROPY_CLOCK,
	ITEM_WARP_ORB,
	ITEM_BOWLING_BOMB_X3,
	ITEM_TRACKING_MISSILE_X3,
	ITEM_INVISIBILITY,
	ITEM_SUPER_ENGINE
};


bool CanThrowItems(struct Driver* driver) {
    if (driver == NULL) return false;
    
    if (botPaused) return false;
    
    // If the driver has a TNT on their head or is affected by a clock, they cannot throw items
    if ((driver->instTntRecv != 0) || (driver->clockReceive != 0)) return false;

    // Check if the driver is in a valid state to throw items
    bool isAI = (driver->driverID > 0);
    if (isAI && PAUSE_BOT_PATH(driver->kartState)) return false; // Bot is spinning or blasted

    bool isHuman = !isAI;

    return !isHuman;
}

void ItemChaos_Init(bool enabled) {
	gGT = sdata->gGT;
    if (!enabled) return;

    // Initialize item timer, wait 5 seconds to start throwing items
    itemTimer = FPS_DOUBLE(160);

        itemChaosDifficulty = bot_Itemdifficulty;
        maxWarpOrbs = itemChaosDifficulty + 1;
        
        

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
// Each second (timer), force a random driver to throw a random item
// All players on last lap will have 99 wumpas
void HandleItemChaos(bool enabled) {
    if (!enabled) return;
    

    unsigned char* curr_botItemset = &botItemList[0];

    // Give human players that are in last lap 99 wumpas
    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
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
            //if boss race shoot from boss [1]
            if((gGT->gameMode1 & ADVENTURE_BOSS)) randomDriverIndex = 1;
            struct Driver* randomDriver = gGT->drivers[randomDriverIndex];

            // Ensure the random driver can throw items
            if (!CanThrowItems(randomDriver)) return;
            
            if (randomDriver != NULL) {
                // Check if a human player is in first
                bool humanInFirst = false;

                if (firstPlaceDriver->driverID == 0) {
                    humanInFirst = true;
                }
                
                
                
                // Check if the random driver is in first place
                bool isDriverInFirst = (randomDriver->driverRank == 0);
                
                char orb_clock_missile_indexes[3] = {-1, -1 , -1};
                
                
                for (unsigned char it = 0; it < 6; it++)
                {
                    
                    unsigned char curr_item = curr_botItemset[it];
                    
                    if (curr_item == ITEM_WARP_ORB)
                        orb_clock_missile_indexes[0] = it;
                    else if (curr_item == ITEM_N_TROPY_CLOCK)
                        orb_clock_missile_indexes[1] = it;
                    else if (curr_item == ITEM_TRACKING_MISSILE)
                        orb_clock_missile_indexes[2] = it;

                }
                
                
                // Define item weights
                int bombWeight = 15;
                int missileWeight = 30;
                int crateWeight = 60;
                int beakerWeight = 35;
                int clockWeight = (humanInFirst) ? 4 : 2;
                int orbWeight = (humanInFirst) ? 14 : 7;
                

                    

                // Players in first place can't shoot orb, clock or missile
                if (isDriverInFirst) {
                    missileWeight = 0;
                    clockWeight = 0;
                    orbWeight = 0;
                }
                
                //if the itemset doesnt have any of these then discard weight reset
                if (orb_clock_missile_indexes[0] == -1)
                    orbWeight = 14;
                
                if (orb_clock_missile_indexes[1] == -1)
                    clockWeight = 4;
                
                if (orb_clock_missile_indexes[2] == -1)
                    missileWeight = 30;

                // Calculate total weight
                int totalWeight = bombWeight + missileWeight + crateWeight + beakerWeight + clockWeight + orbWeight;
                
                
                if (isDriverInFirst) {
                    // If all weights are 0, use bomb, crate and beaker only
                    bombWeight = 35;
                    crateWeight = 10;
                    beakerWeight = 60;
                    totalWeight = bombWeight + crateWeight + beakerWeight;
                }
                
                
                // Generate a random number in the range [0, totalWeight)
                int randWeight = rand() % totalWeight;
                
                int item;
                
                // Select item based on weights
                
                if (randWeight < bombWeight) {
                    item = curr_botItemset[0];
                } else if (randWeight < bombWeight + missileWeight) {
                    item = curr_botItemset[1];
                } else if (randWeight < bombWeight + missileWeight + crateWeight) {
                    item = curr_botItemset[2];
                } else if (randWeight < bombWeight + missileWeight + crateWeight + beakerWeight) {
                    item = curr_botItemset[3];
                } else if (randWeight < bombWeight + missileWeight + crateWeight + beakerWeight + clockWeight) {
                    item = curr_botItemset[4];
                } else {
                    item = curr_botItemset[5];
                }

                // Check if there are already 5 or more warp orbs active
                if (item == ITEM_WARP_ORB && CountActiveWarpOrbs() >= maxWarpOrbs) {
                    return; // Don't shoot more orbs if there are already max orbs
                }
                
                // If human in first, item is orb, and it's last lap, 
                // force orb to shoot from player in rank 1 (second place)
                if (humanInFirst && item == ITEM_WARP_ORB && isLastLap) {
                    randomDriver = gGT->driversInRaceOrder[1];
                }

                int flag = 0;

                // if boss race and item its an orb or a clock, give a 25% chance to change it to turbo boost 
                if ((gGT->gameMode1 & ADVENTURE_BOSS) && (item == ITEM_WARP_ORB || item == ITEM_N_TROPY_CLOCK)) {
                    if (rand() % 4 == 0) {
                        item = ITEM_TURBO_BOOST;
                    }
                }
                
                bool double_item = false;
                
                //if item is a crate or a breaker give a probability of 25% to trow in the air
                if (isDriverInFirst)
                {
                    if (item == ITEM_BOWLING_BOMB)
                    {
                        bossflag:
                        
                        flag = 2;
                        
                        if (rand() % 3 == 0)
                            double_item = true;
                        
                    }
                    else if (item == ITEM_EXPLOSIVE_CRATE  || item == ITEM_N_BRIO_BEAKER)
                    {
                        if (rand() % 4 == 0)
                        {
                            goto bossflag;
                        }
                    }
                    
                }
                else
                {
                    if (item == ITEM_N_BRIO_BEAKER) {
                        if (rand() % 4 == 0)
                            flag = 4;
                        
                    }
                }

                //BOMB AND MISSILES SHARES CODE AND CHECKS HELDITEMID
                if (
                    item == ITEM_BOWLING_BOMB_X3 || item == ITEM_BOWLING_BOMB || 
                    item == ITEM_TRACKING_MISSILE || item == ITEM_TRACKING_MISSILE_X3
                   )
                   {
                       randomDriver->heldItemID = item;
                       item = ITEM_TRACKING_MISSILE;
                   }
                
                //probability to get juiced items
                randomDriver->numWumpas = (rand() % 4 == 0) ? 10 : 0;
                
                // Fire the selected item
                VehPickupItem_ShootNow(randomDriver, item, flag);
                
                if (double_item)
                    VehPickupItem_ShootNow(randomDriver, item, flag);
                
                //reset wumpas after shooting item
                if (randomDriver->numWumpas != 0) randomDriver->numWumpas = 0;
                
                //remove after shooting a bomb or missile
                if (randomDriver->heldItemID != 0xF) randomDriver->heldItemID = 0xF;
            }
        } else {
            itemTimer--;
        }
    }
}
