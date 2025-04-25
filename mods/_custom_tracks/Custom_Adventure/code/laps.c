#include <common.h>

#define HANDLE_NULL_DRIVER continue

unsigned int PB_lap;
unsigned int Worst_lap;

void PlayLevel_UpdateLapStats(void)
{
	u_char bVar1;
	int finishIndex; //im tired of ivar999 part 1
	u_char lapCounter;
	int iVar4;
	struct Driver* firstRank;
	struct Driver* currDriver;
	int distToFinish_prev;
	int distToFinish_curr;
	int minDistance;
	int iVar9;
	int driverIndex; //im tired of ivar999 part 2
	int iVar13;
	int currRank;
	struct GameTracker* gGT = sdata->gGT;

	iVar9 = 0;
	iVar13 = 0;
	currRank = 0;

	// find farthest-ahead human
	for (int iVar9 = 0; iVar9 < 8; iVar9++)
	{
		firstRank = gGT->driversInRaceOrder[iVar9];

		if ((firstRank != 0) && ((firstRank->actionsFlagSet & 0x100000) == 0))
			break;
	}


	for (driverIndex = 0; driverIndex < 8; driverIndex++)
	{
		gGT->driversInRaceOrder[driverIndex] = NULL;

		// iVar6 = driver struct
		currDriver = gGT->drivers[driverIndex];

		if (currDriver == NULL)
			HANDLE_NULL_DRIVER;

		// before and after
		distToFinish_prev = currDriver->distanceToFinish_curr;
		VehLap_UpdateProgress(currDriver);
		distToFinish_curr = currDriver->distanceToFinish_curr;

		int drivenBackwards = currDriver->distanceDrivenBackwards + (distToFinish_curr - distToFinish_prev);

		// clamp minimum
		if (drivenBackwards < 0)
			drivenBackwards = 0;

		// clamp to max
		else if (drivenBackwards > 1000)
			drivenBackwards = 1000;

		// update distance driven backwards
		currDriver->distanceDrivenBackwards = drivenBackwards;

		// === Natty Video ===
		// https://www.youtube.com/watch?v=lDaT2rY6GKI

		// Part A: Start-line -> 32000 distToFinish
		// Part B: 32000 distToFinish -> 1200 distToFinish
		// Part C: 1200 distToFinish -> Finish-line

		if (
			// crossed finishline (forwards)
			(distToFinish_prev < 1200) &&
			(distToFinish_curr > 32000)
			)
		{
			// Set racer's distance driven backwards to zero
			currDriver->distanceDrivenBackwards = 0;

			// if driving behind startline, now not
			if ((currDriver->actionsFlagSet & 0x1000000) != 0)
			{
				currDriver->actionsFlagSet &= ~(0x1000000);

				// skip next 46 lines of code
				goto LAB_800418b4;
			}

			// update checkpoint with distToFinish
			currDriver->distanceToFinish_checkpoint = distToFinish_curr;

			//this get updated when you cross the finish line
			if (driverIndex == 0 && gGT->numPlyrCurrGame == 1) //driver[0] & single player only
			{
				unsigned int currLapTime = gGT->elapsedEventTime - currDriver->lapTime; //this is lap time, it is restarted each lap

				if (currLapTime < PB_lap)
				{
					PB_lap = currLapTime;
				}
				if (currLapTime > Worst_lap)
				{

					Worst_lap = currLapTime;
				}




			}


			// If finished last lap, clamp
			if (gGT->numLaps < (currDriver->lapIndex + 1))
			{
				lapCounter = currDriver->lapIndex;
			}

			// if this is not final lap
			else
			{


				// time on the clock
				currDriver->lapTime = gGT->elapsedEventTime;

				// lap counter = lap counter + 1
				currDriver->lapIndex++;

				// if farthest-ahead human
				if (currDriver == firstRank)
				{
					OtherFX_Play(0x66, 1);
					Voiceline_ClearTimeStamp();
				}

				lapCounter = currDriver->lapIndex;

				// If Final Lap
				if (lapCounter == (gGT->numLaps - 1))
				{
					// if this is human and not AI
					if ((currDriver->actionsFlagSet & 0x100000) == 0)
					{

						// frames, so the animation lasts 3 seconds
						sdata->finalLapTextTimer[driverIndex] = FPS_DOUBLE(90);
					}
				}
			}

			// If did not just finish race
			if (lapCounter != gGT->numLaps)
				goto LAB_800418b4;

			// === If did just finish race ===

			// if racer hadn't finished the race
			if ((currDriver->actionsFlagSet & 0x2000000) == 0)
			{
				// this one racer has now finished the race
				currDriver->actionsFlagSet |= 0x2000000;

				// === Run on first frame that race ends ===

				// if total event hasn't finished (gGT->gameMode1)
				if ((gGT->gameMode1 & 0x200000) == 0)
				{
					// set driver placement rank, based on
					// how many drivers have finished the race
					currDriver->driverRank = sdata->numPlayersFinishedRace;
					sdata->numPlayersFinishedRace++;
				}

				// you have no weapon
				currDriver->heldItemID = 0xf;


				// If this is human and not AI
				if ((currDriver->actionsFlagSet & 0x100000) == 0)
				{
					// If this racer is in first place
					if (currDriver->driverRank == 0)
					{
						// amount of confetti particles
						gGT->confetti.numParticles_max = 250;
						gGT->confetti.unk2 = 250;

						// one person won,
						// one person gets confetti
						gGT->numWinners = 1;

						char driverID = currDriver->driverID;

						// add driver ID to array of confetti winners
						gGT->winnerIndex[0] = driverID;

						// edit window variables for confetti
						gGT->pushBuffer[driverID].fadeFromBlack_currentValue = 0x1fff;
						gGT->pushBuffer[driverID].fadeFromBlack_desiredResult = 0x1000;
						gGT->pushBuffer[driverID].fade_step = 0xff78;
					}
					if (currDriver->noItemTimer != 0)
					{
						currDriver->noItemTimer = 0;
						currDriver->heldItemID = 0xf;
					}

					// turn driver into robotcar
					BOTS_Driver_Convert(currDriver);
				}
				goto LAB_800418b4;
			}
		}

		// if player did not just finish a lap (correctly)
		else
		{
			if (
				// crossed startline backwards
				(distToFinish_curr < 1200) &&
				(distToFinish_prev > 32000)
				)
			{
				// automatic backwards penalty
				currDriver->distanceDrivenBackwards = 600;
				currDriver->actionsFlagSet |= 0x1000000;
			}

			// if player did not JUST cross finish backwards
			else
			{
				unsigned int trackLen =
					gGT->level1->ptr_restart_points[0].distToFinish;

				if (
					// if player did not EVER cross finish backwards
					((currDriver->actionsFlagSet & 0x1000000) == 0) &&

					(
						// if distance driven this frame is less than...
						(currDriver->distanceToFinish_checkpoint - distToFinish_curr) <=

						// level's distance to finish
						((trackLen >> 2) << 3)
						)
					)
				{
					// save distance for next frame
					currDriver->distanceToFinish_checkpoint = distToFinish_curr;
				}
			}

		LAB_800418b4:
			// if racer hasn't finished the race
			if ((currDriver->actionsFlagSet & 0x2000000) == 0)
			{
				// set rank to "unsorted"
				currDriver->driverRank = -1;

				// skip next 5 lines of code
				continue;
			}
		}

		// === Driver Finished Race ===

		if (currDriver->instSelf->thread->modelIndex == DYNAMIC_PLAYER)
		{
			// count humans to finish race
			iVar13 = iVar13 + 1;
		}

		// increase your rank in the race, someone passed you
		char newRank = currDriver->driverRank + 1;

		// get human in last
		if (currRank < newRank)
		{
			currRank = newRank;
		}
	}


	// sort all drivers that have NOT finished race
	for (currRank; currRank < 8; currRank++)
	{

		if (gGT->drivers[currRank] == 0)
			HANDLE_NULL_DRIVER;

		// set "min" distance to max
		minDistance = 0x3fffffff;

		// set "highest" lap to min
		finishIndex = -1;

		// lap index
		iVar9 = -10;

		// look for "next" farthest driver,
		// out of all unsorted drivers remaining
		for (driverIndex = 0; driverIndex < 8; driverIndex++)
		{
			// get current driver
			currDriver = gGT->drivers[driverIndex];

			if (currDriver == 0)
				HANDLE_NULL_DRIVER;

			if (currDriver->driverRank != -1)
				continue;

			// driver lap index
			iVar4 = currDriver->lapIndex;

			// if drive backwards behind startline
			if ((currDriver->actionsFlagSet & 0x1000000) != 0)
				iVar4 -= 1;

			if (
				// new highest lap
				(iVar4 > iVar9) ||

				// OR

				(
					// same lap
					(iVar9 == iVar4) &&

					// AND

					// new lowest distance (max progress)
					(currDriver->distanceToFinish_curr < (unsigned int)minDistance)
					)
				)
			{
				// set new min distToFinish (max progress)
				minDistance = currDriver->distanceToFinish_curr;

				// highest lap
				iVar9 = iVar4;

				// index of driver closest to finish
				finishIndex = driverIndex;
			}
		}

		if (finishIndex != -1)
		{
			// If traffic lights run out
			if (gGT->trafficLightsTimer < 1)
			{
				gGT->drivers[finishIndex]->driverRank = currRank;

			}

			// if traffic lights >= 1
			else
			{
				//reset times
				PB_lap = HOURS(1);
				Worst_lap = 0;

				// set every driver position rank,
				// to the order that they spawn on the starting line
				gGT->drivers[finishIndex]->driverRank = sdata->kartSpawnOrderArray[finishIndex];
				gGT->humanPlayerPositions[finishIndex] = sdata->kartSpawnOrderArray[finishIndex];

			}
		}
	}

	for (driverIndex = 0; driverIndex < 8; driverIndex++)
	{
		// get pointer to each player structure
		currDriver = gGT->drivers[driverIndex];

		if (currDriver == 0)
			HANDLE_NULL_DRIVER;

		// should be impossible to be -1 here
		if (currDriver->driverRank > -1)
		{
			gGT->driversInRaceOrder[currDriver->driverRank] = currDriver;
		}
	}

	for (driverIndex = 0; driverIndex < gGT->numPlyrCurrGame; driverIndex++)
	{
		// pointer to each player structure
		currDriver = gGT->drivers[driverIndex];

		if (currDriver == NULL)
			HANDLE_NULL_DRIVER;

		int currRank = currDriver->driverRank;

		if (
			(-1 < currRank) &&
			(0x4b00 < gGT->elapsedEventTime) &&
			(gGT->humanPlayerPositions[driverIndex] < currRank)
			)
		{
			int characterID =
				data.characterIDs[gGT->driversInRaceOrder[currRank - 1]->driverID];

			// Make driver talk
			Voiceline_RequestPlay(8, characterID, 0x10);
		}
		gGT->humanPlayerPositions[driverIndex] = currRank;
	}

	// If already finished race
	if ((gGT->gameMode1 & END_OF_RACE) != 0)
		return;


	int numPlyr = gGT->numPlyrCurrGame;

	// Check if race should end
	if (
		(
			// 1P game, with 1 human finished
			(numPlyr == 1) &&
			(iVar13 > 0)

			) ||

		(
			// Multiplayer VS, all finished except one
			(numPlyr > 1) &&
			((gGT->gameMode1 & ARCADE_MODE) == 0) &&
			(iVar13 >= (numPlyr - 1))
			) ||

		(
			// Arcade mode, all humans finished
			((gGT->gameMode1 & ARCADE_MODE) != 0) &&
			(numPlyr <= iVar13)
			)
		)
	{
		// End race for all drivers
		for (currRank = 0; currRank < 8; currRank++)
		{
			// Get address of each player structure
			currDriver = gGT->drivers[currRank];

			if (currDriver == NULL)
				HANDLE_NULL_DRIVER;

			// if driver already finished race
			if ((currDriver->actionsFlagSet & 0x2000000) != 0)
				continue;

			// End the race for this player
			currDriver->actionsFlagSet |= 0x2000000;

			// remove weapon
			currDriver->heldItemID = 0xf;

			// skip AIs
			if ((currDriver->actionsFlagSet & 0x100000) != 0)
				continue;

			// === VS Mode ===

			// Make the player Blasted
			VehPickState_NewState(currDriver, 2, currDriver, 0);

			// Reduce counters for AttackingPlayer and AttackedByPlayer
			currDriver->numTimesAttackedByPlayer[currDriver->driverID]--;
			currDriver->numTimesAttackingPlayer[currDriver->driverID]--;

		}

		MainGameEnd_Initialize();
	}
}