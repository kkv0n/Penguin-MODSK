//these make it not compile?
//#include <common.h>
//#include "global.h"
void (*funcs[NUM_STATES]) () =
{
	StatePS1_Launch_EnterPID,
	StatePS1_Launch_PickServer,
	StatePS1_Launch_PickRoom,
	StatePS1_Launch_Error,
	StatePS1_Lobby_AssignRole,
	StatePS1_Lobby_HostTrackPick,
	StatePS1_Lobby_SpecialPick,
	StatePS1_Lobby_GuestTrackWait,
	StatePS1_Lobby_CharacterPick,
	StatePS1_Lobby_EnginePick,
	StatePS1_Lobby_WaitForLoading,
	StatePS1_Lobby_StartLoading,
	StatePS1_Game_WaitForRace,
	StatePS1_Game_StartRace,
	StatePS1_Game_EndRace
};

RECT endRaceRECT =
{
	.x = 8,
	.y = 8,
	.w = 0x200-0x20,
	.h = 0xc8,
};

void ThreadFunc(struct Thread* t)
{
	
	int isIdle = 0;

	struct GameTracker* gGT = sdata->gGT;
	octr->boolPlanetLEV = gGT->levelID == 33; //lobby level1

	if(octr->boolPlanetLEV)
	{
		//idk what this do
		//but apparently this is responsible of load screen crashes
		//*(int*)0x800ae54c = 0x3e00008;
		//*(int*)0x800ae550 = 0;

        // freecam mode
        gGT->cameraDC[0].cameraMode = 3;

//pushbuffer
		struct PushBuffer* pb = &gGT->pushBuffer[0];

//cam position to right or left
		pb->pos[0] = 0x96C;
// cam position to up or down
		pb->pos[1] = 0xA64;
//zoom in or zoom out the camera		
		pb->pos[2] = 0xF879;
//same thing but with rotation "0 = left or right" "1 = up or down"
// "2 = zoom in or zoom out btw probably im wrong about the order"
		pb->rot[0] = 0x841 + 300;
		pb->rot[1] = 0x77c + 400;
		pb->rot[2] = 0xff5;
	}

	// only disable for no$psx testing,
	// which can force in-game with 8000c000=LOBBY_START_LOADING
	#if 1

	// if client is intentionally idle
	if(octr->boolClientBusy)
	{
		// isIdle = 0; // <- unnecesary but i'll leave for readability
	}

	// if client should not be idle
	else
	{
        // If windowsClientSync hasn't been updated, it means it is idle/gone/lagging
        if(octr->windowsClientSync == octr->lastWindowsClientSync){
            isIdle = 1; // the counter is the same as last, start unsync procedure
        } else {
            octr->lastWindowsClientSync = octr->windowsClientSync; // client did update, change last counter
        }
	}

    // count frames that the client didn't update the game
    if(isIdle==1 && octr->CurrState > LAUNCH_ENTER_PID){
        octr->frames_unsynced++;
    } else {
        octr->frames_unsynced = 0;
    }

	// close if client didn't update the game in DISCONNECT_AT_UNSYNCED_FRAMES
	int boolCloseClient = (octr->frames_unsynced > DISCONNECT_AT_UNSYNCED_FRAMES);

	// if client closed, or server disconnected
	if(boolCloseClient || (octr->CurrState < 0))
	{
		sdata->ptrActiveMenu = 0;

		if(octr->boolPlanetLEV)
		{
			// if closed==1, go to 0 ("please open client")
			// if closed==0, go to 1 (server select)
			octr->CurrState = !boolCloseClient;

			octr->serverLockIn1 = 0;
			octr->serverLockIn2 = 0;
			return;
		}

		// calls memset on OnlineCTR struct
		octr_entryHook();

		// if closed==1, go to 0 ("please open client")
		// if closed==0, go to 1 (server select)
		octr->CurrState = !boolCloseClient;

		// load next level
		//sdata->gGT->gameMode1 = LOADING;
		//sdata->Loading.stage = 0;

		// load with flag animation
		DECOMP_MainRaceTrack_RequestLoad(33);

		// kill thread,
		// dont execute again until game loads
		t->flags |= 0x800;
		return;
	}
	#endif

	// gameplay
	if (octr->CurrState >= GAME_WAIT_FOR_RACE && octr->CurrState < GAME_END_RACE)
	{
		void DrawOverheadNames();
		DrawOverheadNames();
	}

	if (octr->CurrState >= 0)
		funcs[octr->CurrState]();

	// if connecting, or entering IP
	if (octr->boolClientBusy)
	{
		DecalFont_DrawLine(
		
#ifdef GASMOX_ENG
"SEE CLIENT WINDOW",
#elif defined(GASMOX_ES)
"REVISA EL CLIENTE",
#elif defined(GASMOX_BR)
"VERIFIQUE CLIENTE",
#endif
			0x100,0x74,FONT_SMALL,JUSTIFY_CENTER|OXIDE_LIGHT_GREEN);
	}

	// not gameplay, must draw LAST
	if (octr->CurrState <= LOBBY_WAIT_FOR_LOADING)
	{
		//for now mute sounds in lobby to avoid sound bugs
		//in the future move this to (HOST_TRACK_PICK - WAIT_FOR_LOADING) to allow lobby music
		//tbh idk how to play lobby music
		
		Music_Stop();
	
		howl_StopAudio(1,1,0);
		sdata->unkAudioState = 0;
		
		
	    //if not in race then erase driving function
		
        for (int loop = 0; loop < 0xd; loop++)
        {
            gGT->drivers[0]->funcPtrs[loop] = 0;
        }
		
        //if not in race then disable all HUD flags
        gGT->hudFlags = 0;
		
		void PrintTimeStamp();
		PrintTimeStamp();
        lobbysquare();
		
		
//delete this later -penta3
//i forgot it XD
//already deleted

	}

	#if 0
	if(strcmp("debugcam", octr->nameBuffer[0]) == 0)
	{
		void Freecam();
		Freecam();
	}
	#endif
}

//separated to being able to move it in the future
void lobbysquare(struct GameTracker* gGT)
{
	gGT = sdata->gGT;
				struct RectMenu* m = sdata->ptrActiveMenu;

			// draw menu now because it is drawn
			// later, which puts it behind our background
			if(m != 0)
			{
				// clear width, then get width
				int width = 0;
				DECOMP_RECTMENU_GetWidth(m, &width, 1);

				// draw
				DECOMP_RECTMENU_DrawSelf(m, 0, 0, (int)width);
			}

			DECOMP_RECTMENU_DrawInnerRect(
				&endRaceRECT, 0, gGT->backBuffer->otMem.startPlusFour);
}