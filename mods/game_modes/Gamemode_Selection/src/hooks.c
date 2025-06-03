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
bool USE_BOUNDLESS = false;
bool USE_WALL_RIDE = false;
bool USE_SPEEDWAY_PHYSICS = false;

short gravity = 900;
char* decalText = (char*)0x1F800000;
struct GameTracker *gGT;

static bool initialized = false;

// Code to run once on game init
void RunInitHook() {
    gGT = sdata->gGT;
}

//Code to run each frame
void RunUpdateHook() {
    //Only run if game is not paused
    if ((gGT->gameMode1 & PAUSE_ALL) != 0) return;

    // Handle mod menu input and rendering
    struct GamepadBuffer* controller = &sdata->gGamepads->gamepad[0];
    HandleMenuInput(controller);
    RenderMenu();

    // Draw version info on main menu
    if (D230.MM_State == 1) {
        DecalFont_DrawLine("MOD MASHUP v0.8.0", 5, 197, FONT_SMALL, LIME_GREEN);
        DecalFont_DrawLine(__DATE__, 5, 206, FONT_SMALL, ORANGE);
        DecalFont_DrawLine(__TIME__, 170, 206, FONT_SMALL, ORANGE);
    }

    // DrawDebugString();
    // sprintf(decalText, "value: %d, address: %p", gGT->arcadeDifficulty, &gGT->arcadeDifficulty);
    // DecalFont_DrawLine(decalText, 0x100, 0xc8 - 10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
    // sprintf(decalText, "Skybox pointer: %p\n", gGT->level1->ptr_skybox);
    // DecalFont_DrawLine(decalText, 0x100, 0xc8 - 10, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));


    // if the game is not in a race then quit
	if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING))
		return;

    struct Driver* driver = sdata->gGT->drivers[0];

    // Run once before the race starts (should be replaced with a hook injection)
    // ---------------------------------------------------------------------------------------------
    if (gGT->trafficLightsTimer > 0 && !initialized) {

        // Reset timers and skip prevention structs
        InitShortcutless(USE_SHORTCUTLESS);

        // Init driver floating states and lap skip prevention
        Init_N_Verted(USE_N_VERTED);

        if(USE_MOON_GRAVITY){
            gravity = 369;
        } else {
            gravity = 900;
        }

        // Stats modifiers
        SetGravity(gravity);
        ApplyModifiers();

        // Item modifiers
        ItemChaos_Init(USE_ITEM_CHAOS);

        if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
            InitDynamicLighting(gGT->level1);
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
    HandleMirrorInput(USE_MIRROR);

    // Handle shortcutless logic, detect and prevent shortcuts
    HandleShortcutless(USE_SHORTCUTLESS);
    
    // Handle N-VERTED logic, which involves jump blocks and prevent lap skips
    Handle_N_Verted(USE_N_VERTED);

    // Handle item chaos (Every second a random driver will trow a random item)
    HandleItemChaos(USE_ITEM_CHAOS);

    // Special case for maximun difficulty
    int currentLevel = *superHardAddr / 0x50;
    if (currentLevel >= 9){
        // Give all bots USF
        GiveBotsUSF();
    }

    // HandleRainbowColors(gGT->level1);

    if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
        HandleDynamicLighting(gGT->level1);
    }

    // Draw reserver metter for 1P
    if ((gGT->numPlyrCurrGame == 1) && ((gGT->gameMode1 & END_OF_RACE) == 0))
	{
		DrawReservesMeter(data.hud_1P_P1[0x8].x, data.hud_1P_P1[0x8].y + 5, (struct Driver*)sdata->gGT->threadBuckets[0].thread->object);
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





