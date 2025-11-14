#include <common.h>
#include "header/bot.h"

// byte budget
// 840/1108


void MM_MenuProc_Main(struct RectMenu* mainMenu)
{
	short choose;
	struct GameTracker* gGT = sdata->gGT;

	MM_ParseCheatCodes();
	
	//draw font messages
	Lines_OnMainMenu();


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


	// do this for every track selected in the menu
	if (choose == 133 || choose == 134)
	{
		ghostMode = (choose == 133);
		

        LOAD_BOT_PATH(ghostMode);


		gGT->numPlyrNextGame = 1;
		
		if (ghostMode)
		{
			gGT->gameMode1 &= ~(ADVENTURE_BOSS);
			gGT->gameMode1 |= ARCADE_MODE;
			gGT->gameMode2 &= ~(CHEAT_WUMPA | CHEAT_MASK | CHEAT_TURBO | CHEAT_ENGINE | CHEAT_BOMBS);
		}
		else
		{
			gGT->gameMode1 |= ADVENTURE_BOSS;
		}
        
        //unlock 16 characters
        sdata->gameProgress.unlocks[0] |= UNLOCK_CHARACTERS;
        
        // Leave main menu hierarchy
		D230.MM_State = 2;
		D230.desiredMenuIndex = 2;

	}


	// DONT change, should only work in Arcade, and VS
	if ((gGT->gameMode2 & CHEAT_ONELAP) != 0)
	{
		gGT->numLaps = 1;
	}

	return;
}

