#include <common.h>
#include "adventure.h"

void MainFreeze_MenuPtrDefault(struct RectMenu* menu)
{
	int levID = 0;
	u_short stringID;
	u_int gameMode;

	struct GameTracker* gGT = sdata->gGT;
	gameMode = gGT->gameMode1;

	// if you have not waited 5 frames since the game was paused then quit
	if (gGT->cooldownfromPauseUntilUnpause != 0) return;

	// assume 5 frames have passed since paused
	if (menu->unk1e != 0)
	{
		menu->drawStyle &= 0xfeff;

		// if more than 2 screens
		if (2 < gGT->numPlyrCurrGame)
		{
			menu->drawStyle |= 0x100;
		}

		if
		(
			(menu->state & NEEDS_TO_CLOSE)
		)
		{
			return;
		}

		// quit adv hub if it's not loaded
		if (current_track != CUSTOM_HUB)
			return;

		 AH_Pause_Update();
		return;
	}

	if (menu->rowSelected < 0) return;

	// get stringID from selected row
	stringID = menu->rows[menu->rowSelected].stringIndex;

	// stringID 14: "OPTIONS"
	if (stringID == 14)
	{
		// Set Menu to Options
		sdata->ptrDesiredMenu = &data.menuRacingWheelConfig;

		data.menuRacingWheelConfig.rowSelected = 8;
		return;
	}

	// stringID 11: "AKU AKU HINTS"
	// stringID 12: "UKA UKA HINTS"
	if (stringID == 11 || stringID == 12)
	{
		// Set Menu to Hints
		sdata->ptrDesiredMenu = &D232.menuHintMenu; // in 232
		return;
	}

	// stringID 3: "QUIT"
	if (stringID == 3)
	{
		// Set Menu to Quit
		sdata->ptrDesiredMenu = &data.menuQuit;
		data.menuQuit.rowSelected = 1;
		return;
	}

	// must wait 5 frames until next pause
	gGT->cooldownFromUnpauseUntilPause = FPS_DOUBLE(5);

	// hide Menu
	 RECTMENU_Hide(menu);

	// get rid of pause flag
	gGT->gameMode1 &= ~PAUSE_1;

	 MainFreeze_SafeAdvDestroy();

	// careful, it's stringID MINUS one
	switch (stringID)
	{
		// stringID 1: "RESTART"
		case 1:

		// stringID 4: "RETRY"
		case 4:

			// restart race
			sdata->Loading.stage = -5;

			if ( RaceFlag_IsFullyOffScreen() == 1)
			{
				// checkered flag, begin transition on-screen
				 RaceFlag_BeginTransition(1);
			}

			// if you are not showing a ghost during a race
			if (sdata->boolReplayHumanGhost == 0) return;

			// If the ghost playing buffer is nullptr
			if (sdata->ptrGhostTapePlaying == 0) return;

			// Make P2 the character that is saved in the header of the
			// ghost that you will see in the race
			data.characterIDs[1] = sdata->ptrGhostTapePlaying->characterID;
			return;

		// stringID 2: "RESUME"
		case 2:

			// unpause game
			 ElimBG_Deactivate(gGT);

			// unpause audio
			 MainFrame_TogglePauseAudio(0);

			// play pause/unpause sound
			 OtherFX_Play(1, 1);
			return;

		// stringID 5: "CHANGE CHARACTER"
		case 5:

			// erase ghost of previous race from RAM
			 GhostTape_Destroy();

			// set level ID to main menu
			levID = MAIN_MENU_LEVEL;

			// return to character selection
			sdata->mainMenuState = 1;

			// when loading is done, add bit for "in mb"
			sdata->Loading.OnBegin.AddBitsConfig0 |= 0x2000;
			break;

		// stringID 6: "CHANGE LEVEL"
		case 6:

			// erase ghost of previous race from RAM
			 GhostTape_Destroy();

			// level ID of main mb
			levID = MAIN_MENU_LEVEL;

			// return to track selection
			sdata->mainMenuState = 2;

			// when loading is done
			// add bit for "in mb"
			sdata->Loading.OnBegin.AddBitsConfig0 |= 0x2000;
			break;

		// stringID 10: "CHANGE SETUP"
		case 10:

			// set level ID to main menu
			levID = MAIN_MENU_LEVEL;

			// return to battle setup
			sdata->mainMenuState = 3;

			// when loading is done
			// add bit for "in mb"
			sdata->Loading.OnBegin.AddBitsConfig0 |= 0x2000;
			break;

		// stringID 13: "EXIT TO MAP"
		case 13:
		current_track = CUSTOM_HUB;
		adventure_options();
		levID = load_track; 
		
		break;
		
		default:
			break;
	}

	 MainRaceTrack_RequestLoad(levID);
	return;
}