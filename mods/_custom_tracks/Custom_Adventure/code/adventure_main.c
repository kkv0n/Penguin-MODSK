#include <common.h>
#include "adventure.h"

unsigned char current_track;
unsigned char load_track;
unsigned char laps;
bool unlock_track;
bool ttrack_finished;
bool slidec_finished;
bool _endgame;
bool show_stars;
char* timeToWin;
bool hardcore = true; //enable this by default
char* difficulty;

extern const char* character_names[NUM_CHARACTERS];

void adventure_options();

//main menu
struct MenuRow adventure_row[2] =
{
	[0] =
	{
	  .stringIndex = 0x4c, //adventure mode text
	  .rowOnPressUp = 0,
	  .rowOnPressDown = 0,
	  .rowOnPressLeft = 0,
	  .rowOnPressRight = 0,
	},


	[1] =
	{
	 .stringIndex = 0xFFFF,
	}
};


void custom_adventure(unsigned char levelID, bool boss)
{
	unsigned char i;
	
	prev_levID = levelID; //save the original warp pad level to fix slide col & turbo track bug

	//just in case
	if ((sdata->gGT->gameMode1 & (CRYSTAL_CHALLENGE | RELIC_RACE)) != 0)
	{
		sdata->gGT->gameMode1 &= ~(CRYSTAL_CHALLENGE);
		sdata->gGT->gameMode1 &= ~(RELIC_RACE);
		sdata->gGT->gameMode1 |= ADVENTURE_MODE;

	}


	if (boss)
	{
#define OXIDE_GARAGE_ (levelID == 0)

		current_track = (OXIDE_GARAGE_) ? FINAL_BOSS : CITADEL_5 + levelID; //levelID in this call is just the hubID, check AH_Garage_ThTick
		adventure_options();
		sdata->gGT->numLaps = laps % 128;
		MainRaceTrack_RequestLoad(load_track);
		return;
	}


	unsigned char warppad_id[WARPPAD_TRACKS] = { CRASH_COVE, ROO_TUBES, MYSTERY_CAVES, SKULL_ROCK, SEWER_SPEEDWAY,
	SLIDE_COLISEUM, TURBO_TRACK, TIGER_TEMPLE, COCO_PARK, PAPU_PYRAMID, RAMPAGE_RUINS,
	 DINGO_CANYON, BLIZZARD_BLUFF, DRAGON_MINES, POLAR_PASS, ROCKY_ROAD, TINY_ARENA,
	 N_GIN_LABS, CORTEX_CASTLE, HOT_AIR_SKYWAY, NITRO_COURT, OXIDE_STATION };


	//set this in AH_WarpPad_ThTick
	for (i = 0; i < WARPPAD_TRACKS; i++) //not crystal challenge, not adventure cups
	{
		if (levelID == warppad_id[i])
		{
			current_track = i;
		}
	}
	adventure_options();
	sdata->gGT->numLaps = laps % 128;
	MainRaceTrack_RequestLoad(load_track);

}

//overlay 222 modified
void adventure_endrace(unsigned int endTime, unsigned int requiredTime) //manages end of race and teleporting in adv hub
{

	struct GameTracker* gGT;
	struct Driver* driver;
	struct AdvProgress* adv;


	char i;


	short t;
	short elapsedFrames;
	short levSpawn;

	int bitIndex;

	bitIndex = -1;
	gGT = sdata->gGT;
	driver = gGT->drivers[0];
	adv = &sdata->advProgress;

	elapsedFrames = sdata->framesSinceRaceEnded;

	// count frames if hasn't been 30 seconds
	if (elapsedFrames < FPS_DOUBLE(900))
		elapsedFrames++;

	sdata->framesSinceRaceEnded = elapsedFrames;

	if (driver->instBigNum->scale[0] != 0x1e00)
	{
		struct Instance* instFruitDisp = driver->instFruitDisp;
		instFruitDisp->scale[0] = 0;
		instFruitDisp->scale[1] = 0;
		instFruitDisp->scale[2] = 0;
	}



	// For trophy race, check 1st place
	int boolWin = (!hardcore) ? (driver->driverRank == 0) : (endTime < requiredTime);


	elapsedFrames = sdata->framesSinceRaceEnded;

	void time_helper(u_char show_this, bool win);
	time_helper(1, boolWin);

	// If it hasn't been 1 second from race ended
	if (elapsedFrames < FPS_DOUBLE(30))
	{
		sdata->menuReadyToPass = 0;
		return;
	}


	// 0x78 + 0x6e = 0xe6 (230) frames waited for Token Race
	if ((elapsedFrames - 0) < FPS_DOUBLE(110))
		return;


	// if the menu is already drawing
	if (sdata->menuReadyToPass & 1)
		return;

	// If you are in adventure mode
	if ((gGT->gameMode1 & ADVENTURE_MODE) == 0)
		return;


	// PRESS * TO CONTINUE
	DecalFont_DrawLine(sdata->lngStrings[0xc9], 0x100, 0xbe, 1, 0xffff8000);

	// If you have not pressed X
	if ((sdata->AnyPlayerTap & 0x50) == 0)
		return;

	// === If Pressed X ===

	RECTMENU_ClearInput();

	sdata->Loading.OnBegin.AddBitsConfig0 |= ADVENTURE_ARENA;
	sdata->Loading.OnBegin.RemBitsConfig0 |= (ADVENTURE_BOSS | TOKEN_RACE);

	// If you are in boss mode
	if (gGT->gameMode1 < 0)
	{
		sdata->Loading.OnBegin.AddBitsConfig8 |= SPAWN_AT_BOSS;
	}

	if (!boolWin)
	{
		RECTMENU_Show(&data.menuRetryExit);
		sdata->menuReadyToPass |= 1;
		return;
	}

	// === If you won the race ===

	sdata->framesSinceRaceEnded = 0;

	// Load the levelID for Adventure Hub that you came from
	levSpawn = gGT->prevLEV;

	// If you are in boss mode
	if (gGT->gameMode1 < 0)
	{


		// bitIndex of keys unlocked, and boss beaten
		bitIndex = gGT->bossID + 0x5e;

		// If the number of keys you have is less than 4
		if (gGT->bossID < 4)
		{
			// only if first time beating boss
			if (CHECK_ADV_BIT(adv->rewards, bitIndex) == 0)
			{
				// Go to Podium after returning to Adventure Hub
				gGT->podiumRewardID = 99; // key
				unlock_track = false; //reset battle tracks

				// hot air skyway
				if (current_track == CITADEL_BOSS)
				{
					// If you just beat Pinstripe
					// Load gemstone valley
					levSpawn = 0x19;
				}
			}
		}

		// If you have 4 keys (only here if you beat oxide)
		else
		{
			// Always go to podium after oxide,
			// with no key (0x38 = empty)
			gGT->podiumRewardID = 0x38;

			// assume oxide beaten 1st time
			adv->rewards[3] |= 0x80004;

			// if beaten oxide 2nd time
			if (gGT->bossID == 5)
			{
				// beat 2nd time
				adv->rewards[3] |= 0x100008;
			}
		}
	}

	// if something needs unlocking
	if (bitIndex > 0)
	{
		// Unlock reward
		UNLOCK_ADV_BIT(adv->rewards, bitIndex);
	}




#define turbo_slide (current_track == GEMSTONE_1 || current_track == GEMSTONE_2)




	//if slide coliseum or any battle/crystal challenge track
	if (battle_levels_ || turbo_slide)
	{
		if (current_track == GEMSTONE_1) slidec_finished = true;

		if (current_track == GEMSTONE_2) ttrack_finished = true;

		if (battle_levels_) unlock_track = true; //unlock last track of the hub

		goto GoPodium; //get fake trophy
	}

	else // if race tracks
	{



		unsigned char prize_id[WARPPAD_TRACKS - 6] = {
CRASH_COVE, ROO_TUBES, MYSTERY_CAVES, SEWER_SPEEDWAY,
TIGER_TEMPLE, COCO_PARK, PAPU_PYRAMID, DINGO_CANYON, BLIZZARD_BLUFF, DRAGON_MINES, POLAR_PASS,
 TINY_ARENA, N_GIN_LABS, CORTEX_CASTLE, HOT_AIR_SKYWAY, OXIDE_STATION };


		//i dont have ideas to optimize this lol
		u_char prize_index =
			(current_track == GLACIER_5) ? current_track - 5 :
			(current_track == CITADEL_5) ? current_track - 6 :
			(current_track == LOST_RUINS_5) ? current_track - 4 :
			(current_track == N_SANITY_5) ? current_track - 1 :
			(current_track >= LOST_RUINS_1 && current_track <= LOST_RUINS_3) ? current_track - 3 :
			(current_track >= GLACIER_1 && current_track <= GLACIER_3) ? current_track - 4 :
			(current_track >= CITADEL_1 && current_track <= CITADEL_3) ? current_track - 5 :
			current_track;
			


		//for bosses, just in case
		bitIndex = (prize_index > 15) ? ROO_TUBES + ADV_PRIZE : prize_id[prize_index] + ADV_PRIZE;


		// if trophy is not won,
		// Dingo Bingo needs to win trophy and token in the same race
		if (CHECK_ADV_BIT(adv->rewards, bitIndex) == 0)
		{
			// unlock tropy
			UNLOCK_ADV_BIT(adv->rewards, bitIndex);

		GoPodium:
			// go to podium with trophy
			gGT->podiumRewardID = 0x62;
		}
	}

	MainRaceTrack_RequestLoad(levSpawn);

}

void adventure_main()
{
	unsigned char i;
	int buttonTap;
	struct GameTracker* gGT = sdata->gGT;

	//disable player to bot swap
	*(int*)0x80017318 = 0x3E00008;
	*(int*)0x8001731c = 0;

	void quad_main();
	quad_main();
	
	if (gGT->levelID < GEM_STONE_VALLEY && gGT->cameraDC->unk8E != 0)
		gGT->cameraDC->unk8E = 0;
	
	
	
	//avoid bug of wumpa dissapearing
	
	//if (gGT->levelID == load_track)
	//{
	//	if (gGT->trafficLightsTimer > 0)
	//	{
	//	struct Instance* instFruitDisp = gGT->drivers[0]->instFruitDisp;
	//	if (instFruitDisp->scale[0] == 0)
	//	{
			
	//	sdata->ptrFruitDisp =
	//	(int)UI_INSTANCE_BirthWithThread(0x37,(int)UI_ThTick_CountPickup,3,1,sdata->ptrPushBufferUI,/*sdata->s_fruitdisp*/0);
	//	}
	//	}
	//}

	if (show_stars && gGT->levelID == load_track)
	{

		sdata->gGT->renderFlags |= 8;
		sdata->gGT->stars.numStars = 768;
		sdata->gGT->stars.spread = 1;
		sdata->gGT->stars.seed = 65535;
		sdata->gGT->stars.distance = 1022;

	}


	if (gGT->levelID == MAIN_MENU_LEVEL) D230.menuMainMenu.rows = &adventure_row;
	
	if (D230.MM_State == 1)
	{
		
		buttonTap = sdata->gGamepads->gamepad[0].buttonsTapped;
		
		if (buttonTap & BTN_SELECT)
		{
			hardcore ^= true;
			OtherFX_Play(1, 0);
		}
		
		if (hardcore)
		{
			difficulty = "enabled";
			
		}
		else
		{
			difficulty = "disabled";
		}
		
		DecalFont_DrawLine("PRESS SELECT TO DISABLE", 6, 0x10, FONT_SMALL, ORANGE);
		DecalFont_DrawLine("TIME LIMIT TO WIN THE RACE", 6, 0x10 + 10, FONT_SMALL, ORANGE);
		DecalFont_DrawLine(difficulty, 6, 0x10 + 20, FONT_SMALL, PENTA_WHITE);
	}


	unsigned short tracks_lng[MAX_TRACKS] = { CRASH_COVE_NAME, ROO_TUBES_NAME, MYSTERY_CAVES_NAME,
   SKULL_ROCK_NAME, SEWER_SPEEDWAY_NAME, SLIDE_COLISEUM_NAME, TURBO_TRACK_NAME, TIGER_TEMPLE_NAME,
	COCO_PARK_NAME, PAPU_PYRAMID_NAME, RAMPAGE_RUINS_NAME, DINGO_CANYON_NAME, BLIZZARD_BLUFF_NAME,
	DRAGON_MINES_NAME, POLAR_PASS_NAME, ROCKY_ROAD_NAME, TINY_ARENA_NAME, N_GIN_LABS_NAME,
	CORTEX_CASTLE_NAME, HOT_AIR_SKYWAY_NAME, NITRO_COURT_NAME, OXIDE_STATION_NAME, BOSS1_GARAGE_NAME,
	BOSS2_GARAGE_NAME, BOSS3_GARAGE_NAME, BOSS4_GARAGE_NAME, BOSS5_GARAGE_NAME };

	for (i = 0; i < MAX_TRACKS; i++)
	{
		sdata->lngStrings[tracks_lng[i]] = track_names[i];

	}




	unsigned char character_engines[NUM_CHARACTERS] = { INTERMEDIATE, INTERMEDIATE, ADVANCED, SKILLED,
	SKILLED, ADVANCED, BEGINNER, BEGINNER, SKILLED, ADVANCED, BEGINNER, INTERMEDIATE,
	ADVANCED, BEGINNER, INTERMEDIATE, INTERMEDIATE };









	static u_char characterID;

#define IS_ADV_HUB (gGT->levelID >= GEM_STONE_VALLEY && gGT->levelID <= CITADEL_CITY)

	if (IS_ADV_HUB)
	{
		sdata->lngStrings[BOSS1_NAME] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? track_names[N_SANITY_BOSS] : character_names[RIPPER_ROO];
		sdata->lngStrings[BOSS2_NAME] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? track_names[LOST_RUINS_BOSS] : character_names[PAPU_PAPU];
		sdata->lngStrings[BOSS3_NAME] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? track_names[GLACIER_BOSS] : character_names[KOMODO_JOE];
		sdata->lngStrings[BOSS4_NAME] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? track_names[CITADEL_BOSS] : character_names[PINSTRIPE];
		sdata->lngStrings[BOSS5_NAME] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? track_names[FINAL_BOSS] : character_names[NITROS_OXIDE];
		data.font_charPixWidth[FONT_BIG] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? WIDE_34(13) : WIDE_34(17);
	}

	data.gamepadMapBtn[2].output = (gGT->levelID == ADVENTURE_GARAGE) ? BTN_UP : BTN_LEFT;
	data.gamepadMapBtn[3].output = (gGT->levelID == ADVENTURE_GARAGE) ? BTN_DOWN : BTN_RIGHT;

	if (sdata->RaceFlag_Position != 0 && gGT->levelID == ADVENTURE_GARAGE)
	{
		buttonTap = sdata->AnyPlayerTap;

#define BASIC_ROSTER 8 // the 8 original characters in adv mode

		if (sdata->ptrDesiredMenu != &data.menuSubmitName)
		{
			//reset this
			ttrack_finished = false;
			slidec_finished = false;
			unlock_track = false;

			if ((buttonTap & BTN_UP) != 0)
			{
				characterID = (characterID == CRASH_BANDICOOT) ? NITROS_OXIDE : characterID - 1;
				sdata->advCharSelectIndex_curr = characterID % BASIC_ROSTER;

				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					character_engines[characterID];

				OtherFX_Play(4, 0);
			}
			if ((buttonTap & BTN_DOWN) != 0)
			{
				characterID = (characterID == NITROS_OXIDE) ? CRASH_BANDICOOT : characterID + 1;
				sdata->advCharSelectIndex_curr = characterID % BASIC_ROSTER;
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					character_engines[characterID];

				OtherFX_Play(4, 0);
			}



			if ((buttonTap & BTN_L1) != 0)
			{
				data.MetaDataCharacters[characterID].engineID =
					(data.MetaDataCharacters[characterID].engineID == INTERMEDIATE) ?
					BEGINNER : data.MetaDataCharacters[characterID].engineID - 1;

				//show engine stats
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					data.MetaDataCharacters[characterID].engineID;
				OtherFX_Play(103, 0);
			}
			if ((buttonTap & BTN_R1) != 0)
			{
				data.MetaDataCharacters[characterID].engineID =
					(data.MetaDataCharacters[characterID].engineID == BEGINNER) ?
					INTERMEDIATE : data.MetaDataCharacters[characterID].engineID + 1;

				//show engine stats
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					data.MetaDataCharacters[characterID].engineID;
				OtherFX_Play(103, 0);
			}

			DecalFont_DrawLine("SWAP ENGINE WITH L1 OR R1", 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | ORANGE));

#define INTERMEDIATE_LNG 585

			//change accel name in adventure mode
			sdata->lngStrings[INTERMEDIATE_LNG] = (data.MetaDataCharacters[characterID].engineID == SKILLED) ?
				"  Skilled   " : "Intermediate";




			sdata->lngStrings[data.MetaDataCharacters
				[sdata->advCharSelectIndex_curr].name_LNG_long] = character_names[characterID];
		}


		else //if submit menu is shown
		{

			data.characterIDs[0] = characterID;
			strcpy(gGT->prevNameEntered, "CUSTOM_A");
			strcpy(sdata->advProgress.name, gGT->prevNameEntered);
			sdata->advProgress.characterID = data.characterIDs[0];
			sdata->ptrActiveMenu = 0;
			sdata->ptrDesiredMenu = 0;
			MainRaceTrack_RequestLoad(N_SANITY_BEACH);

		}

		if (characterID > BASIC_ROSTER - 1) //draw character icons for other characters
		{
			//ui_drawdriver corrupts big sized icons, so drawWeapon is better for this
			DecalHUD_DrawWeapon(

				sdata->gGT->ptrIcons[data.MetaDataCharacters[characterID].iconID],

				(int)180, (int)85,


				&gGT->backBuffer->primMem,


				gGT->pushBuffer_UI.ptrOT,

				TRANS_50_DECAL, (int)0x3900, 0);

		}

	}

}