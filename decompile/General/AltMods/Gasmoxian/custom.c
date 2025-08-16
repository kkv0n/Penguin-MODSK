#include <common.h>
#include "global.h"

//N_Verted
// #include "GameModes/N-Verted/n_verted.c"
// #include "GameModes/N-Verted/n_verted_driver_state.c"
// #include "GameModes/N-Verted/n_verted_lap_skip_prevention.c"

// #include "GameModes/LevelModifiers/level_modifiers.c"

enum FIRE_LEVEL {
	SACRED = 0x100,
	USF = 0x800
};

enum ITEMS {
	ITEM_TURBO_BOOST = 0,
	ITEM_BOWLING_BOMB,
	ITEM_TRACKING_MISSILE,
	ITEM_EXPLOSIVE_CRATE,
	ITEM_N_BRIO_BEAKER,
	ITEM_SPRING,
	ITEM_POWER_SHIELD,
	ITEM_MASK,
	ITEM_N_TROPY_CLOCK,
	ITEM_WARP_ORB,
	ITEM_BOWLING_BOMB_X3,
	ITEM_TRACKING_MISSILE_X3,
	ITEM_INVISIBILITY,
	ITEM_SUPER_ENGINE
};

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
	Q_COLLISION_TRIGGER = (1 << 11),
	Q_GROUND = (1 << 12),
	Q_WALL = (1 << 13),
	Q_NO_COLL = (1 << 14),
	Q_INV_TRIGGERS = (1 << 15),
	Q_ALL = -1

};

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

bool USE_N_VERTED;

char* decalText = (char*)0x1F800000;
struct GameTracker *gGT;

static bool init_initialized = false;
static bool initialized = false;

// Helper function for absolute value
int abs_val(int value) { return value < 0 ? -value : value; }

// Track previous quadblocks for each player
struct QuadBlock* lastValid_prev[4] = {NULL, NULL, NULL, NULL};
struct QuadBlock* prevTouchedBlock[4] = {NULL, NULL, NULL, NULL};

void MaskGrab(struct Thread* t, struct Driver* d) {

    //if mask grab its already going then quit
    if (d->kartState == KS_MASK_GRABBED) return;

    // Play sound effect for mask grab
    OtherFX_Play(fx_menu_locked, 0);

    // Store original position values
    int origX = d->posCurr.x;
    int origY = d->posCurr.y;
    int origZ = d->posCurr.z;
    
    // Call original mask grab
    VehStuckProc_MaskGrab_Init(t, d);
    
    // Restore original position
    d->posCurr.x = origX;
    d->posCurr.y = origY;
    d->posCurr.z = origZ;
    
    // Fix previous position too (prevents teleporting on next frame)
    d->posPrev.x = origX;
    d->posPrev.y = origY;
    d->posPrev.z = origZ;
    
    // Position mask to follow player
    struct MaskHeadWeapon* mask = d->KartStates.MaskGrab.maskObj;
    if (mask != NULL) {
        mask->pos[0] = origX >> 8;
        mask->pos[1] = (origY >> 8) + 0x140; // Position mask above player
        mask->pos[2] = origZ >> 8;
    }

    // Get player index from the driver pointer
    int playerIndex = -1;
    for (int i = 0; i < 4; i++) {
        if (sdata->gGT->drivers[i] == d) {
            playerIndex = i;
            break;
        }
    }
    
}

// Prevent lap skips (NMZ Abuse)
void PreventLapSkip(struct Driver* driver, int driverIndex) {
    struct GameTracker *gGT = sdata->gGT;
    
    // Continue only if driver has a valid quadblock
    if (driver != NULL && driver->lastValid != NULL) {
        int maxCheckpoint = gGT->level1->cnt_restart_points - 1;
        int currentCheckpoint = (int)(unsigned char) driver->lastValid->checkpointIndex;
        
        // int maxCheckpointSkip = (maxCheckpoint + 1) / 5; // 20% of the track length
        //Max checkpoint skip will be 20% if CORTEX_CASTLE and 26% for HOT_AIR_SKYWAY, else no limit
        int maxCheckpointSkip = (gGT->levelID == CORTEX_CASTLE) ? (maxCheckpoint / 5) : (gGT->levelID == HOT_AIR_SKYWAY) ? ((maxCheckpoint * 13) / 50) : maxCheckpoint + 1;
        
        // Only check if we have a previous valid checkpoint
        if (lastValid_prev[driverIndex] != NULL && currentCheckpoint != lastValid_prev[driverIndex]->checkpointIndex) {
            int prevCheckpoint = (int)(unsigned char)lastValid_prev[driverIndex]->checkpointIndex;

            // Detect lap skip (NMZ abuse)
            if (
                currentCheckpoint == maxCheckpoint 							//If player lands on NMZ
                && prevCheckpoint > 5										//If player was a bit ahead of the start
                && abs_val(currentCheckpoint - prevCheckpoint) > 15			//If it was doing a skip
            ) { 
                // Reset to previous valid quadblock
                driver->lastValid = lastValid_prev[driverIndex];

                //Mask grab the player
                MaskGrab(driver->instSelf->thread, driver);

                // sprintf(decalText, "LAP SKIP BLOCKED! P%d", driverIndex+1);
                // DecalFont_DrawLine(decalText, 0x100, 0xc8 + 10, FONT_SMALL, (JUSTIFY_CENTER | RED));
            }
            //This conditional can be removed but will allow too broken SCs on Castle and HAS
            else if (
                prevCheckpoint != 0xFF
                && currentCheckpoint != 0xFF
                && prevCheckpoint < maxCheckpoint - 1
                && prevCheckpoint > 5
                && abs_val(currentCheckpoint - prevCheckpoint) > maxCheckpointSkip
            ) {
                driver->lastValid = lastValid_prev[driverIndex];
                MaskGrab(driver->instSelf->thread, driver);
            }
        }
        
        // Update previous valid quadblock for this driver
        lastValid_prev[driverIndex] = driver->lastValid;
    }
}

// Initialize or reset lap skip prevention system
void InitLapSkipPrevention() {
    for (unsigned char i = 0; i < 4; i++) {
        lastValid_prev[i] = NULL;
    }
}
// Define structure for block and its timer
typedef struct {
    short blockId;
    unsigned char frameCount;
    unsigned char frameCountDelay; // delay before starting jump
} JumpBlock;

//(0.5s at 30FPS ≈ 15 frames)

JumpBlock BLIZZARD_BLUFF_blocks[] = {
    // Big jump 1
    {1555, 12, 0},
    {1556, 12, 0},
    {1558, 12, 0},
    {1559, 12, 0},
    {1557, 12, 0},
    {1316, 12, 0},
    {1314, 12, 0},

    // Big jump 2
    {1482, 13, 0},
    {1526, 13, 0},
    {1481, 13, 0},
    {1473, 13, 0},
    {1472, 13, 0},
    {1476, 13, 0},
    {1485, 13, 0},
    {1489, 13, 0},
    {1475, 13, 0}    
};

JumpBlock DINGO_CANYON_blocks[] = {
    {957, 13, 0},
    {881, 13, 0},
};

JumpBlock MYSTERY_CAVES_blocks[] = {
    //Jump 1
    {2313, 4, 0},
    {1789, 4, 0},

    //Jump 2
    {2427, 2, 0},
    
    //Jump 3
    {2485, 3, 0},
    {2480, 3, 0},
    {2539, 3, 0},

    //Turtle ponds
    {768, 1, 1},
    {769, 1, 1},

    {1225, 1, 1},
    {1226, 1, 1},

    {1392, 1, 1},
    {1348, 1, 1},

    {713, 1, 1},
    {763, 1, 1},

    {607, 1, 1},
    {505, 1, 1},
};

JumpBlock HOT_AIR_SKYWAY_blocks[] = {
    //Big jump 1
    {1612, 28, 4},
    {1611, 28, 4},

    //SC
    {466, 20, 0},

    // Big jump 2
    {430, 28, 0},
    {431, 28, 0},

    //Disable respawns here
    {1623, 0, 0},
    {1622, 0, 0},

    {467, 0, 0},
    {433, 0, 0},
    {432, 0, 0},

};

JumpBlock PAPU_PYRAMID_blocks[] = {
    //Bottom stairs 
    {472, 3, 0},
    {518, 3, 0},

    //Middle stairs
    // {542, 17, 0},
    // {433, 17, 0},
    {432, 19, 0},
    {541, 19, 0},

    //Top stairs
    // {434, 6, 0},
    // {543, 6, 0},
    {140, 10, 0},
    {222, 10, 0}
};

JumpBlock SEWER_SPEEDWAY_blocks[] = {
    // Small jump at the end
    {142, 1, 0},
    {141, 1, 0},

    // Waterfall jump
    {1188, 9, 0},
    {1197, 9, 0},
    {1234, 9, 0},
    {1233, 9, 0},

    // Sewer entrance
    {1541, 4, 0},
    {1542, 4, 0},
    {1543, 4, 0},
    {1544, 4, 0},

    //Disable respawns here)
    {1213, 0, 0},
    {1192, 0, 0},
    {1193, 0, 0},
    {1230, 0, 0},
};

JumpBlock CORTEX_CASTLE_blocks[] = {
    //Big fall
    {627, 65, 9},

    //Stairs bottom
    {1401, 34, 0},
    {1402, 34, 1},
    {1403, 34, 0},
    {1407, 34, 1},

    {1345, 35, 3},
    {1406, 35, 3},

    {1368, 3, 0},
    {1366, 3, 0},

    {1373, 3, 0},
    {1357, 3, 0},

    {1280, 3, 0},
    {1282, 3, 0},

    {1365, 3, 3},
    {1367, 3, 3},
    {1356, 3, 3},
    {1372, 3, 3},
    {1281, 3, 3},
    {1343, 3, 3},

    //Wood ramp
    {1501, 9, 4}
};

JumpBlock POLAR_PASS_blocks[] = {
    // Big fall
    {832, 37, 0},
    {836, 37, 0},
    {959, 37, 0},
    {958, 37, 0},

    // Small jump
    {1136, 11, 0},
    {1137, 11, 0},

    {522, 1, 1},
    {304, 2, 1},
    {303, 2, 5},

    // {567, 1, 1},
    {566, 2, 2},
    // {551, 2, 1},
    {550, 2, 1},
};

JumpBlock N_GIN_LABS_blocks[] = {
    //Big jump
    {705, 29, 0},

    // small jump (barrels)
    {1101, 5, 6},

    //small jump (intersection)
    {910, 6, 1},

    {519, 9, 16},

    {1821, 4, 13}
};

JumpBlock OXIDE_STATION_blocks[] = {
    //Just to disabble respawns here
    {1738, 0, 0},
    {620, 0, 0},
    {619, 0, 0},
    {629, 0, 0},
    {612, 0, 0},
    {611, 0, 0},
    {682, 0, 0},
    {613, 0, 0},
    {684, 0, 0},
    {683, 0, 0},
    {243, 0, 0},
    {239, 0, 0},
    {2245, 0, 0},

    //Set these as fake turbo pads
    {1736, 0, 8},
    {1705, 0, 8},
    {1703, 0, 8},
    {1704, 0, 8},
    
    //Big jump
    {581, 52, 1},
    {1737, 52, 5},

    //space 1
    {244, 44, 6},
    {240, 41, 6},

    //space 2
    {84, 1, 0},
    {85, 1, 0},

    //medium jump
    {423, 30, 0},
    {424, 30, 0},
    {422, 30, 0},
    {421, 30, 0},

    //small jump 1
    {2176, 4, 0},
    {2177, 4, 0},
    {2343, 4, 0},
    {2344, 4, 0},

    //small jump 2
    {2246, 4, 3}
};

// Check if a quadblock is a jump block, also return the frame count and delay
bool isJumpBlock(short levelID, short blockID, unsigned char* outFrames, unsigned char* outDelay) {
    JumpBlock* blocksArray = NULL;
    unsigned char blockCount = 0;

    // Select the appropriate blocks array based on levelId
    switch (levelID) {
        case BLIZZARD_BLUFF:
            blocksArray = BLIZZARD_BLUFF_blocks;
            blockCount = sizeof(BLIZZARD_BLUFF_blocks) / sizeof(JumpBlock);
            break;
        case DINGO_CANYON:  
            blocksArray = DINGO_CANYON_blocks;
            blockCount = sizeof(DINGO_CANYON_blocks) / sizeof(JumpBlock);
            break;
        case MYSTERY_CAVES:
            blocksArray = MYSTERY_CAVES_blocks;
            blockCount = sizeof(MYSTERY_CAVES_blocks) / sizeof(JumpBlock);
            break;
        case HOT_AIR_SKYWAY:
            blocksArray = HOT_AIR_SKYWAY_blocks;
            blockCount = sizeof(HOT_AIR_SKYWAY_blocks) / sizeof(JumpBlock);
            break;
        case PAPU_PYRAMID:
            blocksArray = PAPU_PYRAMID_blocks;
            blockCount = sizeof(PAPU_PYRAMID_blocks) / sizeof(JumpBlock);
            break;
        case SEWER_SPEEDWAY:
            blocksArray = SEWER_SPEEDWAY_blocks;
            blockCount = sizeof(SEWER_SPEEDWAY_blocks) / sizeof(JumpBlock);
            break;
        case CORTEX_CASTLE:
            blocksArray = CORTEX_CASTLE_blocks;
            blockCount = sizeof(CORTEX_CASTLE_blocks) / sizeof(JumpBlock);
            break;
        case POLAR_PASS:
            blocksArray = POLAR_PASS_blocks;
            blockCount = sizeof(POLAR_PASS_blocks) / sizeof(JumpBlock);
            break;
        case N_GIN_LABS:
            blocksArray = N_GIN_LABS_blocks;
            blockCount = sizeof(N_GIN_LABS_blocks) / sizeof(JumpBlock);
            break;
        case OXIDE_STATION:
            blocksArray = OXIDE_STATION_blocks;
            blockCount = sizeof(OXIDE_STATION_blocks) / sizeof(JumpBlock);
            break;
        default:
            // Level ID not found, variables already set to NULL/0
            break;
    }

    // If blockID is specified, search for it
    if (blockID != -1 && blocksArray != NULL) {
        for (unsigned char i = 0; i < blockCount; i++) {
            if ((blocksArray)[i].blockId == blockID) {
                if (outFrames) *outFrames = FPS_DOUBLE((blocksArray)[i].frameCount);
                if (outDelay) *outDelay = FPS_DOUBLE((blocksArray)[i].frameCountDelay);
                return true;
            }
        }
        return false;
    }

    // Block not found or not searching for a block
    return (blockID == -1 && blocksArray != NULL);
}

// Per-driver floating state
typedef struct {
    unsigned char frameTimer;          // Frames remaining for floating
    unsigned char frameTimerDelay;     // Delay before floating begins
    short lastBlockID;         // Last block ID touched
} DriverFloatState;

// State for each driver
DriverFloatState driverState[4] = {0};

void Init_N_Verted(bool enabled) {
    if (!enabled) return;

    InitDriverStates();
    InitLapSkipPrevention();
}

void InitDriverStates() {
    for (unsigned char i = 0; i < 4; i++) {
        resetDriverFloatState(i);
    }
}

void resetDriverFloatState(unsigned char driverIndex) {
    driverState[driverIndex].frameTimer = 0;
    driverState[driverIndex].frameTimerDelay = 0;
    driverState[driverIndex].lastBlockID = -1;
}

bool updateDriverFloatState(unsigned char driverIndex, struct Driver* driver) {
    struct GameTracker* gGT = sdata->gGT;
    DriverFloatState* state = &driverState[driverIndex];
    
    // First, handle delay timer
    if (state->frameTimerDelay > 0) {
        state->frameTimerDelay--;
        
        // Give player sacred fire while waiting for jump
        if (driver) {
            int fire = 0x100;
            VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), fire);
        }

        // Play sound when delay timer expires and we're about to start floating
        if (state->frameTimerDelay == 0 && state->frameTimer > 0) {
            OtherFX_Play_Echo(0x09, 0, 1); // Play spring sound
        }
        
        // Don't float yet
        return false;
    }
    
    // Process active float timer
    if (state->frameTimer > 0) {
        state->frameTimer--;
        return true; // Should float
    }
    
    // Check for new jump block
    if (driver && driver->currBlockTouching) {
        short currentBlockID = driver->currBlockTouching->blockID;
        
        // Only set the timer when we move to a new block
        if (currentBlockID != state->lastBlockID) {
            unsigned char frames = 0, delay = 0;
            
            if (isJumpBlock(gGT->levelID, currentBlockID, &frames, &delay)) {
                state->frameTimer = frames;
                state->frameTimerDelay = delay;
                
                // Save the block ID
                if (currentBlockID != 0 && 
                    (driver->currBlockTouching->quadFlags & (Q_WALL | Q_MASK_GRAB)) == 0) {
                    state->lastBlockID = currentBlockID;
                }
                
                // Play sound at start of jump
                if (delay == 0 && frames > 0) {
                    OtherFX_Play_Echo(0x09, 0, 1); // Play spring sound
                    
                    // Give sacred fire on first frame
                    int fire = 0x100;
                    VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), fire);
                }
                
                // Return false because we want to wait for the next frame to start floating
                return false;
            }
            
            // Remember this block to avoid rechecking
            if (currentBlockID != 0 && 
                (driver->currBlockTouching->quadFlags & (Q_WALL | Q_MASK_GRAB)) == 0) {
                state->lastBlockID = currentBlockID;
            }
        }
    }
    
    return false;
}

void Handle_N_Verted(bool enabled) {
    if (!enabled) return;
    struct GameTracker* gGT = sdata->gGT;

    for (unsigned char i = 0; i < gGT->numPlyrCurrGame; i++) {
        struct Driver* driver = gGT->drivers[i];

        if (driver == NULL) continue;
        #ifdef USE_CUSTOM_TRACKS
        if (gGT->levelID > LAST_CUSTOM_TRACK_ID) continue;
        #else
        if (gGT->levelID > TURBO_TRACK) continue;

        #endif

        #ifdef USE_CUSTOM_TRACKS
        if ((gGT->gameMode1 & BATTLE_MODE) != 0) continue; // skip battle
        //skip adventure if level is between tracks 1 and 7
        if ((gGT->gameMode1 & ADVENTURE_MODE) != 0 && gGT->levelID >= FIRST_CUSTOM_TRACK_ID && gGT->levelID <= LAST_CUSTOM_TRACK_ID) continue;
        #endif

        // Update driver float state
        if (updateDriverFloatState(i, driver)) {
            // Float the driver
            driver->forcedJump_trampoline = 2;
            driver->jump_unknown = 0x180;
            driver->jump_InitialVelY = driver->const_JumpForce * 3;
            
            // Change tire color while floating
            driver->tireColor = 0x2e0c0cc2;
        } else {
            // Reset tire color when not floating
            driver->tireColor = 0x2e808080;
        }

        // Check for lap skips, if it happens, mask grab the player
        PreventLapSkip(driver, i);
    }
}

// Code to run once on game init
void RunInitHook() {
    if (init_initialized) return;
    gGT = sdata->gGT;
    USE_N_VERTED = octr->special == 9;

    init_initialized = true;
}

//Code to run each frame
void RunUpdateHook() {

    USE_N_VERTED = octr->special == 9;

    //Only run if game is not paused
    if ((gGT->gameMode1 & PAUSE_ALL) != 0) return;

    // if the game is not in a race then quit
	if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING))
		return;

    struct Driver* driver = sdata->gGT->drivers[0];

    // Run once before the race starts (should be replaced with a hook injection)
    // ---------------------------------------------------------------------------------------------
    if (gGT->trafficLightsTimer > 0 && !initialized) {

        // Init driver floating states and lap skip prevention
        Init_N_Verted(USE_N_VERTED);

        initialized = true;
    }
    // Reset flag when timer is not > 0
    else if (gGT->trafficLightsTimer <= 0) {
        initialized = false;
    }
    // ---------------------------------------------------------------------------------------------
    
    // Handle N-VERTED logic, which involves jump blocks and prevent lap skips
    Handle_N_Verted(USE_N_VERTED);

}