#include <common.h>
#include "global.h"
extern const char* options[16];
extern int label;

char gamemode_buffers[8][64];

//special menu text, probably will move it later
// OCTR SPECIAL MENU BY PENTA3
char* special_nameEN[] = {

    "NORMAL", "MIRROR MODE", "ICY TRACK", "TIME TRIAL", "MOON MODE", "RETRO FUELED", "FIRST PERSON", "BOSS RACE", "DEMO CAMERA", "N-VERTED", "SHORTCUTLESS", "NIGHT MODE", "DARKNESS", "ITEM CHAOS", "SURVIVAL", "TIMED SURVIVAL",
};
char* special_nameES[] = {
    "NORMAL", "MODO ESPEJO", "PISO DE HIELO", "SIN ITEMS", "MODO LUNAR", "RETRO FUELED", "PRIMERA PERSONA", "MODO JEFE", "CAMARA DEMO", "N-VERTED", "SIN ATAJOS", "MODO NOCHE", "OSCURIDAD", "CAOS DE ITEMS", "SUPERVIVENCIA", "SUPERVIVENCIA-T",
};
char* special_namePT[] = {
    "NORMAL", "ESPELHADO", "PISTA GELO", "SEM ITENS", "MODO LUNAR", "RETRO FUELED", "PRIMEIRA PESSOA", "CONTRA CHEFE", "DEMO CAMERA", "N-VERTED", "SEM ATAJOS", "MODO NOITE", "ESCURIDÃO", "CAOS DE ITENS", "SUPERVIVÊNCIA", "SUPERVIVÊNCIA-T",
};

char* special_abbrEN[] = {
    "NRM", "MIRR", "ICY", "TT", "MOON", "RETRO", "FP", "BOSS", "DEMO", "N-VER", "NOSC", "NIGHT", "DARK", "CHAOS", "SURV", "TMSRV",
};
char* special_abbrES[] = {
    "NRM", "ESPJ", "HIELO", "NOITM", "LUNA", "RETRO", "PP", "JEFE", "DEMO", "N-VER", "NOATJ", "NOCHE", "OSCUR", "CAOS", "SUPER", "STIEM",
};
char* special_abbrPT[] = {
    "NRM", "ESPEL", "GELO", "NOITM", "LUNAR", "RETRO", "PP", "CHEFE", "DEMO", "N-VER", "NOATJ", "NOITE", "ESCUR", "CAOS", "SUPER", "STIEM",
};

char** abbrLangs[] = {special_abbrEN, special_abbrES, special_abbrPT};

short special_size = sizeof(special_nameEN) / sizeof(special_nameEN[0]);

char* engine_namesEN[] = {
		
	"BALANCED",
	"ACCEL",
	"SPEED",
	"TURN"
};
char* engine_namesES[] = {
	"BALANCEADO",
	"ACELERACION",
	"VELOCIDAD",
	"GIRO"
};

char* engine_namesPT[] = {
	"EQUILIBRADO",
	"ACELERACAO",
	"VELOCIDADE",
	"MAXIMIZADO"	

};

//todo: substract the rows for server country and engine menus
//{stringIndex, rowOnPressUp, rowOnPressDown, rowOnPressLeft, rowOnPressRight}
//stringIndex its the text from the menu option
struct MenuRow menuRows[9] =
{
	{0,0,1,0,0},
	{0,0,2,1,1},
	{0,1,3,2,2},
	{0,2,4,3,3},
	{0,3,5,4,4},
	{0,4,6,5,5},
	{0,5,7,6,6},
	{0,6,7,7,7},

	// NULL, end of menu
	{
		.stringIndex = 0xFFFF,
		.rowOnPressUp = 0,
		.rowOnPressDown = 0,
		.rowOnPressLeft = 0,
		.rowOnPressRight = 0,
	}
};

void RECTMENU_OnPressX(struct RectMenu* b);

struct RectMenu menu =
{
	// custom string made myself
	.stringIndexTitle = 0x4e,

	.posX_curr = 0, // X position
	.posY_curr = 0,  // Y position

	.unk1 = 0,

	// 0b11, 2 centers X, 1 centers Y, 0x80 for tiny text
	// 0x100000 disables TRIANGLE button to prevent crashing
	.state = 0x100083,

	.rows = menuRows,

	.funcPtr = RECTMENU_OnPressX,

	.drawStyle = 0x4,	// 0xF0 looks like load/save

	.posX_prev = 0,
	.posY_prev = 0,

	.rowSelected = 0,
	.unk1c = 0,
	.unk1e = 0,
	.width = 0,
	.height = 0,

	.ptrNextBox_InHierarchy = 0,
	.ptrPrevBox_InHierarchy = 0,
};

char* OnPressX_SetPtr;
char* OnPressX_SetLock;
int pageMax;

int MenuFinished()
{
	return *OnPressX_SetLock;
}
//server names can be changed without problems

//TO DO: add "private" translation for every language later.
char* countryNames[4] =
{
	"Mednafen Peru",
	"Mednafen USA",
	"Gasmox Chile",
    "Private server",	
};

bool sv_menuopen;

void NewPage_ServerCountry()
{
	//fix server menu
	menuRows[3].rowOnPressDown = 3;
	
	if (!sv_menuopen)
	{
	  menu.rowSelected = 0;
	  sv_menuopen = true;
	}
	
	label = 0;
	int i;

menu.posX_curr = 0xFE;  // X position
menu.posY_curr = 0x84;  // Y position

	// override "LAPS" "3/5/7",
	// and other unimportant strings
	for(i = 0; i < 8; i++)
	{
		if (i < 4)
		{
		 menuRows[i].stringIndex = 0x9a+i;
		 sdata->lngStrings[0x9a+i] = countryNames[i];
		}
		else
		{
		   menuRows[i].stringIndex = -1;
	

	       menuRows[i].stringIndex |= 0x8000;
		}
		
	}

}

void MenuWrites_ServerCountry()
{
	pageMax = 0;
	OnPressX_SetPtr = &octr->serverCountry;
	OnPressX_SetLock = &octr->serverLockIn1;
}

int GetNumRoom()
{
	int numRooms = 0;

//what is this --penta3
/*
#if 0
	switch(octr->serverCountry)
	{

	}
#endif
*/

	return 16;
}
//room letters//number
int GetRoomChar(int pn)
{
	//again, this can be done with sprintf
	//this shows the room name
    if (pn <= 9)
    {
        return '0' + pn;
    }
    else
    {
        return 'A' + (pn-10); //and i dont think we should use hexadecimal to represent room numbers
    }
}


void NewPage_ServerRoom()
{
	label = 0;
	int i;
	
	sv_menuopen = false;
	
	
	//remove server menu fix
	menuRows[3].rowOnPressDown = 4;
	
	    unsigned char desired_index = (unsigned char)sdata->unused_8008d700;
         
    		if (sdata->unused_8008d700 > 1)
		{
			if (sdata->unused_8008d700 == 3)
			{
			    desired_index = 2;
			}
			else
			{
				desired_index = 0;
			}
		}
		

	// override "LAPS" "3/5/7"
	//room names, the names can be translated or rewrite
	if (desired_index == 0)
	{
	sdata->lngStrings[0x9a] = "ROOM 1 - x/8";
	sdata->lngStrings[0x9b] = "ROOM 2 - x/8";
	sdata->lngStrings[0x9c] = "ROOM 3 - x/8";
	sdata->lngStrings[0x9d] = "ROOM 4 - x/8";
	sdata->lngStrings[0x9e] = "ROOM 5 - x/8";
	sdata->lngStrings[0x9f] = "ROOM 6 - x/8";
	sdata->lngStrings[0xa0] = "ROOM 7 - x/8";
	sdata->lngStrings[0xa1] = "ROOM 8 - x/8";
	}
	else
	{
	sdata->lngStrings[0x9a] = "SALA 1 - x/8";
	sdata->lngStrings[0x9b] = "SALA 2 - x/8";
	sdata->lngStrings[0x9c] = "SALA 3 - x/8";
	sdata->lngStrings[0x9d] = "SALA 4 - x/8";
	sdata->lngStrings[0x9e] = "SALA 5 - x/8";
	sdata->lngStrings[0x9f] = "SALA 6 - x/8";
	sdata->lngStrings[0xa0] = "SALA 7 - x/8";
	sdata->lngStrings[0xa1] = "SALA 8 - x/8";	
	}

	int pn = octr->PageNumber;
    
	//all of this can use sprintf instead, but im too lazy to change it --penta3
	for(i = 0; i < 8; i++)
	{
		menuRows[i].stringIndex = 0x809a+i;
		sdata->lngStrings[0x9a+i][5] = GetRoomChar(8*pn + i+1);
		sdata->lngStrings[0x9a+i][9] = '0' + (octr->clientCount[8*pn+i]);

		// handle locked rows
		if(octr->clientCount[8*pn+i] > 8)
			sdata->lngStrings[0x9a+i][9] = '0' + (octr->clientCount[8*pn+i]) - 8;
	}

	int numRooms = GetNumRoom();
		int buttons = sdata->gGamepads->gamepad[0].buttonsTapped;

	for(i = 0; i < 8; i++)
	{
		// unlock row if...
		if (8 * pn + i < numRooms)
		{
			if (octr->clientCount[8 * pn + i] <= 7)
			{
				menuRows[i].stringIndex &= 0x7FFF;
			}
			else if ((buttons & BTN_CROSS) && (menu.rowSelected == i))
			{
				int rn = i + (pn * 8);
				if (octr->autoRetryJoinRoomIndex == rn)
					octr->autoRetryJoinRoomIndex = -1;
				else
					octr->autoRetryJoinRoomIndex = rn;
			}
		}
	}
}

void MenuWrites_ServerRoom()
{
	// pageMax
	// 0: 1-4 rooms
	// 1: 5-8 rooms
	// 2: 9-12 rooms

	int numRooms = GetNumRoom();

	pageMax = ((numRooms-1)&0xfffc)/8;

	OnPressX_SetPtr = &octr->serverRoom;
	OnPressX_SetLock = &octr->serverLockIn2;
}

void NewPage_Tracks()
{
	label = 1;
	int i, id;
    
	//restore menu if not in engine menu
    menuRows[3].rowOnPressDown = 4;
	
	for (int i = 0; i < 8; i++)
	{
		
		
		int id = 8 * octr->PageNumber + i;
		
		menuRows[i].stringIndex = (id > TURBO_TRACK) ? 0x9a + i : data.metaDataLEV[id].name_LNG;
		
		if (id > TURBO_TRACK)
		{
			sdata->lngStrings[0x9a + i] = "-";
			menuRows[i].stringIndex |= 0x8000;
		}

		
		
		   
	}
}

void MenuWrites_Tracks()
{
	pageMax = 2;
	OnPressX_SetPtr = &octr->levelID;
	OnPressX_SetLock = &octr->boolLockedInLevel;
}

// Check if a gamemode is incompatible with enabled modes
bool IsGamemodeIncompatible(int modeToCheck) {
    // NORMAL is always compatible
    if (modeToCheck == NORMAL) return false;
    
    // Check incompatibility rules
    if (octr->gamemodes[ITEMLESS] && 
        (modeToCheck == ITEM_CHAOS || modeToCheck == BOSS_RACE))
        return true;
        
    if (octr->gamemodes[MOON_MODE] && modeToCheck == SHORTCUTLESS)
        return true;
        
    if (octr->gamemodes[BOSS_RACE] && modeToCheck == ITEMLESS)
        return true;
        
    if (octr->gamemodes[N_VERTED] && modeToCheck == SHORTCUTLESS)
        return true;
        
    if (octr->gamemodes[SHORTCUTLESS] && 
        (modeToCheck == N_VERTED || modeToCheck == MOON_MODE))
        return true;
        
    if (octr->gamemodes[NIGHT] && modeToCheck == DARKNESS)
        return true;
        
    if (octr->gamemodes[DARKNESS] && modeToCheck == NIGHT)
        return true;
        
    if (octr->gamemodes[ITEM_CHAOS] && modeToCheck == ITEMLESS)
        return true;
        
    if (octr->gamemodes[SURVIVAL] && modeToCheck == SURVIVAL_TIMER)
        return true;
        
    if (octr->gamemodes[SURVIVAL_TIMER] && modeToCheck == SURVIVAL)
        return true;
	
	if (octr->gamemodes[N_VERTED] && modeToCheck == DEMO_CAMERA)
		return true;
	if (octr->gamemodes[DEMO_CAMERA] && modeToCheck == N_VERTED)
		return true;

	if(octr->gamemodes[FIRST_PERSON] && (modeToCheck == DEMO_CAMERA))
		return true;
	if(octr->gamemodes[DEMO_CAMERA] && (modeToCheck == FIRST_PERSON))
		return true;
		        
    return false;
}


void NewPage_Events()
{
    label = 2;
    int i;
    
	
	      unsigned char desired_index = (unsigned char)sdata->unused_8008d700;
         
    		if (sdata->unused_8008d700 > 1)
		{
			if (sdata->unused_8008d700 == 3)
			{
			    desired_index = 2;
			}
			else
			{
				desired_index = 0;
			}
		}
	
	char* press1Text[3] =
	{
		"PRESS ^ TO TOGGLE",
		"PRESIONA ^ PARA ALTERNAR",
		"PRESSIONE ^ PARA ALTERNAR"
	};
	
	char* press2Text[3] =
	{
		"PRESS * TO CONFIRM",
		"PRESIONA * PARA CONFIRMAR",
		"PRESSIONE * PARA CONFIRMAR"
	};
	
    // Add instruction text at the top
    DecalFont_DrawLine(press1Text[desired_index], 0x074, 0x30, FONT_SMALL, JUSTIFY_CENTER | TINY_GREEN);
    DecalFont_DrawLine(press2Text[desired_index], 0x074, 0x38, FONT_SMALL, JUSTIFY_CENTER | CRASH_BLUE);

    char** specials[] = {special_nameEN, special_nameES, special_namePT};
	
    // Base position for menu rows
    int baseY = 105;
    int iconOffsetX = 30;
    
    for (i = 0; i < 8; i++)
    {
       int max = 8 * octr->PageNumber + i;
       menuRows[i].stringIndex = 0x9a + i;
       
       if (max < special_size) {
            // Check if this mode is incompatible with any enabled modes
            bool incompatible = IsGamemodeIncompatible(max);
            
			char* curr_name = specials[desired_index][max];
			
            // Show mode name
            sprintf(gamemode_buffers[i], "%s", curr_name);
            sdata->lngStrings[0x9a + i] = gamemode_buffers[i];
            
            // Calculate Y position for this row (16 pixels between rows)
            int posY = baseY + (i * 8);
            
            // Draw appropriate icon based on mode status
            if (octr->gamemodes[max]) {
                // Mode is enabled - draw NITRO icon
                DECOMP_DecalHUD_DrawWeapon(
                    sdata->gGT->ptrIcons[NITRO_ICON],
                    iconOffsetX, posY,
                    &sdata->gGT->backBuffer->primMem,
                    sdata->gGT->pushBuffer_UI.ptrOT,
                    TRANS_50_DECAL, FP(0.31), 1);
            } else {
                // Mode is disabled - draw TNT icon
                DECOMP_DecalHUD_DrawWeapon(
                    sdata->gGT->ptrIcons[TNT_ICON],
                    iconOffsetX, posY,
                    &sdata->gGT->backBuffer->primMem,
                    sdata->gGT->pushBuffer_UI.ptrOT,
                    TRANS_50_DECAL, FP(0.31), 1);
            }
            
            // Always make NORMAL active
            if (max == NORMAL) {
                octr->gamemodes[NORMAL] = true;
            }
            
            // Disable incompatible modes in the menu
            if (incompatible && !octr->gamemodes[max]) {
                menuRows[i].stringIndex |= 0x8000;
            }
       }
       else {
            sdata->lngStrings[0x9a + i] = "-";
            menuRows[i].stringIndex |= 0x8000;  
       }
    }
}

void ToggleGamemode(int index) {
    // If it's NORMAL mode (0), always keep it enabled
    if (index == NORMAL) return;
    
    // Check if this mode is incompatible with any enabled modes
    if (IsGamemodeIncompatible(index)) {
        // Play error sound and don't toggle
        DECOMP_OtherFX_Play(fx_menu_locked, 1);
        return;
    }
    
    // Toggle the gamemode
    octr->gamemodes[index] = !octr->gamemodes[index];
    
    // If turning ON a mode, check for incompatibilities
    if (octr->gamemodes[index]) {
        // Enforce incompatible mode restrictions by disabling incompatible modes
        switch(index) {
            case ITEMLESS:
                octr->gamemodes[ITEM_CHAOS] = false;
                octr->gamemodes[BOSS_RACE] = false;
                break;
                
            case MOON_MODE:
                octr->gamemodes[SHORTCUTLESS] = false;
                break;
                
            case BOSS_RACE:
                octr->gamemodes[ITEMLESS] = false;
                break;
                
            case N_VERTED:
                octr->gamemodes[SHORTCUTLESS] = false;
                break;
                
            case SHORTCUTLESS:
                octr->gamemodes[N_VERTED] = false;
                octr->gamemodes[MOON_MODE] = false;
                break;
                
            case NIGHT:
                octr->gamemodes[DARKNESS] = false;
                break;
                
            case DARKNESS:
                octr->gamemodes[NIGHT] = false;
                break;
                
            case ITEM_CHAOS:
                octr->gamemodes[ITEMLESS] = false;
                break;
                
            case SURVIVAL:
                octr->gamemodes[SURVIVAL_TIMER] = false;
                break;
                
            case SURVIVAL_TIMER:
                octr->gamemodes[SURVIVAL] = false;
                break;
        }
    }
    
    // Play a sound to indicate toggle
    DECOMP_OtherFX_Play(fx_letter_del, 1);
}

void MenuWrites_Events()
{
    // Allow up to 2 pages of gamemodes
    pageMax = 1;
    
    OnPressX_SetLock = &octr->boolLockedInSpecial;
    
    // Handle Triangle button to toggle modes
    int buttons = sdata->gGamepads->gamepad[0].buttonsTapped;
    if ((label == 2) && buttons & BTN_TRIANGLE) {
        int selectedMode = (8 * octr->PageNumber) + menu.rowSelected;
        if (selectedMode < special_size) {
            ToggleGamemode(selectedMode);
            // Refresh menu after toggling
            NewPage_Events();
        }
    }
}

//these are the laps available in the menu
//only visual text, if you want to change the real number
//search int numLaps in CL_main.c
void NewPage_Laps()
{
	label = 3;
	int i;

	// menu.rowSelected = 0;

//moved to other file

	for(i = 0; i < 8; i++)
    {
        sdata->lngStrings[0x9a + i] = options[8 * octr->PageNumber + i];
        menuRows[i].stringIndex = 0x9a + i;
    }
}

void MenuWrites_Laps()
{
	//2 pages
    pageMax = 1;
    OnPressX_SetPtr = &octr->lapID;
    OnPressX_SetLock = &octr->boolLockedInLap;
}


void NewPage_Characters()
{
	label = 4;
	int i;
	
	//Restore menu if not in engine menu
    menuRows[3].rowOnPressDown = 4;
	
	for(i = 0; i < 8; i++)
	{
		menuRows[i].stringIndex =
			data.MetaDataCharacters[8*octr->PageNumber+i].name_LNG_long;
	}
}

void MenuWrites_Characters()
{
	pageMax = 1;
	OnPressX_SetPtr = &data.characterIDs[0];
	OnPressX_SetLock = &octr->boolLockedInCharacters[octr->DriverID];
}
// ENGINE MENU BY PENTA3
void NewPage_Engine()
{
	label = 5;
    int i;
	
	//fix menu bug
    menuRows[3].rowOnPressDown = 3;
	
	
		 unsigned char desired_index = (unsigned char)sdata->unused_8008d700;
         
    		if (sdata->unused_8008d700 > 1)
		{
			if (sdata->unused_8008d700 == 3)
			{
			    desired_index = 2;
			}
			else
			{
				desired_index = 0;
			}
		}
	
	char** langs[] = {
		engine_namesEN, engine_namesES, engine_namesPT
	};
	
    for (i = 0; i < 8; i++)
    {
		if (i < 4)
		sdata->lngStrings[0x9a + i] = langs[desired_index][i];
		
        menuRows[i].stringIndex = 0x9a + i;
		
		if (i > 3)
		{
			sdata->lngStrings[0x9a + i] = "-";
			menuRows[i].stringIndex = -1;
			
		}
		
    }

	extern void DrawClassStats(Point pos, u_char engineID);
	DrawClassStats(MakePoint(106, 48), octr->enginetype[0]);

}

void MenuWrites_Engine()
{

    pageMax = 0;


    OnPressX_SetPtr = &octr->enginetype[0];          
    OnPressX_SetLock = &octr->boolLockedInEnginee[octr->DriverID];  
}
int pressedX = 0;
void UpdateMenu()
{
	if (pressedX == 1)
	{
		pressedX = 0;

		menu.rowSelected = 0;
		// Set default engine selection based on selected character
		if (label == 5 - 1)
			menu.rowSelected = data.MetaDataCharacters[data.characterIDs[0]].engineID;
	}

	RECTMENU_Show(&menu);

	int buttons = sdata->gGamepads->gamepad[0].buttonsTapped;

	if (buttons & BTN_LEFT) { octr->PageNumber = max(0, octr->PageNumber - 1); }
	if (buttons & BTN_RIGHT) { octr->PageNumber = min(pageMax, octr->PageNumber + 1); }
	if (buttons & (BTN_LEFT | BTN_RIGHT)) { DECOMP_OtherFX_Play(0, 1); }

	if (pageMax == 0) { return; }
	
	
	//can just use sprintf there i think //YES IT CAN.
	int string =
		(('1' + octr->PageNumber) << 0) |
		('/' << 8) |
		(('1' + pageMax) << 16);

	DECOMP_MainFreeze_ConfigDrawArrows(menu.posX_curr, 0x48, &string);

	DecalFont_DrawLine(&string,menu.posX_curr,0x48,FONT_BIG,JUSTIFY_CENTER|WHITE);
}

// void RECTMENU_OnPressX(struct RectMenu* b)
// {
//     int i;

//     // Special handling for the Events menu
//     if (label == 2) {
//         // Make sure NORMAL is always enabled
//         octr->gamemodes[NORMAL] = true;
        
//         // Set the lock flag but DON'T hide the menu yet
//         octr->boolLockedInSpecial = 1;
        
//         // Reset page number for next menu
//         octr->PageNumber = 0;
        
//         // Don't do anything else - let the state machine handle the transition to laps menu
//         return;
//     }

//     RECTMENU_Hide(b);
//     sdata->ptrDesiredMenu = 0;

//     *OnPressX_SetPtr = (8 * octr->PageNumber) + b->rowSelected;
//     *OnPressX_SetLock = 1;

//     octr->PageNumber = 0;
//     pressedX = 1;

//     RECTMENU_ClearInput();
// }

int numLapsSurvival;

void RECTMENU_OnPressX(struct RectMenu* b)
{
    int i;

    // Special handling for the Events menu
    if (label == 2) {
        // Make sure NORMAL is always enabled
        octr->gamemodes[NORMAL] = true;
        
        // Set the lock flag
        octr->boolLockedInSpecial = 1;
        
        // If survival mode is active, auto-set laps and skip lap menu
        if (octr->gamemodes[SURVIVAL] || octr->gamemodes[SURVIVAL_TIMER]) {
			extern int GetActiveDriversCount();
            int player_count = GetActiveDriversCount();

			numLapsSurvival = sdata->gGT->numLaps;
            // Set lap count based on mode
            if (octr->gamemodes[SURVIVAL]) {
                // Set laps to player count or 1 if only one player
				numLapsSurvival = (player_count < 2) ? 1 : (player_count - 1);
				// octr->lapID = numLapsSurvival;
				sdata->gGT->numLaps = numLapsSurvival;
            } else if (octr->gamemodes[SURVIVAL_TIMER]) {
                // Set laps to 127 for timed survival
				numLapsSurvival = 127;
				// octr->lapID = numLapsSurvival;
				sdata->gGT->numLaps = numLapsSurvival;
            }
            
            // Skip laps menu by setting the lock flag
            octr->boolLockedInLap = 1;
        }
        
        // Reset page number for next menu
        octr->PageNumber = 0;

		pressedX = 1;
        
        return;
    }

    RECTMENU_Hide(b);
    sdata->ptrDesiredMenu = 0;

    *OnPressX_SetPtr = (8 * octr->PageNumber) + b->rowSelected;
    *OnPressX_SetLock = 1;

    octr->PageNumber = 0;
    pressedX = 1;

    RECTMENU_ClearInput();
}

void PrintTimeStamp()
{
	int posX = 25;
	int posY = 186;
	DECOMP_DecalFont_DrawLine(__TIME__, posX, posY, FONT_SMALL, PAPU_YELLOW);
	DECOMP_DecalFont_DrawLine(__DATE__, posX, posY+8, FONT_SMALL, PAPU_YELLOW);
}

void PrintCharacterStats()
{
	char message[32];
	int slot;
	int i;
	int color;

    char title_buffer[256] = "";
    int mode_count = 0;

	int posX = 0x100;
	int posY = 0x18;
	
	
		unsigned char desired_index = (unsigned char)sdata->unused_8008d700;
         
    		if (sdata->unused_8008d700 > 1)
		{
			if (sdata->unused_8008d700 == 3)
			{
			    desired_index = 2;
			}
			else
			{
				desired_index = 0;
			}
		}
    
	
	
	// Skip NORMAL mode (i=0) and start from i=1
	for (int i = 1; i < special_size; i++) {
		if (octr->gamemodes[i]) {
			if (mode_count > 0) {
				strcat(title_buffer, "-");  // Use dash instead of plus
			}
			
			char* current_abbr = abbrLangs[desired_index][i];
			
			strcat(title_buffer, current_abbr);  // Use abbreviation
			mode_count++;
			
			// Break into new line if string gets too long
			if (strlen(title_buffer) > 30) {
				DECOMP_DecalFont_DrawLine(title_buffer, posX, posY, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
				title_buffer[0] = '\0';
				mode_count = 0;
				posY += 8;
			}
		}
	}

	// If no active modes besides NORMAL, show just NORMAL
	if (mode_count == 0) {
		strcpy(title_buffer, abbrLangs[desired_index][0]);
	}
	
	// Draw any remaining modes
	if (strlen(title_buffer) > 0) {
		DECOMP_DecalFont_DrawLine(title_buffer, posX, posY, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
	}


	DecalFont_DrawLine(
		countryNames[octr->serverCountry],
		0x10, 0x10, FONT_SMALL, 0);
		

char* roomText[3] = {
	"ROOM x",
	"SALA x",
	"SALA x",	
};

char* playersText[3] =
{
	"Players: ",
	"JUGADORES: ",
	"JOGADORES: "
	
};

//i think this is the room name in the top left of the screen

	char* roomName = roomText[desired_index];
	
	roomName[5] = GetRoomChar(octr->serverRoom+1); //sprintf pls
	DecalFont_DrawLine(
		roomName,
		0x10, 0x18, FONT_SMALL, 0);



	int numDead = 0;
	for(i = 0; i < octr->NumDrivers; i++)
		if(octr->nameBuffer[i][0] == 0)
			numDead++;

	posX = 0x110;
	sprintf(message, "%s%d/8", playersText[desired_index],(octr->NumDrivers-numDead));
	DecalFont_DrawLine(message,posX,0x58,FONT_SMALL,0);

	int h = 0;




	// UI-test
	// octr->NumDrivers = 8;

	for(i = 0; i < octr->NumDrivers; i++)
	{
		// convert client index to local index
		if(i == octr->DriverID) slot = 0;
		if(i < octr->DriverID) slot = i+1;
		if(i > octr->DriverID) slot = i;

		char* str = octr->nameBuffer[slot];

		// UI-test
		// str[0] = 'A';

		if(str[0] == 0) continue;

		// 0x19 - red
		// 0x1A - green
		int color = octr->boolLockedInEnginee[i] ? PURA_VIOLET : PAPU_YELLOW;

		posY = 0x60+h;
		h += 8;

		posX = 0x110;
		sprintf(message, "%s:", str);
		DecalFont_DrawLine(message,posX,posY,FONT_SMALL,color);
		
		if(octr->CurrState < LOBBY_CHARACTER_PICK)
			continue;
		
		DECOMP_DecalHUD_DrawWeapon(
		// pointer to icon, from array of icon pointers
		sdata->gGT->ptrIcons[data.MetaDataCharacters[data.characterIDs[slot]].iconID],
	
		(int)(posX - 18),(int)(posY - 2),
	
		// PrimMem
		&sdata->gGT->backBuffer->primMem,
	
		// OTMem
		sdata->gGT->pushBuffer_UI.ptrOT,
	
		TRANS_50_DECAL,FP(0.40),0);
		
		
		

		if(octr->CurrState < LOBBY_ENGINEPICK)
			continue;
        
			char** langs[] = {
		engine_namesEN, engine_namesES, engine_namesPT
	};
	
		char* curr_engine =
					langs[desired_index][octr->enginetype[slot]];

		posX = 0x18C;
		DecalFont_DrawLine(curr_engine, (posX - 11),posY,FONT_SMALL,color);
	}

	posX = 0x11E;
	posY = 0xB3;
	
	char* g1Text[3] = {
		"Gasmoxian is a modified",
		"gasmoxian es una version",
		"gasmoxian eh uma versao"
	};
	
	char* g2Text[3] =
	{
	  	"version of OnlineCTR.",
		"modificada de onlinectr,",
		"modificada do octr, para",
		
	};
	
	char* g3Text[3] =
	{
		"BY PENTA3-ANZU-ANFROST",
		"POR PENTA3-ANZU-ANFROST",
		"POR PENTA3-ANZU-ANFROST"
	};
	

//i think a function to draw multiple lines already exists.
	DecalFont_DrawLine(g1Text[desired_index],posX,posY,FONT_SMALL,0);
	DecalFont_DrawLine(g2Text[desired_index],posX+0x10,posY+0x8,FONT_SMALL,0);
	DecalFont_DrawLine(g3Text[desired_index],posX,posY+0x10,FONT_SMALL, PAPU_YELLOW);

}
	

char* onlineLapString = "Laps: 000\0";
void PrintRecvTrack()
{
	char message[32];
	
			unsigned char desired_index = (unsigned char)sdata->unused_8008d700;
         
    		if (sdata->unused_8008d700 > 1)
		{
			if (sdata->unused_8008d700 == 3)
			{
			    desired_index = 2;
			}
			else
			{
				desired_index = 0;
			}
		}
		
		char* trackText[3] =
		{
			"Track: ",
			"Pista: ",
			"Pista: "
		};



	sprintf(message, "%s%s", trackText[desired_index],

				sdata->lngStrings
				[
					data.metaDataLEV[octr->levelID].name_LNG
				]
			);

	// UI-test
	// boolEndOfRace = 1;

	int posX = 0x0F8;

	if(octr->gamemodes[SURVIVAL] || octr->gamemodes[SURVIVAL_TIMER]){
		sdata->gGT->numLaps = numLapsSurvival;
	}

	int numLaps = sdata->gGT->numLaps;
	
	//can do sprintf
	onlineLapString[6] = '0' + ((numLaps / 100) % 10);
	onlineLapString[7] = '0' + ((numLaps / 10) % 10);
	onlineLapString[8] = '0' + (numLaps % 10);

	if(octr->gamemodes[SURVIVAL_TIMER]){
		onlineLapString[6] = '-';
		onlineLapString[7] = '-';
		onlineLapString[8] = '-';
	}

	DecalFont_DrawLine(message,posX,0x38,FONT_SMALL,PAPU_YELLOW);

	if(!octr->gamemodes[SURVIVAL] && !octr->gamemodes[SURVIVAL_TIMER]){ // screw this, string still bugs sometimes
		DecalFont_DrawLine(onlineLapString,posX+2,0x40,FONT_SMALL,PAPU_YELLOW);
	}
}