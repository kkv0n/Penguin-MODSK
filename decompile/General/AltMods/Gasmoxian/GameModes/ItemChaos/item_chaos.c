#include <common.h>
#include "../../utils.h"

int numTimesOrbWeaponUsed = 0;

//Rank 0
char ItemChaos_RNG_itemSetRace1[] = {
	// 1/20 Bomb (1)
	1,
	// 9/20 Crate (3)
	3,3,3,3,3,3,3,3,3,
	// 6/20 Beaker (4)
	4,4,4,4,4,4,
	// 4/20 Shield (6)
	6,6,6,6
};

// Rank 1,2
char ItemChaos_RNG_itemSetRace2[] = {
    // 5/52 Turbo (0)
    0,0,0,0,0,
    // 2/52 Bomb (1)
    1,1,
    // 5/52 Missile (2)
    2,2,2,2,2,
    // 11/52 Crate (3)
    3,3,3,3,3,3,3,3,3,3,
    // 9/52 Beaker (4)
    4,4,4,4,4,4,4,4,4,
	// 3/52 Spring (5)
	14,14,14,
    // 7/52 Shield (6)
    6,6,6,6,6,6,6,6,
    // 3/52 Mask (7)
    7,7,7,
    // 3/52 Warp Orb (9)
    9,9,9,
    // 3/52 Invisibility (12)
    12,12,12
};

// Rank 3,4
char ItemChaos_RNG_itemSetRace3[] = {
    // 1/20 Turbo (0)
    0,
    // 3/20 Missile (2)
    2,2,2,
    // 1/20 Crate (3)
    3,
	// 2/20 Spring (5)
	14,14,
    // 2/20 Shield (6)
    6,6,
    // 3/20 Mask (7)
    7,7,7,7,
	// 1/20 Clock (8)
    8,
    // 4/20 Warp Orb (9)
    9,9,9,9,
	// 2/20 Invincibility (12)
	12,12
};

// Rank 5,6
char ItemChaos_RNG_itemSetRace4[] = {
    // 1/20 Turbo (0)
    0,
    // 1/20 Missile (2)
    2,
	// 1/20 Spring (5)
	14,
    // 6/20 Mask (7)
    7,7,7,7,7,7,
    // 2/20 Clock (8)
    8,8,
    // 5/20 Warp Orb (9)
    9,9,9,9,9,
    // 4/20 Super Engine (13)
    13,13,13,13
};

// Rank 7
char ItemChaos_RNG_itemSetBattleDefault[] = {
    // 6/20 Mask (7)
    7,7,7,7,7,7,
    // 2/20 Clock (8)
    8,8,
    // 6/20 Warp Orb (9)
    9,9,9,9,9,9,
    // 5/20 Super Engine (13)
    13,13,13,13,13
};

extern char* charPtr[7];
void ItemChaosItemSets(void) {

	if(USE_ITEM_CHAOS){
		// Directly modify each element of the global charPtr array
		charPtr[0] = &ItemChaos_RNG_itemSetRace1[0]; // Rank 0
		charPtr[1] = &ItemChaos_RNG_itemSetRace2[0]; // Rank 1,2
		charPtr[2] = &ItemChaos_RNG_itemSetRace3[0]; // Rank 3,4
		charPtr[3] = &ItemChaos_RNG_itemSetRace4[0]; // Rank 5,6
		charPtr[4] = &ItemChaos_RNG_itemSetBattleDefault[0]; // Rank 7
		
		// Leave these as default (unused)
		charPtr[5] = (char*)&sdata_static.gameTracker.battleSetup.RNG_itemSetCustom[0];
		charPtr[6] = &data.RNG_itemSetBossrace[0];
	}else{
		charPtr[0] = &data.RNG_itemSetRace1[0]; // Rank 0
		charPtr[1] = &data.RNG_itemSetRace2[0]; // Rank 1,2
		charPtr[2] = &data.RNG_itemSetRace3[0]; // Rank 3,4
		charPtr[3] = &data.RNG_itemSetRace4[0]; // Rank 5,6
		charPtr[4] = &data.RNG_itemSetBattleDefault[0]; // Rank 7
		charPtr[5] = (char*)&sdata_static.gameTracker.battleSetup.RNG_itemSetCustom[0];
		charPtr[6] = &data.RNG_itemSetBossrace[0];
	}
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

void ItemChaosSetHeldItem(struct Driver * driver){

	// if(
	// 	driver->driverRank == octr->NumDrivers - 1
	// 	&&
	// 	(
	// 	driver->heldItemID == ITEM_TRACKING_MISSILE_X3
	// 	|| driver->heldItemID == ITEM_BOWLING_BOMB_X3
	// 	)
	// ){
	// 	driver->heldItemID = ITEM_WARP_ORB;
	// }

	//Increment of item amount
	char increment = driver->lapIndex == gGT->numLaps - 1 ? 1 : 0;

	// Check if there are already maximum warp orbs active
	char maxWarpOrbs = octr->NumDrivers < 6 ? 4 : 3;
	maxWarpOrbs += increment;
	if (driver->heldItemID == ITEM_WARP_ORB && CountActiveWarpOrbs() >= maxWarpOrbs) {
		driver->heldItemID = ITEM_TURBO_BOOST;
	}

	if(driver->heldItemID == ITEM_EXPLOSIVE_CRATE){
		driver->numHeldItems = rand() % 3 + 1 + increment;

		if (rand() % 2 == 0) {
			air_throw = true;
		} else {
			air_throw = false;
		}
	}

	else if(driver->heldItemID == ITEM_N_BRIO_BEAKER){
		driver->numHeldItems = rand() % 5 + 1;
	}

	else if(driver->heldItemID == ITEM_TRACKING_MISSILE){
		driver->numHeldItems = rand() % 5 + 1;
	}

	else if(driver->heldItemID == ITEM_POWER_SHIELD){
		if(octr->NumDrivers > 5){
			// 75% chance of 1, 15% chance of 2, 5% chance of 3
			driver->numHeldItems = (rand() % 100 < 75) ? 1 : (rand() % 100 < 90) ? 2 : 3;
		}else{
			//85% chance of 1, 15% chance of 2
			driver->numHeldItems = (rand() % 100 < 85) ? 1 : 2;
		}
	}

	else if(driver->heldItemID == ITEM_MASK){
		// 85% chance of 1, 10% chance of 2, 5% chance of 3
		driver->numHeldItems = (rand() % 100 < 85) ? 1 : (rand() % 100 < 95) ? 2 : 3 + increment;
	}

	if(octr->NumDrivers < 6){
		if(driver->heldItemID == ITEM_WARP_ORB){
			// 80% chance of 1, 20% chance of 2
			driver->numHeldItems = (rand() % 5 == 0) ? 2 : 1;
		}
	}

	if(driver->heldItemID == ITEM_NOTHING){
		// 60% chance of 1, 25% chance of 2, 15% chance of 3
		driver->numHeldItems = (rand() % 100 < 60) ? 1 : (rand() % 100 < 85) ? 2 : 3;
	}

	// Restrict orbs and clock abuse
	// This is to avoid players to keep smashing crates and get constant items

	if(driver->heldItemID == ITEM_WARP_ORB){
		if(numTimesOrbWeaponUsed > max(gGT->numLaps + 1, 7)){
			driver->heldItemID = ITEM_TURBO_BOOST;
			return;
		}
	}

	if(driver->heldItemID == ITEM_N_TROPY_CLOCK){
		if(driver->numTimesClockWeaponUsed >  max(gGT->numLaps, 4)){
			driver->heldItemID = ITEM_TURBO_BOOST;
			return;
		}
	}
}

void InitItemChaos(bool enabled) {
    if(!enabled) return;

	struct Driver* driver = gGT->drivers[0];
    if(driver == NULL) return;

	driver->numTimesClockWeaponUsed = 0;
    numTimesOrbWeaponUsed = 0;
}

void HandleItemChaos(bool enabled){
    if(!enabled) return;

    struct Driver* driver = gGT->drivers[0];
    if(driver == NULL) return;

    //Handle air throw (komodo TNTs)
    if(driver->heldItemID == ITEM_NONE // If no item is held
		|| gGT->gameMode1 & ROLLING_ITEM // If rolling item
		|| driver->thCloud != NULL // If hit a red potion
	){
		air_throw = false;
	}
	extern int bossflag;
	extern int bossrace;
	bossflag = bossrace || air_throw ? 2 : 0;

    //Give player infinte wumpas on last lap
	if (driver->lapIndex == gGT->numLaps - 1) {
		driver->numWumpas = 99;
	}
}