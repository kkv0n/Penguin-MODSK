#include <common.h>
extern void DECOMP_UI_DrawRankedDrivers(void);
extern bool bossrace;
#ifdef USE_ONLINE
#include "../AltMods/OnlineCTR/global.h"
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

	// Not in Battle Mode
	if ((gGT->gameMode1 & BATTLE_MODE) == 0)
	{
		

		// Not in Crystal Challenge
		if ((gGT->gameMode1 & CRYSTAL_CHALLENGE) == 0)
		{
			// Choose Itemset based on number of Drivers
			int mode = gGT->numPlyrCurrGame + gGT->numBotsNextGame;
//enabled items in all rooms
	#ifdef USE_ONLINE
    mode = octr->NumDrivers;

             bossrace = 0;
			if (octr->NumDrivers >= 1 && octr->NumDrivers <= 7) 
			{
		
	 
				mode = octr->NumDrivers + 2;
			}
            
			#endif

			switch(mode)
			{
				//all the itemsets asigned to the players
				//maybe this should be in #ifdef use_online
				//but at this point it doesnt matter for us bc we are a fork
				
				// if boss race
				case 2:
					
					// if in first place
					if (driver->driverRank == 0)
					{
						
						// 1st Itemset
						itemSet = ITEMSET_Race1;
					}
	
if (driver->driverRank != 0) {


	itemSet = ITEMSET_Race2;
	
}							

					break;

				// 1vs1 race
				case 3:

					// if first place
					if (driver->driverRank == 0)
					{
								  
						itemSet = ITEMSET_Race1;
					}

					if (driver->driverRank == 1)
					{
						itemSet = ITEMSET_Race4;
									 
								 
					}

					break;
	 
	  //3P Online
				case 4:
					if (driver->driverRank == 0)
					{
						itemSet = ITEMSET_Race1;
					}
					if (driver->driverRank == 1)
					{
						itemSet = ITEMSET_Race4;
					}
					if (driver->driverRank == 2)
					{
						itemSet = ITEMSET_BattleDefault;
	                }
					
					break;
	 
	  //4P Online
				case 5:
					if (driver->driverRank == 0)
					{
						itemSet = ITEMSET_Race1;
					}
					if (driver->driverRank == 1)
					{
						itemSet = ITEMSET_Race3;
					}
					if (driver->driverRank == 2)
					{
						itemSet = ITEMSET_Race4;
					}
					if (driver->driverRank == 3)
					{
						itemSet = ITEMSET_BattleDefault;
					}
					
		   				break;

				// 5P Online
		   case 6:
					if (driver->driverRank == 0)
					{
						itemSet = ITEMSET_Race1;
					}
					if (driver->driverRank == 1)
					{
						itemSet = ITEMSET_Race2;
					}
					if (driver->driverRank == 2)
					{
						itemSet = ITEMSET_Race3;
					}
					if (driver->driverRank == 3)
					{
						itemSet = ITEMSET_Race4;
					}
					if (driver->driverRank == 4)
					{
						itemSet = ITEMSET_BattleDefault;
					}

		   					break;

				// 6P Online
				case 7:
					if (driver->driverRank == 0)
					{
						itemSet = ITEMSET_Race1;
					}
					if (driver->driverRank >= 1 && driver->driverRank <= 2)
					{
						itemSet = ITEMSET_Race2;
					}
					if (driver->driverRank == 3)
					{
						itemSet = ITEMSET_Race3;
					}
					if (driver->driverRank == 4)
					{
						itemSet = ITEMSET_Race4;
					}
					if (driver->driverRank == 5)
					{
						itemSet = ITEMSET_BattleDefault;
				    }		
	                  break;
		  
				
		   
			     // 7P Online
				     case 8:
               // if player is in last place				
					itemSet = ITEMSET_BattleDefault;
					
					if (driver->driverRank == 0)
					{
						itemSet = ITEMSET_Race1;
					}
					if (driver->driverRank >= 1 && driver->driverRank <= 2)
					{
						itemSet = ITEMSET_Race2;
					}
					if (driver->driverRank >= 3 && driver->driverRank <= 4)
					{
						itemSet = ITEMSET_Race3;
					}
					if (driver->driverRank == 5)
					{
						itemSet = ITEMSET_Race4;
					}
					if (driver->driverRank == 6)
					{
						itemSet = ITEMSET_BattleDefault;
					}
				
		              break;
				
				     case 9:
					// if player is in last place x2
					itemSet = ITEMSET_BattleDefault;

					if (driver->driverRank == 0)
					{
						itemSet = ITEMSET_Race1;
					}
					if (driver->driverRank >= 1 && driver->driverRank <= 3)
					{
						Itemset2:
						itemSet = ITEMSET_Race2;
					}
					if (driver->driverRank >= 4 && driver->driverRank <= 5)
					{
						itemSet = ITEMSET_Race3;
					}
					if (driver->driverRank == 6)
					{
						itemSet = ITEMSET_Race4;
					}
					if (driver->driverRank == 7)
					{
						itemSet = ITEMSET_BattleDefault;
					}					 
			}
			//end of the players itemsets asignation
		}

         //anzu changes in itemset logic
							   
		if (itemSet == ITEMSET_BattleDefault && driver->lapIndex == 0)
			itemSet = ITEMSET_Race4;
	}

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


		// "-1st place": Undecided rank
		default:
			rng = DECOMP_MixRNG_Scramble();
			item = (char)rng + -0xc*((char)(rng / 6 + (rng >> 0x1f) >> 1) - (char)(rng >> 0x1f));
			SetItem:
			driver->heldItemID = item;
	}

#ifdef USE_ONLINE
if (gGT->gameMode1 & ARCADE_MODE)
{
	//avoid ghost bug
	if (octr->special == 1 ) {
	 if (driver->heldItemID == 0xc)
	{
		//nothing item is back!
		driver->heldItemID = 0xe;
	}
		
	}
	else if (octr->special == 0 && driver->heldItemID == 0xc)
	{
		driver->heldItemID = 0x7;
	}
// if retrofueled, ngin labs or oxide station then replace warpball with an clock
if (octr->warpclock == 0) 
{
	if (octr->NumDrivers >= 5 && driver->driverRank < 5)
	{
		if (driver->heldItemID == 0x9 || driver->heldItemID == 0x8)
		{
			driver->heldItemID = 0x6;
		}
	}
    else if (octr->special == 5 || octr->levelID == N_GIN_LABS || octr->levelID == OXIDE_STATION) 
    {
	if (driver->heldItemID == 0x9)
	{
		driver->heldItemID = 0x8;
	}
}
}
//if someone drop an orb or clock
else if (octr->warpclock == 1)
{

	if (driver->heldItemID == 0x9 || driver->heldItemID == 0x8) {
		driver->heldItemID = 0xd;
	}
}
}
// if boss race special
if (gGT->gameMode1 & ADVENTURE_BOSS)
	{

if (driver->driverRank == 0)
					{
						
		  bossrace = 1;  
	      driver->numHeldItems = 0x6;
		  driver->numWumpas = 0;
		  
		  
			if (driver->heldItemID == 0x6 || driver->heldItemID == 0x0) {

				driver->heldItemID = 0x1;
			}
					}
if (driver->driverRank != 0) {
	
	 bossrace = 0;
	 
	if (driver->heldItemID == 0x4 || driver->heldItemID == 0x1 || driver->heldItemID == 0x3) {
                
				driver->heldItemID = 0x2;

				
			}
			else if (driver->heldItemID == 0xc) {
				driver->heldItemID = 0x6;
			}
			else if (driver->heldItemID == 0x0) {
            driver->heldItemID = 0xb;
			}
			else if (driver->heldItemID == 0x8 || driver->heldItemID == 0x9) {
            driver->heldItemID = 0x5;
			}
		}
	}
#else
	// In Boss race
	if (gGT->gameMode1 & ADVENTURE_BOSS)
	{
		bossFails = sdata->advProgress.timesLostBossRace[gGT->bossID];

		if (bossFails < 0x3)
		{
			// Replace Clock, Mask,  with 3 Missiles
			if ((u_int)driver->heldItemID - 0x7 < 0x3)
				driver->heldItemID = 0xb;
		}

		else if (bossFails < 0x4)
		{
			// Replace Clock, Mask with 3 Missiles
			if ((u_int)driver->heldItemID - 0x7 < 0x2)
				driver->heldItemID = 0xb;
		}

		else if (bossFails < 0x5 && driver->heldItemID == 0x8)
		{
			// Replace Clock with 3 Missiles
			driver->heldItemID = 0xb;
		}

		// Replace 3 Missiles with 1 Missile if racing Komodo Joe
		if (gGT->levelID == DRAGON_MINES && driver->heldItemID == 0xb)
			driver->heldItemID = 0x2;
	}
#endif
#if 0
	// === Removed ND Code ===
	// Spring is not in the RNG anyway

	// Replace unused Spring item with Turbo
	if (driver->heldItemID == 0x5)
		driver->heldItemID = 0x0;
#endif

	// Set number of held items
	if ((u_int)driver->heldItemID - 0xA < 0x2)
		driver->numHeldItems = 0x3;

	return;
}

char* charPtr[7] =
{
	&data.RNG_itemSetRace1[0],
	&data.RNG_itemSetRace2[0],
	&data.RNG_itemSetRace3[0],
	&data.RNG_itemSetRace4[0],
	&data.RNG_itemSetBattleDefault[0],
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