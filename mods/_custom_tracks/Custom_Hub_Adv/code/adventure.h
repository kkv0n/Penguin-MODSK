#ifndef ADVENTURE_H
#define ADVENTURE_H
#include <common.h>

//=============================================================================
//  ADVENTURE LAYOUT CONFIG
//  - Set NUM_HUBS below.
//  - Set hub_track_count[] and spawn_until[] in adventure_settings.c.
//  Hard limits: 6 hubs, 27 tracks, 8 spawn points. We always spawn in hub 1, so
//  NUM_DOORS = NUM_HUBS - 1. The LAST track of every hub is its boss.
//=============================================================================
// ---- HARD CAPS (not user-editable; just the maximum array sizes) ----
#define MAX_HUBS 6                 // never exceed 6 hubs
#define MAX_TRACKS 27              // never exceed 27 tracks
#define MAX_SPAWNS 8               // never exceed 8 hub spawn points
#define MAX_DOORS (MAX_HUBS - 1)   // never exceed 5 doors
#define SPAWN_FINAL 0xFF           // spawn_until[] marker: spawn used once everything is done

// NUM_HUBS, hub_track_count[] and spawn_until[] are USER-EDITABLE in adventure_settings.c.
#define NUM_DOORS (NUM_HUBS - 1)   // doors actually used = one between consecutive used hubs

#define WARPPAD_TRACKS 22 //not bosses (legacy)
#define NUM_CHARACTERS 16
#define BOSS_RACE (current_track > CUSTOM_21 && current_track < CUSTOM_HUB)

//---- layout tables, built once at runtime from the config (build_layout in adv.c) ----
extern unsigned char TOTAL_TRACKS;              //sum of hub_track_count[]
extern unsigned char hub_start[MAX_HUBS];       //adv_order index where each hub starts
extern unsigned char adv_position[MAX_TRACKS];  //inverse of adv_order (track -> progression slot)
extern unsigned char track_hub[MAX_TRACKS];     //hub index each track belongs to
extern const unsigned char NUM_HUBS;   //USER-EDITABLE in adventure_settings.c (hubs used, <= MAX_HUBS)
extern const unsigned char hub_track_count[MAX_HUBS];
extern const unsigned char spawn_until[MAX_SPAWNS];

//warppad slots in PROGRESSION order, for spawn_until[]. WARPPAD_1 = the very first track;
//each hub's boss is the slot right AFTER that hub's normal tracks, so the names keep
//counting (1,2,3,4,5,...) instead of jumping to the boss enum. Names are 1-based but the
//underlying value is 0-based (WARPPAD_1 == 0). Mapped to a real track via adv_order[].
enum WARPPAD_SLOT
{
	WARPPAD_1 = 0, WARPPAD_2, WARPPAD_3, WARPPAD_4, WARPPAD_5, WARPPAD_6, WARPPAD_7,
	WARPPAD_8, WARPPAD_9, WARPPAD_10, WARPPAD_11, WARPPAD_12, WARPPAD_13, WARPPAD_14,
	WARPPAD_15, WARPPAD_16, WARPPAD_17, WARPPAD_18, WARPPAD_19, WARPPAD_20, WARPPAD_21,
	WARPPAD_22, WARPPAD_23, WARPPAD_24, WARPPAD_25, WARPPAD_26, WARPPAD_27
};


extern const char* character_names[NUM_CHARACTERS];
extern struct Instance* relicptr;
extern struct Instance* keyptr;

void adventure_options();
void build_layout();
void CTR_Box_DrawWireBox(RECT* r, int* unk, u_long* ot, struct PrimMem* primMem);
extern unsigned char curr_page;

extern unsigned char load_track;
extern bool isRelic;
extern unsigned char current_track;
extern bool show_stars;
extern char* timeToWin;
extern bool hardcore;
extern unsigned char relic_color;
extern unsigned char numTrophys;
extern unsigned char numKeys;
extern unsigned char numRelics;
extern unsigned char BOSSES_IDS[6];
extern bool track_is_relic[MAX_TRACKS];

extern bool adv_progress[MAX_TRACKS];
extern unsigned short warppad_id[MAX_TRACKS];
extern unsigned char adv_order[MAX_TRACKS];
extern unsigned short warppad_blockIndex[MAX_TRACKS];


extern unsigned short invisible_texture_blockID;
extern unsigned short locked_warppad_texture_blockID;
extern void* unlocked_warppad_texture[4];
extern const char* HUB_NAMES[MAX_HUBS + 1]; //one per hub + the final/gemstone page

extern unsigned short warp_pad1_blockID;
extern unsigned short warp_pad2_blockID;
extern unsigned short warp_pad3_blockID;
extern unsigned short warp_pad4_blockID;
extern unsigned short warp_pad5_blockID;
extern unsigned short warp_pad6_blockID;

extern unsigned short door_1;

extern unsigned short warp_pad7_blockID;
extern unsigned short warp_pad8_blockID;
	
extern unsigned short door_2;
	
extern unsigned short warp_pad9_blockID;
extern unsigned short warp_pad10_blockID;
extern unsigned short warp_pad11_blockID;
extern unsigned short warp_pad12_blockID;	
extern unsigned short warp_pad13_blockID;
extern unsigned short warp_pad14_blockID;


extern unsigned short door_3;
	
extern unsigned short warp_pad15_blockID;	
extern unsigned short warp_pad16_blockID;
extern unsigned short warp_pad17_blockID;
extern unsigned short warp_pad18_blockID;
extern unsigned short warp_pad19_blockID;
extern unsigned short warp_pad20_blockID;
	

extern unsigned short door_4;

	
extern unsigned short warp_pad21_blockID;
extern unsigned short boss1_blockID;
extern unsigned short boss2_blockID;
extern unsigned short boss3_blockID;
extern unsigned short boss4_blockID;
extern unsigned short boss5_blockID;
extern unsigned short boss6_blockID;

extern unsigned short door_5;

//get ctr color e.g CRASH_BLUE
#define CTR_FontColor(x) ((unsigned int)(*data.ptrColor[x]))

//convert bgr color to RGB
#define BGR_TO_RGB(x) ((unsigned int)((x & 0xFF) << 16 | ((x) & 0xFF00) | ((x >> 16) & 0xFF)))

//convert relic color from BGR -> to RGB because instances apparently uses RGBA (?)
#define instanceColor(x) ((unsigned int)(x) << 4)



extern unsigned char laps;
extern const char* track_names[MAX_TRACKS];

enum TRACK_FILE_IN_BIGFILE
{
	CUSTOM_HUB_VRM = 203,
	CUSTOM_HUB_LEV = 204,
	
	//RELIC RACE/TIME TRIAL IN BIGFILE
	COVE_VRM = 30,
	COVE_LEV = 31,
	TUBES_VRM = 54,
	TUBES_LEV = 55,
	MCAVES_VRM = 78,
	MCAVES_LEV = 79,
	SEWER_VRM = 70,
	SEWER_LEV = 71,
	SKROCK_VRM = 168,
	SKROCK_LEV = 169,
	TEMPLE_VRM = 38,
	TEMPLE_LEV = 39,
	CPARK_VRM = 118,
	CPARK_LEV = 119,
	PPYRAMID_VRM = 46,
	PPYRAMID_LEV = 47,
	DCANYON_VRM = 6,
	DCANYON_LEV = 7,
	RAMPAGER_VRM = 152,
	RAMPAGER_LEV = 153,
	BBLUFF_VRM = 22,
	BBLUFF_LEV = 23,
	DMINES_VRM = 14,
	DMINES_LEV = 15,
	POLARP_VRM = 102,
	POLARP_LEV = 103,
	TINYA_VRM = 126,
	TINYA_LEV = 127,
	ROCKYR_VRM = 184,
	ROCKYR_LEV = 185,
	NGLABS_VRM = 94,
	NGLABS_LEV = 95,
	CTCASTLE_VRM = 86,
	CTCASTLE_LEV = 87,
	HTSKYWAY_VRM = 62,
	HTSKYWAY_LEV = 63,
	OXSTATION_VRM = 110,
	OXSTATION_LEV = 111,
	NITRO_COURT_VRM = 144,
	NITRO_COURT_LEV = 145,
	SLIDEC_VRM = 134,
	SLIDEC_LEV = 135,
	TURBOT_VRM = 142,
	TURBOT_LEV = 143,


	//1P ARCADE FILES
	BOSS1_VRM = 48,
	BOSS1_LEV = 49,
	BOSS2_VRM = 40,
	BOSS2_LEV = 41,
	BOSS3_VRM = 8,
	BOSS3_LEV = 9,
	BOSS4_VRM = 56,
	BOSS4_LEV = 57,
	BOSS5_VRM = 104,
	BOSS5_LEV = 105


};

enum TRACK_NAME_IN_LNG
{
	CUSTOM_HUB_NAME = 129,
	
	//THE NAME OF EVERY TRACK IN ADVENTURE MODE
	DINGO_CANYON_NAME = 110,
	DRAGON_MINES_NAME,
	BLIZZARD_BLUFF_NAME,
	CRASH_COVE_NAME,
	TIGER_TEMPLE_NAME,
	PAPU_PYRAMID_NAME,
	ROO_TUBES_NAME,
	HOT_AIR_SKYWAY_NAME,
	SEWER_SPEEDWAY_NAME,
	MYSTERY_CAVES_NAME,
	CORTEX_CASTLE_NAME,
	N_GIN_LABS_NAME,
	POLAR_PASS_NAME,
	OXIDE_STATION_NAME,
	COCO_PARK_NAME,
	TINY_ARENA_NAME,
	SLIDE_COLISEUM_NAME,
	TURBO_TRACK_NAME,
	SKULL_ROCK_NAME = 101,
	NITRO_COURT_NAME = 98,
	RAMPAGE_RUINS_NAME,
	ROCKY_ROAD_NAME = 103,

	//BOSS GARAGE NAME
	BOSS1_GARAGE_NAME = 355,
	BOSS2_GARAGE_NAME,
	BOSS3_GARAGE_NAME,
	BOSS4_GARAGE_NAME,
	BOSS5_GARAGE_NAME,

	//BOSS NAME
	BOSS4_NAME = 52,
	BOSS2_NAME,
	BOSS1_NAME,
	BOSS3_NAME,
	BOSS5_NAME = 57,


};

enum ADVENTURE_TRACKS
{

	CUSTOM_1,
	CUSTOM_2,
	CUSTOM_3,
	CUSTOM_4,
	CUSTOM_5,
	CUSTOM_6,
	CUSTOM_7,
	CUSTOM_8,
	CUSTOM_9,
	CUSTOM_10,
	CUSTOM_11,
	CUSTOM_12,
	CUSTOM_13,
	CUSTOM_14,
	CUSTOM_15,
	CUSTOM_16,
	CUSTOM_17,
	CUSTOM_18,
	CUSTOM_19,
	CUSTOM_20,
	CUSTOM_21,
	CUSTOM_BOSS_0,
	CUSTOM_BOSS1,
	CUSTOM_BOSS2,
	CUSTOM_BOSS3,
	CUSTOM_BOSS4,
	CUSTOM_BOSS5,
	CUSTOM_HUB

};

enum CLASSES
{
	INTERMEDIATE,
	SKILLED,
	ADVANCED,
	BEGINNER,
};

enum engineLNG
{
   BEGINNER_LNG = 584,
   INTERMEDIATE_LNG,
   ADVANCED_LNG
};

//custom stats by penta3
enum CLASS_FLAGS
{
  ACCEL_MED = (1 << 0), //balanced characters accel
  ACCEL_MAX = (1 << 1),  //accel characters acceleration
  ACCEL_LOW = (1 << 2), //speed characters accel
  ACCEL_HIGH = (1 << 3), //turning characters accel
  SPEED_MED = (1 << 4), //balanced characters speed
  SPEED_HIGH = (1 << 5), //accel characters speed
  SPEED_MAX = (1 << 6), //speed characters speed stat
  SPEED_LOW = (1 << 7), //turning characters speed
  TURN_HIGH = (1 << 8), //balanced characters turn
  TURN_MED = (1 << 9), //accel characters turn
  TURN_LOW = (1 << 10), //speed characters turn
  TURN_MAX = (1 << 11), //turning characters turn
};



#endif