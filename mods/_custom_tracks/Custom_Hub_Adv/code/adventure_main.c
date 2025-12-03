#include <common.h>
#include "adventure.h"

unsigned char load_track;
unsigned char laps;
bool _endgame;
bool show_stars;
char* timeToWin;
bool hardcore = true; //enable this by default
char* difficulty;








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



void custom_adventure(unsigned short blockID)
{
	unsigned char i;
	unsigned char backup;
	unsigned char fixed_index;
	

	

	
	for (i = 0; i < MAX_TRACKS; i++)
	{
		
		if (blockID == warppad_id[i])
		{
			//special requirements for some warp pads because i dont want to flip boss track IDs
			//yes
			fixed_index =
			
			//custom5 = first track zone 2, boss 0 = 5th track zone 1
			(i == CUSTOM_5) ? adv_order[HUB_1 + 4] : (i == CUSTOM_BOSS_0) ? adv_order[HUB_1 + 3] :
			
			//custom6 = first track zone 3, boss 1 = 2nd track zone 2
			(i == CUSTOM_6) ? adv_order[HUB_2 + 1] : (i == CUSTOM_BOSS1) ? adv_order[HUB_2 + 0] :
			
			//custom10 = first track zone 4, boss2 = 5th track zone 3
			(i == CUSTOM_10) ? adv_order[HUB_3 + 4] : (i == CUSTOM_BOSS2) ? adv_order[HUB_3 + 3] :
			
			//custom14 = first track zone 5, boss3 = 5th track zone 4
			(i == CUSTOM_14) ? adv_order[HUB_4 + 4] : (i == CUSTOM_BOSS3) ? adv_order[HUB_4 + 3] :
			
			//custom18 = first track zone 6, boss4 = 5th track zone 5
		    (i == CUSTOM_18) ? adv_order[HUB_5 + 4] : (i == CUSTOM_BOSS4) ? adv_order[HUB_5 + 3] :

			(i == CUSTOM_BOSS5) ? adv_order[HUB_FINAL + 3] : i - 1;
			
			
			
			if (adv_progress[i]) return;
			
			if (i >= CUSTOM_2)
			{
			if (!adv_progress[fixed_index])
				return;
			}
			
			backup = i;
			
			break;
		}
		else
		{
			//if loop just end without matches
			if (i == MAX_TRACKS - 1)
				return;
		}
	}

    //show warp animation on our kart
	sdata->gGT->drivers[0]->funcPtrs[0] = VehStuckProc_Warp_Init;
	
	//skip if warp animation didnt finished
    if (sdata->gGT->drivers[0]->KartStates.Warp.timer != 800)
		return;
	 
	 current_track = backup;
	 adventure_options();
	 
	 //set laps and spawn
	 sdata->gGT->numLaps = laps % 128;
	 sdata->kartSpawnOrderArray[0] = 1;
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
	int boolWin = (!hardcore) ? (driver->driverRank == 0) : (endTime < requiredTime);
	
	
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
	
	if (BOSS_RACE)
		numKeys++;
	else
		numTrophys++;
	}

	sdata->framesSinceRaceEnded = 0;

	// Load the levelID for Adventure Hub that you came from
	current_track = (current_track != CUSTOM_BOSS5) ? CUSTOM_HUB : current_track;

	
    adventure_options();
	
	MainRaceTrack_RequestLoad((current_track == CUSTOM_BOSS5) ? CREDITS_CRASH : load_track);

}

void custom_engine(unsigned char ogEngine, unsigned char curr_stat, unsigned char stat_value);

void adventure_main()
{
	unsigned char i;
	int buttonTap;
	static bool stats_editor = false;
	static u_char curr_stat = 1;
	static u_char newStat[3];
	struct GameTracker* gGT = sdata->gGT;
	

     void CUSTOM_ADV_HUB(struct GameTracker* gGT);
	 CUSTOM_ADV_HUB(gGT);

	
	if (gGT->levelID == MAIN_MENU_LEVEL)
	{
	    //change main menu entries
		D230.menuMainMenu.rows = &adventure_row;
		
		//remove mask hints from pause menu
		data.menuAdvHub.rows = &adv_rows_nohints;
	}
	
	
	//enable or disable the times condition to win races
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
		
		DecalFont_DrawLine("PRESS SELECT TO DISABLE", 6, 3, FONT_SMALL, ORANGE);
		DecalFont_DrawLine("TIME LIMIT TO WIN THE RACE", 6, 13, FONT_SMALL, ORANGE);
		DecalFont_DrawLine(difficulty, 6, 23, FONT_SMALL, PENTA_WHITE);
	}
	
	
	//disable player to bot swap
	*(int*)0x80017318 = 0x3E00008;
	*(int*)0x8001731c = 0;
	
	
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
			if (hardcore)
			{
			
				if (!BOSS_RACE)
				{

					//use relic instance as decoration
					relicptr = UI_INSTANCE_BirthWithThread(STATIC_RELIC, (int)UI_ThTick_Reward,0xe,1,0,/*sdata->s_relic1*/0);
					relicptr->matrix.t[0] = -206;
					relicptr->matrix.t[1] = 86;
					relicptr->matrix.t[2] = 256;
					relicptr->colorRGBA = instanceColor(BGR_TO_RGB(CTR_FontColor(relic_color)));
				}
				else
				{
					//use key instance as decoration
					keyptr = UI_INSTANCE_BirthWithThread(STATIC_KEY,(int)UI_ThTick_Reward,0xf,1,0,/*sdata->s_key1*/0);
					keyptr->matrix.t[0] = -400;
					keyptr->matrix.t[1] = 152;
				}
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