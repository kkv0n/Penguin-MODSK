#include <common.h>

// byte budget
// 840/1108
extern bool _endgame;

void MM_Characters_MenuProc();

void MM_MenuProc_Main(struct RectMenu* mainMenu)
{
	short choose;
	struct GameTracker* gGT = sdata->gGT;

	MM_ParseCheatCodes();

	if (_endgame)
	{
		DecalFont_DrawLine("THANKS FOR PLAYING!", 5, 187, FONT_SMALL, ORANGE);
	}

	DecalFont_DrawLine("CUSTOM ADVENTURE", 5, 197, FONT_SMALL, ORANGE);
	DecalFont_DrawLine("BY CUSTOM TEAM RACING", 5, 206, FONT_SMALL, ORANGE);


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

	mainMenu->state |= ONLY_DRAW_TITLE;


	// get LNG index of row selected
	choose = mainMenu->rows[mainMenu->rowSelected].stringIndex;

	// Adventure Mode
	if (choose == 0x4c)
	{
		// Leave main menu hierarchy
		D230.MM_State = 2;

		// dont show load/save menu
		D230.desiredMenuIndex = 0;

		// Turn on Adventure Mode, turn off item cheats
		gGT->numPlyrNextGame = 1;
		gGT->gameMode1 |= ADVENTURE_MODE;
		gGT->gameMode2 &= ~(CHEAT_WUMPA | CHEAT_MASK | CHEAT_TURBO | CHEAT_ENGINE | CHEAT_BOMBS);


	}

	return;

}
