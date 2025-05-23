#include "common.h"
#include "utils.h"

// cheats are located in 0x80096b28
int* cheats = (int*)0x80096B28;

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
} Menu;

// --------------------- MENU STATE ---------------------
bool optionValues[9] = {false}; // Values for each option

// Special option states
unsigned short* superHardAddr = (unsigned short*)0x80012658;

// Define menu options
MenuOption menuOptions[9] = {
    {
        "Retro Fueled",
        "on", "off", &optionValues[0],
        NULL,
        {"NF Blue fire and u-turn", "Made by Redhot"}
    },
    {
        "Shortcutless",
        "on", "off", &optionValues[1],
        NULL,
        {"All shortcuts are banned", "Made by Anfrost"}
    },
    {
        "Super Hard",
        NULL, NULL, NULL, // Special handling
        NULL,
        {"Swap difficulty of Super Hard Mode", "Made by Niko"}
    },
    {
        "Disable HUD",
        "on", "off", &optionValues[3],
        NULL,
        {"Disable HUD while racing", "Made by Niko"}
    },
    {
        "Nitro Fuel 2P",
        "on", "off", &optionValues[4],
        NULL,
        {"Side by side 2P splitscreen", "Requires Disable HUD"}
    },
    {
        "Icy Tracks",
        "on", "off", &optionValues[5],
        NULL,
        {"Enable icy tracks cheatcode", "____"}
    },
    {
        "N-Verted",
        "on", "off", &optionValues[6],
        NULL,
        {"Race tracks in backwads", "Made by Anfrost"}
    },
    {
        "Moon Gravity",
        "on", "off", &optionValues[7],
        NULL,
        {"Low gravity everywhere", "___"}
    },
    {
        "Mirror Mode",
        "on", "off", &optionValues[8],
        NULL,
        {"Swap left and right", "Made by Niko"}
    }
};

Menu gameMenu = {
    0,              // selectedIndex
    true,           // visible
    {0, 0, 300, 130}, // bgRect
    {0, 150, 480, 64}, // descRect
    9,              // numOptions
    menuOptions,    // options
    "Mod menu selector" // footerText
};

char valueBuffer[4] = "";

// --------------------- MENU FUNCTIONS ---------------------

// Apply effects based on menu option changes
void ApplyMenuEffects() {
    // Retro Fueled
    if (optionValues[0]) {
        EnableRetroFueled();
    } else {
        DisableRetroFueled();
    }

    // Shortcutless
    USE_SHORTCUTLESS = optionValues[1];

    // Disable HUD
    *(unsigned char*)0x8001B038 = optionValues[3] ? 0 : 1;

    // If Disable HUD is off, turn off NF 2P
    if (!optionValues[3]) {
        optionValues[4] = false;
    }

    // If NF 2P is enabled, turn on Disable HUD
    if (optionValues[4]) {
        optionValues[3] = true;
        *(unsigned char*)0x8001B038 = 0;
    }

    // Enable/Disable NF 2P
    if (optionValues[4]) {
        *(unsigned char*)0x800375A8 = 0x3;
        *(unsigned char*)0x80037644 = 0x2;
        *(unsigned char*)0x800427B5 = 0x1;
        *(unsigned char*)0x800427C0 = 0x0;
        *(unsigned char*)0x800427C1 = 0x1;
        *(unsigned char*)0x800427D0 = 0xD8;
        *(unsigned char*)0x800427EC = 0x1C;
        *(unsigned char*)0x800427F1 = 0x1;
        *(unsigned char*)0x800427F8 = 0xD8;
        *(unsigned char*)0x80042818 = 0x1E;
    } else {
        *(unsigned char*)0x800375A8 = 0x2;
        *(unsigned char*)0x80037644 = 0x3;
        *(unsigned char*)0x800427B5 = 0x2;
        *(unsigned char*)0x800427C0 = 0x6E;
        *(unsigned char*)0x800427C1 = 0x0;
        *(unsigned char*)0x800427D0 = 0x6A;
        *(unsigned char*)0x800427EC = 0x1E;
        *(unsigned char*)0x800427F1 = 0x2;
        *(unsigned char*)0x800427F8 = 0x6A;
        *(unsigned char*)0x80042818 = 0x1C;
    }

    // Unlock All
    // if (optionValues[5]) {
    //     // 101 adventure mode
    //     *(unsigned int*)0x8008fba4 = 0xFFFFFFFF;
    //     *(unsigned int*)0x8008fba8 = 0xFFFFFFFF;
    //     *(unsigned int*)0x8008fbac = 0xFFFFFFFF;
    //     *(unsigned int*)0x8008fbb0 = 0xFFFFFFFF;

    //     // All characters, tracks, cups
    //     *(unsigned int*)0x8008e6ec = 0xFFFFFFFF;
    // }

    //Disable All cheats
    // *cheats &= ~(0x100000 | 0x80000 | 0x400 | 0x80000 | 0x400000 | 0x8000000 | 0x10000);

    // Icy Tracks
    if (optionValues[5]) {
        //Enable just icy tracks cheat
        *cheats |= CHEAT_ICY;
        
    } else {
        //Disable just icy tracks cheat
        *cheats &= ~CHEAT_ICY;
    }

    // N-Verted
    USE_N_VERTED = optionValues[6];

    // No weapons
    // *(unsigned int*)0x8006540C = optionValues[7] ? 0x3e00008 : 0x27BDFFA0;

    // Moon Gravity
    USE_MOON_GRAVITY = optionValues[7];

    // Mirror mode
    USE_MIRROR = optionValues[8];
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

    // Handle option changes
    int currentOption = gameMenu.selectedIndex;
    
    // Special handling for Super Hard option
    if (currentOption == 2) {
        if (tap & BTN_LEFT) {
            *superHardAddr = *superHardAddr - 0x50;
            if ((*superHardAddr / 0x50) < 4) {
                *superHardAddr = 0x140;
            }
        }
        
        if (tap & BTN_RIGHT) {
            *superHardAddr = *superHardAddr + 0x50;
            if ((*superHardAddr / 0x50) > 9) {
                *superHardAddr = 0x2D0;
            }
        }
        
        // Enable/Disable SHM cheat based on value
        if ((*superHardAddr / 0x50) > 4) {
            *(unsigned int*)0x80096b28 |= 0x200000;
        } else {
            *(unsigned int*)0x80096b28 &= 0xFFDFFFFF;
        }
    }

    // Toggle boolean options
    else if (tap & BTN_R2) {
        // Toggle the boolean value for standard options
        if (gameMenu.options[currentOption].valuePtr != NULL) {
            bool* valuePtr = gameMenu.options[currentOption].valuePtr;
            *valuePtr = !(*valuePtr);
            
            // Call change handler if one exists
            if (gameMenu.options[currentOption].onChange != NULL) {
                gameMenu.options[currentOption].onChange(*valuePtr);
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
    
    // Draw selection arrow
    DecalFont_DrawLine("-", 20, 10 + (gameMenu.selectedIndex * 10), FONT_SMALL, TINY_GREEN);
    
    // Draw options
    int i;
    for (i = 0; i < gameMenu.numOptions; i++) {
        MenuOption* option = &gameMenu.options[i];
        
        // Draw option title
        DecalFont_DrawLine(option->title, 40, 10 + (i * 10), FONT_SMALL, PAPU_YELLOW);
        
        // Draw option value
        if (i == 2) { // Super Hard (special numeric value)
            sprintf(valueBuffer, "%d", (*superHardAddr / 0x50));
            DecalFont_DrawLine(valueBuffer, 240, 30, FONT_SMALL, PERIWINKLE);
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
    MenuOption* selectedOption = &gameMenu.options[gameMenu.selectedIndex];
    DecalFont_DrawLine(selectedOption->description[0], 20, 160, FONT_SMALL, PERIWINKLE);
    DecalFont_DrawLine(selectedOption->description[1], 20, 170, FONT_SMALL, PERIWINKLE);
    
    // Draw footer
    DecalFont_DrawLine(gameMenu.footerText, 20, 200, FONT_SMALL, PERIWINKLE);
}