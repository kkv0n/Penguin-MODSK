#include "tracks.h"

//Test 
//game language
unsigned char g_language = ENGLISH;

/*
LANGUAGES:

	JAPANESE,
	ENGLISH,
	UNITED_KINGDOM,
    FRENCH,
	DEUTSCH,
	ITALIAN,
	SPANISH,
	NEDERLANDS
*/


//cup tittle & credits
const char* cup_tittle = "CUSTOM TRACK CUP";
const char* madeby = "BY N.CORTEX";
unsigned char t_color = PENTA_WHITE; //if you want to change the color then take a look to the color guide


//track names in main menu
#ifdef CUSTOM_CUP

char* names[5] = {
	"track1",
	"track2",
	"track3",
	"track4",
	"ALL"
	};
	
#elif defined(DOUBLE_TRACK)

char* names[2] = {
	"track1",
	"track2"
};

#else
char* names = "track1";
#endif

//GUIDES:

  /*
  COLOR GUIDE:
  
  	ORANGE
	PERIWINKLE
	ORANGE_DARKENED
	RED
	WHITE
	CRASH_BLUE
	CORTEX_RED
	TINY_GREEN
	COCO_MAGENTA
	N_GIN_PURPLE
	DINGODILE_OLIVE
	POLAR_CYAN
	PURA_VIOLET
	PINSTRIPE_PALE_DARK_BLUE
	PAPU_YELLOW
	ROO_ORANGE
	JOE_COLOR
	TROPY_LIGHT_BLUE
	PENTA_WHITE
	FAKE_CRASH_GRAY
	OXIDE_LIGHT_GREEN
	BLACK
	SILVER
	GRAY
	PLAYER_BLUE
	PLAYER_RED
	PLAYER_GREEN
	PLAYER_YELLOW

	DARK_RED
	LIGHT_GREEN
	FOREST_GREEN
	CREDITS_FADE
  */
  
  /*
  MODS GUIDE: //needs to enable the macro in tracks.h before!
  	NO_MODS
	RETRO_FUELED
	
  */
  
  /*
  TRACK GUIDE:
  
	CANYON = DINGO CANYON
	MINES = DRAGON MINES
	BLUFF = BLIZZARD BLUFF
	COVE = CRASH COVE
	TTEMPLE = TIGER TEMPLE
	PYRAMID = PAPU PYRAMID
	TUBES = ROO TUBES
	HAS = HOT AIR SKYWAY
	SEWER = SEWER SPEEDWAY
	CAVES = MISTERY CAVES
	CASTLE = CORTEX CASTLE
	N_LABS = NGIN LABS
	PASS = POLAR PASS
	STATION = OXIDE STATION
	CPARK = COCO PARK
	ARENA = TINY ARENA
	COLISEUM = SLIDE COLISEUM
	TURBOT = TURBO TRACK
	NITROC = NITRO COURT
	RRUINS = RAMPAGE RUINS
	PKLOT = PARKING LOT
	SKROCK = SKULL ROCK
	BOWL = NORTH BOWL
	RROAD = ROCKY ROAD
	LABB = LAB BASEMENT
	*/



/*
GAMEMODE GUIDE: //PD: OTHER MODES THAN ARCADE_TIME CAN CRASH THE GAME!!! BE CAREFUL

    BATTLE_MODE  //NEED TO BE TESTED	
	ARCADE_TIME //ARCADE MERGED WITH TIME TRIAL
    VS_MODE	 //VERSUS MODE
	RELIC_RACE			
	CRYSTAL_CHALLENGE               
	ADVENTURE_BOSS      
*/

/*
EFFECTS GUIDE:

	NO_EFFECT //NO EFFECT TO THIS QUADBLOCK
	ITEMS //SIMULATES ITEM BOX
	HAZARD_ //GIVES DAMAGE TO THE PLAYER
	WUMPA_JUICED, GIVES YOU 10 WUMPAS
	LAP_COUNT //THIS INCREMENTS THE LAP
	ELEVATOR //YOUR KART FLY FOR SOME SECONDS!
	SINGLE_ITEM //GIVES YOU AN SPECIFIC ITEM

*/

/*
TERRAINS GUIDE: //FLOOR TYPE

    TERRAIN_ASPHALT
    TERRAIN_DIRT
    TERRAIN_GRASS
    TERRAIN_WOOD
    TERRAIN_WATER
    TERRAIN_STONE
    TERRAIN_ICE
    TERRAIN_TRACK
    TERRAIN_ICY_ROAD
    TERRAIN_SNOW
    TERRAIN_NONE
    TERRAIN_HARDPACK
    TERRAIN_METAL
    TERRAIN_FASTWATER
    TERRAIN_MUD
    TERRAIN_SIDESLIP
    TERRAIN_RIVERASPHALT
    TERRAIN_STEAMASPHALT
    TERRAIN_OCEANASPHALT
    TERRAIN_SLOWGRASS
    TERRAIN_SLOWDIRT
	
*/


/*
QUADFLAGS GUIDE:

    Q_NONE                   
    Q_INVISIBLE      
    Q_GRAVITY    
    Q_REFLECTION      
    Q_KICKERS       
    Q_OOF_BOUNDS
    Q_NV_USED 
    Q_TRIGGER_SCRIPT 
    Q_REVERB          
    Q_KICKERS2     
    Q_MASK_GRAB      
    Q_TEMPLE_DOOR
    Q_UNKNOWN_TRIGGER  // COLLISION TRIGGER IN LEV EDITOR(?)
    Q_GROUND      
    Q_WALL           
    Q_NO_COLL    
    Q_INV_TRIGGERS

*/


/*	
HAZARD GUIDE:

	NO_FX_SOUND //only to skip sounds, can only be assigned to hazard_fx
	
	//hazards
	K_SPIN
	K_BLASTED
	K_SQUASHED
	K_FIRE
	K_EATEN
	CLOCK_EFFECT
	TRAMPOLINE
*/

//END OF GUIDES







void track_options()
{
	if (selected_track == 0) //track 1
	{
		//track info
		track_id = COVE;
		gamemode = ARCADE_TIME;
		d_laps = 3;
		show_stars = false;
		LAP_COUNT_TIMER = 60;
		
		
		
		
		//quadblock options	
		//1
		d_terrain[0] = TERRAIN_WATER;
		hazard_id[0] = K_SPIN;
		hazard_fx[0] = fx_semaphor;
		q_flag[0] = NO_QUAD;
		terrain_effect[0] = NO_EFFECT;
		quad_effect[0] = NO_EFFECT;
		ELEVATOR_TIMER[0] = 3; //seconds 1 2 3 etc
		single_item[0] = TNTNITRO;
		
		//2
		d_terrain[1] = TERRAIN_FASTWATER;
		hazard_id[1] = K_SPIN;
		hazard_fx[1] = fx_weapon_tnt_explosion;
		q_flag[1] = NO_QUAD;
		terrain_effect[1] = NO_EFFECT;
		quad_effect[1] = NO_EFFECT;
		ELEVATOR_TIMER[1] = 3; //seconds 1 2 3 etc
		single_item[1] = TNTNITRO;
		
		//3
		d_terrain[2] = TERRAIN_METAL;
		hazard_id[2] = K_SPIN;
		hazard_fx[2] = NO_FX_SOUND;
		q_flag[2] = NO_QUAD;
		terrain_effect[2] = NO_EFFECT;
		quad_effect[2] = NO_EFFECT;
		ELEVATOR_TIMER[2] = 3; //seconds 1 2 3 etc
		single_item[2] = TNTNITRO;
		
		//4
		d_terrain[3] = TERRAIN_MUD;
		hazard_id[3] = K_SPIN;
		hazard_fx[3] = NO_FX_SOUND;
		q_flag[3] = NO_QUAD;
		terrain_effect[3] = NO_EFFECT;
		quad_effect[3] = NO_EFFECT;
		ELEVATOR_TIMER[3] = 3; //seconds 1 2 3 etc
		single_item[3] = TNTNITRO;

		


		





		
		
		#ifdef USE_MODS
		selected_mod = RETRO_FUELED;
		#endif
	}
	
	#if defined (CUSTOM_CUP) || defined(DOUBLE_TRACK)
	if (selected_track == 1) //track 2
	{
		
		//track info
		track_id = CPARK;
		gamemode = ARCADE_TIME;
		d_laps = 3;
		show_stars = false;
		LAP_COUNT_TIMER = 60;
				
		
		
		
		
		//quadblock options	
		//1
		d_terrain[0] = TERRAIN_WATER;
		hazard_id[0] = K_SPIN;
		hazard_fx[0] = fx_semaphor;
		q_flag[0] = NO_QUAD;
		terrain_effect[0] = NO_EFFECT;
		quad_effect[0] = NO_EFFECT;
		ELEVATOR_TIMER[0] = 3; //seconds 1 2 3 etc
		single_item[0] = TNTNITRO;
		
		//2
		d_terrain[1] = TERRAIN_FASTWATER;
		hazard_id[1] = K_SPIN;
		hazard_fx[1] = fx_weapon_tnt_explosion;
		q_flag[1] = NO_QUAD;
		terrain_effect[1] = NO_EFFECT;
		quad_effect[1] = NO_EFFECT;
		ELEVATOR_TIMER[1] = 3; //seconds 1 2 3 etc
		single_item[1] = TNTNITRO;
		
		//3
		d_terrain[2] = TERRAIN_METAL;
		hazard_id[2] = K_SPIN;
		hazard_fx[2] = NO_FX_SOUND;
		q_flag[2] = NO_QUAD;
		terrain_effect[2] = NO_EFFECT;
		quad_effect[2] = NO_EFFECT;
		ELEVATOR_TIMER[2] = 3; //seconds 1 2 3 etc
		single_item[2] = TNTNITRO;
		
		//4
		d_terrain[3] = TERRAIN_MUD;
		hazard_id[3] = K_SPIN;
		hazard_fx[3] = NO_FX_SOUND;
		q_flag[3] = NO_QUAD;
		terrain_effect[3] = NO_EFFECT;
		quad_effect[3] = NO_EFFECT;
		ELEVATOR_TIMER[3] = 3; //seconds 1 2 3 etc
		single_item[3] = TNTNITRO;
		
		
		
		
		
		#ifdef USE_MODS
		selected_mod = RETRO_FUELED;
		#endif
	}
	#endif
	
	#ifdef CUSTOM_CUP
	if (selected_track == 2) //track 3
	{
		//track info
		track_id = TTEMPLE;
		gamemode = ARCADE_TIME;
		d_laps = 3;
		show_stars = false;
		LAP_COUNT_TIMER = 60;
				
		
		
		
		
		//quadblock options	
		//1
		d_terrain[0] = TERRAIN_WATER;
		hazard_id[0] = K_SPIN;
		hazard_fx[0] = fx_semaphor;
		q_flag[0] = NO_QUAD;
		terrain_effect[0] = NO_EFFECT;
		quad_effect[0] = NO_EFFECT;
		ELEVATOR_TIMER[0] = 3; //seconds 1 2 3 etc
		single_item[0] = TNTNITRO;
		
		//2
		d_terrain[1] = TERRAIN_FASTWATER;
		hazard_id[1] = K_SPIN;
		hazard_fx[1] = fx_weapon_tnt_explosion;
		q_flag[1] = NO_QUAD;
		terrain_effect[1] = NO_EFFECT;
		quad_effect[1] = NO_EFFECT;
		ELEVATOR_TIMER[1] = 3; //seconds 1 2 3 etc
		single_item[1] = TNTNITRO;
		
		//3
		d_terrain[2] = TERRAIN_METAL;
		hazard_id[2] = K_SPIN;
		hazard_fx[2] = NO_FX_SOUND;
		q_flag[2] = NO_QUAD;
		terrain_effect[2] = NO_EFFECT;
		quad_effect[2] = NO_EFFECT;
		ELEVATOR_TIMER[2] = 3; //seconds 1 2 3 etc
		single_item[2] = TNTNITRO;
		
		//4
		d_terrain[3] = TERRAIN_MUD;
		hazard_id[3] = K_SPIN;
		hazard_fx[3] = NO_FX_SOUND;
		q_flag[3] = NO_QUAD;
		terrain_effect[3] = NO_EFFECT;
		quad_effect[3] = NO_EFFECT;
		ELEVATOR_TIMER[3] = 3; //seconds 1 2 3 etc
		single_item[3] = TNTNITRO;
		
		
		
		
		
		
		#ifdef USE_MODS
		selected_mod = NO_MODS;
		#endif
	}
	
	if (selected_track == 3) //track 4
	{
		//track info
		track_id = COLISEUM;
		gamemode = ARCADE_TIME;
		d_laps = 3;
		show_stars = false;
		LAP_COUNT_TIMER = 60;
				
		
		
		

		//quadblock options	(4 available for each track)
		//1
		d_terrain[0] = TERRAIN_WATER;
		hazard_id[0] = K_SPIN;
		hazard_fx[0] = fx_semaphor;
		q_flag[0] = NO_QUAD;
		terrain_effect[0] = NO_EFFECT;
		quad_effect[0] = NO_EFFECT;
		ELEVATOR_TIMER[0] = 3; //seconds 1 2 3 etc
		single_item[0] = TNTNITRO;
		
		
		//2
		d_terrain[1] = TERRAIN_FASTWATER;
		hazard_id[1] = K_SPIN;
		hazard_fx[1] = fx_weapon_tnt_explosion;
		q_flag[1] = NO_QUAD;
		terrain_effect[1] = NO_EFFECT;
		quad_effect[1] = NO_EFFECT;
		ELEVATOR_TIMER[1] = 3; //seconds 1 2 3 etc
		single_item[1] = TNTNITRO;
		
		//3
		d_terrain[2] = TERRAIN_METAL;
		hazard_id[2] = K_SPIN;
		hazard_fx[2] = NO_FX_SOUND;
		q_flag[2] = NO_QUAD;
		terrain_effect[2] = NO_EFFECT;
		quad_effect[2] = NO_EFFECT;
		ELEVATOR_TIMER[2] = 3; //seconds 1 2 3 etc
		single_item[2] = TNTNITRO;
		
		//4
		d_terrain[3] = TERRAIN_MUD;
		hazard_id[3] = K_SPIN;
		hazard_fx[3] = NO_FX_SOUND;
		q_flag[3] = NO_QUAD;
		terrain_effect[3] = NO_EFFECT;
		quad_effect[3] = NO_EFFECT;
		ELEVATOR_TIMER[3] = 3; //seconds 1 2 3 etc
		single_item[3] = TNTNITRO;
		
		
		
		
		
		
		#ifdef USE_MODS
		selected_mod = NO_MODS;
		#endif
	}
	#endif
	
	
}


//itemset available if you enable items in your track, if you want to change it then only replace them, dont add more
//PD: THIS IS ONLY FOR ROULETTE! IF YOU WANT A SINGLE ITEM YOU CAN USE SINGLE_ITEM EFFECT!
unsigned char item_set[] = {
    IMASK, IMASK, IMASK, IMASK, IMASK, IMASK, IMASK, IMASK,
    TNTNITRO, TNTNITRO, TNTNITRO, TNTNITRO, TNTNITRO, TNTNITRO, TNTNITRO, TNTNITRO,
    ITURBO, ITURBO,
    ITURBO,
    ITURBO,
};

/*
WEAPONS GUIDE:

    ITURBO
    BOMBX1
    MISSILEX1
    TNTNITRO
    I_POTION
    I_SPRING
    PSHIELD
    IMASK
    ICLOCK
    IWARPBALL
    BOMBX3
    MISSILEX3
    I_INVISIBILITY
    I_SUPER_ENGINE
    NOTHING_WEAPON
    NOTHING_NO_WEAPON
*/


/*
SOUND FX GUIDE:
  fx_menu_selection_change
  fx_menu_confirm
  fx_button_back
  fx_letter_del
  fx_some_ding_sound
  fx_menu_locked
  fx_kart_hit_wall
  fx_kart_land
  fx_kart_hop
  fx_weapon_spring
  fx_kart_collide_strong
  fx_kart_collide_weak
  fx_barrel_land
  fx_kart_turbo_mini
  fx_kart_turbo_full
  fx_weapon_bomb_hit2
  fx_kart_slowdown
  fx_kart_skid
  fx_kart_roll_offroad
  fx_kart_roll_ice
  fx_kart_curb
  fx_kart_roll_wood
  fx_kart_roll_ice2
  fx_kart_roll_snow
  fx_kart_roll_offroad2
  fx_ambient_water
  fx_kart_roll_metal
  fx_roll_water
  fx_en_kart_crash_yes
  fx_en_kart_cortex_yes
  fx_en_kart_tiny_yes
  fx_en_kart_coco_yes
  fx_en_kart_ngin_yes
  fx_en_kart_dingo_yes
  fx_en_kart_polar_yes
  fx_en_kart_pura_yes
  fx_en_kart_pinstripe_yes
  fx_en_kart_papu_yes
  fx_en_kart_roo_yes
  fx_en_kart_joe_yes
  fx_en_kart_ntropy_yes
  fx_en_kart_pen_yes
  fx_en_kart_fake_yes
  fx_en_kart_oxide_yes
  fx_en_kart_crash_hit
  fx_en_kart_cortex_hit
  fx_en_kart_tiny_hit
  fx_en_kart_coco_hit
  fx_en_kart_ngin_hit
  fx_en_kart_dingo_hit
  fx_en_kart_polar_hit
  fx_en_kart_pura_hit
  fx_en_kart_pinstripe_hit
  fx_en_kart_papu_hit
  fx_en_kart_roo_hit
  fx_en_kart_joe_hit
  fx_en_kart_ntropy_hit
  fx_en_kart_pen_hit
  fx_en_kart_fake_hit
  fx_en_kart_oxide_hit
  fx_crate_smash
  fx_weapon_tnt_explosion
  fx_weapon_tnt_buzz
  fx_weapon_beaker_hit
  fx_time_crate_freeze
  fx_juiced_up
  fx_pickup_wumpa_crate
  fx_pickup_wumpa_single
  fx_weapon_clock_deploy
  fx_semaphor
  fx_semaphor2
  fx_weapon_bomb_fire
  fx_weapon_bomb_loop
  fx_weapon_bomb_hit
  fx_weapon_rocket_fire
  fx_weapon_rocket_loop
  fx_weapon_rocket_hit
  fx_weapon_orb_deploy
  fx_weapon_orb_loop
  fx_weapon_orb_gone
  fx_weapon_tnt_pickup
  fx_bounce
  fx_weapon_deploy
  fx_weapon_aku_aku
  fx_weapon_uka_uka
  fx_char_pitfall
  fx_weapon_tracking
  fx_weapon_shield_on
  fx_weapon_shield_off
  fx_weapon_shield_loop
  fx_squash_in
  fx_squash_out
  fx_water_splash
  fx_weapon_roulette_loop
  fx_weapon_roulette_done
  fx_weapon_rocket_hit2
  fx_weapon_engine
  fx_weapon_invisible_in
  fx_weapon_invisible_out
  fx_relic_discount
  fx_letter_pickup
  fx_menu_switch
  fx_lap_cross
  fx_prize_awarded
  fx_temple_flames
  fx_fire_bum
  fx_weapon_spring2
  fx_plant_chew
  fx_plant_hit
  fx_plant_hit2
  fx_plant_chew2
  fx_plant_spit
  fx_armadillo_roll
  fx_amadillo_hit
  fx_minecart_roll
  fx_boulder_roll
  fx_barrel_roll
  fx_temple_door
  fx_skyway_engine
  fx_seal_tum_around
  fx_seal_hit
  fx_spider_up
  fx_spider_down
  fx_spider_hit
  fx_spider_hit_loop
  fx_turtle_splash
  fx_turtle_bounce
  fx_ambient_station
  fx_squash_out2
  fx_fireball_spawn
  fx_ambient_water2
  fx_ambient_water3
  fx_ambient_mines
  fx_seagull
  fx_ambient_drip
  fx_ambient_tubes
  fx_ambient_speedway
  fx_ambient_wind
  fx_ambient_skyway
  fx_pipe_hiss
  fx_roll_water2
  fx_roll_water3
  fx_ambient_lab
  fx_glide_or_wind
  fx_ambient_lab2
  fx_neon_buzz
  fx_stadium_audience
  fx_hub_door_unlock
  fx_hub_door_open
  fx_boss_door
  fx_oxide_door
  fx_warppad_warp
  fx_warppad_loop
  fx_savescreen_scanline
  fx_prize_pickup
  fx_menu_intro_c
  fx_menu_intro_t
  fx_menu_intro_r
  fx_menu_intro_ctr
  fx_menu_intro_ring
  fx_menu_intro_transition
  fx_menu_intro_throwtrophy
  fx_menu_intro_catchtrophy
  fx_podium_crash_step
  fx_podium_crash_swing
  fx_podium_crash_sleep01
  fx_podium_crash_sleep02
  fx_podium_cortex_laugh
  fx_podium_cortex_sad
  fx_podium_crash_step2
  fx_podium_tiny_roar1
  fx_podium_tiny_roar2
  fx_podium_coco_typing01
  fx_podium_coco_typing02
  fx_podium_ngin_idle
  fx_podium_ngin_steam
  fx_podium_dingo_step
  fx_podium_dingo_laser
  fx_en_kart_polar_yes2
  fx_podium_polar_snore
  fx_en_kart_pura_yes2
  fx_podium_pura_lick
  fx_podium_stripe_gunfire
  fx_podium_stripe_gunclick
  fx_podium_stripe_gunshake
  fx_podium_papu_belly
  fx_podium_papu_step
  fx_podium_roo_bounce
  fx_podium_roo_step
  fx_podium_joe_sad
  fx_podium_joe_woosh01
  fx_podium_joe_woosh02
  fx_podium_tropy_sad
  fx_podium_tropy_spring
  fx_podium_penta_step
  fx_podium_penta_spin
  fx_podium_fakecrash_swing
  fx_podium_fakecrash_step
  fx_podium_oxide_step
  fx_cutscenes_birds
  fx_cutscenes_forest
  fx_cutscenes_racing
  fx_cutscene_tinytiger1
  fx_cutscene_ship_terminal1
  fx_cutscene_ship_terminal2
  fx_cutscene_ship_terminal3
  fx_cutscene_ship_terminal4
  fx_cutscenes_cocotyping1
  fx_cutscenes_cocotyping2
  fx_cutscene_cortex_laugh
  fx_cutscenes_machineactive
  fx_cutscene_tinytiger2
  fx_cutscene_dingodile
  fx_cutscene_ambience_blizzard
  fx_cutscene_cortex_power
  fx_cutscene_cameramove
  fx_cutscene_cameramove2
  fx_chara_crash_yoyo1
  fx_chara_crash_yoyo2
  fx_chara_crash_yoyo3
  fx_neon_buzz2
  fx_whoosh_high
  fx_chara_cortex_buttons
  fx_chara_cortex_machine
  fx_chara_tiny_weights1
  fx_chara_tiny_weights2
  fx_chara_tiny_weights03_unused
  fx_chara_tiny_grunt1
  fx_chara_tiny_grunt2
  fx_chara_coco_buttons1
  fx_chara_coco_buttons2
  fx_glide_or_wind2
  fx_chara_ngin_hammer
  fx_chara_ngin_hammer2
  fx_chara_dingo_refill
  fx_chara_dingo_refill2
  fx_podium_joe_woosh01_2
  fx_podium_joe_woosh02_2
  fx_chara_dingo_fumance
  fx_en_kart_polar_yes3
  fx_ambient_wind2
  fx_chara_pura_pur
  fx_chara_pura_steps
  fx_chara_pura_birds1
  fx_chara_pura_birds2
  fx_chara_pura_birds3
  fx_mask_spawn_instrumental
  fx_box_intro_smash1
  fx_box_intro_smash2
  fx_box_intro_smash3
  fx_box_intro_open
  fx_box_intro_teaser
  fx_pew_zap01
  fx_box_intro_aku
  fx_whoosh_long_doppler
  fx_box_intro_karts
  fx_mask_spawn
  fx_mask_gone
*/
