#include "common.h"
#include "../utils.h"

int itemTimer = FPS_DOUBLE(160);


        // LIST_Clear(&D231.minePoolTaken);
        // LIST_Clear(&D231.minePoolFree);    
        // int numMines = 40;
        // for(int i = 0; i < numMines; i++){
        //     LIST_AddFront(&D231.minePoolFree, (struct Item*)&D231.minePoolItem[i]);
        // }

            // if (((controller->buttonsHeldCurrFrame & BTN_TRIANGLE) != 0)){
    // 	VehPickupItem_ShootNow(gGT->driversInRaceOrder[7], 9, 0);
    // }

    // Trow item from a random driver
    // if (gGT->levelID <= TURBO_TRACK && gGT->trafficLightsTimer < 0) {
    //     if (itemTimer <= 0) {
    //         itemTimer = FPS_DOUBLE(32);
    //         // int randomDriverIndex = rand() % (gGT->numPlyrCurrGame + gGT->numBotsNextGame);
    //         // struct Driver* randomDriver = gGT->drivers[randomDriverIndex];
            
    //         // if (randomDriver != NULL) {
    //         //     VehPickupItem_ShootNow(randomDriver, ITEM_EXPLOSIVE_CRATE, 0);
    //         // }

    //         //Make all drivers to throw an item
    //         for (unsigned char i = 0; i < gGT->numPlyrCurrGame + gGT->numBotsNextGame; i++) {
    //             struct Driver* driver = gGT->drivers[i];
    //             if (driver != NULL && (driver->kartState == KS_NORMAL || driver->kartState == KS_DRIFTING)) {
    //                 VehPickupItem_ShootNow(driver, ITEM_EXPLOSIVE_CRATE, 0);
    //             }
    //         }
    //     } else {
    //         itemTimer--;
    //     }
    // }