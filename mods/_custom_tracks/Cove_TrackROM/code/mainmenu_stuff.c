#include <common.h>
#include "header/cove_rom.h"

char* track_name;


void MM_MenuProc_Main(struct RectMenu* mainMenu)
{
	short choose;
	struct GameTracker* gGT = sdata->gGT;

	MM_ParseCheatCodes();



  track_name = "MY TRACK";
  CUSTOM_LEVEL_ID = CRASH_COVE; //level music
  reserves_bar = TRUE; //change "FALSE" to -> "TRUE" to enable reserves bar
  weather_type = CLEAR; // change "CLEAR" to -> "RAIN" for rain, or "SNOW" for snow "weather_type = RAIN;" "weather_type = SNOW;"
  RETRO_FUELED = FALSE; //change "FALSE" to -> "TRUE" to enable retro fueled mechanics + blue fire texture
  
  DecalFont_DrawLine("COVE ROM MOD", 5, 197, FONT_SMALL, CORTEX_RED);
  DecalFont_DrawLine(__DATE__, 	5, 206, FONT_SMALL, ORANGE);
  DecalFont_DrawLine(__TIME__, 	170, 206, FONT_SMALL, ORANGE);


	// If you are at the highest hierarchy level of main menu
	if (mainMenu->unk1e == 1)
	{
		MM_Title_MenuUpdate();
	}

	MM_Title_Init();

	// if funcPtr is null
	if ((mainMenu->state & EXECUTE_FUNCPTR) == 0)
	{
		return;
	}

	struct Title* titleObj = D230.titleObj;

	// if "title" object exists
	if (titleObj != NULL)
	{
		// CameraPosOffset X
		titleObj->cameraPosOffset[0] = 0;
	}


	// If you are here, then you must not be
	// at the highest level of menu hierarchy

	// if row is negative, do nothing
	if ((mainMenu->unk1e != 0) || ((mainMenu->rowSelected) < 0))
	{
		return;
	}

	// clear flags from game mode
	gGT->gameMode1 &= ~(BATTLE_MODE | ADVENTURE_MODE | TIME_TRIAL | ADVENTURE_ARENA | ARCADE_MODE | ADVENTURE_CUP);

	// clear more game mode flags
	gGT->gameMode2 &= ~(CUP_ANY_KIND);
    
    
    //laps
    gGT->numLaps = 3;

	mainMenu->state |= ONLY_DRAW_TITLE;


	// get LNG index of row selected
	choose = mainMenu->rows[mainMenu->rowSelected].stringIndex;

	// Time trial
	if (choose == 379)
	{

		// Turn on Adventure Mode, turn off item cheats
		gGT->numPlyrNextGame = 1;
		gGT->gameMode1 |= ARCADE_MODE;
		gGT->gameMode2 &= ~(CHEAT_WUMPA | CHEAT_MASK | CHEAT_TURBO | CHEAT_ENGINE | CHEAT_BOMBS);
        
        //unlock 16 characters
        sdata->gameProgress.unlocks[0] |= UNLOCK_CHARACTERS;
		
		//reset some time trial flag?
        data.metaDataLEV[CUSTOM_LEVEL_ID].timeTrial = 0x7FFFFFFF;
        
        // Leave main menu hierarchy
		D230.MM_State = 2;
		D230.desiredMenuIndex = 2;
        
		//allocate memory for ghosts
        sdata->ptrGhostTapePlaying = MEMPACK_AllocHighMem(0x3e00);
		
		//reset ghost data
        memset(sdata->ptrGhostTapePlaying, 0, sizeof(struct GhostHeader));
        sdata->boolReplayHumanGhost = 0;


	}

	return;

}
