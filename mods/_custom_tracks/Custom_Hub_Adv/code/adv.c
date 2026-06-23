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
unsigned char numRelics;
bool track_is_relic[MAX_TRACKS]; //per-track mode derived from isRelic (see build_relic_table)
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

	//layout tables derived from hub_track_count[] by build_layout(). The adv_order
	//initializer above is just a fallback that build_layout() overwrites from config.
	unsigned char adv_position[MAX_TRACKS];
	unsigned char hub_start[MAX_HUBS];
	unsigned char track_hub[MAX_TRACKS];
	unsigned char TOTAL_TRACKS = MAX_TRACKS;

	//one boss character per hub, indexed by (current_track - CUSTOM_BOSS_0): hub1..hub6
	unsigned char BOSSES_IDS[6] = {RIPPER_ROO, PAPU_PAPU, KOMODO_JOE, PINSTRIPE, NITROS_OXIDE, PENTA_PENGUIN};


	
//build adv_order / adv_position / hub_start / track_hub / TOTAL_TRACKS from the
//hub_track_count[] config in adventure_settings.c. Idempotent (runs once). Bosses
//are always CUSTOM_BOSS_0 + hubIndex and sit as the LAST track of each hub.
void build_layout()
{
	static bool done = false;
	if (done) return;
	done = true;

	unsigned char pos = 0;
	unsigned char normal = CUSTOM_1;

	for (unsigned char h = 0; h < NUM_HUBS; h++)
	{
		hub_start[h] = pos;

		unsigned char count = hub_track_count[h];

		//normal tracks of this hub (all but the last)
		for (unsigned char j = 0; j + 1 < count; j++)
		{
			adv_order[pos] = normal;
			adv_position[normal] = pos;
			track_hub[normal] = h;
			normal++;
			pos++;
		}

		//boss = last track of the hub
		unsigned char boss = CUSTOM_BOSS_0 + h;
		adv_order[pos] = boss;
		adv_position[boss] = pos;
		track_hub[boss] = h;
		pos++;
	}

	TOTAL_TRACKS = pos;
}

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

	  //relic counter
	  DecalFont_DrawLine(&sdata->s_x[0], 66, 18, FONT_SMALL, ORANGE);
	  sprintf(num, "%u", numRelics);
	  DecalFont_DrawLine(num, 79, 14, FONT_BIG, ORANGE);

	  //key counter
	  DecalFont_DrawLine(&sdata->s_x[0], 272, 18, FONT_SMALL, ORANGE);
	  sprintf(num, "%u", numKeys);
	  DecalFont_DrawLine(num, 285, 14, FONT_BIG, ORANGE);

		
	}
}

unsigned short warppad_blockIndex[MAX_TRACKS];



void set_spawns()
{
	//Pick the first spawn whose "until" track (spawn_until[s], from adventure_settings.c)
	//has NOT been completed yet. SPAWN_FINAL is the spawn used once all gates are cleared.
	for (unsigned char s = 0; s < MAX_SPAWNS; s++)
	{
		unsigned char gate = spawn_until[s];

		if (gate == SPAWN_FINAL)
		{
			sdata->kartSpawnOrderArray[0] = s;
			curr_page = NUM_HUBS; //final / gemstone page
			return;
		}

		//gate is a warppad slot (progression order); map it to a real track via adv_order
		if (!adv_progress[adv_order[gate]])
		{
			sdata->kartSpawnOrderArray[0] = s;
			curr_page = (s == 0) ? 0 : s - 1;
			return;
		}
	}

	//fallback if no SPAWN_FINAL was configured
	sdata->kartSpawnOrderArray[0] = 0;
	curr_page = 0;
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

	
	for (unsigned char i = 0; i < TOTAL_TRACKS; i++)
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
	 //the warppad of the i-th track in progression order is warppad_id[adv_order[i]]
	 //(derived from the layout instead of the old hardcoded permutation)
	for (unsigned char i = 0; i < TOTAL_TRACKS; i++)
	{
	  if (sdata->gGT->level1->ptr_mesh_info->ptrQuadBlockArray[quadblockIndex].blockID == warppad_id[adv_order[i]])
	  {
		  
		  warppad_blockIndex[i] = quadblockIndex;
		  
		  break;
	  }
	}
	
}

//this happens when a custom track is being loaded
void build_layout();

void Custom_TrackLoading(struct GameTracker* gGT)
{
		//make sure the derived layout tables exist before we touch warppads/doors
		build_layout();

		if (current_track == CUSTOM_HUB)
		{

		unsigned short invi_quadblock;
		//sized to the max possible doors (5) so lowering NUM_HUBS can't overflow it;
		//only the first NUM_DOORS entries are actually used below
		unsigned short doors_array[5];
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
			break;
		}
	}



	switch (run_this)
	{
	//(ITEMS quad trigger removed: weapon_roulette is no longer driven by quads)

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
	//(SINGLE_ITEM quad trigger removed)
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



void weapon_roulette(unsigned char index)
{
	struct Driver* d = sdata->gGT->drivers[index];

	// if driver already has a weapon, quit
	if ((d->heldItemID != 0xf) && (d->noItemTimer == 0))
		return;

	// held item count
	if (d->numHeldItems != 0)
		return;

	// if driver is firing weapon, quit
	if ((d->actionsFlagSet & 0x8000) != 0)
		return;

	// if driver has raincloud and weapon is shuffling, quit
	if (d->thCloud != 0)
	{
		if (((struct RainCloud*)d->thCloud->object)->boolScrollItem == 1)
			return;
	}

	// if driver is influenced by clock weapon, quit
	if (d->clockReceive != 0)
		return;

	// set weapon to roulette
	d->heldItemID = 0x10;
	d->numTimesHitWeaponBox++;
	d->itemRollTimer = FPS_DOUBLE(90);

	// if no roulette running yet, start the shuffle loop
	if ((sdata->gGT->gameMode1 & ROLLING_ITEM) == 0)
	{
		OtherFX_Play(0x5d, 0);
		sdata->gGT->gameMode1 |= ROLLING_ITEM;
	}

	d->noItemTimer = 0;

	if (d->heldItemID == 0x10)
	{
		if (d->itemRollTimer == 0)
			VehPhysGeneral_SetHeldItem(d);
		else d->itemRollTimer--; // item roll not done yet
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

//PSX trampoline helper: returns to the caller's caller (mimics the hooked tail)
static inline s32 get_ret(void) {
    s32 r;
    asm volatile("lw %0, 284($gp)" : "=r"(r));
    return r;
}
void LOAD_Custom_LOD_Driver(struct BigHeader* bigfile, unsigned char levelLOD, void* arg2)
{
	struct GameTracker* gGT = sdata->gGT;
	s32 gameMode1 = gGT->gameMode1;
	s32 i;
	unsigned char LT_DRAM = 2;

	//boss race (1v1): load the player + this hub's boss character.
	//ADVENTURE_BOSS is only set for BOSS_RACE && !isRelic (see adventure_main.c),
	//so an isRelic boss track falls through to a normal/relic load instead.
	if ((gameMode1 & ADVENTURE_BOSS) != 0)
	{
		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[0] + BI_RACERMODELHI,
			&data.driverModelExtras[0], -2);

		data.characterIDs[1] = BOSSES_IDS[current_track - CUSTOM_BOSS_0];

		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[1] + BI_RACERMODELHI,
			&data.driverModelExtras[1], -2);

		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[1] + BI_TIMETRIALPACK,
			NULL, arg2);

		return get_ret();
	}

	//preserve Custom_Hub purple gem cup roster
	if (((gameMode1 & ADVENTURE_CUP) != 0) && (gGT->cup.cupID == 4))
	{
		data.characterIDs[1] = 0xA;
		data.characterIDs[2] = 0x9;
		data.characterIDs[3] = 0xB;
		data.characterIDs[4] = 0x8;

		for (i = 0; i < 5; i++)
		{
			LOAD_AppendQueue(bigfile, LT_DRAM,
				data.characterIDs[i] + BI_RACERMODELHI,
				&data.driverModelExtras[i], -2);
		}

		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[7] + BI_TIMETRIALPACK,
			NULL, arg2);

		return get_ret();
	}

	//main menu (not time trial): just the 1P arcade pack
	if (((gameMode1 & MAIN_MENU) != 0) && ((gameMode1 & TIME_TRIAL) == 0))
	{
		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[0] + BI_1PARCADEPACK,
			NULL, arg2);

		return get_ret();
	}

	//cutscene / credits / garage: adventure pack
	if (
			((gameMode1 & GAME_CUTSCENE) != 0) ||
			((gGT->gameMode2 & CREDITS) != 0) ||
			(gGT->levelID == ADVENTURE_GARAGE))
	{
		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[0] + BI_ADVENTUREPACK,
			NULL, arg2);

		return get_ret();
	}

	//adventure arena: player model + time trial pack
	if ((gameMode1 & ADVENTURE_ARENA) != 0)
	{
		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[0] + BI_RACERMODELHI,
			&data.driverModelExtras[0], -2);

		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[7] + BI_TIMETRIALPACK,
			NULL, arg2);

		return get_ret();
	}

	//default: load all 8 racers so the AI grid has its models
	for (i = 0; i < 8; i++)
	{
		if (i > 0)
			data.characterIDs[i] = (data.characterIDs[0] + i) % 8;

		LOAD_AppendQueue(bigfile, LT_DRAM,
			data.characterIDs[i] + BI_RACERMODELHI,
			&data.driverModelExtras[i], -2);
	}

	LOAD_AppendQueue(bigfile, LT_DRAM,
		data.characterIDs[7] + BI_TIMETRIALPACK,
		NULL, arg2);

	return get_ret();
}

//used by VehBirth_GetModelByName (getmodel.c) to locate a loaded character model
struct Model* gtm(char *searchName)
{
	struct Model *m;
	struct Model **models;
	int i;

	// array of character models loaded (up to 8)
	models = (struct Model **)&data.driverModelExtras[0];

	for (i = 0; i < 8; i++)
	{
		m = models[i];

		// 16 bytes is enough to match the name
		if (
				(m != NULL) &&
				(*(u_int *)&m->name[0] == *(u_int *)&searchName[0]) &&
				(*(u_int *)&m->name[4] == *(u_int *)&searchName[4]) &&
				(*(u_int *)&m->name[8] == *(u_int *)&searchName[8]) &&
				(*(u_int *)&m->name[12] == *(u_int *)&searchName[12])
			)
		{
			return m;
		}
	}

	models = (struct Model**)sdata->PLYROBJECTLIST;

	if (
			(models != NULL) &&
			(models[0] != NULL)
		)
	{
		for (
				i = 0,	m = models[i];
				m != NULL;
				i++,	m = models[i]
			)
		{
			if (
					(*(u_int *)&m->name[0] == *(u_int *)&searchName[0]) &&
					(*(u_int *)&m->name[4] == *(u_int *)&searchName[4]) &&
					(*(u_int *)&m->name[8] == *(u_int *)&searchName[8]) &&
					(*(u_int *)&m->name[12] == *(u_int *)&searchName[12])
				)
			{
				return m;
			}
		}
	}

	return NULL;
}