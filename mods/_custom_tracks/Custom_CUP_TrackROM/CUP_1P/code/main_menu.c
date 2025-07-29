#include <common.h>
#include "../../tracks.h"

// byte budget
// 840/1108


void MM_MenuProc_Main(struct RectMenu* mainMenu)
{
	short choose;
	struct GameTracker* gGT = sdata->gGT;



	MM_ParseCheatCodes();

	DecalFont_DrawLine(cup_tittle, 5, 188, FONT_SMALL, t_color);
	DecalFont_DrawLine(madeby, 5, 197, FONT_SMALL, t_color);
	DecalFont_DrawLine(__DATE__, 5, 206, FONT_SMALL, t_color);
	DecalFont_DrawLine(__TIME__, 170, 206, FONT_SMALL, t_color);



	// If you are at the highest hierarchy level of main menu
	if (mainMenu->unk1e == 1)
	{
		MM_Title_MenuUpdate();
	}


	MM_Title_Init();
	/*
	  // if drawing ptrNextBox_InHierarchy
	  if ((mainMenu->state & DRAW_NEXT_MENU_IN_HIERARCHY) != 0)
	  {
		D230.timerInTitle = 1000;
	  }
	 */


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


	// if you are at highest level of menu hierarchy
	if ((mainMenu->unk1e != 0) || ((mainMenu->rowSelected) < 0))
	{
		// leave the function
		return;
	}



	// clear flags from game mode
	gGT->gameMode1 &= ~(BATTLE_MODE | ADVENTURE_MODE | TIME_TRIAL | ADVENTURE_ARENA | ARCADE_MODE | ADVENTURE_CUP);

	// clear more game mode flags
	gGT->gameMode2 &= ~(CUP_ANY_KIND);

	mainMenu->state |= ONLY_DRAW_TITLE;

	// get LNG index of row selected
	choose = mainMenu->rows[mainMenu->rowSelected].stringIndex;


	switch (choose)
	{
	case 133:
	{
		selected_track = 0;
		break;
	}
#if defined(CUSTOM_CUP) || defined (DOUBLE_TRACK)
	case 134:
	{
		selected_track = 1;
		break;
	}
#endif
	
#ifdef CUSTOM_CUP
	case 135:
	{
		selected_track = 2;
		break;
	}
	case 136:
	{
		selected_track = 3;
		break;
	}
#endif
	}

#ifdef CUSTOM_CUP
	if (choose == 137)
	{
		cup_enabled = true;
		D230.MM_State = 2;

		// Set next stage to 2 for Time Trial
		D230.desiredMenuIndex = 2;

		sdata->gameProgress.unlocks[0] |= UNLOCK_CHARACTERS;
	}
#endif


	// do this for every track selected in the menu
	if (choose >= 133 && choose <= 136)
	{
		cup_enabled = false;
		track_options();





			D230.MM_State = 2;
			D230.desiredMenuIndex = 2;





		gGT->numLaps = d_laps % 128;
		gGT->numLaps = (gGT->numLaps == 0) ? 1 : gGT->numLaps;
		gGT->numPlyrNextGame = 1;
		gGT->gameMode1 |= gamemode;


		sdata->gameProgress.unlocks[0] |= UNLOCK_CHARACTERS;
	}


	// DONT change, should only work in Arcade, and VS
	if ((gGT->gameMode2 & CHEAT_ONELAP) != 0)
	{
		gGT->numLaps = 1;
	}

	return;
}

