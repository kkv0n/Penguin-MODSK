#include <common.h>
#include "code/adventure.h"
#include "code/quad.h"

unsigned char g_language = ENGLISH;


/*
LANGUAGES:

	"JAPANESE"
	"ENGLISH"
	"UNITED_KINGDOM"
    "FRENCH"
	"DEUTSCH"
	"ITALIAN"
	"SPANISH"
	"NEDERLANDS"
*/


const char* track_names[MAX_TRACKS] = {
"TRACK1", // CRASH COVE
"TRACK2", // ROO TUBES
"TRACK3", //MYSTERY CAVES
"TRACK4", //SKULL ROCK
"TRACK5", //SEWER SPEEDWAY
"TRACK6", //SLIDE COLISEUM
"TRACK7", //TURBO TRACK
"TRACK8", //TIGER TEMPLE
"TRACK9", //COCO PARK
"TRACK10", //PAPU PYRAMID
"TRACK11", //RAMPAGE RUINS
"TRACK12", //DINGO CANYON
"TRACK13", //BLIZZARD BLUFF
"TRACK14", //DRAGON MINES
"TRACK15", //POLAR PASS
"TRACK16", //ROCKY ROAD
"TRACK17", //TINY ARENA
"TRACK18", //NGIN LABS
"TRACK19", //CORTEX CASTLE
"TRACK20", //HOT AIR SKYWAY
"TRACK21", //NITRO COURT
"TRACK22", //OXIDE STATION
"TRACK23", //RIPPER ROO GARAGE
"TRACK24", //PAPU PAPU GARAGE
"TRACK25", //KOMODO JOE GARAGE
"TRACK26", //PINSTRIPE GARAGE
"TRACK27", //OXIDE GARAGE
};


//GUIDES

/*
EFFECTS GUIDE:

	"NO_EFFECT" //NO EFFECT TO THIS QUADBLOCK
	"ITEMS" //SIMULATES ITEM BOX
	"HAZARD_" //GIVES DAMAGE TO THE PLAYER
	"WUMPA_JUICED" GIVES YOU 10 WUMPAS
	"LAP_COUNT" //THIS INCREMENTS THE LAP
	"ELEVATOR" //YOUR KART FLY FOR SOME SECONDS!
	"SINGLE_ITEM"	//GIVES YOU AN SPECIFIC ITEM

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

    "Q_NONE"                   
    "Q_INVISIBLE"      
    "Q_GRAVITY"    
    "Q_REFLECTION"      
    "Q_KICKERS"       
    "Q_OOF_BOUNDS"
    "Q_NV_USED" 
    "Q_TRIGGER_SCRIPT" 
    "Q_REVERB"          
    "Q_KICKERS2"     
    "Q_MASK_GRAB"      
    "Q_TEMPLE_DOOR"
    "Q_UNKNOWN_TRIGGER"  // COLLISION TRIGGER IN LEV EDITOR(?)
    "Q_GROUND"      
    "Q_WALL"           
    "Q_NO_COLL"    
    "Q_INV_TRIGGERS"

*/


/*	
HAZARD GUIDE:

	NO_FX_SOUND //only to skip sounds, can only be assigned to hazard_fx
	
	//hazards
	"K_SPIN"
	"K_BLASTED"
	"K_SQUASHED"
	"K_FIRE"
	"K_EATEN"
	"CLOCK_EFFECT"
	"TRAMPOLINE"
*/

//END OF GUIDES

void adventure_options()
{
 switch(current_track)
 {
	 case N_SANITY_1: //CRASH COVE IN OG
	 {
		load_track = CRASH_COVE;
		laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		
		break; 
	 }
	 	 case N_SANITY_2: //ROO TUBES IN OG
	 {
		 load_track = ROO_TUBES;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case N_SANITY_3: //MYSTERY CAVES IN OG
	 {
		 load_track = MYSTERY_CAVES;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case N_SANITY_4: //SKULL ROCK IN OG
	 {
		 load_track = SKULL_ROCK;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case N_SANITY_5: //SEWER SPEEDWAY IN OG
	 {
		 load_track = SEWER_SPEEDWAY;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GEMSTONE_1: //SLIDE COLISEUM IN OG
	 {
		 load_track = SLIDE_COLISEUM;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GEMSTONE_2: //TURBO TRACK IN OG
	 {
		 load_track = TURBO_TRACK;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case LOST_RUINS_1: //TIGER TEMPLE IN OG
	 {
		 load_track = TIGER_TEMPLE;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case LOST_RUINS_2: //COCO PARK IN OG
	 {
		 load_track = COCO_PARK;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case LOST_RUINS_3: //PAPU PYRAMID IN OG
	 {
		 load_track = PAPU_PYRAMID;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case LOST_RUINS_4: //RAMPAGE RUINS IN OG
	 {
		 load_track = RAMPAGE_RUINS;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case LOST_RUINS_5: //DINGO CANYON IN OG
	 {
		 load_track = DINGO_CANYON;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GLACIER_1: //BLIZZARD BLUFF IN OG
	 {
		 load_track = BLIZZARD_BLUFF;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GLACIER_2: //DRAGON MINES IN OG
	 {
		 load_track = DRAGON_MINES;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GLACIER_3: //POLAR PASS IN OG
	 {
		 load_track = POLAR_PASS;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GLACIER_4: //ROCKY ROAD IN OG
	 {
		 load_track = ROCKY_ROAD;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GLACIER_5: //TINY ARENA IN OG
	 {
		 load_track = TINY_ARENA;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case CITADEL_1: //N GIN LABS IN OG
	 {
		 load_track = N_GIN_LABS;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case CITADEL_2: //CORTEX CASTLE IN OG
	 {
		 load_track = CORTEX_CASTLE;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case CITADEL_3: //HOT AIR SKYWAY IN OG
	 {
		 load_track = HOT_AIR_SKYWAY;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case CITADEL_4: //NITRO COURT IN OG
	 {
		 load_track = NITRO_COURT;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case CITADEL_5: //OXIDE STATION IN OG
	 {
		 load_track = OXIDE_STATION;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }

        //PD: BOSSES USES BOSS MUSIC, CHANGING THE TRACK DOES NOTHING
		
	 	 case N_SANITY_BOSS: //RIPPER ROO GARAGE
	 {
		 load_track = ROO_TUBES;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case LOST_RUINS_BOSS: //PAPU PAPU GARAGE
	 {
		 load_track = PAPU_PYRAMID;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case GLACIER_BOSS: //KOMODO JOE GARAGE
	 {
		 load_track = DRAGON_MINES;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }

	 	 case CITADEL_BOSS: //PINSTRIPE GARAGE
	 {
		 load_track = HOT_AIR_SKYWAY;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 	 case FINAL_BOSS: //OXIDE GARAGE
	 {
		 load_track = OXIDE_STATION;
		 laps = 3;
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
		single_item[1] = TNTNITRO;
		
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
		
		
		
		
		
		

		break; 
	 }
	 
	 
	 
 }
	
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