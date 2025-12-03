#include <common.h>

extern bool _endgame;

struct Ovr233_Credits_BSS* creditsBSS = (struct Ovr233_Credits_BSS*)0x800b9488;

void CS_Credits_End()
{
	int levID;
	struct GameTracker* gGT = sdata->gGT;

	// erase 5 instances
	CS_Credits_DestroyCreditGhost();

	// kill thread
	creditsBSS->CreditThread->flags |= 0x800;
	
	//reset menus to being able to load main menu
	sdata->ptrDesiredMenu = 0;
	sdata->ptrActiveMenu = 0;
	sdata->mainMenuState = 0;
	
	//show "thanks for playing" text in main menu
	_endgame = true;
	
	//load main menu
	MainRaceTrack_RequestLoad(MAIN_MENU_LEVEL);

	gGT->renderFlags &= 0xfffffffb;
}