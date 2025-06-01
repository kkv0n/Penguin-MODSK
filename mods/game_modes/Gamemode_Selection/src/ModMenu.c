// Based on ModMenuV3 by Niko
// https://github.com/CTR-tools/CTR-ModSDK/tree/main/mods/ModPacks/ModMenu_OnlyUSA

#include "common.h"
#include "utils.h"

// cheats are located in 0x80096b28
// int* cheats = (int*)0x80096B28;
extern unsigned short* superHardAddr;
extern int itemChaosDifficulty;

extern int NightFilterBrightness;
extern int NightFilterBlueTint;

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
        {"NF Blue fire and u-turn", "Made by Redhot"}
    },
    {
        "Moon Gravity",
        "on", "off", &optionValues[1],
        NULL,
        {"Low gravity everywhere", "___"}
    },
    {
        "Mirror Mode",
        "on", "off", &optionValues[2],
        NULL,
        {"Swap left and right", "by Niko"}
    },
    {
        "N-Verted",
        "on", "off", &optionValues[3],
        NULL,
        {"Race tracks in backwads", "by Anfrost"}
    },
    {
        "Icy Tracks",
        "on", "off", &optionValues[4],
        NULL,
        {"Enable icy tracks cheatcode", "____"}
    },
    {
        "Difficulty",
        NULL, NULL, NULL, // Special handling
        NULL,
        {"Swap speed of the bots", "Swap with d-pad - by Niko"}
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
        "lightning",
        NULL, NULL, NULL, // Special handling
        NULL,
        {"Normal - Night - Darkness", "Swap with d-pad - by Anfrost"}

    },
    
    // PAGE 2
    {
        "UNLOCK ALL",
        "on", "off", &optionValues[9],
        NULL,
        {"101", "___"}
    },
    {
        "Disable HUD",
        "on", "off", &optionValues[10],
        NULL,
        {"Disable HUD while racing", "by Niko"}
    },
    {
        "", // Empty slot
        "", "", &optionValues[11],
        NULL,
        {"", ""}
    },
    {
        "", // Empty slot
        "", "", &optionValues[12],
        NULL,
        {"", ""}
    },
    {
        "", // Empty slot
        "", "", &optionValues[13],
        NULL,
        {"", ""}
    },
    {
        "", // Empty slot
        "", "", &optionValues[14],
        NULL,
        {"", ""}
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
    true,           // visible
    {0, 0, 350, 130}, // bgRect
    {0, 150, 480, 64}, // descRect
    9,              // numOptions
    menuOptions,    // options
    "Mod menu selector", // footerText
    0 // Current page index
};

char valueBuffer[4] = "";

// --------------------- MENU FUNCTIONS ---------------------

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
    
    // UNLOCK ALL (Page 2, index 0)
    if (optionValues[9]) {
        // 101 in adventure
        *(unsigned int*)0x8008fba4 = 0xFFFFFFFF;
        *(unsigned int*)0x8008fba8 = 0xFFFFFFFF;
        *(unsigned int*)0x8008fbac = 0xFFFFFFFF;
        *(unsigned int*)0x8008fbb0 = 0xFFFFFFFF;

        // All characters, tracks, cups
        *(unsigned int*)0x8008e6ec = 0xFFFFFFFF;
    }
    
    // Disable HUD (Page 2, index 1)
    *(unsigned char*)0x8001B038 = optionValues[10] ? 0 : 1;
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
    }
    
    if (tap & BTN_RIGHT || tap & BTN_R2) {
        if (currentLevel == 1) currentLevel = 2;        // EASY -> MED
        else if (currentLevel == 2) currentLevel = 3;   // MED -> HARD
        else if (currentLevel == 3) currentLevel = 4;   // HARD -> S-HARD
        else if (currentLevel == 4) currentLevel = 8;   // S-HARD -> U-HARD
        else if (currentLevel == 8) currentLevel = 9;   // U-HARD -> USF
        else if (currentLevel == 9) currentLevel = 1;   // USF -> EASY
        else currentLevel = 1;                          // Default -> EASY
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
    }
    
    if (tap & BTN_RIGHT || tap & BTN_R2) {
        itemChaosDifficulty++;
        if (itemChaosDifficulty > 3) itemChaosDifficulty = 0;
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
    if (tap & BTN_LEFT) {
        // Cycle through states
        if (NightFilterBrightness == 255) {
            NightFilterBrightness = 5; // Darkness
            NightFilterBlueTint = 0; // No blue tint
        } else if (NightFilterBrightness == 5) {
            NightFilterBrightness = 64; // Night
            NightFilterBlueTint = 15; // Default blue tint
        } else {
            NightFilterBrightness = 255; // Off
            NightFilterBlueTint = 0; // No blue tint
        }
    }

    if (tap & BTN_RIGHT || tap & BTN_R2) {
        // Cycle through states in reverse
        if (NightFilterBrightness == 255) {
            NightFilterBrightness = 64; // Night
            NightFilterBlueTint = 15; // Default blue tint
        } else if (NightFilterBrightness == 64) {
            NightFilterBrightness = 5; // Darkness
            NightFilterBlueTint = 0; // No blue tint
        } else {
            NightFilterBrightness = 255; // Off
            NightFilterBlueTint = 0; // No blue tint
        }
    }

    // Update the USE_NIGHT_FILTER flag based on brightness
    USE_NIGHT_FILTER = (NightFilterBrightness < 255);
}

// Handle menu input
void HandleMenuInput(struct GamepadBuffer* controller) {
    int tap = controller->buttonsTapped;
    
    // Toggle menu visibility with Select button
    if (tap & BTN_SELECT) {
        gameMenu.visible = !gameMenu.visible;
        return;
    }

    if (!gameMenu.visible) return;

    // Page navigation with L1/R1
    if (tap & BTN_L1) {
        if (gameMenu.currentPage > 0) {
            gameMenu.currentPage--;
            gameMenu.selectedIndex = 0; // Reset selection when changing pages
        }
    }
    
    if (tap & BTN_R1) {
        if (gameMenu.currentPage < 1) { // We have 2 pages (0 and 1)
            gameMenu.currentPage++;
            gameMenu.selectedIndex = 0; // Reset selection when changing pages
        }
    }

    // Menu navigation
    if (tap & BTN_UP) {
        if (gameMenu.selectedIndex > 0) {
            gameMenu.selectedIndex--;
        }
    }

    if (tap & BTN_DOWN) {
        if (gameMenu.selectedIndex < gameMenu.numOptions - 1) {
            gameMenu.selectedIndex++;
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
    else if (tap & BTN_R2) {
        // Toggle the boolean value for standard options
        if (gameMenu.options[actualOptionIndex].valuePtr != NULL) {
            bool* valuePtr = gameMenu.options[actualOptionIndex].valuePtr;
            *valuePtr = !(*valuePtr);
            
            // Call change handler if one exists
            if (gameMenu.options[actualOptionIndex].onChange != NULL) {
                gameMenu.options[actualOptionIndex].onChange(*valuePtr);
            }
        }
    }
    
    // Apply all changes
    ApplyMenuEffects();
}

// Render the menu
void RenderMenu() {
    if (!gameMenu.visible) return;
    
    // Draw background rectangles
    RECTMENU_DrawInnerRect(&gameMenu.bgRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
    RECTMENU_DrawInnerRect(&gameMenu.descRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
    
    // Draw menu title and instructions
    DecalFont_DrawLine("Press Select to Hide", 20, 110, FONT_SMALL, PERIWINKLE);
    
    // Calculate page offset
    int pageOffset = gameMenu.currentPage * gameMenu.numOptions;
    
    // Draw selection arrow
    DecalFont_DrawLine("-", 20, 10 + (gameMenu.selectedIndex * 10), FONT_SMALL, TINY_GREEN);
    
    // Draw options for the current page
    int i;
    for (i = 0; i < gameMenu.numOptions; i++) {
        int actualIndex = i + pageOffset;
        MenuOption* option = &gameMenu.options[actualIndex];
        
        // Skip empty options
        if (option->title[0] == '\0') continue;
        
        // Draw option title
        DecalFont_DrawLine(option->title, 40, 10 + (i * 10), FONT_SMALL, PAPU_YELLOW);
        
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
            DecalFont_DrawLine(diffText, 240, 10 + i * 10, FONT_SMALL, CRASH_BLUE);
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
            DecalFont_DrawLine(chaosText, 240, 10 + i * 10, FONT_SMALL, 
                            itemChaosDifficulty > 0 ? CRASH_BLUE : CORTEX_RED);
        }

        else if (gameMenu.currentPage == 0 && i == 8) { // Night Filter (special numeric value)
            const char* nightText;
            if (NightFilterBrightness == 255) {
                nightText = "NORMAL";
            } else if (NightFilterBrightness == 64) {
                nightText = "NIGHT";
            } else {
                nightText = "DARKNESS";
            }
            DecalFont_DrawLine(nightText, 240, 10 + i * 10, FONT_SMALL, CRASH_BLUE);
        }

        else if (option->valuePtr != NULL) { // Standard toggle options
            DecalFont_DrawLine(
                *(option->valuePtr) ? option->onText : option->offText,
                240,
                10 + i * 10,
                FONT_SMALL,
                *(option->valuePtr) ? TINY_GREEN : CORTEX_RED
            );
        }
    }
    
    // Draw description for selected option
    int selectedOptionIndex = gameMenu.selectedIndex + pageOffset;
    MenuOption* selectedOption = &gameMenu.options[selectedOptionIndex];
    DecalFont_DrawLine(selectedOption->description[0], 20, 160, FONT_SMALL, PAPU_YELLOW);
    DecalFont_DrawLine(selectedOption->description[1], 20, 170, FONT_SMALL, PAPU_YELLOW);
    
    // Draw footer);
    // DecalFont_DrawLine(gameMenu.footerText, 20, 200, FONT_SMALL, PERIWINKLE);
    sprintf(decalText, "Page %d/2 (L1/R1)", gameMenu.currentPage + 1);
    DecalFont_DrawLine(decalText, 20, 200, FONT_SMALL, PAPU_YELLOW);
}