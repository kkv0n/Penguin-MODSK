#include <common.h>

#ifdef USE_GASMOXIAN
#include "../AltMods/Gasmoxian/global.h"
#include "../AltMods/Gasmoxian/utils.h"

//for air throw
int bossflag;

	
#endif

enum ItemSet
{
	ITEMSET_Race1=0,
	ITEMSET_Race2,
	ITEMSET_Race3,
	ITEMSET_Race4,
	ITEMSET_BattleDefault,
	ITEMSET_BattleCustom,

	// these two swapped,
	// for the sake of array
	ITEMSET_BossRace,
	ITEMSET_CrystalChallenge
};

// all except CrystalChallenge
extern char* charPtr[7];
extern char numWeapons[7];

// Itemset infographic (outdated):
// https://discord.com/channels/330945093416779787/550106151887568906/734368526294450267
void DECOMP_VehPhysGeneral_SetHeldItem(struct Driver* driver) {
	u_int rng;
	int itemSet;
	char item;
	char bossFails;
	struct GameTracker* gGT;

	gGT = sdata->gGT;

	// 6th Itemset (Battle Mode Custom Itemset)
	itemSet = ITEMSET_BattleCustom;

	// 5th Itemset (Battle Mode Default Itemset, 0x34de)
	if (gGT->battleSetup.enabledWeapons == 0x34de)
		itemSet = ITEMSET_BattleDefault;
	
#ifdef USE_GASMOXIAN
if (gGT->gameMode1 & ARCADE_MODE || gGT->gameMode1 & ADVENTURE_BOSS)
{
	//start of itemset assignation
	
	int lastplace = octr->NumDrivers - 1;
	
	//just in case something breaks then assign itemset1 to this player
	itemSet = ITEMSET_Race1; //Rank 0

	const enum ItemSet itemSets[] = {
		ITEMSET_Race2, // Rank 1
		ITEMSET_Race2, // Rank 2
		ITEMSET_Race3, // Rank 3
		ITEMSET_Race3, // Rank 4
		ITEMSET_Race4, // Rank 5
		ITEMSET_Race4  // Rank 6
	};

	// Change charPtr RNG weights if ITEM_CHAOS is enabled
	extern void ItemChaosItemSets();
	ItemChaosItemSets();

	extern void BossRaceItemSets();
	BossRaceItemSets();
		
	if (driver->driverRank == 0)
	{
		itemSet = ITEMSET_Race1;
	}
	//assign itemsets based on the driverrank, last place always get battledefault, if more than 5 players
	if (driver->driverRank >= 1 && driver->driverRank <= 7) {

		if(octr->NumDrivers > 5){
			itemSet = (driver->driverRank == lastplace) 
					? ITEMSET_BattleDefault 
					: itemSets[driver->driverRank - 1];
		} else{
			itemSet = (driver->driverRank == lastplace) 
				? ITEMSET_Race4
				: itemSets[driver->driverRank - 1];
		}
	}

	//end of itemset assignation
}
#else
	// Not in Battle Mode
	if ((gGT->gameMode1 & BATTLE_MODE) == 0)
	{
		// 7th Itemset (Crystal Challenge)
		itemSet = ITEMSET_CrystalChallenge;

		// Not in Crystal Challenge
		if ((gGT->gameMode1 & CRYSTAL_CHALLENGE) == 0)
		{
			// Choose Itemset based on number of Drivers
			int mode = gGT->numPlyrCurrGame + gGT->numBotsNextGame;




			switch(mode)
			{
				// if boss race
				case 2:

					// boss race, last place
					itemSet = ITEMSET_BossRace;

					// if in first place
					if (driver->driverRank == 0)
					{
						Itemset1:
						// 1st Itemset
						itemSet = ITEMSET_Race1;
					}
					break;

				// 3P VS race
				case 3:

					// if first place
					if (driver->driverRank == 0) goto Itemset1;

					// default (2nd or 3rd place)
					itemSet = ITEMSET_Race4;

					// 50/50 chance of an upgrade,
					// while in 2nd place

					if (driver->driverRank == 1)
					{
						itemSet = ITEMSET_Race3;
						rng = DECOMP_MixRNG_Scramble();
						if (rng & 1) goto Itemset2;
					}

					break;
				case 4:
					itemSet = driver->driverRank;
					break;
				case 5:
					itemSet = driver->driverRank;
					// 5th rank is 4th Itemset
					if (itemSet == 4) itemSet = 3;
					break;

				// 2P Arcade
				case 6:

					// careful, dont get confused by names
					itemSet = driver->driverRank;

					// if 1st place, ItemSet1
					if (itemSet == 0) goto Itemset1;

					// if 6th place, ItemSet4
					if (itemSet == 5) itemSet = ITEMSET_Race4;

					// 2nd, 3rd place, gets 2nd Itemset
					// 4th, 5th place, gets 3rd Itemset
					else itemSet = (itemSet - 1)/2 + 1;

					break;

				// 1P Arcade
				case 8:

					// 0,1 = 0 (itemset1)
					// 2,3 = 1 (itemset2)
					// 4,5 = 2 (itemset3)
					// 6,7 = 3 (itemset4)
					itemSet = driver->driverRank >> 1;

					// if in 2nd place, get itemSet2
					if (itemSet == 1)
					{
						Itemset2:
						itemSet = ITEMSET_Race2;
					}
			}
		}

		// if you have 4th-place itemset on first lap,
		// then override to 3rd place
		if (itemSet == ITEMSET_Race4 && driver->lapIndex == 0)
			itemSet = ITEMSET_Race3;
	}
#endif

	// Decide item for Driver
	rng = (DECOMP_MixRNG_Scramble() >> 0x3) % 0xc8;

	// number of weapons for RNG
	numWeapons[ITEMSET_BattleCustom] = gGT->battleSetup.numWeapons;

	switch(itemSet)
	{
		case ITEMSET_Race1:
		case ITEMSET_Race2:
		case ITEMSET_Race3:
		case ITEMSET_Race4:
		case ITEMSET_BattleDefault:
		case ITEMSET_BossRace:
			driver->heldItemID = charPtr[itemSet][(rng * numWeapons[itemSet]) / 0xc8];
			break;

		// uses int array instead of char,
		// should fix that later, requires 230 rewrite
		case ITEMSET_BattleCustom:
			driver->heldItemID = ((int*)charPtr[itemSet])[(rng * numWeapons[itemSet]) / 0xc8];
			break;

#ifndef USE_GASMOXIAN
		case ITEMSET_CrystalChallenge:
			// Item is bomb at Rocky Road, Nitro Court
			// Item is turbo at Skull Rock and Rampage Ruins
			item = ITEM_BOWLING_BOMB;
			if (gGT->levelID != SKULL_ROCK && gGT->levelID != RAMPAGE_RUINS) goto SetItem;
			driver->heldItemID = ITEM_TURBO_BOOST;
			break;
#endif

		// "-1st place": Undecided rank
		default:
			rng = DECOMP_MixRNG_Scramble();
			item = (char)rng + -0xc*((char)(rng / 6 + (rng >> 0x1f) >> 1) - (char)(rng >> 0x1f));
			SetItem:
			driver->heldItemID = item;
	}

#ifdef USE_GASMOXIAN
if (gGT->gameMode1 & ARCADE_MODE || gGT->gameMode1 & ADVENTURE_BOSS)
{
	// ITEM CHAOS
	if(USE_ITEM_CHAOS){
		extern void ItemChaosSetHeldItem(struct Driver * driver);
		ItemChaosSetHeldItem(driver);
	}

	// In BOSS RACE allow air throw
	if(USE_BOSS_RACE && driver->driverRank == 0){
		if(driver->heldItemID == ITEM_EXPLOSIVE_CRATE || driver->heldItemID == ITEM_N_BRIO_BEAKER){
			extern bool air_throw;
			if (rand() % 2 == 0) {
				air_throw = true;
			} else {
				air_throw = false;
			}
		}
	}

	// Avoid Ghost BUG on mirror mode
	if (USE_MIRROR && driver->heldItemID == ITEM_INVISIBILITY) {
		driver->heldItemID = ITEM_TURBO_BOOST;		
	}

	// If only NORMAL its enabled, ban invisibility, nothing and super engine
	extern bool ItsOnlyNormalEnabled();
	if(ItsOnlyNormalEnabled())
	{
		if (driver->heldItemID == ITEM_INVISIBILITY || driver->heldItemID == ITEM_NONE || driver->heldItemID == ITEM_SUPER_ENGINE)
		{
			driver->heldItemID = ITEM_TURBO_BOOST;
		}
	}

	// If shortcutless ban nothing
	if (USE_SHORTCUTLESS && driver->heldItemID == ITEM_NOTHING) {
		driver->heldItemID = ITEM_TURBO_BOOST;
	}


	if (octr->warpclock == 0) 
	{
		// if retrofueled, ngin labs or oxide station then replace warpball with an clock

		// if (USE_RETRO_FUELED || octr->levelID == N_GIN_LABS || octr->levelID == OXIDE_STATION) {
		// 	if (driver->heldItemID == ITEM_WARP_ORB){
		// 		driver->heldItemID = ITEM_N_TROPY_CLOCK;
		// 	}
		// }
	}

	//if someone drop an orb or clock
	if (octr->warpclock == 1) {
		if (!USE_ITEM_CHAOS){
			//give super engine to players when the warpball/clock are in cooldown
			if (driver->heldItemID == ITEM_WARP_ORB || driver->heldItemID == ITEM_N_TROPY_CLOCK) {
				driver->heldItemID = ITEM_SUPER_ENGINE;
			}
		}
	}

	// allow super engine for the last 3 players
	if (
		driver->heldItemID == ITEM_SUPER_ENGINE
		&& driver->driverRank < octr->NumDrivers - 3
		&& (gGT->gameMode1 & ARCADE_MODE) != 0
	) {
		driver->heldItemID = ITEM_POWER_SHIELD; //if not last 2 players then replace super engine with shield
	}

	//ban orbs,clocks and super engine on lap 1, replace with mask
	if (
		(driver->heldItemID == ITEM_N_TROPY_CLOCK
		|| driver->heldItemID == ITEM_WARP_ORB
		|| driver->heldItemID == ITEM_SUPER_ENGINE
		) && driver->lapIndex == 0
		&& !USE_ITEM_CHAOS
	) {
		driver->heldItemID = ITEM_MASK;
	}
}

// if boss race special
if (USE_BOSS_RACE){
	// if first place
	if (driver->driverRank == 0){ 
			// driver->numHeldItems = 7;
			// driver->numWumpas = 0;
			
			// if (driver->heldItemID == ITEM_POWER_SHIELD || driver->heldItemID == ITEM_TURBO_BOOST) {
			// 	driver->heldItemID = ITEM_BOWLING_BOMB;
			// }
	}

	if (driver->driverRank > 0) {	
		if (driver->heldItemID == ITEM_N_BRIO_BEAKER || driver->heldItemID == ITEM_BOWLING_BOMB || driver->heldItemID == ITEM_EXPLOSIVE_CRATE) {
			driver->heldItemID = ITEM_TRACKING_MISSILE;		
		}
		// else if (driver->heldItemID == ITEM_INVISIBILITY) {
		// 	driver->heldItemID = ITEM_POWER_SHIELD;
		// }
		// else if (driver->heldItemID == ITEM_TURBO_BOOST) {
		// driver->heldItemID = ITEM_TRACKING_MISSILE_X3;
		// }
		// else if (driver->heldItemID == ITEM_N_TROPY_CLOCK || driver->heldItemID == ITEM_WARP_ORB) {
		// driver->heldItemID = ITEM_SUPER_ENGINE;
		// }
	}
}

#else
	// In Boss race
	if (gGT->gameMode1 & ADVENTURE_BOSS)
	{
		bossFails = sdata->advProgress.timesLostBossRace[gGT->bossID];

		if (bossFails < 3)
		{
			// Replace Clock, Mask,  with 3 Missiles
			if ((u_int)driver->heldItemID - 0x7 < 0x3)
				driver->heldItemID = ITEM_TRACKING_MISSILE_X3;
		}

		else if (bossFails < 4)
		{
			// Replace Clock, Mask with 3 Missiles
			if ((u_int)driver->heldItemID - 0x7 < 0x2)
				driver->heldItemID = ITEM_TRACKING_MISSILE_X3;
		}

		else if (bossFails < 5 && driver->heldItemID == ITEM_N_TROPY_CLOCK)
		{
			// Replace Clock with 3 Missiles
			driver->heldItemID = ITEM_TRACKING_MISSILE_X3;
		}

		// Replace 3 Missiles with 1 Missile if racing Komodo Joe
		if (gGT->levelID == DRAGON_MINES && driver->heldItemID == ITEM_TRACKING_MISSILE_X3)
			driver->heldItemID = ITEM_TRACKING_MISSILE;

#if 0
	// === Removed ND Code ===
	// Spring is not in the RNG anyway

	// Replace unused Spring item with Turbo
	if (driver->heldItemID == 0x5)
		driver->heldItemID = 0x0;
#endif

	// Make sure only 1 Warpball is instanced at once
	if (driver->heldItemID == ITEM_WARP_ORB)
	{
		// if nobody has warpball, then set flag that somebody has it
		if ((gGT->gameMode1 & WARPBALL_HELD) == 0)
			gGT->gameMode1 |= WARPBALL_HELD;

		// if somebody has warpball already, then give 3 missiles
		else driver->heldItemID = ITEM_TRACKING_MISSILE_X3;
	}

	if (
			// if you got 3 missiles
			driver->heldItemID == ITEM_TRACKING_MISSILE_X3 &&

			// if more than 2 players
			gGT->numPlyrCurrGame > 2 &&

			// if not in battle mode
			((gGT->gameMode1 & BATTLE_MODE) == 0)
		)
	{
		// if less than 2 drivers have 3 missiles, then increase number of drivers that have it
		if (gGT->numPlayersWith3Missiles < 2)
			gGT->numPlayersWith3Missiles++;

		// if 2 drivers already have 3 missiles, now you have 1 missile
		else driver->heldItemID = ITEM_TRACKING_MISSILE;
	}
#endif
	// Set number of held items
	if ((u_int)driver->heldItemID - 0xA < 0x2)
		driver->numHeldItems = 3;

	return;
}

char* charPtr[7] =
{
	&data.RNG_itemSetRace1[0], // Rank 0
	&data.RNG_itemSetRace2[0], // Rank 1,2
	&data.RNG_itemSetRace3[0], // Rank 3,4
	&data.RNG_itemSetRace4[0], // Rank 5,6
	&data.RNG_itemSetBattleDefault[0], // Rank 7
	(char *)&sdata_static.gameTracker.battleSetup.RNG_itemSetCustom[0],
	&data.RNG_itemSetBossrace[0]
};

char numWeapons[7] =
{
	0x14,
	0x34,
	0x14,
	0x13,
	0x14,
	-1,
	0x14
};