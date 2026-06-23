#include <common.h>
#include "adventure.h"

unsigned char load_track;
unsigned char laps;
bool _endgame;
bool show_stars;
char* timeToWin;
bool hardcore = false; //ultra hard mode (affects bots only), toggled with SELECT
char* difficulty;
bool isRelic = false;







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

//fix pause menu in adv
struct MenuRow adv_rows_nohints[4] =
{
		{0x2, 2, 1, 0, 0}, // RESUME
		{0x3, 0, 2, 1, 1}, // QUIT
		{0xE, 1, 0, 2, 2}, // OPTIONS
		{-1,  0, 0, 0, 0}  // NULL
};



//draw the track name while the player stands inside a warppad's bounding box
void show_levelname()
{
	short X, Z;

	// only runs this (inside custom_adventure) once the race flag is fully
	//off-screen. Before that the hub mesh / warppad_blockIndex[] aren't ready yet,
	//the bbox test passes for everything and we flood the prim/OT buffer -- which
	//then makes the hub draw (AH_Pause_Draw -> DecalHUD_DrawPolyGT4) crash.
	if (!RaceFlag_IsFullyOffScreen())
		return;

	X = sdata->gGT->drivers[0]->posCurr.x >> 8;
	Z = sdata->gGT->drivers[0]->posCurr.z >> 8;

	unsigned short add = 1000;

	//warppad_blockIndex[] is stored in adv_order positions, so the matching
	//track name is track_names[adv_order[pos]] (track_names is in CUSTOM_X order)
	for (unsigned char pos = 0; pos < TOTAL_TRACKS; pos++)
	{
		//warppads not present in this hub mesh keep index 0; skip them so we don't
		//read quadblock 0's bbox and draw bogus names every frame
		if (warppad_blockIndex[pos] == 0)
			continue;

		struct QuadBlock* qb = &sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[warppad_blockIndex[pos]];

		if (X >= (qb->bbox.min[0] - add) && X <= (qb->bbox.max[0] + add)
			&& Z >= (qb->bbox.min[2] - add) && Z <= (qb->bbox.max[2] + add))
		{
			DecalFont_DrawLine
			(
				track_names[adv_order[pos]],
				sdata->gGT->pushBuffer[0].rect.x + sdata->gGT->pushBuffer[0].rect.w/2,
				sdata->gGT->pushBuffer[0].rect.x + sdata->gGT->pushBuffer[0].rect.h - 30,
				FONT_BIG, (JUSTIFY_CENTER | ORANGE)
			);

			//only the pad the player is standing on should show; this also keeps the
			//prim/OT buffer from being flooded if pad boxes ever overlap
			break;
		}
	}
}

void custom_adventure(unsigned short blockID)
{
	unsigned char i;
	unsigned char backup;
	unsigned char pos;
	unsigned char matched = 0;

	//show the name of the track whose warppad the player is standing on
	show_levelname();
	

	

	
	for (pos = 0; pos < TOTAL_TRACKS; pos++)
	{
		i = adv_order[pos];

		if (blockID == warppad_id[i])
		{
			//already cleared -> nothing to do
			if (adv_progress[i]) return;

			//locked until the previous track in progression order is completed
			if (pos > 0 && !adv_progress[adv_order[pos - 1]]) return;

			backup = i;
			matched = 1;
			break;
		}
	}

	//the player isn't standing on any active warppad
	if (!matched) return;

    //show warp animation on our kart
	sdata->gGT->drivers[0]->funcPtrs[0] = VehStuckProc_Warp_Init;
	
	//skip if warp animation didnt finished
    if (sdata->gGT->drivers[0]->KartStates.Warp.timer != 800)
		return;
	 
	 current_track = backup;
	 adventure_options();
	 
	 //set laps and spawn
	 sdata->gGT->numLaps = laps % 128;

	 //race spawn order (ported from , made isRelic-aware):
	 //relic race -> player at slot 1 with a full grid; boss -> player at slot 0 with a
	 //single opponent (the boss, 1v1); normal race -> player at slot 7 with a full grid
	 sdata->kartSpawnOrderArray[0] = (isRelic) ? 1 : (BOSS_RACE) ? 0 : 7;
	 unsigned char numSpawns = (BOSS_RACE && !isRelic) ? 1 : 7;
	 for (unsigned char sp = numSpawns; sp > 0; sp--)
		sdata->kartSpawnOrderArray[sp] = 7 - sp;

	 relicptr = 0;
	 keyptr = 0;
	 MainRaceTrack_RequestLoad(load_track);
	

}


bool playXA;

//overlay 222 modified
//this is responsible of end of race UI, teleporting and prizes in adventure
void adventure_endrace(unsigned int endTime, unsigned int requiredTime) 
{

	struct GameTracker* gGT;
	struct Driver* driver;
	struct AdvProgress* adv;


	char i;


	short t;
	short elapsedFrames;

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



	// check if your time was enough to win the trophy, if hardcore is disabled then check 1st place (we are always 1st in custom tracks)
	int boolWin = (!isRelic) ? (driver->driverRank == 0) : (endTime < requiredTime);
	
	
	//end race music
	if (playXA)
	{
		
		if(boolWin)
		{
						// amount of confetti particles
						gGT->confetti.numParticles_max = 250;
						gGT->confetti.unk2 = 250;

						// one person won,
						// one person gets confetti
						gGT->numWinners = 1;

						// add driver ID to array of confetti winners
						gGT->winnerIndex[0] = 0;

						// edit window variables for confetti
						gGT->pushBuffer[0].fadeFromBlack_currentValue = 0x1fff;
						gGT->pushBuffer[0].fadeFromBlack_desiredResult = 0x1000;
						gGT->pushBuffer[0].fade_step = 0xff78;
		}
		
	 sdata->desiredXA_RaceEndIndex = (boolWin) ? 4 : 5;
     OtherFX_Play(0x5f, 0);
	 Audio_SetState_Safe(0x10);
     playXA = false;
	}

                    


	elapsedFrames = sdata->framesSinceRaceEnded;
    
	//show best, total and pb time
	void time_helper(u_char show_this, bool win);
	time_helper(1, boolWin);
	
	//clear this to avoid UI bugs
	relicptr = 0;
	keyptr = 0;
	sdata->ptrFruitDisp = (int)0;


	if ((elapsedFrames) < FPS_DOUBLE(60))
	{
		sdata->menuReadyToPass = 0; //avoid softlocking bug
		return;
	}


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


	if (!boolWin)
	{
		RECTMENU_Show(&data.menuRetryExit);
		sdata->menuReadyToPass |= 1;
		return;
	}

	// === If you won the race ===
	if (!adv_progress[current_track])
	{
	adv_progress[current_track] = true;
	
	//runtime mode detection (no extra config beyond isRelic):
	//relic race -> relic, boss race -> key, otherwise -> trophy
	if (isRelic)
		numRelics++;
	else if (BOSS_RACE)
		numKeys++;
	else
		numTrophys++;
	}

	sdata->framesSinceRaceEnded = 0;

	// Load the levelID for Adventure Hub that you came from
	unsigned char last_boss = CUSTOM_BOSS_0 + NUM_HUBS - 1;
	current_track = (current_track != last_boss) ? CUSTOM_HUB : current_track;

	
    adventure_options();
	



	MainRaceTrack_RequestLoad((current_track == last_boss) ? CREDITS_CRASH : load_track);

}

void custom_engine(unsigned char ogEngine, unsigned char curr_stat, unsigned char stat_value);

//ultra hard mode: feed every bot a continuous turbo
void GiveBotsTurbo(int fire_level)
{
	struct GameTracker* gGT = sdata->gGT;

	for (unsigned char i = 0; i <= gGT->numBotsNextGame; i++) {
		struct Driver* driver = gGT->drivers[i];

		if (driver == NULL) continue;

		//If its a bot
		if ((driver->actionsFlagSet & 0x100000) != 0) {

			// If bot has a TNT on their head or is affected by a clock
			if ((driver->instTntRecv != 0) || (driver->clockReceive != 0)) continue;
			// If bot is spinning or blasted
			if ((driver->botData.botFlags & 2) != 0) continue;

			VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), fire_level);
		}
	}
}

//rubber-band assist (replaces the old custom item roulette / items.c): along the track,
//when it is NOT a relic race and the player is holding nothing, hand them a wumpa + a
//roulette item at evenly spaced points. Ported from , gated on isRelic.
void Item_Crate(struct GameTracker* gGT)
{
	struct Driver* player = gGT->drivers[0];

	if (gGT->level1 == NULL || player == NULL || isRelic) return;
	if (gGT->gameMode1 & END_OF_RACE) return;

	static int timer = 0;

	//throttle, and only when the player holds nothing (0xF = no item)
	if ((gGT->timer + FPS_DOUBLE(96)) < (timer + FPS_DOUBLE(96)) || player->heldItemID != 0xF)
		return;
	else
		timer = gGT->timer;

	unsigned char numDiff = (hardcore) ? 14 : 10;
	unsigned char numdist = (hardcore) ? 125 : 225;

	unsigned int trackLength = gGT->level1->ptr_restart_points[0].distToFinish * numDiff;
	unsigned int progress = player->distanceToFinish_curr;

	for (unsigned char i = 1; i < numDiff; i++)
	{
		if (progress >= ((trackLength / numDiff) * i) && progress <= ((trackLength / numDiff) * i) + numdist)
		{
			if (player->numWumpas < 10)
				player->numWumpas++;

			//give the player (driver index 0) a roulette item
			weapon_roulette(0);
			break;
		}
	}
}

//derive each track's mode (relic vs trophy) straight from isRelic in adventure_options,
//so nothing extra has to be configured. Built once; current_track is saved and restored.
void build_relic_table()
{
	build_layout(); //adv_order / TOTAL_TRACKS must be ready first

	unsigned char saved = current_track;

	//only the active tracks (incl. bosses), in progression order
	for (unsigned char pos = 0; pos < TOTAL_TRACKS; pos++)
	{
		unsigned char t = adv_order[pos];
		current_track = t;
		adventure_options();
		track_is_relic[t] = isRelic;
	}

	current_track = saved;
	adventure_options();
}

void adventure_main()
{
	unsigned char i;
	int buttonTap;
	static bool stats_editor = false;
	static u_char curr_stat = 1;
	static u_char newStat[3];
	struct GameTracker* gGT = sdata->gGT;
	

     void CUSTOM_ADV_HUB(struct GameTracker* gGT);

	 //make sure the derived layout tables exist before anything reads them
	 build_layout();

	 CUSTOM_ADV_HUB(gGT);

	
	if (gGT->levelID == MAIN_MENU_LEVEL)
	{
	    //change main menu entries
		D230.menuMainMenu.rows = &adventure_row;
		
		//remove mask hints from pause menu
		data.menuAdvHub.rows = &adv_rows_nohints;
	}
	
	
	//toggle ultra hard mode (bot difficulty) with SELECT
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
		
		DecalFont_DrawLine("PRESS SELECT TO ENABLE", 6, 3, FONT_SMALL, ORANGE);
		DecalFont_DrawLine("ULTRA HARD MODE", 6, 13, FONT_SMALL, ORANGE);
		DecalFont_DrawLine(difficulty, 6, 23, FONT_SMALL, PENTA_WHITE);
	}
	
	
	//ultra hard mode: give bots continuous turbo once the race is underway
	if (gGT->levelID <= GEM_STONE_VALLEY && gGT->trafficLightsTimer < 1 && RaceFlag_IsFullyOffScreen())
	{
		//hand the player a wumpa + roulette item along the track (skips relic races internally)
		Item_Crate(gGT);

		if (hardcore)
			GiveBotsTurbo(0x100);
	}

	//avoid flying cam bug in old custom tracks
	if (gGT->levelID < INTRO_RACE_TODAY && gGT->cameraDC->unk8E != 0)
		gGT->cameraDC->unk8E = 0;

	
	//check quadblock modifiers
	void quad_main();
	quad_main();
	
	  if (current_track == CUSTOM_HUB)
	  {
		  if (sdata->ptrActiveMenu == &data.menuAdvRace)
		  {
			  sdata->ptrActiveMenu = 0;
			  sdata->ptrDesiredMenu = 0;
			  
			  RECTMENU_Hide(&data.menuAdvRace);
			  
			  RECTMENU_Show(&data.menuAdvHub);
		  }
		  
		  if (gGT->drivers[0] != NULL)
			custom_adventure(gGT->drivers[0]->underDriver->blockID);
		  
	  }

	

	//adjust bot/arcade difficulty (ultra hard mode raises it); hardcore affects bots only
	if (gGT->levelID <= INTRO_RACE_TODAY)
	{
		*(unsigned short*)0x80012658 = (hardcore) ? 0x280 : 0xF0;

		sdata->gGT->arcadeDifficulty = *(unsigned short*)0x80012658;

		if (*(unsigned short*)0x80012658 == 0x280)
			sdata->gGT->gameMode2 |= CHEAT_SUPERHARD;
		else
			sdata->gGT->gameMode2 &= ~CHEAT_SUPERHARD;

		//boss races are 1v1 against this hub's boss character (model loaded in adv.c);
		//if the boss track has isRelic set it becomes a solo relic race instead
		if (BOSS_RACE && !isRelic)
		{
			sdata->gGT->bossID = current_track - CUSTOM_BOSS_0;
			sdata->gGT->gameMode1 |= ADVENTURE_BOSS;
		}
		else
		{
			sdata->gGT->gameMode1 &= ~ADVENTURE_BOSS;
		}

		if (sdata->Loading.stage != -1)
		{
			u_short add = 0x2008;
			u_short modifier = *(unsigned short*)0x80012658;

			*(unsigned short *)0x800126b8 = modifier;
			*(unsigned short *)0x800126ba = add;
			*(unsigned short *)0x80012728 = modifier;
			*(unsigned short *)0x8001272a = add;
			*(unsigned short *)0x800127b0 = modifier * 2;
			*(unsigned short *)0x800127b2 = add;
			*(unsigned short *)0x80012844 = modifier;
			*(unsigned short *)0x80012846 = add;

			BOTS_Adv_AdjustDifficulty();
		}
	}

    //enable stars
	if (show_stars && gGT->levelID == load_track)
	{

		sdata->gGT->renderFlags |= 8;
		sdata->gGT->stars.numStars = 768;
		sdata->gGT->stars.spread = 1;
		sdata->gGT->stars.seed = 65535;
		sdata->gGT->stars.distance = 1022;

	}
	
	if (sdata->Loading.stage == -5 || gGT->levelID >= GEM_STONE_VALLEY)
	{
		sdata->ptrFruitDisp = (int)0;
		relicptr = 0;
		keyptr = 0;
	}
	
	
	
	//execute this only in one of the first frames of the race to avoid wumpa bug	
  if (gGT->trafficLightsTimer > 0  && gGT->levelID < INTRO_RACE_TODAY && current_track != CUSTOM_HUB)
	{
		if (relicptr == 0 && keyptr == 0)
		{
			//relic race -> draw the relic model; any other race -> draw the big1 trophy
			//decoration (like ). The key is NOT shown in-race anymore; it only
			//appears in the custom hub (nopos.c).
			if (isRelic)
			{
				relicptr = UI_INSTANCE_BirthWithThread(STATIC_RELIC, (int)UI_ThTick_Reward,0xe,1,0,/*sdata->s_relic1*/0);
				relicptr->matrix.t[0] = -206;
				relicptr->matrix.t[1] = 86;
				relicptr->matrix.t[2] = 256;
				relicptr->colorRGBA = instanceColor(BGR_TO_RGB(CTR_FontColor(relic_color)));
			}
			else
			{
				relicptr = UI_INSTANCE_BirthWithThread(0x38,(int) UI_ThTick_big1,2,0,0,/*sdata->s_big1*/0);
			}
		}
		
		  //fix wumpa
		  if (sdata->ptrFruitDisp == 0)
		  {
				sdata->ptrFruitDisp =
				(int)  UI_INSTANCE_BirthWithThread(0x37,(int) UI_ThTick_CountPickup,3,1,sdata->ptrPushBufferUI,/*sdata->s_fruitdisp*/0);
		  }
	}
	

	
		if (gGT->levelID == MAIN_MENU_LEVEL)
		{
			sdata->lngStrings[ADVANCED_LNG - 5] = "ACCEL";
			sdata->lngStrings[INTERMEDIATE_LNG - 3] = "SPEED";
			
			//reset progress
			for (i = 0; i < MAX_TRACKS; i++)
			{
					adv_progress[i] = false;
					numKeys = 0;
					numTrophys = 0;
					current_track = 0;
					
					//overwrite engine LNGs
					if (i < 3)
						sdata->lngStrings[BEGINNER_LNG + i] = "CUSTOM STATS";
			}

			numRelics = 0;

			//derive the per-track relic/trophy table once (current_track is 0 here)
			static bool relic_table_built = false;
			if (!relic_table_built)
			{
				build_relic_table();
				relic_table_built = true;
			}
		}



    //the og engines of every character
	unsigned char character_engines[NUM_CHARACTERS] = { INTERMEDIATE, INTERMEDIATE, ADVANCED, SKILLED,
	SKILLED, ADVANCED, BEGINNER, BEGINNER, SKILLED, ADVANCED, BEGINNER, INTERMEDIATE,
	ADVANCED, BEGINNER, INTERMEDIATE, INTERMEDIATE };



	static u_char characterID;



    //change boss names in pause menu while you are in ADVENTURE hub
	if (current_track == CUSTOM_HUB)
		data.font_charPixWidth[FONT_BIG] = ((gGT->gameMode1 & PAUSE_ALL) != 0) ? WIDE_34(13) : WIDE_34(17);
	
    
	//make a trick for custom stats selector
	data.gamepadMapBtn[0].output = (gGT->levelID == ADVENTURE_GARAGE) ? ((stats_editor) ? BTN_L2 : BTN_UP) : BTN_UP;
	data.gamepadMapBtn[1].output = (gGT->levelID == ADVENTURE_GARAGE) ? ((stats_editor) ? BTN_R2 : BTN_DOWN) : BTN_DOWN;
	
	//ban left and right in adventure garage because it wont allow you to select extra characters, also make a trick for custom stats selector
	data.gamepadMapBtn[2].output = (gGT->levelID == ADVENTURE_GARAGE) ? ((stats_editor) ? BTN_L1 : BTN_UP) : BTN_LEFT;
	data.gamepadMapBtn[3].output = (gGT->levelID == ADVENTURE_GARAGE) ? ((stats_editor) ? BTN_R1 : BTN_DOWN) : BTN_RIGHT;

	if (RaceFlag_IsFullyOffScreen() && gGT->levelID == ADVENTURE_GARAGE)
	{
		buttonTap = sdata->AnyPlayerTap;

		#define BASIC_ROSTER 8 // the 8 original characters in adv mode

		if (sdata->ptrDesiredMenu != &data.menuSubmitName)
		{
			
                if (stats_editor)
	            {
	            	void draw_stats_arrow(u_char characterID, u_char curr_stat);
	                draw_stats_arrow(characterID, curr_stat);
	            }

			if ((buttonTap & BTN_UP) != 0)
			{
				//characterIDs is a char, avoid negative values
				characterID = (characterID == CRASH_BANDICOOT) ? NITROS_OXIDE : characterID - 1;
				
                //adventure garage can crash if the selected index is higher than 7
				sdata->advCharSelectIndex_curr = characterID % BASIC_ROSTER;
				
				
				//restore default stats
				custom_engine(character_engines[characterID], ACCEL, character_engines[characterID]);
				custom_engine(character_engines[characterID], SPEED, character_engines[characterID]);
				custom_engine(character_engines[characterID], TURN, character_engines[characterID]);
				
				newStat[ACCEL - 1] = character_engines[characterID];
				newStat[SPEED - 1] =  character_engines[characterID];
				newStat[TURN - 1] =  character_engines[characterID];
                
				//restore default engineID
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					character_engines[characterID];

				OtherFX_Play(4, 0);
			}
			if ((buttonTap & BTN_DOWN) != 0)
			{
				//characterIDs is a char, avoid negative values
				characterID = (characterID == NITROS_OXIDE) ? CRASH_BANDICOOT : characterID + 1;
				
				//adventure garage can crash if the selected index is higher than 7
				sdata->advCharSelectIndex_curr = characterID % BASIC_ROSTER;
				
				//restore default stats
				custom_engine(character_engines[characterID], ACCEL, character_engines[characterID]);
				custom_engine(character_engines[characterID], SPEED, character_engines[characterID]);
				custom_engine(character_engines[characterID], TURN, character_engines[characterID]);
				
				newStat[ACCEL - 1] = character_engines[characterID];
				newStat[SPEED - 1] =  character_engines[characterID];
				newStat[TURN - 1] =  character_engines[characterID];
                
				//restore default engineID
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					character_engines[characterID];
                
				//play some fx
				OtherFX_Play(4, 0);
			}
             if (curr_stat == 0)
				 curr_stat++;

			if ((buttonTap & BTN_SELECT) != 0)
			{
				stats_editor ^= true;
				
			}
			
			if (stats_editor)
			{
			if ((buttonTap & BTN_L1) != 0)
			{
				
				//show engine stats
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					data.MetaDataCharacters[characterID].engineID;
					
				
				newStat[curr_stat - 1] = ((unsigned char)(newStat[curr_stat - 1] - 1) % 4);
				custom_engine(data.MetaDataCharacters[characterID].engineID, curr_stat, newStat[curr_stat - 1]);
				
					
				//play some fx
				OtherFX_Play(103, 0);
			}
			if ((buttonTap & BTN_R1) != 0)
			{

				//show engine stats
				data.MetaDataCharacters[sdata->advCharSelectIndex_curr].engineID =
					data.MetaDataCharacters[characterID].engineID;
					
				newStat[curr_stat - 1] = ((unsigned char)(newStat[curr_stat - 1] + 1) % 4);
				custom_engine(data.MetaDataCharacters[characterID].engineID, curr_stat, newStat[curr_stat - 1]);
					
					
				//play some fx
				OtherFX_Play(103, 0);
			}
			
			if ((buttonTap & BTN_L2) != 0)
			{
				curr_stat = (curr_stat == ACCEL) ? TURN : curr_stat - 1;
			}
			
			if ((buttonTap & BTN_R2) != 0)
			{
				curr_stat = (curr_stat == TURN) ? ACCEL : curr_stat + 1;
			}
			}
			


			DecalFont_DrawLine("PRESS SELECT TO TOGGLE STAT EDITOR", 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | ORANGE));



            //rewrite LNG string from current characters (i dont have cs_garage code for vanilla,so this is better)
			sdata->lngStrings[data.MetaDataCharacters
				[sdata->advCharSelectIndex_curr].name_LNG_long] = character_names[characterID];
		}


		else //if submit menu is shown
		{
			//set adv data
			data.characterIDs[0] = characterID;
			strcpy(gGT->prevNameEntered, "CUSTOM_A");
			strcpy(sdata->advProgress.name, gGT->prevNameEntered);
			sdata->advProgress.characterID = data.characterIDs[0];
			
			//change engine sound
			data.MetaDataCharacters[characterID].engineID = newStat[TURN - 1];
			stats_editor = false;
			
			//skip menus
			sdata->ptrActiveMenu = 0;
			sdata->ptrDesiredMenu = 0;
			
			//load custom hub
			current_track = CUSTOM_HUB;
			adventure_options();
			MainRaceTrack_RequestLoad(load_track);

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