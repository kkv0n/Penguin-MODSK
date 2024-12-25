#include <common.h>
#include "global.h"

extern struct RectMenu menu;
struct Driver* driver;
bool oxidecam = 0;
extern int shouldExecuteSpecText;
unsigned int afk = 80;
extern void queuetojoin();
int label;

void menu_tittle() {
	if (octr->CurrState == LAUNCH_PICK_ROOM)
	{
		strcpy(sdata->lngStrings[0x4e], countryNames[octr->serverCountry]);
	}
	else
	{
     #ifdef GASMOX_ENG
	char* text[7] = { "GASMOXIAN", "SELECT TRACK", "GAME MODE", "LAPS", "SEL CHARACTER", "ENGINE TURNING", "GL EVERYONE!" };
	#elif defined(GASMOX_ES)
	char* text[7] = { "GASMOXIAN", "ELIGE PISTA", "ELIGE MODO", "VUELTAS", "ELIG PERSONAJE", "GIRO DE MOTOR", "SUERTE A TODOS!" };
	#elif defined(GASMOX_BR)
	char* text[7] = { "GASMOXIAN", "SELEC pista", "SELEC MODO", "VOLTAS", "SEL PERSONAGEM", "CURVA DE MOTOR", "boa sorte a todos!" };
	#endif
		//the text above the server list
		strcpy(sdata->lngStrings[0x4e], text[label]);
	}
}



//timer to finish the race by penta3
void finishracetimer() {
	static unsigned msCount = 0;

		if (octr->finishracetimer > 0)
	{
		msCount += sdata->gGT->elapsedTimeMS;
		if (msCount >= SECONDS(1))
		{
			msCount -= SECONDS(1);
			octr->finishracetimer--;
		}
		char msg[10];
		sprintf(msg, "%u", octr->finishracetimer);
		DecalFont_DrawLine(msg, 0x100, 0x67 - 11, FONT_BIG, JUSTIFY_CENTER | PLAYER_BLUE);
	}
	else { msCount = 0; }
}

//afk timer by penta3
void afktimer()
{

	static unsigned msCount1 = 0;
	
		if (afk > 0)
	{
		msCount1 += sdata->gGT->elapsedTimeMS;
		if (msCount1 >= SECONDS(1))
		{
			msCount1 -= SECONDS(1);
			afk--;
		}
		char msg[10];
		sprintf(msg, "%u", afk);
		DecalFont_DrawLine(msg, 0xCB, 0x7D, FONT_BIG, JUSTIFY_CENTER | CORTEX_RED);
	}
	else 
	{

	msCount1 = 0; 
	
    }
	
}
void StatePS1_Launch_EnterPID()
{
	//client closed text message
#ifdef GASMOX_ENG
	DECOMP_DecalFont_DrawLine(
		"Attach Windows Client To Continue",
		0x100,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
#elif defined(GASMOX_ES)
				DECOMP_DecalFont_DrawLine(
		"ABRE EL CLIENTE PARA CONTINUAR",
		0x100,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
#elif defined(GASMOX_BR)
							DECOMP_DecalFont_DrawLine(
		"Conecte com o Client para Continuar",
		0x100,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
#endif
}

extern char* countryNames[3];
bool initString = true;
//oxide icon

void ShowCharacterIcon(int characterID, int x, int y)
{
	if (octr->boolClientBusy || driver->heldItemID == 0x7 || driver->heldItemID == 0xe || driver->heldItemID == 0x6)
	{
		return;
	}
    
    struct Icon* icon = sdata->gGT->ptrIcons[data.MetaDataCharacters[characterID].iconID];

    
    Point pos = MakePoint(x, y);
    int scale = FP(1); 

    
    DECOMP_UI_DrawDriverIcon(icon, pos, sdata->gGT->pushBuffer_UI.ptrOT, 1, scale, MakeColor(0x80, 0x80, 0x80));
}

void StatePS1_Launch_PickServer()
{
	
	
	octr->autoRetryJoinRoomIndex = -1;



#ifdef GASMOX_ENG
DecalFont_DrawLine("WELCOME TO GASMOXIAN", 257, 23, FONT_BIG, JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);
#elif defined(GASMOX_ES)
DecalFont_DrawLine("BIENVENIDO,GASMOXIAN", 257, 23, FONT_BIG, JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);
#elif defined(GASMOX_BR)
DecalFont_DrawLine("BEMVINDO AO GASMOXIAN", 257, 23, FONT_BIG, JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);
#endif
	//print oxide icon
	ShowCharacterIcon(15, 235, 82);

	MenuWrites_ServerCountry();

	// If already picked
	if(MenuFinished() == 1)
	{

		return;
	}

	UpdateMenu();
	NewPage_ServerCountry();
}

void ResetPsxGlobals()
{
	// unlock everything (0,1,2,3,4,5)
	for(int i = 0; i < 6; i++)
		sdata->advProgress.rewards[i] = 0xffffffff;

	// 8mb RAM expansion, for emulators that support it.
	// Needed for 3 or more players on Adv Hub
	sdata->mempack[0].lastFreeByte = 0x807ff800;
	sdata->mempack[0].endOfAllocator = 0x807ff800;

	sdata->ptrActiveMenu = 0;

	// keep running till the client gets a result,
	// DriverID is set to -1 on windows-side before this.

	for(int i = 0; i < 8; i++)
	{
		data.characterIDs[i] = 0;
		octr->boolLockedInCharacters[i] = 0;
		octr->boolLockedInEnginee[i] = 0;
		octr->enginetype[i] = 0;
		afk = 80;
	}
}

// should rename to EnterRoom
void StatePS1_Launch_PickRoom()
{
#ifdef GASMOX_ENG
	DecalFont_DrawLine("BY ANZUP AND PENTA3", 274, 23, FONT_BIG, JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);
#elif defined(GASMOX_ES)
	DecalFont_DrawLine("POR ANZUP Y PENTA3", 274, 23, FONT_BIG, JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);
#elif defined(GASMOX_BR)
	DecalFont_DrawLine("POR ANZUP E PENTA3 ", 274, 23, FONT_BIG, JUSTIFY_CENTER | OXIDE_LIGHT_GREEN);
#endif
	DECOMP_DecalFont_DrawLine("THX FARADISE", 25, 178, FONT_SMALL, PAPU_YELLOW);
	ShowCharacterIcon(15, 120, 16);
	MenuWrites_ServerRoom();

	// If already picked
	if(MenuFinished() == 1)
	{
		ResetPsxGlobals();
		return;
	}

	UpdateMenu();
	NewPage_ServerRoom();

int serverTotal = 0;
for (int i = 0; i < 16; i++) {
    int curr = octr->clientCount[i];
    if (curr > 8) curr -= 8;
    serverTotal += curr;
}

// Players online counter
#ifdef GASMOX_ENG
char text[25] = "Players Online: 000";
#elif defined(GASMOX_ES)
char text[25] = "Jugadores actv: 000";
#elif defined(GASMOX_BR)
char text[25] = "Jogadores aqui: 000";
#endif
text[16] = '0' + ((serverTotal / 100) % 10);
text[17] = '0' + ((serverTotal / 10) % 10);
text[18] = '0' + (serverTotal % 10);

	DecalFont_DrawLine(
		text,
		menu.posX_curr,0xb8,
		FONT_SMALL,JUSTIFY_CENTER|PAPU_YELLOW);
		
		
		queuetojoin();
}

void StatePS1_Launch_Error()
{
	char str[32];
#ifdef GASMOX_ENG

	char* str2 = "PLEASE UPDATE YOUR GAME TO PLAY";
#elif defined(GASMOX_ES)
char* str2 = "ACTUALIZA TU JUEGO PARA JUGAR.";
#elif defined(GASMOX_BR)
char* str2 = "PLEASE UPDATE YOUR GAME TO PLAY";
#endif
	DECOMP_DecalFont_DrawLine(str2,0x100,0x74,FONT_SMALL,JUSTIFY_CENTER);

	sdata->ptrActiveMenu = 0;
}

void StatePS1_Lobby_AssignRole()
{
	afk = 80;
	menu.posX_curr = 0x70; // X position
	menu.posY_curr = 0x84;  // Y position

	if(octr->DriverID == 0)
	{
		octr->CurrState = LOBBY_HOST_TRACK_PICK;
	}

	else if (octr->DriverID > 0)
	{
		octr->CurrState = LOBBY_GUEST_TRACK_WAIT;
	}
}


void StatePS1_Lobby_HostTrackPick()
{
	
	MenuWrites_Tracks();

	// If already picked
	if(MenuFinished() == 1)
	{

      StatePS1_Lobby_SpecialPick();
		return;
	}

	PrintCharacterStats();

	UpdateMenu();
	NewPage_Tracks();
	
}

void StatePS1_Lobby_SpecialPick()
{
    MenuWrites_Events();

    
    if (MenuFinished() == 1)
	{

    void FakeState_Lobby_HostLapPick();
        FakeState_Lobby_HostLapPick();
        return;
	}

    PrintCharacterStats();
	

    UpdateMenu();
    NewPage_Events();
}


void FakeState_Lobby_HostLapPick()
{
	MenuWrites_Laps();

	// If already picked
	if(MenuFinished() == 1) return;
		

	PrintCharacterStats();
	

	UpdateMenu();
	NewPage_Laps();
	
	
}

void StatePS1_Lobby_GuestTrackWait()
{
	PrintCharacterStats();

	// close menu
	sdata->ptrActiveMenu = 0;
//ready to start the race text
#ifdef GASMOX_ENG
	DECOMP_DecalFont_DrawLine(
		"waiting for host",
		menu.posX_curr,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);

	DECOMP_DecalFont_DrawLine(
		"to pick the track",
		menu.posX_curr,0x7C,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
		
#elif defined(GASMOX_ES)
			DECOMP_DecalFont_DrawLine(
		"esperando a que el",
		menu.posX_curr,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);

	DECOMP_DecalFont_DrawLine(
		"host elija pista",
		menu.posX_curr,0x7C,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
		
#elif defined(GASMOX_BR)
			DECOMP_DecalFont_DrawLine(
		"Esperando o lider",
		menu.posX_curr,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);

	DECOMP_DecalFont_DrawLine(
		"escolher a pista",
		menu.posX_curr,0x7C,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
		
#endif
}

void StatePS1_Lobby_CharacterPick()
{

	afktimer();
	
	MenuWrites_Characters();

	// If already picked
	if(MenuFinished() == 1) return;

	PrintCharacterStats();
	PrintRecvTrack();

	UpdateMenu();
	NewPage_Characters();


	// get menu
	struct RectMenu* b = sdata->ptrActiveMenu;

	if(b != 0)
	{
		// update real-time
		data.characterIDs[0] = (8 * octr->PageNumber) + b->rowSelected;

//show icon in character selection
		int selectedchar = data.characterIDs[0];
		ShowCharacterIcon(selectedchar, 0x5D, 0x2A);
		
		//oxide custom cam
        if (data.characterIDs[0] == 15)
        {
            oxidecam = 1;
        }
        else
        {
            oxidecam = 0;
        }
    }
}



void StatePS1_Lobby_EnginePick()
{
	
	afktimer();
	
    MenuWrites_Engine();

    if (MenuFinished() == 1) return;


		
	PrintCharacterStats();
	PrintRecvTrack();

	UpdateMenu();
    NewPage_Engine();
	
	
}
void StatePS1_Lobby_WaitForLoading()
{
	afktimer();
	PrintCharacterStats();
	PrintRecvTrack();

}

static bool initRace = true;
static bool endRace = true;


void StatePS1_Lobby_StartLoading()
{
    label = 6;
	shouldExecuteSpecText = 1;
	initRace = true;
	endRace = true;
	PrintCharacterStats();
	PrintRecvTrack();
//i think this is the loading track text in the online menu
	DECOMP_DecalFont_DrawLine(
		"LOADING...",
		menu.posX_curr,menu.posY_curr,
		FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);

	// variable reuse, wait a few frames,
	// so screen updates with green names
	octr->CountPressX++;
	if(octr->CountPressX < FPS_DOUBLE(15)) return;

	// stop music,
	// stop "most FX", let menu FX ring
	Music_Stop();
	howl_StopAudio(1,1,0);
	sdata->unkAudioState = 0;

	struct GameTracker* gGT = sdata->gGT;

	// for battle tracks
	gGT->gameMode1 = LOADING | BATTLE_MODE;

	if(
		(octr->levelID <= TURBO_TRACK) ||
		(octr->levelID >= GEM_STONE_VALLEY)
	  )
	{
		
		//boss mode event
		if (octr->special == 7) {
		
		gGT->gameMode1 = LOADING | ADVENTURE_BOSS;
		
		}
		else
		{
			// for all other tracks
		gGT->gameMode1 = (octr->special == 3) ?  LOADING | TIME_TRIAL : LOADING | ARCADE_MODE;
		}
	}

	// instant load
	//sdata->Loading.stage = 0;

	// load with flag animation
	DECOMP_MainRaceTrack_RequestLoad(octr->levelID);

	// dont kill thread,
	// it will die when loading screen covers screen
}

RECT drawTimeRECT =
{
	.x = 0xffec,
	.y = 0x46 - 3,
	.w = 0x228,
	.h = 0
};

extern unsigned int checkpointTimes[(MAX_LAPS * CPS_PER_LAP) + 1];

static void OnRaceInit()
{
	for (int i = 0; i < MAX_NUM_PLAYERS; i++)
	{
		checkpointTracker[i].currCheckpoint = 0;
		checkpointTracker[i].timer = 0;
		checkpointTracker[i].raceFinished = 0;
	}
	for (int i = 0; i < MAX_LAPS * CPS_PER_LAP; i++)
	{
		checkpointTimes[i] = 0;
	}
	sdata->gGT->drivers[0]->bestLapTime = HOURS(10);
}

void StatePS1_Game_WaitForRace()
{
	
	struct GameTracker* gGT = sdata->gGT;
	if (initRace)
	{
		OnRaceInit();
		initRace = false;
	}

	gGT->trafficLightsTimer = 0xf40;


	if((gGT->gameMode1 & START_OF_RACE) != 0)
		return;

	// Copy from DrawUnpluggedMsg
	int posY;
	int i;

	posY = 0x46;
	drawTimeRECT.h = 0;
//more ui text for rooms
#ifdef GASMOX_ENG
	DECOMP_DecalFont_DrawLine(
		"WAITING FOR PLAYERS...",
		0x100, posY + drawTimeRECT.h,
		FONT_SMALL, (JUSTIFY_CENTER | OXIDE_LIGHT_GREEN));
#elif defined(GASMOX_ES)
			DECOMP_DecalFont_DrawLine(
		"ESPERANDO JUGADORES...",
		0x100, posY + drawTimeRECT.h,
		FONT_SMALL, (JUSTIFY_CENTER | OXIDE_LIGHT_GREEN));
#elif defined(GASMOX_BR)
			DECOMP_DecalFont_DrawLine(
		"ESPERANDO JOGADORES...",
		0x100, posY + drawTimeRECT.h,
		FONT_SMALL, (JUSTIFY_CENTER | OXIDE_LIGHT_GREEN));
#endif

	// add for each line
	drawTimeRECT.h += 8;

	// add 3 pixels above, 3 pixels bellow
	drawTimeRECT.h += 6;

	DECOMP_RECTMENU_DrawInnerRect(
		&drawTimeRECT, 1, gGT->backBuffer->otMem.startPlusFour);

	for(i = 0; i < 8; i++)
	{
		octr->Shoot[i].boolNow = 0;
	}
}

// not really "Start", it's the trafficLights,
// and entire duration of race, should rename
void StatePS1_Game_StartRace()
{

	int i;


	for(i = 1; i < 8; i++)
	{
		if(octr->Shoot[i].boolNow != 0)
		{
			octr->Shoot[i].boolNow = 0;

			struct Driver* d = sdata->gGT->drivers[i];

			if (d->instBombThrow != 0)
			{

	          // Detonate the bomb
				struct TrackerWeapon* bomb = (struct TrackerWeapon*)d->instBombThrow->thread->object;
				bomb->flags |= 2;
				d->instBombThrow = NULL;
			}
			else if (d->instBubbleHold != 0)
			{
				// Shoot the shield
				struct Shield* shield = (struct Shield*)d->instBubbleHold->thread->object;
				shield->flags |= 2;
				d->instBubbleHold = NULL;
			}
			else
			{
				if (octr->Shoot[i].boolJuiced)
					d->numWumpas = 10;
				d->heldItemID = octr->Shoot[i].Weapon;
				// copy/paste from ShootOnCirclePress
				int weapon;
				weapon = d->heldItemID;
				// Missiles and Bombs share code,
				// Change Bomb1x, Bomb3x, Missile3x, to Missile1x
				if (
					(weapon == 1) ||
					(weapon == 10) ||
					(weapon == 11)
					)
				{
					weapon = 2;
				}
				DECOMP_VehPickupItem_ShootNow(
					d,
					weapon,
					octr->Shoot[i].flags);
			}
		}
	}
	finishracetimer();
}

static void OnRaceEnd()
{
	struct Driver* before = sdata->gGT->cameraDC[0].driverToFollow;
	struct Driver** drivers = sdata->gGT->drivers;
	bool foundRacer = false;
	for (int driverID = 1; driverID < MAX_NUM_PLAYERS; driverID++)
	{

		if (!foundRacer && octr->nameBuffer[driverID][0] && !checkpointTracker[driverID].raceFinished)
		{
			sdata->gGT->cameraDC[0].driverToFollow = drivers[driverID];
			foundRacer = true;
		}
	}
	//tbh I don't think this fixes the spectator name bug.
	sdata->gGT->cameraDC[0].driverToFollow = before;
}

void StatePS1_Game_EndRace()
{
	if (endRace)
	{
		OnRaceEnd();
		endRace = false;
	}
}