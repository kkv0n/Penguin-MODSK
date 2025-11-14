#include <common.h>
#include "header/bot.h"

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



void VehPhysGeneral_SetHeldItem(struct Driver* d) {
	u_int rng;
	int itemSet;
	char item;
	char bossFails;
	struct GameTracker* gGT;

	gGT = sdata->gGT;
	
	unsigned char* current_itemset;
	
	unsigned char itemset_Rank1[20] = {4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 0, 6};
	
	unsigned char itemset_Rank2[20] = {4, 4, 3, 3, 1, 9, 10, 11, 6, 9, 2, 2, 8, 11, 11, 9, 7, 7, 7, 8};
	

	// 6th Itemset (Battle Mode Custom Itemset)
	itemSet = ITEMSET_BattleCustom;

	// 5th Itemset (Battle Mode Default Itemset, 0x34de)
	if (gGT->battleSetup.enabledWeapons == 0x34de)
		itemSet = ITEMSET_BattleDefault;

	// Not in Battle Mode
	if ((gGT->gameMode1 & BATTLE_MODE) == 0)
	{
		// 7th Itemset (Crystal Challenge)
		itemSet = ITEMSET_CrystalChallenge;

		// Not in Crystal Challenge
		if ((gGT->gameMode1 & CRYSTAL_CHALLENGE) == 0)
		{
			
			int mode = d->driverRank; //keep 1 case for all players
			itemSet = ITEMSET_Race1;

			switch(mode)
			{
				// if boss race
				case 0:
				{

						 //assign itemset1 (actually it is changed for a custom one later)
						 current_itemset = &itemset_Rank1;
					
					break;
				}
					
					case 1:
					{
						current_itemset = &itemset_Rank2;
					}

			}
		}

		// if you have 4th-place itemset on first lap,
		// then override to 3rd place
		if (itemSet == ITEMSET_Race4 && d->lapIndex == 0)
			itemSet = ITEMSET_Race3;
	}

	// Decide item for Driver
	rng = (MixRNG_Scramble() >> 0x3) % 0xc8;


	switch(itemSet)
	{
		case ITEMSET_Race1:
		case ITEMSET_Race2:
		case ITEMSET_Race3:
		case ITEMSET_Race4:
		case ITEMSET_BattleDefault:
		case ITEMSET_BossRace:
			d->heldItemID = current_itemset[(rng * 20) / 0xc8];
			break;

		// uses int array instead of char,
		// should fix that later, requires 230 rewrite
		case ITEMSET_BattleCustom:
			d->heldItemID = current_itemset[(rng * 20) / 0xc8];
			break;

		case ITEMSET_CrystalChallenge:
			// Item is bomb at Rocky Road, Nitro Court
			// Item is turbo at Skull Rock and Rampage Ruins
			item = 0x1;
			if (gGT->levelID != SKULL_ROCK && gGT->levelID != RAMPAGE_RUINS) goto SetItem;
			d->heldItemID = 0x0;
			break;

		// "-1st place": Undecided rank
		default:
			rng = MixRNG_Scramble();
			item = (char)rng + -0xc*((char)(rng / 6 + (rng >> 0x1f) >> 1) - (char)(rng >> 0x1f));
			SetItem:
			d->heldItemID = item;
	}

/*
	// In Boss race
	if (gGT->gameMode1 & ADVENTURE_BOSS)
	{
		bossFails = sdata->advProgress.timesLostBossRace[gGT->bossID];

		if (bossFails < 0x3)
		{
			// Replace Clock, Mask,  with 3 Missiles
			if ((u_int)d->heldItemID - 0x7 < 0x3)
				d->heldItemID = 0xb;
		}

		else if (bossFails < 0x4)
		{
			// Replace Clock, Mask with 3 Missiles
			if ((u_int)d->heldItemID - 0x7 < 0x2)
				d->heldItemID = 0xb;
		}

		else if (bossFails < 0x5 && d->heldItemID == 0x8)
		{
			// Replace Clock with 3 Missiles
			d->heldItemID = 0xb;
		}

		// Replace 3 Missiles with 1 Missile if racing Komodo Joe
		if (gGT->levelID == DRAGON_MINES && d->heldItemID == 0xb)
			d->heldItemID = 0x2;
	}
	*/

#if 0
	// === Removed ND Code ===
	// Spring is not in the RNG anyway

	// Replace unused Spring item with Turbo
	if (d->heldItemID == 0x5)
		d->heldItemID = 0x0;
#endif

	// Make sure only 1 Warpball is instanced at once
	if (d->heldItemID == 0x9)
	{
		// if nobody has warpball, then set flag that somebody has it
		if ((gGT->gameMode1 & WARPBALL_HELD) == 0)
			gGT->gameMode1 |= WARPBALL_HELD;

		// if somebody has warpball already, then give 3 missiles
		//else d->heldItemID = 0xb;
	}

	if (
			// if you got 3 missiles
			d->heldItemID == 0xb &&

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
		else d->heldItemID = 0x2;
	}

	// Set number of held items
	if ((u_int)d->heldItemID - 0xA < 0x2)
		d->numHeldItems = 0x3;

	return;
}
