#include <common.h>
#include "global.h"
extern const char* options[16];
extern int label;

//special menu text, probably will move it later
#ifdef GASMOX_ENG
const char* special_0 = "NORMAL";
const char* special_1 = "MIRROR MODE";
const char* special_2 = "ICY TRACK";
const char* special_3 = "TIME TRIAL";
const char* special_4 = "MOON MODE";
const char* special_5 = "RETROFUELED";
const char* special_6 = "VOID WORLD";
const char* special_7 = "BOSS RACE";
const char* special_8 = "DEMO CAMERA";
#elif defined(GASMOX_ES)
const char* special_0 = "NORMAL";
const char* special_1 = "MODO ESPEJO";
const char* special_2 = "PISO DE HIELO";
const char* special_3 = "SIN ITEMS";
const char* special_4 = "MODO LUNAR";
const char* special_5 = "RETROFUELED";
const char* special_6 = "MUNDO VACIO";
const char* special_7 = "MODO JEFE";
const char* special_8 = "CAMARA DEMO";
#elif defined(GASMOX_BR)
const char* special_0 = "NORMAL";
const char* special_1 = "ESPELHADO";
const char* special_2 = "PISTA GELO";
const char* special_3 = "SEM ITENS";
const char* special_4 = "MODO LUNAR";
const char* special_5 = "RETROFUELED";
const char* special_6 = "PISTA VAZIA";
const char* special_7 = "CONTRA CHEFE";
const char* special_8 = "DEMO CAMERA";
#endif



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
char* countryNames[3] =
{
	"Mednafen Peru",
	"Gasmox USA",
#ifdef GASMOX_ENG
    "Private server",
#elif defined(GASMOX_ES) || defined(GASMOX_BR)
    "Sala privada",
#endif
	
};

void NewPage_ServerCountry()
{
	label = 0;
	int i;

menu.posX_curr = 0xFE;  // X position
menu.posY_curr = 0x84;  // Y position

	// override "LAPS" "3/5/7",
	// and other unimportant strings
	for(i = 0; i < 3; i++)
	{
		menuRows[i].stringIndex = 0x9a+i;
		sdata->lngStrings[0x9a+i] = countryNames[i];
	}

	for(i = 3; i < 8; i++) {
		menuRows[i].stringIndex = -1;
	}

	menuRows[i].stringIndex |= 0x8000;
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


	for (int i = 0; i < 8; i++)
	{
		int id = 8 * octr->PageNumber + i;
		menuRows[i].stringIndex = data.metaDataLEV[id].name_LNG;

		if (id > TURBO_TRACK)
		{
			for (int j = TURBO_TRACK + 1; j <= id; j++)
			{
				sdata->lngStrings[data.metaDataLEV[j].name_LNG] = "-";
			}

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

void NewPage_Events()
{
	label = 2;
    int i;



    // OCTR SPECIAL MENU BY PENTA3
char* special_name[9] = { special_0, special_1, special_2, special_3, special_4, special_5, special_6, special_7, special_8 };

    for (i = 0; i < 8; i++)
    {
       int max = 8 * octr->PageNumber + i;
	   
	   if (max < 9) {
        sdata->lngStrings[0x9a + i] = special_name[max];

        
        menuRows[i].stringIndex = 0x9a + i;
	   }

     else {
        
        sdata->lngStrings[0x9a + i] = "-";
        menuRows[i].stringIndex |= 0x8000;  
    }
}
}


void MenuWrites_Events()
{
	//1 PAGE
	pageMax= 1;
    OnPressX_SetPtr = &octr->special;
    OnPressX_SetLock = &octr->boolLockedInSpecial;
}

//these are the laps available in the menu
//only visual text, if you want to change the real number
//search int numLaps in CL_main.c
void NewPage_Laps()
{
	label = 3;
	int i;


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

    #ifdef GASMOX_ENG
    sdata->lngStrings[0x9a] = "BALANCED";
    sdata->lngStrings[0x9b] = "ACCELERATION";
    sdata->lngStrings[0x9c] = "SPEED";
    sdata->lngStrings[0x9d] = "MAXIMUM";
    sdata->lngStrings[0x9e] = "-";
    sdata->lngStrings[0x9f] = "-";
	sdata->lngStrings[0xa0] = "-";
    sdata->lngStrings[0xa1] = "-";
	#elif defined(GASMOX_ES)
	sdata->lngStrings[0x9a] = "BALANCEADO";
    sdata->lngStrings[0x9b] = "ACELERACION";
    sdata->lngStrings[0x9c] = "VELOCIDAD";
    sdata->lngStrings[0x9d] = "MAXIMO";
    sdata->lngStrings[0x9e] = "-";
    sdata->lngStrings[0x9f] = "-";
    sdata->lngStrings[0xa0] = "-";
    sdata->lngStrings[0xa1] = "-";	
	#elif defined(GASMOX_BR)
	sdata->lngStrings[0x9a] = "EQUILIBRADO";
    sdata->lngStrings[0x9b] = "ACELERACAO";
    sdata->lngStrings[0x9c] = "VELOCIDADE";
    sdata->lngStrings[0x9d] = "MAXIMIZADO";
    sdata->lngStrings[0x9e] = "-";
    sdata->lngStrings[0x9f] = "-";	
	sdata->lngStrings[0xa0] = "-";
    sdata->lngStrings[0xa1] = "-";
	#endif

    for (i = 0; i < 8; i++)
    {
        menuRows[i].stringIndex = 0x9a + i;
    }
	for (i = 4; i < 8; i++)
	{
	menuRows[i].stringIndex = -1;
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

	int string =
		(('1' + octr->PageNumber) << 0) |
		('/' << 8) |
		(('1' + pageMax) << 16);

	DECOMP_MainFreeze_ConfigDrawArrows(menu.posX_curr, 0x48, &string);

	DecalFont_DrawLine(&string,menu.posX_curr,0x48,FONT_BIG,JUSTIFY_CENTER|WHITE);
}

void RECTMENU_OnPressX(struct RectMenu* b)
{
	int i;

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

//special events text when you are in a room
//for the special events logic search octr special in cl_main.c
char* special_titles[9] = { special_0, special_1, special_2, special_3, special_4, special_5, special_6, special_7, special_8 };

char* title = nullptr;


if (octr->special >= 0 && octr->special < 9) {
    title = special_titles[octr->special];
} 
else
{
    
    title = special_0;  
}
	DecalFont_DrawLine(title,0x100,0x18,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);





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

	int posX;

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

	int posX;

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

	int posX;

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

		int posY = 0x60+h;
		h += 8;

		posX = 0x110;
		sprintf(message, "%s:", str);
		DecalFont_DrawLine(message,posX,posY,FONT_SMALL,color);

		if(octr->CurrState < LOBBY_CHARACTER_PICK)
			continue;

		char* characterName =
			sdata->lngStrings[
				data.MetaDataCharacters[
					data.characterIDs[slot]
				].name_LNG_short];

		posX = 0x18C;
		DecalFont_DrawLine(characterName,posX,posY,FONT_SMALL,color);
	}

	posX = 0x11E;
	int posY = 0xB3;
#ifdef GASMOX_ENG
	DecalFont_DrawLine("gasmoxian is a modified",posX,posY,FONT_SMALL,0);
	DecalFont_DrawLine("version of onlinectr,we",posX-0x8,posY+0x8,FONT_SMALL,0);
	DecalFont_DrawLine("do things for fans n fun",posX-0x18,posY+0x10,FONT_SMALL,PAPU_YELLOW);
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