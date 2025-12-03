#include <common.h>
#include "adventure.h"


void UI_RaceEnd_MenuProc(struct RectMenu* menu)
{
  short option;
  struct GameTracker *gGT;
  u_int uVar3;
  int iVar4;
  u_short style;

  gGT = sdata->gGT;
  
  if (menu->unk1e != 0)
  {
    menu->drawStyle &= ~(0x100);

    // if more than 2 screens
    if (2 < gGT->numPlyrCurrGame)
      menu->drawStyle |= 0x100;
  
	return;
  }
  
  int row = menu->rowSelected;
  if(row < 0) return;
  
  option = menu->rows[row].stringIndex;
  
  // if not SAVE GHOST
  if (option != 9)
  {
    // make Menu invisible
     RECTMENU_Hide(menu);
  }
  
  sdata->framesSinceRaceEnded = 0;
  sdata->numIconsEOR = 1;
  
  // Press * To Continue
  // do not put this in the switch,
  // switch needs to be a "small" jump table,
  // and an offest this large could bloat table
  if(option == 0xc9)
  {
    sdata->menuReadyToPass |= 1;
    return;
  }
  
  switch(option)
  {
	// Quit
	case 3:
	{
		// Erase ghost of previous race from RAM
		 GhostTape_Destroy();
		
		// go back to main menu
		sdata->mainMenuState = 0;
		
		// load LEV of main menu
		 MainRaceTrack_RequestLoad(0x27);
		break;
	}
	
	case 4:
	{
		// Turn off HUD
		gGT->hudFlags &= 0xfe;
	
		if ( RaceFlag_IsFullyOffScreen() == 1)
			 RaceFlag_BeginTransition(1);
	
		sdata->Loading.stage = -5;
	
		// clear backup,
		// keep music,
		// destroy "most" fx, let menu fx play to end
		 howl_StopAudio(1, 0, 0);
	
		// if did not improve time, then dont
		// overwrite old ghost with new ghost
		if ((gGT->gameModeEnd & PLAYER_GHOST_BEAT) == 0)
			break;
		
		sdata->boolReplayHumanGhost = 1;
			
		// slower than ND's copy, I know, we'll
		// come up with a modern-gcc friendly way
		// to sort the LWs and SWs later
		memcpy(
			sdata->ptrGhostTapePlaying, 
			sdata->GhostRecording.ptrGhost, 
			0x3e00);
	
		// Make P2 the character that is saved in the
		// header of the ghost that you will see in the race
		data.characterIDs[1] = sdata->ptrGhostTapePlaying->characterID;
	
		// no ghosts are drawing
		sdata->boolGhostsDrawing = 0;
	
		break;
	}
	
	// Change Character, or Change Level
	case 5:
	case 6:
	{
		// Erase ghost of previous race from RAM
		 GhostTape_Destroy();
	
		// 1 for character select
		// 2 for track select
		sdata->mainMenuState = option - 4;
		
		// when loading is done
		// add flag for "in menus"
		sdata->Loading.OnBegin.AddBitsConfig0 |= 0x2000;
		
		// load LEV of main menu
		 MainRaceTrack_RequestLoad(0x27);
		break;
	}
	
	// Save Ghost
	case 9:
	{
		sdata->framesSinceRaceEnded = FPS_DOUBLE(0x3f9);
		
		#ifndef REBUILD_PS1
		// Set Load/Save to Ghost mode
		SelectProfile_ToggleMode(0x31);
		#endif
		
		// Change active Menu to GhostSelection
		sdata->ptrActiveMenu = &data.menuGhostSelection;
		break;
	}
	
	// Change Setup
	case 10:
	{
		// go to battle setup screen
		sdata->mainMenuState = 3;
		
		// load LEV of main menu
		 MainRaceTrack_RequestLoad(0x27);
		break;
	}
	
	// Exit To Map
	case 0xd:
	{
		current_track = CUSTOM_HUB;
		adventure_options();
		MainRaceTrack_RequestLoad(load_track);
		break;
	}
  }
}