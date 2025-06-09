// Based on ModMenuV3 by Niko
// https://github.com/CTR-tools/CTR-ModSDK/tree/main/mods/ModPacks/ModMenu_OnlyUSA

#include "common.h"
#include "utils.h"

// Global menu position constants
#define MENU_BASE_X 230
#define MENU_BASE_Y 25

// cheats are located in 0x80096b28
// int* cheats = (int*)0x80096B28;
extern unsigned short* superHardAddr;
extern int itemChaosDifficulty;

extern int NightFilterBrightness;
extern int NightFilterBlueTint;

extern void* PlayerDrivingFuncTable[13];
extern void* PlayerFreezeFuncTable[13];

bool functionsDisabled = false;


// --------------------- MENU STRUCTURE ---------------------
typedef struct {
    char* title;
    char* onText;
    char* offText;
    bool* valuePtr;
    void (*onChange)(bool);
    char* description[2];
} MenuOption;

typedef struct {
    int selectedIndex;
    bool visible;
    RECT bgRect;
    RECT descRect;
    char numOptions;
    MenuOption* options;
    char* footerText;
    int currentPage;  // New field to track current page
} Menu;
// --------------------- MENU STATE ---------------------
bool optionValues[18] = {false}; // Values for each option

// Define menu options
MenuOption menuOptions[18] = {
    // PAGE 1
    {
        "Retro Fueled",
        "on", "off", &optionValues[0],
        NULL,
        {"NF Blue fire and u-turn", "by Redhot"}
    },
    {
        "Moon Gravity",
        "on", "off", &optionValues[1],
        NULL,
        {"Low gravity everywhere", ""}
    },
    {
        "Mirror Mode",
        "on", "off", &optionValues[2],
        NULL,
        {"Mirror the entire track", "left-right by Niko"}
    },
    {
        "N-Verted",
        "on", "off", &optionValues[3],
        NULL,
        {"Race tracks in backwards", "by Anfrost"}
    },
    {
        "Icy Tracks",
        "on", "off", &optionValues[4],
        NULL,
        {"Enable icy tracks cheatcode", ""}
    },
    {
        "Difficulty",
        NULL, NULL, NULL, // Special handling
        NULL,
        {"Select speed of the bots", "Swap with d-pad - by Niko"}
    },
    {
        "Item Chaos",
        NULL, NULL, NULL, // Special handling
        NULL,
        {"Unfair and chaotic item spawns", "Swap with d-pad - by Anfrost"}
    },
    {
        "Shortcutless",
        "on", "off", &optionValues[7],
        NULL,
        {"All shortcuts are banned", "by Anfrost"}
    },
    {
        "lighting",
        NULL, NULL, NULL, // Special handling
        NULL,
        {"Normal - Night - U-Dark", "Swap with d-pad - by Anfrost"}
    },
    
    // PAGE 2
    {
        "Boundless",
        "on", "off", &optionValues[9],
        NULL,
        {"Disable invisible walls,", "killplanes and offroad"}
    },
    {
        "Wall Ride",
        "on", "off", &optionValues[10],
        NULL,
        {"Allows driving on any wall", "by penta3"}
    },
    {
        "Speedway Phys",
        "on", "off", &optionValues[11],
        NULL,
        {"Sewer speedway physics", "everywhere"}
    },
    {
        "UNLOCK ALL",
        "on", "off", &optionValues[12],
        NULL,
        {"101", "___"}
    },
    {
        "Disable HUD",
        "on", "off", &optionValues[13],
        NULL,
        {"Disable HUD while racing", "by Niko"}
    },
    {
        "Fly Cheat",
        "on", "off", &optionValues[14],
        NULL,
        {"Hold L1+^ to fly", ""}
    },
    {
        "", // Empty slot
        "", "", &optionValues[15],
        NULL,
        {"", ""}
    },
    {
        "", // Empty slot
        "", "", &optionValues[16],
        NULL,
        {"", ""}
    },
    {
        "", // Empty slot
        "", "", &optionValues[17],
        NULL,
        {"", ""}
    }
};

Menu gameMenu = {
    0,              // selectedIndex
    false,           // visible
    {MENU_BASE_X, MENU_BASE_Y, 275, 110}, // bgRect position using compile-time constants
    {MENU_BASE_X, MENU_BASE_Y + 120, 275, 45}, // descRect position using compile-time constants
    9,              // numOptions
    menuOptions,    // options
    "Mod menu selector", // footerText
    0 // Current page index
};

char valueBuffer[4] = "";

// --------------------- MENU FUNCTIONS ---------------------

// Function to disable kart controls
void DisableKartControls() {
    struct Driver* driver = sdata->gGT->drivers[0];
    
    // Only disable if we haven't already and driver exists
    if (!functionsDisabled && driver != NULL) {
        // Use freeze functions instead of NULL
        for (int i = 0; i < 13; i++) {
            driver->funcPtrs[i] = PlayerFreezeFuncTable[i];
        }
        
        functionsDisabled = true;
    }
}

// Function to restore kart controls
void RestoreKartControls() {
    struct Driver* driver = sdata->gGT->drivers[0];
    
    // Only restore if we previously disabled and driver exists
    if (functionsDisabled && driver != NULL) {
        // Restore original function pointers
        for (int i = 0; i < 13; i++) {
            driver->funcPtrs[i] = PlayerDrivingFuncTable[i];
        }
        
        functionsDisabled = false;
    }
}

// Apply effects based on menu option changes
void ApplyMenuEffects() {
    // Retro Fueled (Page 1, index 0)
    USE_RETRO_FUELED = optionValues[0];
    if (USE_RETRO_FUELED) {
        sdata->gGT->gameMode2 |= CHEAT_TURBOPAD;
    } else {
        sdata->gGT->gameMode2 &= ~CHEAT_TURBOPAD;
    }
    
    // Moon Gravity (Page 1, index 1)
    USE_MOON_GRAVITY = optionValues[1];
    
    // Mirror mode (Page 1, index 2)
    USE_MIRROR = optionValues[2];
    
    // N-Verted (Page 1, index 3)
    USE_N_VERTED = optionValues[3];
    
    // Icy Tracks (Page 1, index 4)
    if (optionValues[4]) {
        sdata->gGT->gameMode2 |= CHEAT_ICY;
    } else {
        sdata->gGT->gameMode2 &= ~CHEAT_ICY;
    }
    
    // Item Chaos (Page 1, index 6)
    USE_ITEM_CHAOS = (itemChaosDifficulty > 0);
    
    // Shortcutless (Page 1, index 7)
    USE_SHORTCUTLESS = optionValues[7];

    // Night Filter (Page 1, index 8)
    USE_NIGHT_FILTER = (NightFilterBrightness < 255);
    
    // Boundless (Page 2, index 0)
    USE_BOUNDLESS = optionValues[9];
    
    // Wall Ride (Page 2, index 1)
    USE_WALL_RIDE = optionValues[10];

    // Speedway Physics (Page 2, index 2)
    USE_SPEEDWAY_PHYSICS = optionValues[11];
    
    // UNLOCK ALL (Page 2, index 3)
    if (optionValues[12]) {
        // 101 in adventure
        *(unsigned int*)0x8008fba4 = 0xFFFFFFFF;
        *(unsigned int*)0x8008fba8 = 0xFFFFFFFF;
        *(unsigned int*)0x8008fbac = 0xFFFFFFFF;
        *(unsigned int*)0x8008fbb0 = 0xFFFFFFFF;

        // All characters, tracks, cups
        *(unsigned int*)0x8008e6ec = 0xFFFFFFFF;
    }
    
    // Disable HUD (Page 2, index 4)
    *(unsigned char*)0x8001B038 = optionValues[13] ? 0 : 1;
    
    // Fly Cheat (Page 2, index 5)
    USE_FLY_CHEAT = optionValues[14];
}

void HandleDifficultyTap(int tap)
{
    int currentLevel = *superHardAddr / 0x50;
    
    if (tap & BTN_LEFT) {
        if (currentLevel == 1) currentLevel = 9;        // EASY -> USF
        else if (currentLevel == 2) currentLevel = 1;   // MED -> EASY
        else if (currentLevel == 3) currentLevel = 2;   // HARD -> MED
        else if (currentLevel == 4) currentLevel = 3;   // S-HARD -> HARD
        else if (currentLevel == 8) currentLevel = 4;   // U-HARD -> S-HARD
        else if (currentLevel == 9) currentLevel = 8;   // USF -> U-HARD
        else currentLevel = 1;                          // Default -> EASY

        DECOMP_OtherFX_Play(fx_letter_del, 1);
    }
    
    if (tap & BTN_RIGHT) {
        if (currentLevel == 1) currentLevel = 2;        // EASY -> MED
        else if (currentLevel == 2) currentLevel = 3;   // MED -> HARD
        else if (currentLevel == 3) currentLevel = 4;   // HARD -> S-HARD
        else if (currentLevel == 4) currentLevel = 8;   // S-HARD -> U-HARD
        else if (currentLevel == 8) currentLevel = 9;   // U-HARD -> USF
        else if (currentLevel == 9) currentLevel = 1;   // USF -> EASY
        else currentLevel = 1;                          // Default -> EASY

        DECOMP_OtherFX_Play(fx_letter_del, 1);
    }

    // No need to clamp values since we're explicitly setting them
    *superHardAddr = currentLevel * 0x50;
    
    SetDifficultyLevel(currentLevel);
}

void HandleItemChaosTap(int tap)
{
    if (tap & BTN_LEFT) {
        itemChaosDifficulty--;
        if (itemChaosDifficulty < 0) itemChaosDifficulty = 3;
        DECOMP_OtherFX_Play(fx_letter_del, 1);
    }
    
    if (tap & BTN_RIGHT) {
        itemChaosDifficulty++;
        if (itemChaosDifficulty > 3) itemChaosDifficulty = 0;
        DECOMP_OtherFX_Play(fx_letter_del, 1);
    }
    
    // Update the USE_ITEM_CHAOS flag based on difficulty
    USE_ITEM_CHAOS = (itemChaosDifficulty > 0);
}

// Night filter has 3 states:
// Off (default)
// Night (NightFilterBrightness = 64, NightFilterBlueTint = 15)
// Darkness (NightFilterBrightness = 5, NightFilterBlueTint = 0)
void HandleNightFilterTap(int tap)
{
    short NightBrightness = 64;
    short NightBlueTint = 15;

    short DarknessBrightness = 5;
    short DarknessBlueTint = 0;

    if (tap & BTN_LEFT) {
        // Cycle through states
        if (NightFilterBrightness == 255) {
            NightFilterBrightness = DarknessBrightness; // Darkness
            NightFilterBlueTint = DarknessBlueTint; // No blue tint
        } else if (NightFilterBrightness == DarknessBrightness) {
            NightFilterBrightness = NightBrightness; // Night
            NightFilterBlueTint = NightBlueTint; // Default blue tint
        } else {
            NightFilterBrightness = 255; // Off
            NightFilterBlueTint = DarknessBlueTint; // No blue tint
        }
        DECOMP_OtherFX_Play(fx_letter_del, 1);
    }

    if (tap & BTN_RIGHT) {
        // Cycle through states in reverse
        if (NightFilterBrightness == 255) {
            NightFilterBrightness = NightBrightness; // Night
            NightFilterBlueTint = NightBlueTint; // Default blue tint
        } else if (NightFilterBrightness == NightBrightness) {
            NightFilterBrightness = DarknessBrightness; // Darkness
            NightFilterBlueTint = DarknessBlueTint; // No blue tint
        } else {
            NightFilterBrightness = 255; // Off
            NightFilterBlueTint = DarknessBlueTint; // No blue tint
        }
        DECOMP_OtherFX_Play(fx_letter_del, 1);
    }

    // Update the USE_NIGHT_FILTER flag based on brightness
    USE_NIGHT_FILTER = (NightFilterBrightness < 255);
}

bool showingModMenuInTrackSelect = false;
bool continueToTrackSelection = false;
bool justCanceledModMenu = false;

// New variables for adventure New/Load screen
bool showingModMenuInNewLoad = false;
bool continueToAdventure = false;
bool justCanceledModMenuNewLoad = false;

// Function to show ModMenu in track selection context
void ShowModMenuInTrackSelect() {
    gameMenu.visible = true;
    showingModMenuInTrackSelect = true;
    continueToTrackSelection = false; // Reset this flag
    
    // Show a special footer message
    gameMenu.footerText = "Press X/O to continue, Triangle to cancel";
    
    // Apply effects immediately so mods are active
    ApplyMenuEffects();
}

// New function to show ModMenu in New/Load adventure context
void ShowModMenuInNewLoad() {
    gameMenu.visible = true;
    showingModMenuInNewLoad = true;
    continueToAdventure = false; // Reset this flag
    
    // Show a special footer message
    gameMenu.footerText = "Press X/O to continue, Triangle to cancel";
    
    // Apply effects immediately so mods are active
    ApplyMenuEffects();
}

// Function to handle continuing from ModMenu to track selection
void HandleModMenuContinue() {
    struct GameTracker *gGT = sdata->gGT;
    
    // if not Battle or Time Trial, open LapSelectMenu
    if ((gGT->gameMode1 & (BATTLE_MODE | TIME_TRIAL)) == 0) {
        // open lap select menu
        D230.trackSel_boolOpenLapBox = D230.trackSel_transitionState;
        
        // Clear input to prevent the lap menu from immediately detecting the X/O press
        DECOMP_RECTMENU_ClearInput();
        
        // Explicitly clear the button tap arrays to be extra safe
        for (int i = 0; i < 8; i++) {
            sdata->buttonTapPerPlayer[i] = 0;
        }
    } else {
        // if Battle or Time Trial, skip straight to level
        D230.trackSel_StartRaceAfterFadeOut = D230.trackSel_transitionState;
        D230.trackSel_transitionState = EXITING_MENU;
    }
    
    // Reset the footer text and hide the menu
    CleanupModMenuTrackSelect();
}

// New function to cancel and go back to track selection
void CancelModMenuTrackSelect() {
    // Reset the flag to allow track navigation again
    showingModMenuInTrackSelect = false;
    
    // Reset the footer text and hide the menu
    CleanupModMenuTrackSelect();
    
    // Play "go back" sound
    DECOMP_OtherFX_Play(2, 1);
    
    // Set flag to indicate we just canceled the mod menu
    // This prevents the track selection menu from also processing this Triangle press
    justCanceledModMenu = true;
    
    // Thoroughly clear input to prevent the button press from being detected again
    DECOMP_RECTMENU_ClearInput();
    
    // Clear button arrays
    for (int i = 0; i < 8; i++) {
        sdata->buttonTapPerPlayer[i] = 0;
    }
}

// Helper function to clean up the mod menu state
void CleanupModMenuTrackSelect() {
    // Reset the menu state
    gameMenu.footerText = "Mod menu selector";
    gameMenu.visible = false;
    showingModMenuInTrackSelect = false;
}

// Function to handle continuing from ModMenu to adventure
void HandleModMenuContinueToAdventure() {
    // Allow the MM_State transition to happen
    continueToAdventure = true;
    
    // Reset the footer text and hide the menu
    CleanupModMenuNewLoad();
    
    // Explicitly trigger the transition to adventure mode
    D230.MM_State = 2; // MM_Title transitioning out
    
    // Clear input to prevent it from affecting the next menu
    DECOMP_RECTMENU_ClearInput();
    
    // Clear button arrays
    for (int i = 0; i < 8; i++) {
        sdata->buttonTapPerPlayer[i] = 0;
    }
}

// New function to cancel and go back to New/Load selection
void CancelModMenuNewLoad() {
    // Reset the flag to allow navigation again
    showingModMenuInNewLoad = false;
    
    // Reset the footer text and hide the menu
    CleanupModMenuNewLoad();
    
    // Play "go back" sound
    DECOMP_OtherFX_Play(2, 1);
    
    // Set flag to indicate we just canceled the mod menu
    justCanceledModMenuNewLoad = true;
    
    // Thoroughly clear input to prevent the button press from being detected again
    DECOMP_RECTMENU_ClearInput();
    
    // Clear button arrays
    for (int i = 0; i < 8; i++) {
        sdata->buttonTapPerPlayer[i] = 0;
    }
}

// Helper function to clean up the mod menu state for New/Load
void CleanupModMenuNewLoad() {
    // Reset the menu state
    gameMenu.footerText = "Mod menu selector";
    gameMenu.visible = false;
    showingModMenuInNewLoad = false;
}

// Handle menu input
void HandleMenuInput(struct GamepadBuffer* controller) {
    int tap = controller->buttonsTapped;
    
    // Toggle menu visibility with Select button (with additional checks)
    if (tap & BTN_SELECT && gGT->gameMode1 & ADVENTURE_ARENA && !showingModMenuInTrackSelect && !showingModMenuInNewLoad) {
        struct Driver* driver = sdata->gGT->drivers[0];
        
        // Don't open menu if kart is frozen or in warp pad state
        if (!gameMenu.visible && driver != NULL && (driver->kartState == KS_FREEZE || driver->kartState == KS_WARP_PAD)) {
            return;
        }
        
        gameMenu.visible = !gameMenu.visible;
        
        // Disable or restore kart controls based on menu visibility
        if (gameMenu.visible) {
            DisableKartControls();
        } else {
            RestoreKartControls();
        }
        
        return;
    }

    // If kart state is warp pad close the menu
    if (sdata->gGT->drivers[0] != NULL && sdata->gGT->drivers[0]->kartState == KS_WARP_PAD) {
        if (gameMenu.visible) {
            gameMenu.visible = false;
            functionsDisabled = false;
        }
        return;
    }

    // If game is loading close the menu
    if (sdata->load_inProgress != 0){
        if (gameMenu.visible) {
            gameMenu.visible = false;
            RestoreKartControls(); // Restore controls when closing the menu
        }
        return;
    }

    if (!gameMenu.visible) return;
    
    // Special handling for track selection context
    if (showingModMenuInTrackSelect) {
        // If Triangle is pressed, exit the mod menu without proceeding
        if (tap & (BTN_TRIANGLE | BTN_SQUARE_one)) {
            CancelModMenuTrackSelect();
            
            // Clear input to prevent issues
            DECOMP_RECTMENU_ClearInput();
            return;
        }
        
        // If X or Circle is pressed in this mode, continue to track selection
        if (tap & (BTN_CROSS_one | BTN_CIRCLE)) {
            continueToTrackSelection = true;
            // Play the confirm sound
            DECOMP_OtherFX_Play(1, 1);
            
            // Clear tap input to prevent it from affecting the lap selection menu
            controller->buttonsTapped &= ~(BTN_CROSS_one | BTN_CIRCLE);
            return;  // Let the track selection code handle the rest
        }
    }

    // NEW: Special handling for New/Load adventure context
    if (showingModMenuInNewLoad) {
        // If Triangle is pressed, exit the mod menu without proceeding
        if (tap & (BTN_TRIANGLE | BTN_SQUARE_one)) {
            CancelModMenuNewLoad();
            
            // Clear input to prevent issues
            DECOMP_RECTMENU_ClearInput();
            return;
        }
        
        // If X or Circle is pressed in this mode, continue to adventure
        if (tap & (BTN_CROSS_one | BTN_CIRCLE)) {
            HandleModMenuContinueToAdventure();
            // Play the confirm sound
            DECOMP_OtherFX_Play(1, 1);
            
            // Clear tap input
            controller->buttonsTapped &= ~(BTN_CROSS_one | BTN_CIRCLE);
            return;
        }
    }
    
    // Page navigation with L1/R1
    if (tap & BTN_L1) {
        if (gameMenu.currentPage > 0) {
            gameMenu.currentPage--;
            gameMenu.selectedIndex = 0; // Reset selection when changing pages
            DECOMP_OtherFX_Play(fx_menu_selection_change, 1);
        }
    }
    
    if (tap & BTN_R1) {
        if (gameMenu.currentPage < 1) { // We have 2 pages (0 and 1)
            gameMenu.currentPage++;
            gameMenu.selectedIndex = 0; // Reset selection when changing pages
            DECOMP_OtherFX_Play(fx_menu_selection_change, 1);
        }
    }

    // Menu navigation
    if (tap & BTN_UP) {
        if (gameMenu.selectedIndex > 0) {
            gameMenu.selectedIndex--;
            DECOMP_OtherFX_Play(fx_menu_selection_change, 1);
        }
    }

    if (tap & BTN_DOWN) {
        if (gameMenu.selectedIndex < gameMenu.numOptions - 1) {
            gameMenu.selectedIndex++;
            DECOMP_OtherFX_Play(fx_menu_selection_change, 1);
        }
    }

    // Calculate the actual option index based on page
    int actualOptionIndex = gameMenu.selectedIndex + (gameMenu.currentPage * gameMenu.numOptions);
    
    // Handle option changes
    // Special handling for Super Hard option (Page 1, index 5)
    if (gameMenu.currentPage == 0 && gameMenu.selectedIndex == 5) {
        HandleDifficultyTap(tap);
    }
    // Special handling for Item Chaos option (Page 1, index 6)
    else if (gameMenu.currentPage == 0 && gameMenu.selectedIndex == 6) {
        HandleItemChaosTap(tap);
    }

    // Special handling for Night Filter option (Page 1, index 8)
    else if (gameMenu.currentPage == 0 && gameMenu.selectedIndex == 8) {
        HandleNightFilterTap(tap);
    }

    // Toggle boolean options
    else if (tap & BTN_RIGHT || tap & BTN_LEFT) {
        // Toggle the boolean value for standard options
        if (gameMenu.options[actualOptionIndex].valuePtr != NULL) {
            bool* valuePtr = gameMenu.options[actualOptionIndex].valuePtr;
            *valuePtr = !(*valuePtr);
            
            // Call change handler if one exists
            if (gameMenu.options[actualOptionIndex].onChange != NULL) {
                gameMenu.options[actualOptionIndex].onChange(*valuePtr);
            }
            DECOMP_OtherFX_Play(fx_letter_del, 1);
        }
    }
    
    // Apply all changes
    ApplyMenuEffects();
}

// Render the menu
void RenderMenu() {
    if (!gameMenu.visible) return;
    
    // Draw menu title and instructions
    // if (showingModMenuInTrackSelect) {
    //     DecalFont_DrawLine("Configure Mods Before Racing", MENU_BASE_X + 10, MENU_BASE_Y + 100, FONT_SMALL, PAPU_YELLOW);
    //     DecalFont_DrawLine("X/O: Continue", MENU_BASE_X + 235, MENU_BASE_Y + 90, FONT_SMALL, TINY_GREEN);
    //     DecalFont_DrawLine("Triangle: Cancel", MENU_BASE_X + 235, MENU_BASE_Y + 100, FONT_SMALL, CORTEX_RED);
    // } else {
    //     DecalFont_DrawLine("Press Select to Hide", MENU_BASE_X + 235, MENU_BASE_Y + 100, FONT_SMALL, PERIWINKLE);
    // }

    DecalFont_DrawLine("ON/OFF with D-Pad", MENU_BASE_X + 10, MENU_BASE_Y + 100, FONT_SMALL, PAPU_YELLOW);
    
    // Page offset
    int pageOffset = gameMenu.currentPage * gameMenu.numOptions;
    
    // Draw options for the current page
    int i;
    for (i = 0; i < gameMenu.numOptions; i++) {
        int actualIndex = i + pageOffset;
        MenuOption* option = &gameMenu.options[actualIndex];
        
        // Skip empty options
        if (option->title[0] == '\0') continue;
        
        // Draw option title - highlight selection with green color
        int textColor = (i == gameMenu.selectedIndex) ? PAPU_YELLOW : ORANGE;
        DecalFont_DrawLine(option->title, MENU_BASE_X + 10, MENU_BASE_Y + 5 + (i * 10), FONT_SMALL, textColor);
        
        // Draw option value 
        if (gameMenu.currentPage == 0 && i == 5) { // Super Hard (special numeric value)
            int diffLevel = (*superHardAddr / 0x50);
            const char* diffText;
            switch (diffLevel) {
                case 1: diffText = "EASY"; break;
                case 2: diffText = "MEDIUM"; break;
                case 3: diffText = "HARD"; break;
                case 4: diffText = "S-HARD"; break;
                case 8: diffText = "U-HARD"; break;
                case 9: diffText = "USF"; break;
                default: diffText = "???"; break;
            }
            DecalFont_DrawLine(diffText, MENU_BASE_X + 210, MENU_BASE_Y + 5 + i * 10, FONT_SMALL, CRASH_BLUE);
        }
        else if (gameMenu.currentPage == 0 && i == 6) { // Item Chaos (special numeric value)
            const char* chaosText;
            switch (itemChaosDifficulty) {
                case 0: chaosText = "OFF"; break;
                case 1: chaosText = "FLICK"; break;
                case 2: chaosText = "BASH"; break;
                case 3: chaosText = "RAGE"; break;
                default: chaosText = "???"; break;
            }
            DecalFont_DrawLine(chaosText, MENU_BASE_X + 210, MENU_BASE_Y + 5 + i * 10, FONT_SMALL, 
                            itemChaosDifficulty > 0 ? CRASH_BLUE : CORTEX_RED);
        }
        else if (gameMenu.currentPage == 0 && i == 8) { // Night Filter (special numeric value)
            const char* nightText;
            if (NightFilterBrightness == 255) {
                nightText = "NORMAL";
            } else if (NightFilterBrightness == 64) {
                nightText = "NIGHT";
            } else {
                nightText = "U-DARK";
            }
            DecalFont_DrawLine(nightText, MENU_BASE_X + 210, MENU_BASE_Y + 5 + i * 10, FONT_SMALL, CRASH_BLUE);
        }
        else if (option->valuePtr != NULL) { // Standard toggle options
            DecalFont_DrawLine(
                *(option->valuePtr) ? option->onText : option->offText,
                MENU_BASE_X + 210,
                MENU_BASE_Y + 5 + i * 10,
                FONT_SMALL,
                *(option->valuePtr) ? TINY_GREEN : CORTEX_RED
            );
        }
    }
    
    // Draw description for selected option in the description box
    int selectedOptionIndex = gameMenu.selectedIndex + pageOffset;
    MenuOption* selectedOption = &gameMenu.options[selectedOptionIndex];
    DecalFont_DrawLine(selectedOption->description[0], MENU_BASE_X + 10, MENU_BASE_Y + 125, FONT_SMALL, ORANGE);
    DecalFont_DrawLine(selectedOption->description[1], MENU_BASE_X + 10, MENU_BASE_Y + 135, FONT_SMALL, ORANGE);
    
    // Draw footer
    sprintf(decalText, "Page %d/2 (L1/R1)", gameMenu.currentPage + 1);
    DecalFont_DrawLine(decalText, MENU_BASE_X + 10, MENU_BASE_Y + 155, FONT_SMALL, PAPU_YELLOW);

    // Draw background rectangles
    RECTMENU_DrawInnerRect(&gameMenu.bgRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
    RECTMENU_DrawInnerRect(&gameMenu.descRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
}