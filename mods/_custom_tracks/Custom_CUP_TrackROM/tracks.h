#ifndef TRACKS_H
#define TRACKS_H
#include <common.h>

#define NO_QUAD 2 //somehow "2" will never happen in quadflags so maybe it is like nothing
#define ARCADE_TIME ARCADE_MODE //arcade mode was mixed with time trial
#define VS_MODE 0x0 //VERSUS MODE







//compilation modifiers
//#define DEV_MODE // compile developer mode which allows flying, turbo trigger with BUTTON UP, and disable hud + first person cam with select

//#define USE_MODS // compile retro fueled

#define CUSTOM_CUP //compile 4 tracks

//if custom cup is disabled
#ifndef CUSTOM_CUP

//enable this if you want 2 tracks instead of just 1 when cup macro is disabled

//#define DOUBLE_TRACK

#endif









#ifdef USE_MODS
extern unsigned char selected_mod;
#endif

#ifdef DEV_MODE
extern bool firstperson;
#endif

extern bool show_stars;
extern struct GameTracker* GT;
extern struct Driver* driver[4];
extern struct QuadBlock* quadblock[4];
extern unsigned int gamemode;
extern unsigned char terrain_effect[4];
extern unsigned char quad_effect[4];
extern unsigned char hazard_id[4];
extern unsigned short hazard_fx[4];
extern unsigned char d_terrain[4];
extern unsigned char delaytimer[16];
extern unsigned char ELEVATOR_TIMER[4];
extern unsigned int Times[2];
extern uint16_t q_flag[4];
extern unsigned char single_item[4];
#ifdef CUSTOM_CUP
extern char* names[5];
#elif defined(DOUBLE_TRACK)
extern char* names[2];
#else
extern char* names;
#endif
extern unsigned char desired_driver;
extern char* selected_name;
extern const char* cup_tittle;
extern const char* madeby;
extern unsigned char selected_track;
extern unsigned char track_id;
extern unsigned char d_vram;
extern unsigned char d_track;
extern unsigned char d_laps;
extern unsigned char t_color;
extern unsigned char g_language;
extern unsigned char LAP_COUNT_TIMER;
extern bool cup_enabled;
extern unsigned int PB_lap;
extern unsigned int Worst_lap;
extern unsigned char item_set[];
extern void track_options();
extern void weapon_roulette();
extern void call_hazards(unsigned char s_hazard);

/*
track files in ctr bigfile starts with 0 & 1(dingo canyon) and ends in 142 & 143, each track have 4 lev and ram files
battle tracks starts in 144 & 145 and ends in 198 & 199
adventure hubs starts in 200 & 201 and ends in  214 & 215
overlay files starts in 221 & 222 and ends in 232 & 233
*/
typedef enum {

	TRACK_1_RAM = 24,
	TRACK_1_LEV = 25,
	TRACK_2_RAM = 26,
	TRACK_2_LEV = 27,
	TRACK_3_RAM = 28,
	TRACK_3_LEV = 29,
	TRACK_4_RAM = 30,
	TRACK_4_LEV = 31

} CTRBIGFILE;


enum QUADBLOCKFLAGS {

	Q_NONE = 0,
	Q_INVISIBLE = (1 << 0),
	Q_GRAVITY = (1 << 1),
	Q_REFLECTION = (1 << 2),
	Q_KICKERS = (1 << 3),
	Q_OOF_BOUNDS = (1 << 4),
	Q_NV_USED = (1 << 5),
	Q_TRIGGER_SCRIPT = (1 << 6),
	Q_REVERB = (1 << 7),
	Q_KICKERS2 = (1 << 8),
	Q_MASK_GRAB = (1 << 9),
	Q_TEMPLE_DOOR = (1 << 10),
	Q_UNKNOWN_TRIGGER = (1 << 11),  // COLLISION TRIGGER IN LEV EDITOR
	Q_GROUND = (1 << 12),
	Q_WALL = (1 << 13),
	Q_NO_COLL = (1 << 14),
	Q_INV_TRIGGERS = (1 << 15),
	Q_ALL = -1

};


//levelID from ctr levs (0-24)
enum CTRTRACKS
{
	CANYON,
	MINES,
	BLUFF,
	COVE,
	TTEMPLE,
	PYRAMID,
	TUBES,
	HAS,
	SEWER,
	CAVES,
	CASTLE,
	N_LABS,
	PASS,
	STATION,
	CPARK,
	ARENA,
	COLISEUM,
	TURBOT,
	NITROC,
	RRUINS,
	PKLOT,
	SKROCK,
	BOWL,
	RROAD,
	LABB

};

//ctr weapons(0-15)
enum WEAPONS
{
	ITURBO,
	BOMBX1,
	MISSILEX1,
	TNTNITRO,
	I_POTION,
	I_SPRING,
	PSHIELD,
	IMASK,
	ICLOCK,
	IWARPBALL,
	BOMBX3,
	MISSILEX3,
	I_INVISIBILITY,
	I_SUPER_ENGINE,
	NOTHING_WEAPON,
	NOTHING_NO_WEAPON
};

//do this when you touch a quadblock (0-3)
enum EFFECT
{
	NO_EFFECT,
	ITEMS,
	HAZARD_,
	WUMPA_JUICED,
	LAP_COUNT,
	ELEVATOR,
	SINGLE_ITEM,

	MAX_EFFECTS = 4
};

#ifdef USE_MODS
enum MODS
{
	NO_MODS,
	RETRO_FUELED
};
#endif


//language files in the game(0-7)
enum LNGFILES
{
	JAPANESE,
	ENGLISH,
	UNITED_KINGDOM,
	FRENCH,
	DEUTSCH,
	ITALIAN,
	SPANISH,
	NEDERLANDS
};

//hazard applied to a quadblock (1-7)
enum HAZARDS
{
	NO_FX_SOUND = 0,
	K_SPIN,
	K_BLASTED,
	K_SQUASHED,
	K_FIRE,
	K_EATEN,
	CLOCK_EFFECT,
	TRAMPOLINE,

	MAX_HAZARDS = 4
};


//fx played when an effect is active(0-258)
enum SOUNDFX
{
	fx_menu_selection_change,
	fx_menu_confirm,
	fx_button_back,
	fx_letter_del,
	fx_some_ding_sound,
	fx_menu_locked,
	fx_kart_hit_wall,
	fx_kart_land,
	fx_kart_hop,
	fx_weapon_spring,
	fx_kart_collide_strong,
	fx_kart_collide_weak,
	fx_barrel_land,
	fx_kart_turbo_mini,
	fx_kart_turbo_full,
	fx_weapon_bomb_hit2,
	fx_kart_slowdown,
	fx_kart_skid,
	fx_kart_roll_offroad,
	fx_kart_roll_ice,
	fx_kart_curb,
	fx_kart_roll_wood,
	fx_kart_roll_ice2,
	fx_kart_roll_snow,
	fx_kart_roll_offroad2,
	fx_ambient_water,
	fx_kart_roll_metal,
	fx_roll_water,
	fx_en_kart_crash_yes,
	fx_en_kart_cortex_yes,
	fx_en_kart_tiny_yes,
	fx_en_kart_coco_yes,
	fx_en_kart_ngin_yes,
	fx_en_kart_dingo_yes,
	fx_en_kart_polar_yes,
	fx_en_kart_pura_yes,
	fx_en_kart_pinstripe_yes,
	fx_en_kart_papu_yes,
	fx_en_kart_roo_yes,
	fx_en_kart_joe_yes,
	fx_en_kart_ntropy_yes,
	fx_en_kart_pen_yes,
	fx_en_kart_fake_yes,
	fx_en_kart_oxide_yes,
	fx_en_kart_crash_hit,
	fx_en_kart_cortex_hit,
	fx_en_kart_tiny_hit,
	fx_en_kart_coco_hit,
	fx_en_kart_ngin_hit,
	fx_en_kart_dingo_hit,
	fx_en_kart_polar_hit,
	fx_en_kart_pura_hit,
	fx_en_kart_pinstripe_hit,
	fx_en_kart_papu_hit,
	fx_en_kart_roo_hit,
	fx_en_kart_joe_hit,
	fx_en_kart_ntropy_hit,
	fx_en_kart_pen_hit,
	fx_en_kart_fake_hit,
	fx_en_kart_oxide_hit,
	fx_crate_smash,
	fx_weapon_tnt_explosion,
	fx_weapon_tnt_buzz,
	fx_weapon_beaker_hit,
	fx_time_crate_freeze,
	fx_juiced_up,
	fx_pickup_wumpa_crate,
	fx_pickup_wumpa_single,
	fx_weapon_clock_deploy,
	fx_semaphor,
	fx_semaphor2,
	fx_weapon_bomb_fire,
	fx_weapon_bomb_loop,
	fx_weapon_bomb_hit,
	fx_weapon_rocket_fire,
	fx_weapon_rocket_loop,
	fx_weapon_rocket_hit,
	fx_weapon_orb_deploy,
	fx_weapon_orb_loop,
	fx_weapon_orb_gone,
	fx_weapon_tnt_pickup,
	fx_bounce,
	fx_weapon_deploy,
	fx_weapon_aku_aku,
	fx_weapon_uka_uka,
	fx_char_pitfall,
	fx_weapon_tracking,
	fx_weapon_shield_on,
	fx_weapon_shield_off,
	fx_weapon_shield_loop,
	fx_squash_in,
	fx_squash_out,
	fx_water_splash,
	fx_weapon_roulette_loop,
	fx_weapon_roulette_done,
	fx_weapon_rocket_hit2,
	fx_weapon_engine,
	fx_weapon_invisible_in,
	fx_weapon_invisible_out,
	fx_relic_discount,
	fx_letter_pickup,
	fx_menu_switch,
	fx_lap_cross,
	fx_prize_awarded,
	fx_temple_flames,
	fx_fire_bum,
	fx_weapon_spring2,
	fx_plant_chew,
	fx_plant_hit,
	fx_plant_hit2,
	fx_plant_chew2,
	fx_plant_spit,
	fx_armadillo_roll,
	fx_amadillo_hit,
	fx_minecart_roll,
	fx_boulder_roll,
	fx_barrel_roll,
	fx_temple_door,
	fx_skyway_engine,
	fx_seal_tum_around,
	fx_seal_hit,
	fx_spider_up,
	fx_spider_down,
	fx_spider_hit,
	fx_spider_hit_loop,
	fx_turtle_splash,
	fx_turtle_bounce,
	fx_ambient_station,
	fx_squash_out2,
	fx_fireball_spawn,
	fx_ambient_water2,
	fx_ambient_water3,
	fx_ambient_mines,
	fx_seagull,
	fx_ambient_drip,
	fx_ambient_tubes,
	fx_ambient_speedway,
	fx_ambient_wind,
	fx_ambient_skyway,
	fx_pipe_hiss,
	fx_roll_water2,
	fx_roll_water3,
	fx_ambient_lab,
	fx_glide_or_wind,
	fx_ambient_lab2,
	fx_neon_buzz,
	fx_stadium_audience,
	fx_hub_door_unlock,
	fx_hub_door_open,
	fx_boss_door,
	fx_oxide_door,
	fx_warppad_warp,
	fx_warppad_loop,
	fx_savescreen_scanline,
	fx_prize_pickup,
	fx_menu_intro_c,
	fx_menu_intro_t,
	fx_menu_intro_r,
	fx_menu_intro_ctr,
	fx_menu_intro_ring,
	fx_menu_intro_transition,
	fx_menu_intro_throwtrophy,
	fx_menu_intro_catchtrophy,
	fx_podium_crash_step,
	fx_podium_crash_swing,
	fx_podium_crash_sleep01,
	fx_podium_crash_sleep02,
	fx_podium_cortex_laugh,
	fx_podium_cortex_sad,
	fx_podium_crash_step2,
	fx_podium_tiny_roar1,
	fx_podium_tiny_roar2,
	fx_podium_coco_typing01,
	fx_podium_coco_typing02,
	fx_podium_ngin_idle,
	fx_podium_ngin_steam,
	fx_podium_dingo_step,
	fx_podium_dingo_laser,
	fx_en_kart_polar_yes2,
	fx_podium_polar_snore,
	fx_en_kart_pura_yes2,
	fx_podium_pura_lick,
	fx_podium_stripe_gunfire,
	fx_podium_stripe_gunclick,
	fx_podium_stripe_gunshake,
	fx_podium_papu_belly,
	fx_podium_papu_step,
	fx_podium_roo_bounce,
	fx_podium_roo_step,
	fx_podium_joe_sad,
	fx_podium_joe_woosh01,
	fx_podium_joe_woosh02,
	fx_podium_tropy_sad,
	fx_podium_tropy_spring,
	fx_podium_penta_step,
	fx_podium_penta_spin,
	fx_podium_fakecrash_swing,
	fx_podium_fakecrash_step,
	fx_podium_oxide_step,
	fx_cutscenes_birds,
	fx_cutscenes_forest,
	fx_cutscenes_racing,
	fx_cutscene_tinytiger1,
	fx_cutscene_ship_terminal1,
	fx_cutscene_ship_terminal2,
	fx_cutscene_ship_terminal3,
	fx_cutscene_ship_terminal4,
	fx_cutscenes_cocotyping1,
	fx_cutscenes_cocotyping2,
	fx_cutscene_cortex_laugh,
	fx_cutscenes_machineactive,
	fx_cutscene_tinytiger2,
	fx_cutscene_dingodile,
	fx_cutscene_ambience_blizzard,
	fx_cutscene_cortex_power,
	fx_cutscene_cameramove,
	fx_cutscene_cameramove2,
	fx_chara_crash_yoyo1,
	fx_chara_crash_yoyo2,
	fx_chara_crash_yoyo3,
	fx_neon_buzz2,
	fx_whoosh_high,
	fx_chara_cortex_buttons,
	fx_chara_cortex_machine,
	fx_chara_tiny_weights1,
	fx_chara_tiny_weights2,
	fx_chara_tiny_weights03_unused,
	fx_chara_tiny_grunt1,
	fx_chara_tiny_grunt2,
	fx_chara_coco_buttons1,
	fx_chara_coco_buttons2,
	fx_glide_or_wind2,
	fx_chara_ngin_hammer,
	fx_chara_ngin_hammer2,
	fx_chara_dingo_refill,
	fx_chara_dingo_refill2,
	fx_podium_joe_woosh01_2,
	fx_podium_joe_woosh02_2,
	fx_chara_dingo_fumance,
	fx_en_kart_polar_yes3,
	fx_ambient_wind2,
	fx_chara_pura_pur,
	fx_chara_pura_steps,
	fx_chara_pura_birds1,
	fx_chara_pura_birds2,
	fx_chara_pura_birds3,
	fx_mask_spawn_instrumental,
	fx_box_intro_smash1,
	fx_box_intro_smash2,
	fx_box_intro_smash3,
	fx_box_intro_open,
	fx_box_intro_teaser,
	fx_pew_zap01,
	fx_box_intro_aku,
	fx_whoosh_long_doppler,
	fx_box_intro_karts,
	fx_mask_spawn,
	fx_mask_gone
};

#endif