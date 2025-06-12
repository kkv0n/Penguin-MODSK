#include <common.h>
#include "utils.h"

//MOD MENU / UI / HUD
#include "ModMenu.c"
#include "custom_hud.c"

//Retro Fueled
// #include "old_retro_fueled.c"
#include "GameModes/RetroFueled/retro_fueled_bluefire.c"

//Shortcutless
#include "GameModes/Shortcutless/shortcutless.c"

//N_Verted
#include "GameModes/N-Verted/n_verted.c"
#include "GameModes/N-Verted/n_verted_driver_state.c"
#include "GameModes/N-Verted/n_verted_lap_skip_prevention.c"

//Mirror
#include "GameModes/MirrorMode/mirror.c"
#include "GameModes/MirrorMode/mirror_pad_remap.c"

//Stats Modifiers (Gravity, Speed, etc...)
#include "GameModes/stats_modifiers.c"

//Ultra Hard Mode
#include "GameModes/UHM/difficulty_modifiers.c"

//Item Modifiers
#include "GameModes/UHM/item_chaos.c"

//MK Fueled
//#include "mk_fueled.c"

//Level Modifiers
#include "GameModes/LevelModifiers/level_modifiers.c"

#include "GameModes/dynamic_light.c"

bool USE_RETRO_FUELED = false;
bool USE_SHORTCUTLESS = false;
bool USE_N_VERTED = false;
bool USE_MIRROR = false;
bool USE_MOON_GRAVITY = false;
bool USE_ITEM_CHAOS = false;
bool USE_NIGHT_FILTER = false;
bool USE_BOSS_CHALLENGE = false;
bool USE_BOUNDLESS = false;
bool USE_WALL_RIDE = false;
bool USE_SPEEDWAY_PHYSICS = false;
bool USE_FLY_CHEAT = false;
bool USE_USF_CHEAT = false;

short gravity = 900;
char* decalText = (char*)0x1F800000;
struct GameTracker *gGT;

static bool init_initialized = false;
static bool initialized = false;

#ifdef USE_CUSTOM_TRACKS
// Struct for current custom track, this have level and lod
typedef struct {
    short levelID;
    short lod;
} CustomTrack;

struct CustomTrack CUSTOM_TRACK_TO_LOAD = {NULL, NULL};

const char* CUSTOM_TRACK_NAMES[] = {
    "BREEZE HARBOR",
    "LOST CITY",
    "DREAMY HEIGHTS",
    "DARK RUINS",
    "FROZEN DEPTHS",
    "CROW FOREST",
    "NUKE REACTOR"
};

const short CUSTOM_TRACK_IDS[] = {
    CUSTOM_TRACK_1,
    CUSTOM_TRACK_2,
    CUSTOM_TRACK_3,
    CUSTOM_TRACK_4,
    CUSTOM_TRACK_5,
    CUSTOM_TRACK_6,
    CUSTOM_TRACK_7
};

const short CUSTOM_TRACK_LODS[] = {
    4,
    4,
    4,
    4,
    4,
    4,
    4,
};

int BOTS_ThTick_Drive_op;
unsigned int unk_op1;
unsigned int unk_op2;
unsigned short unk_op3;

#endif

extern Menu gameMenu;

// Code to run once on game init
void RunInitHook() {
    if (init_initialized) return;
    gGT = sdata->gGT;

    #ifdef USE_CUSTOM_TRACKS
    
    //disable player to bot swap, fixes crash at the end of the race on tile trauma
    //This will also dissable demos and ghosts bot swap
    // Comment this code if tile trauma its not included
    // *(int*)0x80017318 = 0x3E00008; //BOTS_Driver_Convert
    // *(int*)0x8001731c = 0; //??
    
    // AI nav patches
    // Store original values for AI nav patches that we'll apply conditionally
    BOTS_ThTick_Drive_op = *(int*)0x800150C0; //BOTS_ThTick_Drive <- needed for tile trauma bots
    unk_op1 = *(unsigned int*)0x800277c8; // GAMEPAD_ProcessAnyoneVars ??
    unk_op2 = *(unsigned int*)0x800277d0;
    unk_op3 = *(unsigned short*)0x800277f2;
    
    #endif

    init_initialized = true;
}

//Code to run each frame
void RunUpdateHook() {

    #ifdef USE_CUSTOM_TRACKS
    // WARNING: This solutions sucks
    // This was an attempt to avoid crashes on time trial custom tracks but still crashes

    // Check if we're in Time Trial AND on a custom track
    bool isTimeTrialAndCustomTrack = 
        ((gGT->gameMode1 & TIME_TRIAL) != 0) &&
        (gGT->levelID >= FIRST_CUSTOM_TRACK_ID && 
         gGT->levelID <= LAST_CUSTOM_TRACK_ID && 
         (gGT->gameMode1 & (BATTLE_MODE | ADVENTURE_MODE)) == 0);

    // Apply patches conditionally
    if (isTimeTrialAndCustomTrack) {
        // Disable original instructions
        *(int*)0x800150C0 = 0; 
        *(unsigned int*)0x800277c8 = 0;
        *(unsigned int*)0x800277d0 = 0;
        *(unsigned short*)0x800277f2 = 0x800;
    } else {
        // Restore original instructions
        *(int*)0x800150C0 = BOTS_ThTick_Drive_op;  
        *(unsigned int*)0x800277c8 = unk_op1;
        *(unsigned int*)0x800277d0 = unk_op2;
        *(unsigned short*)0x800277f2 = unk_op3;
    }
    #endif

    // Restore d-pad input
    // So mirror mode remap doesnt apply on menus
    if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING))
        RestoreDpadMapping();

    //Only run if game is not paused
    if ((gGT->gameMode1 & PAUSE_ALL) != 0) return;

    // Handle mod menu input and rendering
    struct GamepadBuffer* controller = &sdata->gGamepads->gamepad[0];
    HandleMenuInput(controller);
    RenderMenu();

    // Draw version info on main menu
    if (D230.MM_State == 1) {
        DecalFont_DrawLine("CTR UNLIMITED v0.9.1", 5, 197, FONT_SMALL, LIME_GREEN);
        DecalFont_DrawLine(__DATE__, 5, 206, FONT_SMALL, ORANGE);
        DecalFont_DrawLine(__TIME__, 170, 206, FONT_SMALL, ORANGE);
    }

    // DrawDebugString();
    // sprintf(decalText, "value: %d, address: %p", gGT->arcadeDifficulty, &gGT->arcadeDifficulty);
    // DecalFont_DrawLine(decalText, 0x100, 0xc8 - 10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
    // sprintf(decalText, "Skybox pointer: %p\n", gGT->level1->ptr_skybox);
    // DecalFont_DrawLine(decalText, 0x100, 0xc8 - 10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));

    gravity = USE_MOON_GRAVITY ? 369 : 900;

    // if the game is not in a race then quit
	if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING))
		return;

    struct Driver* driver = sdata->gGT->drivers[0];

    // Run once before the race starts (should be replaced with a hook injection)
    // ---------------------------------------------------------------------------------------------
    if (gGT->trafficLightsTimer > 0 && !initialized) {

        // Reset timers and skip prevention structs
        if(!USE_N_VERTED){
            InitShortcutless(USE_SHORTCUTLESS);
        }

        // Init driver floating states and lap skip prevention
        Init_N_Verted(USE_N_VERTED);

        // Item modifiers
        ItemChaos_Init(USE_ITEM_CHAOS);

        if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
            InitDynamicLighting(gGT->level1);
        }

        // Stats modifiers
        SetGravity(gravity);
        ApplyModifiers();

        //If mirror mode flip wumpa shine
        if(USE_MIRROR){
            data.hud_1P_P1[0xC].x = 0xAA;
        //     data.hud_2P_P1[0xC].x = 0x38;  
        //     data.hud_2P_P2[0xC].x = 0x38;
        //     data.hud_4P_P1[0xC].x = 8;
        //     data.hud_4P_P2[0xC].x = 0x10D4;
        //     data.hud_4P_P3[0xC].x = 8;
        //     data.hud_4P_P4[0xC].x = 0x10D4;
        }else{
	        data.hud_1P_P1[0xC].x = 286;
        }

        initialized = true;
    }
    // Reset flag when timer is not > 0
    else if (gGT->trafficLightsTimer <= 0) {
        initialized = false;
    }
    // ---------------------------------------------------------------------------------------------

    // Handle retro fueled bluefire (visuals)
    // Rest of retro fueled logic is on physlinear and vehfire
    HandleBlueFire(USE_RETRO_FUELED);

    // Remap pad if mirror is enabled
    // Rest of mirror logic is injected on DF_JalDrawOTag
    if(!gameMenu.visible){
        HandleMirrorInput(USE_MIRROR);
    }

    // Handle shortcutless logic, detect and prevent shortcuts
    if(!USE_N_VERTED){
        HandleShortcutless(USE_SHORTCUTLESS);
    }
    
    // Handle N-VERTED logic, which involves jump blocks and prevent lap skips
    Handle_N_Verted(USE_N_VERTED);

    // Handle item chaos (Every second a random driver will trow a random item)
    HandleItemChaos(USE_ITEM_CHAOS);

    #ifdef USE_CUSTOM_TRACKS
    if (
        gGT->levelID >= FIRST_CUSTOM_TRACK_ID && 
        gGT->levelID <= LAST_CUSTOM_TRACK_ID && 
        (gGT->gameMode1 & (BATTLE_MODE | ADVENTURE_MODE)) == 0
    ){
        HandleWeaponRoulette(true);
    }
    #endif

    int currentLevel = *superHardAddr / 0x50;
    // Special case for ultra hard mode
    if(currentLevel == 8){
        GiveBotsTurboOnLastLap(SACRED);
    }

    // Special case for maximun difficulty (USF)
    if (currentLevel >= 9){
        // Give all bots USF
        GiveBotsTurbo(USF);
    }
    // HandleRainbowColors(gGT->level1);

    if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
        HandleDynamicLighting(gGT->level1);
    }

    // Fly cheat
    if (USE_FLY_CHEAT) {
        int i;
        for (i = 0; i < gGT->numPlyrCurrGame; i++) {
            struct Driver* playerDriver = sdata->gGT->drivers[i];
            struct GamepadBuffer* playerController = &sdata->gGamepads->gamepad[i];
            
            if (((playerController->buttonsHeldCurrFrame & BTN_L1) != 0) && 
                ((playerController->buttonsHeldCurrFrame & BTN_TRIANGLE) != 0)) {
                playerController->buttonsHeldCurrFrame |= BTN_CROSS;
                playerDriver->forcedJump_trampoline = 2;
                playerDriver->jump_unknown = 0x180;
                playerDriver->jump_InitialVelY = playerDriver->const_JumpForce * 3;
            }
        }
    }

    // USF cheat
    if (USE_USF_CHEAT){
        int i;
        for (i = 0; i < gGT->numPlyrCurrGame; i++) {
            struct Driver* playerDriver = sdata->gGT->drivers[i];
            struct GamepadBuffer* playerController = &sdata->gGamepads->gamepad[i];
            
            if ((playerController->buttonsHeldCurrFrame & BTN_CROSS) != 0) {
                VehFire_Increment(playerDriver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), USF);
            }
        }
    }

    // Warp to level test ----------------------
    // #ifdef USE_CUSTOM_TRACKS
    // short origin_id = CUSTOM_TRACK_2;
    // short dest_id = ROO_TUBES;
    // short block_id = 1972;
    // if
	// (
    //     gGT->levelID == origin_id
    //     && (driver->currBlockTouching->blockID == block_id)
	// ){  
    //     // Begin warp pad animation
    //     OtherFX_Play_Echo(fx_warppad_warp, 0, 1); //Sound doesnt play
	// 	driver->funcPtrs[0] = VehStuckProc_Warp_Init;
	// }

	// // If driver is invisible and just used the warp pad
	// if(
    //     driver->instSelf->flags & 0x80
    //     && (driver->funcPtrs[0] == VehStuckProc_Warp_Init)
    //     && (gGT->levelID == origin_id)
    // )
	// {
    //     // Load the destination level
    //     CUSTOM_TRACK_TO_LOAD.levelID = dest_id;
    //     CUSTOM_TRACK_TO_LOAD.lod = 2;
    //     DECOMP_MainRaceTrack_RequestLoad(CUSTOM_TRACK_TO_LOAD.levelID);
    // }
    // #endif
    // -----------------------------------------

    // Draw reserver metter for 1P
    // if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0))
	// {
	// 	DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
	// }

    // Draw Mod menu info only on adveture arena
    if (gGT->gameMode1 & ADVENTURE_ARENA) {
        DecalFont_DrawLine("Press SELECT to toggle gamemode menu", 5, 200, FONT_SMALL, ORANGE);
    }

    //Draw fps
    char* string;
	int timer;
	int p;

	string = (char*)0x8000FFF8;
	timer = sdata->gGT->elapsedTimeMS;

	// at some point, upgrade this to use RCNT (more precise)
	// units, which are harder to get, but they're somewhere

	if(timer > 999) timer = 999;

	string[0] =  '0' + (timer / 100) % 10;
	string[1] =  '0' + (timer / 10) % 10;
	string[2] =  '0' + timer % 10;
	string[3] = 0;
	DecalFont_DrawLine(string, 10, 200, FONT_BIG, ORANGE);
}





