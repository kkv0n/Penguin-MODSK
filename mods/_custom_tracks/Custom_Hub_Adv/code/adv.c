#include <common.h>
#include "quad.h"
#include "../adventure_settings.c"
#include "adventure_main.c"
#include "engine.c"

uint16_t engineFlags;

void* unlocked_warppad_texture[4];

struct Instance* relicptr;
struct Instance* keyptr;

unsigned char numTrophys;
unsigned char numKeys;
unsigned char current_track;
bool adv_progress[MAX_TRACKS];
unsigned char hazard_id[4];
unsigned short hazard_fx[4];
unsigned char terrain_effect[4];
unsigned char quad_effect[4];
unsigned char d_terrain[4];
uint16_t q_flag[4];
unsigned char ELEVATOR_TIMER[4];
unsigned char LAP_COUNT_TIMER;
unsigned char desired_driver;
unsigned char delaytimer[16];
unsigned char single_item[4];



struct GameTracker* GT;
struct Driver* driver[4]; //should delete the array later
struct QuadBlock* quadblock[4]; //should delete the array later
unsigned char task = 0;

	unsigned short warppad_id[MAX_TRACKS];
	 

	 
	 
	unsigned char adv_order[MAX_TRACKS] = {CUSTOM_1, CUSTOM_2, CUSTOM_3, CUSTOM_4, CUSTOM_BOSS_0, CUSTOM_5,
	CUSTOM_BOSS1, CUSTOM_6, CUSTOM_7, CUSTOM_8, CUSTOM_9, CUSTOM_BOSS2, CUSTOM_10, CUSTOM_11, CUSTOM_12, CUSTOM_13,
	CUSTOM_BOSS3, CUSTOM_14, CUSTOM_15, CUSTOM_16, CUSTOM_17, CUSTOM_BOSS4, CUSTOM_18, CUSTOM_19, CUSTOM_20, CUSTOM_21, CUSTOM_BOSS5};


	
void CUSTOM_ADV_HUB(struct GameTracker* gGT)
{
		
	if (current_track == CUSTOM_HUB)
	{
		
		if (gGT->trafficLightsTimer > 0)
		{

        gGT->gameMode1 &= ~(START_OF_RACE);
		gGT->hudFlags &= ~(2);
		gGT->hudFlags |= 1;	
		gGT->trafficLightsTimer = 0;
		
	
		}


		if ((gGT->hudFlags & 1) == 0) return;
		
	 
	  DecalFont_DrawLine(&sdata->s_x[0], 422, 18, FONT_SMALL, ORANGE);
	  
	  char num[4];
	  
	  
	 
	  sprintf(num, "%u", numTrophys);
	  DecalFont_DrawLine(num, 435, 14, FONT_BIG, ORANGE);

	 
	  DecalFont_DrawLine(&sdata->s_x[0], 66, 18, FONT_SMALL, ORANGE);
	  

	   sprintf(num, "%u", numKeys);
	  DecalFont_DrawLine(num, 79, 14, FONT_BIG, ORANGE);

		
	}
}

unsigned short warppad_blockIndex[MAX_TRACKS];



void set_spawns()
{
	    
		//spawn if first track is not completed
		if (!adv_progress[adv_order[HUB_1 + 0]])
		{
			sdata->kartSpawnOrderArray[0] = 0;
			curr_page = 0;
		}
		//spawn while first track from 2nd hub is not completed
		else if (!adv_progress[adv_order[HUB_2 + 0]])
		{
			sdata->kartSpawnOrderArray[0] = 1;
			curr_page = 0;
		}
		//spawn while first track from 3rd hub is not completed
		else if (!adv_progress[adv_order[HUB_3]])
		{
			sdata->kartSpawnOrderArray[0] = 2;
			curr_page = 1;
		}
		//spawn while first track from 4th hub is not completed
		else if (!adv_progress[adv_order[HUB_4]])
		{
			sdata->kartSpawnOrderArray[0] = 3;
			curr_page = 2;	
		}
		//spawn while first track from 5th hub is not completed
		else if (!adv_progress[adv_order[HUB_5]])
		{
			sdata->kartSpawnOrderArray[0] = 4;
			curr_page = 3;
		}
	    
		//spawn while first track from 6th hub is not completed
		else if (!adv_progress[adv_order[HUB_FINAL]])
		{
			sdata->kartSpawnOrderArray[0] = 5;
			curr_page = 4;
		}
	    
		//spawn while final hub is not completed (hub 6)
		else if (!adv_progress[adv_order[HUB_FINAL + 3]])
		{
			sdata->kartSpawnOrderArray[0] = 6;
			curr_page = 5;
		}
		
		//spawn after all tracks are finished and only final boss remains
		else
		{
			sdata->kartSpawnOrderArray[0] = 7;
			curr_page = 6;
		}

}

bool set_door_state(unsigned char curr_door, unsigned short door_index, unsigned short invi_qindex)
{
	
	
              if (
                  (!adv_progress[curr_door + CUSTOM_BOSS_0])
              )
			  {  return false; }
				
				sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[door_index].quadFlags &= ~(1 << 11 | 1 << 13);
				
			    memcpy(sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[door_index].ptr_texture_mid,
				sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[invi_qindex].ptr_texture_mid, sizeof(unlocked_warppad_texture));
				
				return true;
}

void set_warppad_color(unsigned char progress, unsigned short locked_texture_blockIndex)
{

	
	for (unsigned char i = 0; i < MAX_TRACKS; i++)
	{	
		
		if (i >= CUSTOM_2 && !adv_progress[adv_order[i]] && adv_progress[adv_order[i - 1]] || i == CUSTOM_1 && !adv_progress[CUSTOM_1])
		{
			printf("warppad_unlocked: %u\n", i);
			
			memcpy(sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[warppad_blockIndex[i]].ptr_texture_mid,
			unlocked_warppad_texture, sizeof(unlocked_warppad_texture));

		}
		else
		{
			printf("warppad locked:%u\n", i);
			
		  
		
		  
			memcpy(sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[warppad_blockIndex[i]].ptr_texture_mid,
			sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[locked_texture_blockIndex].ptr_texture_mid, sizeof(unlocked_warppad_texture));
		}
	}
}

void get_warppadIndex(unsigned short quadblockIndex)
{
	 //this is just blockIDs, dont touch it, its from adventure_settings.c
	 unsigned short organized_ids[MAX_TRACKS] = { warppad_id[0], warppad_id[1], warppad_id[2], warppad_id[3], warppad_id[21],
		warppad_id[4], warppad_id[22], warppad_id[5], warppad_id[6], warppad_id[7], warppad_id[8],
		warppad_id[23], warppad_id[9], warppad_id[10], warppad_id[11], warppad_id[12], warppad_id[24],
		warppad_id[13], warppad_id[14], warppad_id[15], warppad_id[16], warppad_id[25],
		warppad_id[17], warppad_id[18], warppad_id[19], warppad_id[20], warppad_id[26]};
	

	for (unsigned char i = 0; i < MAX_TRACKS; i++)
	{
		
	  if (sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[quadblockIndex].blockID == organized_ids[i])
	  {
		  
		  warppad_blockIndex[i] = quadblockIndex;
		  
		  break;
	  }
	}
	
}

//this happens when a custom track is being loaded
void Custom_TrackLoading(struct GameTracker* gGT)
{
		if (current_track == CUSTOM_HUB)
		{
			
		unsigned short invi_quadblock;
		unsigned short doors_array[NUM_DOORS];
		unsigned short locked_texture;
		
		for (unsigned short i = 0; i < gGT->level1->ptr_mesh_info->numQuadBlock; i++)
		{
			
			get_warppadIndex(i);
			
			
			
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == invisible_texture_blockID)
				invi_quadblock = i;
			
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == warp_pad1_blockID)
				memcpy(unlocked_warppad_texture, gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].ptr_texture_mid, sizeof(unlocked_warppad_texture));			
			
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == locked_warppad_texture_blockID)
				locked_texture = i;
				
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == door_1)
				doors_array[0] = i;
				
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == door_2)
				doors_array[1] = i;
			
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == door_3)
				doors_array[2] = i;
			
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == door_4)
				doors_array[3] = i;
			
			if (gGT->level1->ptr_mesh_info->ptrQuadBlockArray[i].blockID == door_5)
				doors_array[4] = i;
			
			
			
		}
		    unsigned char skip_next;
			
			for (unsigned char a = 0; a < CUSTOM_BOSS_0; a++)
			{
				
				if (a >= CUSTOM_2 && adv_progress[a - 1])
					skip_next = a;
				
			    
			}

			set_warppad_color(skip_next, locked_texture);
			
			for (unsigned char d = 0; d < NUM_DOORS; d++)
			{															
				if (!set_door_state(d, doors_array[d], invi_quadblock)) break;
			}
		
		   set_spawns();
		
	}
}

// starts at 800b8598,
// last byte of Garage Functions

void custom_engine(unsigned char ogEngine, unsigned char curr_stat, unsigned char newStat)
{	
	
	
	unsigned char s;
	struct OVR233_Garage* gGarage = (struct OVR233_Garage*)0x800b8598;
	
	if (curr_stat > TURN || newStat > TURN)
	return;

	static const unsigned char barStats[12] =
	{
		// balanced
		0x37, 0x37, 0x37,
		
		0x50, 0x30, 0x20,
		
		0x20, 0x50, 0xA,
		
		// turn
		0x30, 0x1c, 0x50
	};
	
	static const uint16_t all_stats[12] = { 
	
	ACCEL_MED, //balanced characters accel
	ACCEL_MAX,  //accel characters acceleration
	ACCEL_LOW, //speed characters accel
	ACCEL_HIGH, //turning characters accel
	SPEED_MED, //balanced characters speed
	SPEED_HIGH, //accel characters speed
	SPEED_MAX, //speed characters speed stat
	SPEED_LOW, //turning characters speed
	TURN_HIGH, //balanced characters turn
	TURN_MED, //accel characters turn
	TURN_LOW, //speed characters turn
	TURN_MAX //turning characters turn
	};
	
		      for (s = 0; s < 4; s++)
			  {
				  engineFlags &= ~(all_stats[(((curr_stat - 1) * 4) + s)]);
			  }
			  
			  engineFlags |= all_stats[(((curr_stat - 1) * 4) + newStat)];
			  
			  printf("new stat: %u , new_engine %u\n", newStat, (curr_stat - 1));
			  
	          //speed = 0 , accel = 1, turning = 2, and dont forget  + engineID x 3
	          gGarage->barStat[((ogEngine * 3) + (curr_stat - 1))] =
	          barStats[((curr_stat - 1) + (newStat * 3))];

		
}

void draw_stats_arrow(u_char characterID, u_char curr_stat)
{
				
		bool flipArrow = ((characterID == TINY_TIGER || characterID == PURA ||
		characterID == NITROS_OXIDE || characterID == RIPPER_ROO));
		
		const unsigned int cl = CTR_FontColor(CORTEX_RED);

        struct Icon **iconPtrArray =
        ICONGROUP_GETICONS(sdata->gGT->iconGroup[4]);

		DecalHUD_Arrow2D(
        iconPtrArray[0x38],
        ((flipArrow) ? 250 : 260),
        23 + (curr_stat * 15),
        &sdata->gGT->backBuffer->primMem,
        sdata->gGT->pushBuffer_UI.ptrOT,
        cl,
        cl,
        cl,
        cl,
        0, 0x1000, ((flipArrow) ? 0x800 : 0));
}

void DrawReservesMeter(short posX, short posY, struct Driver* d)
{
	
	u_int colorAndCode;
	short reservesMeter;
	struct DB* backDB;
	u_long* primmemCurr;
	POLY_F4* p;
	short bottomY;
	RECT box2;
	u_int auStack48[2];
	int reservesMeterWidth;

	reservesMeter = posY - 0x2d;

	backDB = sdata->gGT->backBuffer;
	primmemCurr = backDB->primMem.curr;
	p = 0;

	// if there is room left for more
	if (primmemCurr <= (u_long*)backDB->primMem.endMin100)
	{
		// add primitives
		backDB->primMem.curr = &primmemCurr[6];
		p = (POLY_F4*)primmemCurr;
	}

	if (p != 0)
	{
		box2.y = posY - 3;
		box2.w = 0x31;
		box2.h = 3;
		box2.x = posX - 0x31;
		memset(auStack48, 0, 4);

		CTR_Box_DrawWireBox(&box2, auStack48, sdata->gGT->pushBuffer_UI.ptrOT, &sdata->gGT->backBuffer->primMem);

		colorAndCode = 0x280000ff; // red

		//saffi fix by anfrost
		if (d->reserves >= 32766 || d->reserves < 0)
		{
			d->reserves = 32766;
		}

		if (d->reserves > 3600)
		{
			if (d->reserves < 6840)
			{
				colorAndCode = 0x2800ffff; // yellow
			}
			else
			{
				colorAndCode = 0x2800ff00; // green
			}
		}
		*(u_int*)&p->r0 = colorAndCode;
		reservesMeterWidth = d->reserves * 0x4; //Meter maximum is 8000 reserves units
		bottomY = posY - 3;
		p->y0 = posY;
		p->y1 = posY;
		p->x1 = posX;
		p->y2 = bottomY;
		p->x3 = posX;
		p->y3 = bottomY;

		reservesMeter = posX - ((short)(reservesMeterWidth / 0x960));
		if (reservesMeter < (posX - 0x31))
		{
			reservesMeter = posX - 0x31;
			*(u_int*)&p->r0 = 0x28ff0000; // blue
		}
		p->x0 = reservesMeter;
		p->x2 = reservesMeter;

		primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

		*(int*)p = *primmemCurr | 0x5000000;
		*primmemCurr = (u_int)p & 0xffffff;

		backDB = sdata->gGT->backBuffer;
		primmemCurr = backDB->primMem.curr;
		p = 0;

		// If there is room to add more
		if (primmemCurr <= (u_long*)backDB->primMem.endMin100)
		{
			// Add more primitives
			backDB->primMem.curr = &primmemCurr[6];
			p = (POLY_F4*)primmemCurr;
		}

		if (p != 0)
		{
			*(u_int*)&p->r0 = 0x28808080;
			p->y0 = posY;
			p->y1 = posY;
			p->x1 = posX;
			p->y2 = bottomY;
			p->x3 = posX;
			p->y3 = bottomY;
			p->x0 = posX - 0x31;
			p->x2 = posX - 0x31;

			// pointer to OT memory
			primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

			*(int*)p = *primmemCurr | 0x5000000;
			*primmemCurr = (u_int)p & 0xffffff;
		}
	}

	return;
}

void timer()
{
	static unsigned fxms[16];


	for (int i = 0; i < 16; i++)
	{
		if (delaytimer[i] > 0) 	//count down
		{
			fxms[i] += FPS_DOUBLE(30);
			if (fxms[i] >= SECONDS(1))
			{
				fxms[i] -= SECONDS(1);
				delaytimer[i]--;
			}
		}
		else
		{
			fxms[i] = 0;
		}
	}
}


void check_effect(unsigned char run_this)
{

	int i;
	unsigned char desired_hazard;
	unsigned char desired_elevator;
	unsigned char desired_timer;
	unsigned char desired_item;


	//not working
	//int RB_CrateWeapon_LInC();
	//need parameters that i dont know
	//RB_CrateWeapon_LInC();

	if ((GT->gameMode1 & END_OF_RACE) != 0) return;

	for (i = 0; i < MAX_HAZARDS; i++) {
		if (quadblock[desired_driver]->terrain_type == d_terrain[i] ||
			(quadblock[desired_driver]->quadFlags & q_flag[i]) != 0) {
			desired_hazard = i; //checks which hazard should be called
			desired_elevator = i; //checks which elevator timer should be used
			desired_item = i;
			break;
		}
	}



	switch (run_this)
	{
	case ITEMS:
	{
		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (delaytimer[desired_timer] == 0)
		{
			weapon_roulette();
			OtherFX_Play(fx_crate_smash, 0);
			delaytimer[desired_timer] = 3;
		}

		break;
	}

	case HAZARD_:
	{

		call_hazards(desired_hazard);

		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (delaytimer[desired_timer] == 0 && hazard_fx[desired_hazard] != NO_FX_SOUND)
		{
			OtherFX_Play(hazard_fx[desired_hazard], 0);
			delaytimer[desired_timer] = 2;

		}


		break;
	}

	case WUMPA_JUICED:
	{

		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (driver[desired_driver]->numWumpas != 10) driver[desired_driver]->numWumpas = 10;

		if (delaytimer[desired_timer] == 0)
		{
			OtherFX_Play(fx_juiced_up, 0);
			delaytimer[desired_timer] = 3;
		}


		break;
	}

	case LAP_COUNT:
	{
		desired_timer = (desired_driver == 0) ? 1 : desired_driver * 4 + 1;

		if (delaytimer[desired_timer] == 0) //lap increment only happens 1 time every 60 seconds
		{

			OtherFX_Play(fx_lap_cross, 0);
			if (GT->numLaps > driver[desired_driver]->lapIndex) driver[desired_driver]->lapIndex += 1; // just in case
			delaytimer[desired_timer] = LAP_COUNT_TIMER; //timer

			if (GT->numPlyrCurrGame > 1)
			{
				if (driver[desired_driver]->lapIndex == GT->numLaps) driver[desired_driver]->actionsFlagSet |= 0x2000000;
				driver[desired_driver]->heldItemID = 0xf;
				return;
			}

			if (driver[desired_driver]->lapIndex == GT->numLaps)
			{
				driver[desired_driver]->heldItemID = 0xf;
				GT->gameMode1 |= END_OF_RACE;
			}



		}

		break;
	}
	case ELEVATOR:
	{
		desired_timer = (desired_driver == 0) ? 2 : desired_driver * 4 + 2;
		u_char desired_timer2 = (desired_driver == 0) ? 3 : desired_driver * 4 + 3;

		if (delaytimer[desired_timer] == 0 && delaytimer[desired_timer2] == 0) delaytimer[desired_timer] = ELEVATOR_TIMER[desired_elevator]; //sets the timer for the elevator

		if (delaytimer[desired_timer] > 0) //only call this if elevator timer is not 0
		{

			sdata->gGamepads->gamepad[desired_driver].buttonsHeldCurrFrame |= BTN_CROSS; // keep the kart driving

			//fly
			driver[desired_driver]->forcedJump_trampoline = 2;
			driver[desired_driver]->jump_unknown = 0x180;
			driver[desired_driver]->jump_InitialVelY = driver[i]->const_JumpForce * 3;

			if (delaytimer[desired_timer2] == 0) delaytimer[desired_timer2] = ELEVATOR_TIMER[desired_elevator] + 8; //set a cooldown

		}

		break;
	}
	case SINGLE_ITEM:
	{
		desired_timer = (desired_driver == 0) ? 0 : desired_driver * 4;

		if (delaytimer[desired_timer] == 0)
		{
			OtherFX_Play(fx_crate_smash, 0);
			delaytimer[desired_timer] = 3;
		}

		driver[desired_driver]->heldItemID = single_item[desired_item];
		break;
	}
	}




}

void quad_main()
{
	int i;
	int j;
	GT = sdata->gGT;

	timer();
	

	
	if (GT->levelID >= INTRO_RACE_TODAY)
		return;

        
	if ((GT->drivers[0] != NULL) && ((GT->gameMode1 & END_OF_RACE) == 0) && (RaceFlag_IsFullyOffScreen()) && (current_track != CUSTOM_HUB))
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
	
	
	if (GT->trafficLightsTimer > 0 || sdata->Loading.stage != -1) return;

	for (i = 0; i < GT->numPlyrCurrGame; i++)
	{
		driver[i] = NULL;
		quadblock[i] = NULL;
		
			for (j = 0; j < MAX_EFFECTS; j++)
			{
				if (GT->drivers[i] == NULL)
					break;
				else
					driver[i] = GT->drivers[i];
				
				if (GT->drivers[i]->currBlockTouching != NULL)
					quadblock[i] = driver[i]->currBlockTouching;
				else
					break;
				
				if (quadblock[i] == NULL) break;
				
				task = (quadblock[i]->terrain_type == d_terrain[j]) //checks if you are driving in a quad & decides what effect would be applied
					? terrain_effect[j]
					: ((quadblock[i]->quadFlags & q_flag[j]) != 0)
					? quad_effect[j]
					: NO_EFFECT;







				if (task != NO_EFFECT)
				{
					desired_driver = i; //the driver affected

					//these lines can be improved
					driver[desired_driver] = GT->drivers[desired_driver];
					quadblock[desired_driver] = driver[desired_driver]->currBlockTouching;

					check_effect(task); //call this if the player is driving in the specified terrain
				}
			}
		
	}





}



void weapon_roulette()
{


	// if driver already has a weapon, quit
	if (
		(driver[desired_driver]->heldItemID != 0xf) &&
		(driver[desired_driver]->noItemTimer == 0)
		)
	{
		return;
	}

	// held item count
	if (driver[desired_driver]->numHeldItems != 0)
	{
		return;
	}

	// if driver is firing weapon, quit
	if ((driver[desired_driver]->actionsFlagSet & 0x8000) != 0)
	{
		return;
	}

	// if driver has raincloud and weapon is shuffling, quit
	if (driver[desired_driver]->thCloud != 0)
	{
		if (
			(
				(struct RainCloud*)driver[desired_driver]->thCloud->object
				)->boolScrollItem == 1
			)
		{
			return;
		}
	}


	// if driver is influenced by clock weapon, quit
	if (driver[desired_driver]->clockReceive != 0)
	{
		return;
	}


	// set weapon to roulette
	driver[desired_driver]->heldItemID = 0x10;


	// incrememt
	driver[desired_driver]->numTimesHitWeaponBox++;

	// timer for weapon roulette
	driver[desired_driver]->itemRollTimer = FPS_DOUBLE(90);

	// if no roulette
	if ((sdata->gGT->gameMode1 & ROLLING_ITEM) == 0)
	{
		// start loop
		OtherFX_Play(0x5d, 0);

		sdata->gGT->gameMode1 |= ROLLING_ITEM;
	}

	//driver[desired_driver]->PickupTimeboxHUD.cooldown = FPS_DOUBLE(5);
	driver[desired_driver]->noItemTimer = 0;

	if (driver[desired_driver]->heldItemID == 0x10)
	{

		if (driver[desired_driver]->itemRollTimer == 0)
		{
			VehPhysGeneral_SetHeldItem(driver[desired_driver]);
		}
		//if Item roll is not done
		else driver[desired_driver]->itemRollTimer--;
	}
}


void call_hazards(unsigned char s_hazard)
{


	int RB_Hazard_HurtDriver();
	unsigned char selected_hazard[] = { hazard_id[s_hazard], 0, 0 };
	bool using_trampoline = false;
	short jumpForce;
	unsigned int trampoline_fx;
	bool spawn_clock = false;

	driver[desired_driver]->actionsFlagSet &= ~0x100000;


	if (hazard_id[s_hazard] == K_SPIN)
	{
		selected_hazard[2] = 2;
	}
	else
	{
		if (hazard_id[s_hazard] == CLOCK_EFFECT)
		{
			selected_hazard[0] = 1;
			spawn_clock = true;
		}
		else if (hazard_id[s_hazard] == TRAMPOLINE)
		{

			driver[desired_driver]->forcedJump_trampoline = 2;
			// if first frame (basically)
			if (
				// if not currently airborne from forced jump
				(driver[desired_driver]->jump_ForcedMS == 0) ||

				// if jump_InitialVelY was just now set to const_jump
				(driver[desired_driver]->jump_InitialVelY == driver[desired_driver]->const_JumpForce))
			{
				trampoline_fx = (GT->levelID == MYSTERY_CAVES) ? fx_turtle_bounce : fx_bounce;

				OtherFX_Play(trampoline_fx, 0);

			}

			// currently forced airborne
			driver[desired_driver]->jump_ForcedMS = 0xa0;

			jumpForce = driver[desired_driver]->const_JumpForce * 3;

			// if big force jump (turtles)
			if (driver[desired_driver]->forcedJump_trampoline == 2)
			{
				driver[desired_driver]->jump_unknown = 0x180;
				driver[desired_driver]->jump_InitialVelY = jumpForce;
			}

			// if small force jump (turtles)
			else
			{
				driver[desired_driver]->jump_InitialVelY = (jumpForce >> 1);
			}

			// remove force jump (turtles)
			driver[desired_driver]->forcedJump_trampoline = 0;

			using_trampoline = true;


		}
		selected_hazard[2] = 0;
	}

	if (!using_trampoline && RB_Hazard_HurtDriver(driver[desired_driver], selected_hazard[0], selected_hazard[1], selected_hazard[2]) != 0)
	{
		if (spawn_clock) driver[desired_driver]->clockReceive = 0x1e00;
	}

}


//load driver models in race tracks or adv hubs

void LOAD_Custom_LOD_Driver(struct BigHeader* bigfile, unsigned char levelLOD, void* callback)
{
	unsigned char i;
	int gameMode1;
    short MODEL_QUALITY;
	short MPK_QUALITY;
	
    struct GameTracker* gGT = sdata->gGT;
	
	unsigned char drivers = (gGT->numPlyrCurrGame > 2) ?
	gGT->numPlyrCurrGame - 1 : gGT->numPlyrCurrGame + gGT->numBotsCurrGame - 1; 
	
	unsigned char lastIndex = (drivers == 0) ? 1 : drivers;
	
	gameMode1 = gGT->gameMode1;
	
	
	//Decides which model quality should be used
	switch(levelLOD)
	{
		default:
		case 1:
		{
            MODEL_QUALITY = BI_RACERMODELHI;
	        MPK_QUALITY = BI_TIMETRIALPACK;
			break;
		}
		case 2:
		{
			MODEL_QUALITY = BI_RACERMODELMED;
	        MPK_QUALITY = BI_2PARCADEPACK;
			break;
		}
		case 3:
		case 4:
		{
			MODEL_QUALITY = BI_RACERMODELLOW;
	        MPK_QUALITY = BI_4PARCADEPACK;
			break;
		}
		
	}




	if(
			// If you are in Adventure cup
			((gameMode1 & ADVENTURE_CUP) != 0) &&

			// purple gem cup
			(gGT->cup.cupID == 4)
		)
	{
		data.characterIDs[1] = 0xA;
		data.characterIDs[2] = 0x9;
		data.characterIDs[3] = 0xB;
		data.characterIDs[4] = 0x8;
		lastIndex = 4;

	}
	else if ((gameMode1 & TIME_TRIAL) != 0) 
	{
		lastIndex = 1; //just in case
	}


	if(((gameMode1 & (ADVENTURE_MODE | ARCADE_MODE)) != 0) && (levelLOD == 1))
		 LOAD_Robots1P(data.characterIDs[0]);



            for(i = 0; i < lastIndex; i++)
		   {
			// CTR model
			 LOAD_AppendQueue(bigfile, 2,
				MODEL_QUALITY + data.characterIDs[i],
				&data.driverModelExtras[i],0xfffffffe);
		   }
		   

		   	if(((gameMode1 & ARCADE_MODE) != 0) && (levelLOD == 2))
		   {
			   	LOAD_Robots2P(bigfile, data.characterIDs[0], data.characterIDs[1], callback);
				return;
		   }
			
     //mpk
	 LOAD_AppendQueue(
		bigfile, 2,
		MPK_QUALITY + data.characterIDs[i],
		NULL, callback);
	
}