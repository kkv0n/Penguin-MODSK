#include <common.h>
#include "utils.h"
#define FALSE 0
#define TRUE 1

char* track_name;

//booleans didnt worked here first but when i realized it was a bug in this function i was too lazy to change it lol
extern u_char reserves_bar;
extern u_char showstars;

void MM_MenuProc_Main(struct RectMenu* mainMenu)
{
  struct GameTracker* gGT = sdata->gGT;
  
  track_name = "MYTRACK";
  CUSTOM_LEVEL_ID = CRASH_COVE; //level music
  reserves_bar = FALSE; //change "FALSE" to -> "TRUE" to enable reserves bar
  showstars = FALSE; //change "FALSE" to -> "TRUE" to show stars in the sky
  
  DecalFont_DrawLine("COVE PS1 MOD", 5, 197, FONT_SMALL, ORANGE);
  DecalFont_DrawLine(__DATE__, 	5, 206, FONT_SMALL, ORANGE);
  DecalFont_DrawLine(__TIME__, 	170, 206, FONT_SMALL, ORANGE);

  if (mainMenu->unk1e == 1) { MM_Title_MenuUpdate(); }

  MM_Title_Init();

  if (((mainMenu->state & 0x400) == 0) || (mainMenu->unk1e != 0) || ((mainMenu->rowSelected) < 0)) { return; }

  gGT->gameMode1 &= ~(BATTLE_MODE | ADVENTURE_MODE | TIME_TRIAL | ADVENTURE_ARENA | ARCADE_MODE | ADVENTURE_CUP);
  gGT->gameMode2 &= ~(CUP_ANY_KIND);
  gGT->numLaps = 3;

  // set a flag that you're in main menu
  mainMenu->state |= 4;

  // If you choose Time Trial in Main Menu
  if (mainMenu->rows[mainMenu->rowSelected].stringIndex == 379)
  {
	
    gGT->numPlyrNextGame = 1;
    gGT->gameMode1 |= TIME_TRIAL;
    gGT->gameMode2 &= ~CHEAT_ALL;
    sdata_static.gameProgress.unlocks[0] |= UNLOCK_CHARACTERS;
    data.metaDataLEV[CUSTOM_LEVEL_ID].timeTrial = 0x7FFFFFFF;

    if(sdata->ptrGhostTapePlaying == 0)
    {
      SelectProfile_ToggleMode(0x30);
      sdata->boolReplayHumanGhost = 0;
      sdata->ptrGhostTapePlaying = MEMPACK_AllocHighMem(0x3e00);
    }

    // Leave main menu hierarchy
    D230.MM_State = 2;

    // Set next stage to 2 for Time Trial
    D230.desiredMenuIndex = 2;
  }
}
