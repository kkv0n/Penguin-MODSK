#include <common.h>
#include "global.h"
extern const char* options[16];
extern int label;

char gamemode_buffers[8][64];

//special menu text, probably will move it later
// OCTR SPECIAL MENU BY PENTA3
char* special_name[] = {
#ifdef GASMOX_ENG
    "NORMAL", "MIRROR MODE", "ICY TRACK", "TIME TRIAL", "MOON MODE", "RETRO FUELED", "VOID WORLD", "BOSS RACE", "DEMO CAMERA", "N-VERTED", "SHORTCUTLESS", "NIGHT MODE", "DARKNESS", "ITEM CHAOS", "SURVIVAL", "SURVIVAL TIMED",
#elif defined(GASMOX_ES)
    "NORMAL", "MODO ESPEJO", "PISO DE HIELO", "SIN ITEMS", "MODO LUNAR", "RETRO FUELED", "MUNDO VACIO", "MODO JEFE", "CAMARA DEMO", "N-VERTED", "SIN ATAJOS", "MODO NOCHE", "OSCURIDAD", "CAOS DE ITEMS", "SUPERVIVENCIA", "SUPERVIVENCIA POR TIEMPO",
#elif defined(GASMOX_BR)
    "NORMAL", "ESPELHADO", "PISTA GELO", "SEM ITENS", "MODO LUNAR", "RETRO FUELED", "PISTA VAZIA", "CONTRA CHEFE", "DEMO CAMERA", "N-VERTED", "SEM ATAJOS", "MODO NOITE", "ESCURIDÃO", "CAOS DE ITENS", "SUPERVIVÊNCIA", "SUPERVIVÊNCIA POR TEMPO",
#endif
};

char* special_abbr[] = {
#ifdef GASMOX_ENG
    "NRM", "MIRR", "ICY", "TT", "MOON", "RETRO", "VOID", "BOSS", "DEMO", "N-VER", "NOSC", "NIGHT", "DARK", "CHAOS", "SURV", "TMSRV",
#elif defined(GASMOX_ES)
    "NRM", "ESPJ", "HIELO", "NOITM", "LUNA", "RETRO", "VACIO", "JEFE", "DEMO", "N-VER", "NOATJ", "NOCHE", "OSCUR", "CAOS", "SUPER", "STIEM",
#elif defined(GASMOX_BR)
    "NRM", "ESPEL", "GELO", "NOITM", "LUNAR", "RETRO", "VAZIO", "CHEFE", "DEMO", "N-VER", "NOATJ", "NOITE", "ESCUR", "CAOS", "SUPER", "STIEM",
#endif
};

short special_size = sizeof(special_name) / sizeof(special_name[0]);

char* engine_names[] = {
	#ifdef GASMOX_ENG
	"BALANCED",
	"ACCEL",
	"SPEED",
		"TURN"
	#elif defined(GASMOX_ES)
	"BALANCEADO",
	"ACELERACION",
	"VELOCIDAD",
	"GIRO"
	#elif defined(GASMOX_BR)
	"EQUILIBRADO",
	"ACELERACAO",
	"VELOCIDADE",
	"MAXIMIZADO"	
	#endif
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
//for some reason when i change the number from 8 to 3 i got 20 extra bytebudget 
char* countryNames[4] =
{
	"Mednafen Peru",
	"Mednafen USA",
	"Gasmox Chile",
#ifdef GASMOX_ENG
    "Private server",
#elif defined(GASMOX_ES) || defined(GASMOX_BR)
    "Sala privada",
#endif
	
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

#if 0
	switch(octr->serverCountry)
	{

	}
#endif

	return 16;
}
//room letters//number
int GetRoomChar(int pn)
{
    if (pn <= 9)
    {
        return '0' + pn;
    }
    else
    {
        return 'A' + (pn-10);
    }
}


void NewPage_ServerRoom()
{
	label = 0;
	int i;
	
	sv_menuopen = false;
	
	
	//remove server menu fix
	menuRows[3].rowOnPressDown = 4;
	

	// override "LAPS" "3/5/7"
	//room names, the names can be translated or rewrite
	#ifdef GASMOX_ENG
	sdata->lngStrings[0x9a] = "ROOM 1 - x/8";
	sdata->lngStrings[0x9b] = "ROOM 2 - x/8";
	sdata->lngStrings[0x9c] = "ROOM 3 - x/8";
	sdata->lngStrings[0x9d] = "ROOM 4 - x/8";
	sdata->lngStrings[0x9e] = "ROOM 5 - x/8";
	sdata->lngStrings[0x9f] = "ROOM 6 - x/8";
	sdata->lngStrings[0xa0] = "ROOM 7 - x/8";
	sdata->lngStrings[0xa1] = "ROOM 8 - x/8";
	#elif defined(GASMOX_ES) || defined(GASMOX_BR)
	sdata->lngStrings[0x9a] = "SALA 1 - x/8";
	sdata->lngStrings[0x9b] = "SALA 2 - x/8";
	sdata->lngStrings[0x9c] = "SALA 3 - x/8";
	sdata->lngStrings[0x9d] = "SALA 4 - x/8";
	sdata->lngStrings[0x9e] = "SALA 5 - x/8";
	sdata->lngStrings[0x9f] = "SALA 6 - x/8";
	sdata->lngStrings[0xa0] = "SALA 7 - x/8";
	sdata->lngStrings[0xa1] = "SALA 8 - x/8";	
		
	
	#endif

	int pn = octr->PageNumber;

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
        
    return false;
}


void NewPage_Events()
{
    label = 2;
    int i;

    // Add instruction text at the top
    #ifdef GASMOX_ENG
    DecalFont_DrawLine("PRESS ^ TO TOGGLE", 0x074, 0x30, FONT_SMALL, JUSTIFY_CENTER | TINY_GREEN);
    DecalFont_DrawLine("PRESS * TO CONFIRM", 0x074, 0x38, FONT_SMALL, JUSTIFY_CENTER | CRASH_BLUE);
    #elif defined(GASMOX_ES)
    DecalFont_DrawLine("PRESIONA ^ PARA ALTERNAR", 0x074, 0x30, FONT_SMALL, JUSTIFY_CENTER | TINY_GREEN);
    DecalFont_DrawLine("PRESIONA * PARA CONFIRMAR", 0x074, 0x38, FONT_SMALL, JUSTIFY_CENTER | CRASH_BLUE);
    #elif defined(GASMOX_BR)
    DecalFont_DrawLine("PRESSIONE ^ PARA ALTERNAR", 0x074, 0x30, FONT_SMALL, JUSTIFY_CENTER | TINY_GREEN);
    DecalFont_DrawLine("PRESSIONE * PARA CONFIRMAR", 0x074, 0x38, FONT_SMALL, JUSTIFY_CENTER | CRASH_BLUE);
    #endif

    for (i = 0; i < 8; i++)
    {
       int max = 8 * octr->PageNumber + i;
       menuRows[i].stringIndex = 0x9a + i;
       
       if (max < special_size) {
            // Check if this mode is incompatible with any enabled modes
            bool incompatible = IsGamemodeIncompatible(max);
            
            // Add an "X" to show enabled modes
            sprintf(gamemode_buffers[i], "%s %s", 
                octr->gamemodes[max] ? "X" : "", 
                special_name[max]);
                
            sdata->lngStrings[0x9a + i] = gamemode_buffers[i];
            
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
    
    // Now we're done with the Events menu when Triangle is pressed
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
			
    for (i = 0; i < 8; i++)
    {
		if (i < 4)
		sdata->lngStrings[0x9a + i] = engine_names[i];
		
        menuRows[i].stringIndex = 0x9a + i;
		
		if (i > 3)
		{
			sdata->lngStrings[0x9a + i] = "-";
			menuRows[i].stringIndex = -1;
			
		}
		
    }

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
	}

	RECTMENU_Show(&menu);

	int buttons = sdata->gGamepads->gamepad[0].buttonsTapped;

	if (buttons & BTN_LEFT) { octr->PageNumber = max(0, octr->PageNumber - 1); }
	if (buttons & BTN_RIGHT) { octr->PageNumber = min(pageMax, octr->PageNumber + 1); }
	if (buttons & (BTN_LEFT | BTN_RIGHT)) { DECOMP_OtherFX_Play(0, 1); }

	if (pageMax == 0) { return; }
	
	
	//can just use sprintf there i think
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
            // Calculate player count for survival mode
            int numDead = 0;
            for(i = 0; i < octr->NumDrivers; i++)
                if(octr->nameBuffer[i][0] == 0)
                    numDead++;
            int activePlayers = octr->NumDrivers - numDead;
            
            // Set lap count based on mode
            if (octr->gamemodes[SURVIVAL]) {
                // Set laps to player count or 1 if only one player
                octr->lapID = (activePlayers < 2) ? 0 : (activePlayers - 2);
            } else if (octr->gamemodes[SURVIVAL_TIMER]) {
                // Set laps to 127 for timed survival
                octr->lapID = 127;
            }
            
            // Skip laps menu by setting the lock flag
            octr->boolLockedInLap = 1;
        }
        
        // Reset page number for next menu
        octr->PageNumber = 0;
        
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

	// Skip NORMAL mode (i=0) and start from i=1
	for (int i = 1; i < special_size; i++) {
		if (octr->gamemodes[i]) {
			if (mode_count > 0) {
				strcat(title_buffer, "-");  // Use dash instead of plus
			}
			strcat(title_buffer, special_abbr[i]);  // Use abbreviation
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
		strcpy(title_buffer, special_abbr[0]);
	}
	
	// Draw any remaining modes
	if (strlen(title_buffer) > 0) {
		DECOMP_DecalFont_DrawLine(title_buffer, posX, posY, FONT_SMALL, (JUSTIFY_CENTER | TINY_GREEN));
	}


	DecalFont_DrawLine(
		countryNames[octr->serverCountry],
		0x10, 0x10, FONT_SMALL, 0);
//i think this is the room name in the top left of the screen
#ifdef GASMOX_ENG
	char* roomName = "ROOM x";
	roomName[5] = GetRoomChar(octr->serverRoom+1);

	DecalFont_DrawLine(
		roomName,
		0x10, 0x18, FONT_SMALL, 0);



	int numDead = 0;
	for(i = 0; i < octr->NumDrivers; i++)
		if(octr->nameBuffer[i][0] == 0)
			numDead++;

	posX = 0x110;
	sprintf(message, "Players: %d/8", (octr->NumDrivers-numDead));
	DecalFont_DrawLine(message,posX,0x58,FONT_SMALL,0);

	int h = 0;

#elif defined(GASMOX_ES)
	char* roomName = "SALA x";
	roomName[5] = GetRoomChar(octr->serverRoom+1);

	DecalFont_DrawLine(
		roomName,
		0x10, 0x18, FONT_SMALL, 0);



	int numDead = 0;
	for(i = 0; i < octr->NumDrivers; i++)
		if(octr->nameBuffer[i][0] == 0)
			numDead++;

	posX = 0x110;
	sprintf(message, "JUGADORES: %d/8", (octr->NumDrivers-numDead));
	DecalFont_DrawLine(message,posX,0x58,FONT_SMALL,0);

	int h = 0;

#elif defined(GASMOX_BR)
	char* roomName = "SALA x";
	roomName[5] = GetRoomChar(octr->serverRoom+1);

	DecalFont_DrawLine(
		roomName,
		0x10, 0x18, FONT_SMALL, 0);



	int numDead = 0;
	for(i = 0; i < octr->NumDrivers; i++)
		if(octr->nameBuffer[i][0] == 0)
			numDead++;

	posX = 0x110;
	sprintf(message, "JOGADORES: %d/8", (octr->NumDrivers-numDead));
	DecalFont_DrawLine(message,posX,0x58,FONT_SMALL,0);

	int h = 0;
#endif


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
		int color =
	octr->boolLockedInEnginee[i] ? PURA_VIOLET : PAPU_YELLOW;

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

		char* curr_engine =
					engine_names[octr->enginetype[slot]];


		posX = 0x18C;
		DecalFont_DrawLine(curr_engine, (posX - 11),posY,FONT_SMALL,color);
	}

	posX = 0x11E;
	posY = 0xB3;
#ifdef GASMOX_ENG
	DecalFont_DrawLine("Gasmoxian is a modified",posX,posY,FONT_SMALL,0);
	DecalFont_DrawLine("version of OnlineCTR.",posX+0x10,posY+0x8,FONT_SMALL,0);
	DecalFont_DrawLine("This is for the fans and fun!",posX-0x28,posY+0x10,FONT_SMALL,PAPU_YELLOW);
#elif defined(GASMOX_ES)
	DecalFont_DrawLine("gasmoxian es una version",posX,posY,FONT_SMALL,0);
	DecalFont_DrawLine("modificada de onlinectr,",posX-0x8,posY+0x8,FONT_SMALL,0);
	DecalFont_DrawLine("hacemos esto por los fans",posX-0x18,posY+0x10,FONT_SMALL,PAPU_YELLOW);
#elif defined(GASMOX_BR)
    DecalFont_DrawLine("gasmoxian eh uma versao",posX,posY,FONT_SMALL,0);
    DecalFont_DrawLine("modificada do octr, para",posX-0x8,posY+0x8,FONT_SMALL,0);
    DecalFont_DrawLine("diversao de todos os fas",posX-0x18,posY+0x10,FONT_SMALL,PAPU_YELLOW); 

#endif
	}
	

char* onlineLapString = "Laps: 000\0";
void PrintRecvTrack()
{
	char message[32];

#ifdef GASMOX_ENG
	sprintf(message, "Track: %s",
#elif defined(GASMOX_ES) || defined(GASMOX_BR)
sprintf(message, "Pista: %s",
#endif
				sdata->lngStrings
				[
					data.metaDataLEV[octr->levelID].name_LNG
				]
			);

	// UI-test
	// boolEndOfRace = 1;

	int posX = 0x0F8;

	int numLaps = sdata->gGT->numLaps;
	onlineLapString[6] = '0' + ((numLaps / 100) % 10);
	onlineLapString[7] = '0' + ((numLaps / 10) % 10);
	onlineLapString[8] = '0' + (numLaps % 10);

	DecalFont_DrawLine(message,posX,0x38,FONT_SMALL,PAPU_YELLOW);
	DecalFont_DrawLine(onlineLapString,posX+2,0x40,FONT_SMALL,PAPU_YELLOW);
}