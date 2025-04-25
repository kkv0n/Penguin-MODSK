#include <common.h>

// budget: 4624
// curr: 4380

// see the bottom of this file
extern short PhysLinear_DriverOffsets[14];

void VehPhysProc_Driving_PhysLinear(struct Thread* thread, struct Driver* d)
{
	struct GameTracker* gGT;
	int gameMode2;

	char kartState;
	char heldItemID;
	short noItemTimer;
	char isNumWumpas10;
	u_short driverTimerNegativeFinal;
	u_short driverBaseSpeedUshort;
	int approxTrig;
	int driverBaseSpeed;
	short approximateSpeed;
	short sVar13;
	int iVar14;
	u_int buttonsTapped;
	u_short driverTimerNegativePrelim;
	short driverTimer;
	short timerHazard;
	short approximateSpeed2;
	u_int actionsFlagSetCopy;
	short driverSpeedSmth2;
	struct GamepadBuffer* ptrgamepad;
	u_int cross;
	u_int square;

	short* normSrc;
	short* normDst;
	short* val;
	int i;
	int msPerFrame;
	short driverRankItemValue;
	u_int itemSound;
	u_int uVar20;
	int stickLY;
	int stickRY;
	int driverSpeedOrSmth = 0;
	struct Thread* driverItemThread;
	struct Shield* shield;
	struct TrackerWeapon* bomb;
	u_int superEngineFireLevel;
	int unk0x80;
	short driverSpeedCopy;

	gGT = sdata->gGT;
	gameMode2 = gGT->gameMode2;

	// If race timer is not supposed to stop for this racer
	if ((d->actionsFlagSet & 0x40000) == 0)
	{
		// set racer's timer to the time on the clock
		d->timeElapsedInRace = gGT->elapsedEventTime;
	}


	// === Count Timers ===


	// elapsed milliseconds per frame, ~32
	msPerFrame = gGT->elapsedTimeMS;

	if
	(
		// time on the clock
		(gGT->elapsedEventTime < 10 * MINUTE) &&

		// race timer is not frozen for this player
		((d->actionsFlagSet & 0x40000) == 0)
	)
	{
		// increment timer by (speed * time)
		d->distanceDriven += (d->speedApprox * msPerFrame) >> 8;
	}

	for(i = 0; i < 14; i++)
	{
		short* val = (short*)((int)d + (int)PhysLinear_DriverOffsets[i]);

		if(*val > 0)
		{
			*val -= msPerFrame;
			if(*val < 0) *val = 0;
		}

	}

	if(d->reserves > 0) d->timeSpentUsingReserves += msPerFrame;
	if(d->set_0xF0_OnWallRub > 0) d->timeSpentAgainstWall += msPerFrame;
	if(d->burnTimer > 0) d->timeSpentBurnt += msPerFrame;
	if(d->squishTimer > 0) d->timeSpentSquished += msPerFrame;

	// If Super Engine Cheat is not enabled
	if (!(gameMode2 & CHEAT_ENGINE))
	{
		d->superEngineTimer -= msPerFrame;
		if(d->superEngineTimer < 0) d->superEngineTimer = 0;
	}

	// If invisible, without Permanent Invisibility cheat,
	// dont remove invisibleTimer check, or an invalid
	// instFlagsBackup overwrites instFlags
	if
	(
		(d->invisibleTimer != 0) &&
		((gameMode2 & CHEAT_INVISIBLE) == 0)
	)
	{
		d->invisibleTimer -= msPerFrame;

		// if newly visible
		if(d->invisibleTimer <= 0)
		{
			d->invisibleTimer = 0;
			d->instSelf->flags = d->instFlagsBackup;
			d->instSelf->alphaScale = 0;
			 OtherFX_Play(0x62, 1);
		}
	}

	if (0 < d->jump_TenBuffer) d->jump_TenBuffer--;
	if (9 < d->numWumpas) d->timeSpentInTenWumpa += msPerFrame;
	if (d->currentTerrain == 0xE) d->timeSpentInMud += msPerFrame;


	// === Check Last Place ===


	driverRankItemValue = d->driverRank;

	// Basically, if racer is in last place in any possible race scenario
	if
	(
		(
			(
				(
					// If racer is in 8th place
					(driverRankItemValue == 7) &&

					// If numPlyrCurrGame is 1
					(gGT->numPlyrCurrGame == 1)
				) ||
				(
					(
						// If racer is in 6th place
						driverRankItemValue == 5 &&

						// if numPlyrCurrGame is 2
						(gGT->numPlyrCurrGame == 2)
					)
				)
			) ||
			(
				(
					// if racer is in 4th place
					driverRankItemValue == 3 &&

					// if numPlyrCurrGame is more than 2
					(2 < (u_char)gGT->numPlyrCurrGame)
				)
			)
		) &&
		(
			// race timer is not frozen for this player
			(d->actionsFlagSet & 0x40000) == 0
		)
	)
	{
		// Increase the time racer has been in last place by elapsed milliseconds
		d->timeSpentInLastPlace += msPerFrame;
	}


	// === Determine Hazard ===


	driverRankItemValue = 4;

	// if you have a raincloud over your head from potion
	if (d->thCloud != 0)
		driverRankItemValue =
			((struct RainCloud*)d->thCloud->object)->boolScrollItem;

	// get approximate speed
	approximateSpeed = d->speedApprox;

	// Action flags (isRaceOver, isTimeFrozen, etc)
	actionsFlagSetCopy = d->actionsFlagSet;

	// d->clockReceive
	driverTimer = d->clockReceive;

	d->driverRankItemValue = driverRankItemValue;

	// absolute value of speed
	if (approximateSpeed < 0) approximateSpeed = -approximateSpeed;

	if
	(
		(
			// if you are not impacted by clock weapon
			(driverTimer == 0) &&
			(
				// get squished timer
				driverTimer = d->squishTimer,

				// if you are not squished
				driverTimer == 0
			)
		) &&
		(
			(
				driverRankItemValue != 0 ||
				(
					// if time on the clock is zero
					driverTimer = gGT->elapsedEventTime,
					driverTimer == 0
				)
			)
		)
	)
	{
		timerHazard = d->hazardTimer;

		// hazard timer will not go down unless you keep moving
		// is this for the raincloud from red beakers?

		// if you have high speed
		if (approximateSpeed > 0x100)
		{
			// decrease hazard by elapsed time
			timerHazard -= msPerFrame;
		}

		timerHazard = timerHazard & 0xfffe;
		if (timerHazard > -1) timerHazard = -2;

		d->hazardTimer = timerHazard;
	}

	// if you are not impacted by hazard (other than clock)
	else
	{
		driverTimerNegativePrelim = driverTimer;

		// if you are not touching the ground
		if ((actionsFlagSetCopy & 1) == 0)
		{
			// if speed is low
			if (approximateSpeed < 0x101) goto speedIsLow;

			// if speed is high...

			// absolute value of clock hazard
			if (driverTimer < 0)
			{
				driverTimerNegativePrelim = -driverTimerNegativePrelim;
			}

			turndriverTimerNegative:
			driverTimerNegativeFinal = -driverTimerNegativePrelim | 1;
		}

		// if you are touching the ground
		else
		{
			// if speed is low
			if (approximateSpeed < 0x101)
			{
				speedIsLow:
				driverTimerNegativePrelim = *(u_short*)&d->hazardTimer;
				driverTimerNegativeFinal = driverTimerNegativePrelim | 1;
				if ((short)driverTimerNegativePrelim > 0) goto turndriverTimerNegative;
			}

			// if speed is high
			else
			{
				// absolute value of clock hazard
				if (driverTimer < 0)
				{
					driverTimerNegativePrelim = -driverTimerNegativePrelim;
				}

				// Use trigonometry with speed and
				// clock timer to make the car waddle

				driverTimer = d->clockReceive >> 6;
				if (driverTimer > 0x40) driverTimer = 0x40;

				timerHazard = d->clockReceive << 4;

				// approximate trigonometry
				approxTrig = *(int*)&data.trigApprox[(timerHazard & 0x3ff)];

				if ((timerHazard & 0x400) == 0) approxTrig = approxTrig << 0x10;
				approxTrig = approxTrig >> 0x10;

				if ((timerHazard & 0x800) != 0) approxTrig = -approxTrig;
				approximateSpeed2 = approximateSpeed >> 8;

				if (approximateSpeed2 > 0x20) approximateSpeed2 = 0x20;

				// gamepad vibration
				GAMEPAD_ShockForce1(d, 4, driverTimer + (approxTrig >> 5) + approximateSpeed2 + 0x18);


				driverTimerNegativeFinal = driverTimerNegativePrelim | 1;
			}
		}
		d->hazardTimer = driverTimerNegativeFinal;
	}


	// === Item Roll ===


	// if Held Item = None (rolling)
	if (d->heldItemID == 0x10)
	{

		//if Item roll is done
		if (d->itemRollTimer == 0)
		{
			// Select a random weapon for driver
			 VehPhysGeneral_SetHeldItem(d);

			// if 9 < number of wumpa
			// if wumpa is 10
			isNumWumpas10 = d->numWumpas > 9;

			// if you have less than 10 wumpa

			// "ding" sound
			itemSound = 0x5e;

			// if you have 10 wumpa
			if (isNumWumpas10)
			{
				// "ka-ching" sound
				itemSound = 0x41;
			}

			// sound of getting weapon
			 OtherFX_Play(itemSound, isNumWumpas10);
		}

		//if Item roll is not done
		else d->itemRollTimer--;
	}


	// === Item Used By Player ===


	// Make Item fade away from icon
	if (d->noItemTimer > 0) d->noItemTimer--;

	// if Item is about to be gone and Number of Items = 0
	if ((d->noItemTimer == 1) && (d->numHeldItems == 0))
	{
		if
		(
			// multiplayer game, not battle, weapon was 3 missiles
			(2 < (u_char)gGT->numPlyrCurrGame) &&
			((gGT->gameMode1 & BATTLE_MODE) == 0) &&
			(d->heldItemID == 0xB) &&
			(gGT->numPlayersWith3Missiles > 0)
		)
		{
			// keep count
			gGT->numPlayersWith3Missiles--;
		}

		// take away weapon
		*(u_char*)&d->heldItemID = 0xf;
	}


	// === Normal Vector ===


	// action flags
	d->actionsFlagSetPrevFrame = actionsFlagSetCopy;

	// backup rotation
	*(u_int*)&d->rotPrev.x = *(u_int*)&d->rotCurr.x;
	d->rotPrev.z = d->rotCurr.z;

	// backup position
	d->posPrev.x = d->posCurr.x;
	d->posPrev.y = d->posCurr.y;
	d->posPrev.z = d->posCurr.z;

	// unknown
	d->jumpHeightPrev = d->jumpHeightCurr;
	d->turnAnglePrev = d->turnAngleCurr;

	// ??? --Super
	uVar20 = actionsFlagSetCopy & 0x7f1f83d5;

	// disable input if opening adv hub door with key
	if ((gameMode2 & 0x4004) != 0)
	{
		d->actionsFlagSet = uVar20;
		return;
	}

	// destination
	normDst = &d->AxisAngle4_normalVec[0];
	if(d->normalVecID == -1) normDst = &d->AxisAngle3_normalVec[0];
	d->normalVecID = 0;

	// source
	normSrc = &d->AxisAngle2_normalVec[0];
	if ((actionsFlagSetCopy & 1) != 0) normSrc = &d->AxisAngle1_normalVec.x;

	// copy
	*(u_int*)&normDst[0] = *(u_int*)&normSrc[0];
	normDst[2] = normSrc[2];



	// === Check Mask Weapon ===


	actionsFlagSetCopy = uVar20;
	driverItemThread = thread->childThread;
	while (driverItemThread != 0)
	{
		// If thread->modelIndex is Aku or Uka
		if
		(
			(*(short*)&driverItemThread->modelIndex == 0x3a) ||
			(*(short*)&driverItemThread->modelIndex == 0x39)
		)
		{
			// driver is using mask weapon
			actionsFlagSetCopy = uVar20 | 0x800000;
			break;
		}

		// check next player in linked list
		driverItemThread = driverItemThread->siblingThread;
	}


	// === Check Buttons ===


	// pointer to gamepad input of current player (d)
	ptrgamepad = &sdata->gGamepads->gamepad[(u_int)d->driverID];

	// no hold, no tap
	uVar20 = 0;
	buttonsTapped = 0;

	// If you're not in End-Of-Race menu
	if ((gGT->gameMode1 & END_OF_RACE) == 0)
	{
		uVar20 = ptrgamepad->buttonsHeldCurrFrame;
		buttonsTapped = ptrgamepad->buttonsTapped;
	}

	cross = uVar20 & BTN_CROSS;
	square = uVar20 & BTN_SQUARE;

	// state of kart
	kartState = d->kartState;


	// === Check Weapons ===


	if
	(
		(
			((buttonsTapped & BTN_CIRCLE) != 0) &&

			(
				(kartState == KS_NORMAL) ||
				(kartState == KS_DRIFTING) ||
				(kartState == KS_ANTIVSHIFT)
			)
		) &&

		// if there is no tnt on your head
		(d->instTntRecv == 0)
	)
	{

		// If there is a Bomb Pointer
		if (d->instBombThrow != 0)
		{
			// Detonate the bomb
			bomb = (struct TrackerWeapon*)d->instBombThrow->thread->object;
			bomb->flags |= 2;
			d->instBombThrow = NULL;

			goto CheckJumpButtons;
		}

		// If there is a Bubble Pointer
		if (d->instBubbleHold != 0)
		{
			// Shoot the bubble
			shield = (struct Shield*)d->instBubbleHold->thread->object;
			shield->flags |= 2;
			d->instBubbleHold = NULL;

			goto CheckJumpButtons;
		}

		// item is rolling
		if (d->itemRollTimer != 0)
		{
			// circle button ends timer, if
			// less than 70 frames (2.3s) remain
			if (d->itemRollTimer < FPS_DOUBLE(70))
				d->itemRollTimer = 0;

			// skip weapon firing check
			goto CheckJumpButtons;
		}

		// === Item Roll finished before PhysLinear ===

		// If you dont have "roulette" weapon (0x10), and if you dont have "no weapon" (0xf)
		// and if you did not have a weapon last frame (0x3c->0),
		// and if (unknown driverRankItemValue related to 0x4a0),
		// and if you are not being effected by Clock Weapon
		heldItemID = d->heldItemID;
		if
		(
			(heldItemID != 0xF) &&
			(heldItemID != 0x10) &&
			(d->noItemTimer == 0) &&
			(driverRankItemValue != 1) &&
			(d->clockReceive == 0)
		)
		{
			// This driver wants to fire a weapon
			actionsFlagSetCopy |= 0x8000;

			// if numHeldItems == 0
			// wait a full second before next weapon
			d->noItemTimer = FPS_DOUBLE(0x1e);

			// If "held item quantity" is zero
			if (d->numHeldItems != 0)
			{
				// if numHeldItems > 0,
				// wait 5 frames before next weapon use
				d->noItemTimer = FPS_DOUBLE(5);

				// not spring weapon
				if (heldItemID != 5)
				{
					// only reduce numHeldItem if not using item cheats
					if ((gameMode2 & (CHEAT_BOMBS | CHEAT_TURBO | CHEAT_MASK)) == 0) d->numHeldItems--;
				}

				// no spring in final game
				#if 0

				// If you have the Spring weapon
				else
				{
					if (
						(d->jump_CoyoteTimerMS != 0) &&
						(d->jump_CooldownMS == 0)
					   )
					{
						d->numHeldItems--;
					}
				}

				#endif
			}
		}
	}


	// === Drift Section ===


CheckJumpButtons:

	// Check for Tapping L1 and R1
	buttonsTapped = buttonsTapped & 0xc00;

	if
	(
		//if you're not pressing L1 or R1
		(buttonsTapped == 0) ||
		// or you are sliding
		(d->kartState == 2)
	)
	{
		if
		(
			// If you are holding L1 or R1 and
			((uVar20 & 0xc00) != 0) &&
			(driverRankItemValue != 3)
		)
		{
			if ((actionsFlagSetCopy & 4) == 0)
			{
				// 10 frame jump buffer
				d->jump_TenBuffer = FPS_DOUBLE(10);
			}
			goto LAB_8006222c;
		}
		actionsFlagSetCopy &= 0xfffffffb;
		if (d->jump_TenBuffer > 0) d->jump_TenBuffer = 0;
	}

	//if you're pressing jump buttons and not sliding
	else
	{
		//if L1 and R1 were being tapped at once
		if (buttonsTapped == 0xc00)
		{
			//set Last Jump button pressed to R1
			d->buttonUsedToStartDrift = 0x400;
		}

		//if you're not tapping L1 and R1 at once
		else
		{
			//Last Jump button pressed = buttonsTapped
			d->buttonUsedToStartDrift = buttonsTapped;
		}

		if (driverRankItemValue != 3)
		{
			d->jump_TenBuffer = FPS_DOUBLE(10);
			LAB_8006222c:
			actionsFlagSetCopy |= 4;
		}
	}


	// Assume you're holding cross (X)
	u_char assumeCross = 0x10;
	// if you are holding square
	if (square != 0)
	{

		// held DOWN or have landing boost
		if ((ptrgamepad->buttonsHeldCurrFrame & BTN_DOWN) ||
			(d->jump_LandingBoost))
			{
				// if not holding cross (X)
				if (cross == 0)
				{
					assumeCross = 0;
				}
				goto SKIP_RESERVE_RESET;
			}
	
		// you're on a turbo pad
		if (d->stepFlagSet & 0x3)
			goto SKIP_RESERVE_RESET;

		// Set Reserves to zero
		d->reserves = 0;
	}
	SKIP_RESERVE_RESET:


	// assume normal gas pedal
	stickRY = 0x80;

	// If you're not in End-Of-Race menu
	if ((gGT->gameMode1 & END_OF_RACE) == 0)
	{
		// gamepadBuffer -> stickRY (for gas or reverse)
		stickRY = (int)ptrgamepad->stickRY;
	}

	if
	(
		// If Reserves are not zero
		(d->reserves != 0) ||

		(driverRankItemValue == 6)
	)
	{
		// If you are not holding Cross
		if(cross == 0)
		{
			unk0x80 =  VehPhysJoystick_ReturnToRest(stickRY, 0x80, 0);

			if(unk0x80 > -1)
			{
				actionsFlagSetCopy |= 0x400000;
			}
		}

		// If holding Square while moving fast
		if((square != 0) && (approximateSpeed > 0x300))
		{
			// back wheel skids
			actionsFlagSetCopy |= 0x800;
		}

		// if you're on any turbo pad
		if ((d->stepFlagSet & 3) != 0)
		{
			// assume not holding square until boost is over
			square = 0;
		}


		cross = assumeCross;

	}


	// === Gas/Brake section ===


	stickLY = 0x80;

	// If you're not in End-Of-Race menu
	if ((gGT->gameMode1 & END_OF_RACE) == 0)
	{
		stickLY = ptrgamepad->stickLY;
	}

	if
	(
		(d->simpTurnState < 0) ||
		(actionsFlagSetCopy &= 0xdfffffff, d->simpTurnState < 1)
	)
	{
		actionsFlagSetCopy &= 0xbfffffff;
	}
	approximateSpeed2 = d->speedApprox;
	if (approximateSpeed2 < 0)
	{
		approximateSpeed2 = -approximateSpeed2;
	}
	if (approximateSpeed2 < 0x300)
	{
		actionsFlagSetCopy &= 0x9fffffff;
	}
	approximateSpeed2 = 0;

	// with zero wumpa, should be const_Speed_ClassStat (13140 for Crash Bandicoot)
	// this works for both decomp and original

	// with one wumpa, (25600 in rewrite, 13169 in original)


	// original, for decomp
	driverBaseSpeed = VehPhysGeneral_GetBaseSpeed(d);


	driverBaseSpeedUshort = driverBaseSpeed;

	// If you are not holding Square
	if (square == 0)
	{
		//driverSpeedSmth2 = Racer's Base Speed
		driverSpeedSmth2 = driverBaseSpeed;

		// If you are holding Cross, or if you have Reserves
		if (cross != 0)
		{
			LAB_8006253c:
			actionsFlagSetCopy &= 0xfffdffff;
			goto LAB_80062548;
		}

		// if you are not holding cross, or have no Reserves...
		// driverSpeedSmth2 is replaced

		driverSpeedSmth2 =  VehPhysJoystick_ReturnToRest(stickRY, 0x80, 0);

		driverSpeedOrSmth = -driverSpeedSmth2;
		if (driverSpeedSmth2 < 1)
		{
			if
			(
				(driverSpeedOrSmth == 0) &&
				(
					(
						unk0x80 =  VehPhysJoystick_ReturnToRest(stickLY, 0x80, 0),

						(unk0x80 > 99) ||

						(
							(unk0x80 > 0) &&
							((actionsFlagSetCopy & 0x20000) != 0)
						)
					)
				)
			)
			{
				// driver is steering?
				actionsFlagSetCopy |= 0x20000;

				driverSpeedSmth2 = -d->const_BackwardSpeed;
				goto LAB_80062548;
			}

			driverSpeedOrSmth = driverBaseSpeed * driverSpeedOrSmth;
			driverSpeedSmth2 = driverSpeedOrSmth >> 7;
			if (driverSpeedOrSmth < 0) driverSpeedSmth2 = (driverSpeedOrSmth + 0x7f) >> 7;

			// remove flag for reversing
			goto LAB_8006253c;
		}
		if ((d->speedApprox < 0x301) && ((actionsFlagSetCopy & 0x60000000) == 0))
		{
			driverSpeedOrSmth = d->const_BackwardSpeed * driverSpeedOrSmth;
			if (driverSpeedOrSmth < 0) driverSpeedOrSmth = driverSpeedOrSmth + 0x7f;
			approximateSpeed2 = driverSpeedOrSmth >> 7;
			buttonsTapped = 0x20000;
			LAB_800625c4:
			uVar20 = actionsFlagSetCopy | buttonsTapped;
		}
		else
		{
			uVar20 = actionsFlagSetCopy | 8;
			if (0 < d->simpTurnState) uVar20 = actionsFlagSetCopy | 0x40000008;
			if (d->simpTurnState < 0)
			{
				buttonsTapped = 0x20000000;
				actionsFlagSetCopy = uVar20;
				goto LAB_800625c4;
			}
		}
	}
	// If you are holding Square
	else
	{
		unk0x80 =  VehPhysJoystick_ReturnToRest(stickLY, 0x80, 0);

		if ((unk0x80 < 100) && ((unk0x80 < 1 || ((actionsFlagSetCopy & 0x20000) == 0))))
		{
			// if you are not holding cross, and you have no Reserves
			if (cross == 0)
			{
				driverSpeedOrSmth =  VehPhysJoystick_ReturnToRest(stickRY, 0x80, 0);

				if (driverSpeedOrSmth < 0)
				{
					driverSpeedOrSmth = driverBaseSpeed * -driverSpeedOrSmth;
					if (driverSpeedOrSmth < 0) driverSpeedOrSmth += 0xff;
					driverSpeedSmth2 = driverSpeedOrSmth >> 8;

					// gas and brake together
					actionsFlagSetCopy |= 0x20;

					goto LAB_80062548;
				}

				if (0 < driverSpeedOrSmth)
				{
					driverSpeedOrSmth = d->const_BackwardSpeed * -driverSpeedOrSmth;
					if (driverSpeedOrSmth < 0) driverSpeedOrSmth += 0xff;
					driverSpeedSmth2 = driverSpeedOrSmth >> 8;

					// reversing, and gas+brake
					goto LAB_8006248c;
				}

				// driverSpeedOrSmth == 0,
				// no gas, only brake

				// using the brake
				actionsFlagSetCopy |= 8;

				driverSpeedSmth2 = approximateSpeed2;
			}
			// If you are holding cross, or you have Reserves
			else
			{
				// gas and brake together
				actionsFlagSetCopy |= 0x20;

				driverSpeedSmth2 = driverBaseSpeed / 2;
			}
			goto LAB_8006253c;
		}
		driverSpeedOrSmth = d->const_BackwardSpeed * -3;
		driverSpeedSmth2 = driverSpeedOrSmth >> 2;
		if (driverSpeedOrSmth < 0) 	driverSpeedSmth2 = (driverSpeedOrSmth + 3) >> 2;

		LAB_8006248c:
		// reversing engine, and brakes
		actionsFlagSetCopy |= 0x20020;

		LAB_80062548:
		uVar20 = actionsFlagSetCopy & 0x9fffffff;
		approximateSpeed2 = driverSpeedSmth2;
	}

	// driving backwards
	if ((uVar20 & 0x20000) != 0)
	{
		d->timeSpentReversing += gGT->elapsedTimeMS;
	}

	// not driving backwards
	else
	{
		if (d->superEngineTimer != 0)
		{
			// if Racer is moving
			if (0 < approximateSpeed2)
			{
				// not holding brake
				if ((uVar20 & 0x400020) == 0)
				{
					d->actionsFlagSet = uVar20;

					// fire level, depending on numWumpa
					superEngineFireLevel = 0x80;
					if (d->numWumpas > 9)
						superEngineFireLevel = 0x100;

					// add 0.12s reserves
					 VehFire_Increment(d, 120, (TURBO_PAD | SUPER_ENGINE), superEngineFireLevel);

					uVar20 = d->actionsFlagSet;
				}
			}
		}
	}

	// if accel prevention (hold square)
	actionsFlagSetCopy = uVar20 & 8;
	if (actionsFlagSetCopy != 0)
	{
		// high speed
		if (
				(d->speedApprox > 0x300) ||
				(d->speedApprox < -0x300)
			)
		{
			// record amount of time with high speed
			d->timeSpentWithHighSpeed += msPerFrame;
		}
	}

	if
	(
		(d->mashingXMakesItBig == 0) ||

		(
			(d->kartState != KS_NORMAL) &&
			(d->kartState != KS_ANTIVSHIFT)
		)
	)
	{
		d->mashXUnknown = 0;
	}

	if (d->fireSpeed < 1)
	{
		//if Racer is moving, skip next 4 lines of code
		if (approximateSpeed2 > 0) goto LAB_800626d4;
		LAB_800626fc:
		//Racer struct + 0x39E = Racer's Base Speed
		*(u_short*)&d->fireSpeed = approximateSpeed2;
	}
	else
	{
		//if Racer is not moving
		if (approximateSpeed2 < 1)
		{
			LAB_800626d4:
			if (d->mashingXMakesItBig != 0) d->mashXUnknown++;
			d->mashingXMakesItBig = 0x100;
			goto LAB_800626fc;
		}
		//Racer struct + 0x39E = Racer's Base Speed
		*(u_short*)&d->fireSpeed = approximateSpeed2;
	}

	// brakes
	if ((uVar20 & 0x800020) == 0)
	{

		driverSpeedOrSmth = d->terrainMeta2->unk_0x8;

		if (driverSpeedOrSmth != 0x100)
		{
			//Base Speed = 0xB4 (at Cove water) * Base Speed >> 8
			approximateSpeed2 = 	(driverSpeedOrSmth * approximateSpeed2) >> 8;
			driverBaseSpeedUshort = (driverSpeedOrSmth * driverBaseSpeed) >> 8;
		}
	}
	*(u_short*)&d->unknowndriverBaseSpeed = driverBaseSpeedUshort;
	*(u_short*)&d->baseSpeed = approximateSpeed2;


	// === Steering Section ===


	// assume neutral steer (drive straight)
	driverSpeedOrSmth = 0x80;

	// If you're not in End-Of-Race menu
	if ((gGT->gameMode1 & END_OF_RACE) == 0)
	{
		// gamepadBuffer -> stickLX
		driverSpeedOrSmth = (int)ptrgamepad->stickLX;
	}

	// default steer strength from class stats
	iVar14 = d->const_TurnRate + (d->turnConst << 1) / 5;

	// if mashing X button
	if ((d->mashXUnknown > 6) && (approximateSpeed < 0x2600))
	{
		// sharp turn
		iVar14 = 0x5a;
		goto UseTurnRate;
	}

	// rubbing on wall now, or recently
	if (d->set_0xF0_OnWallRub != 0)
	{
		// restrict turn
		iVar14 = 0x30;
		goto UseTurnRate;
	}

	// === not rubbing on wall now, or recently ===

	// if not holding Square (& 0x8)
	// or not using brakes (& 0x20)
	if ((uVar20 & 0x28) == 0)
	{
		// use const_TurnRate + turnConst<<1/5
		goto UseTurnRate;
	}

	// if only holding Square
	if (cross == 0)
	{
		// turn rate
		iVar14 = 0x40;
		goto UseTurnRate;
	}

	// === if holding Square + Cross ===

	// absolute value driver speed
	driverSpeedCopy = d->speed;
	if (driverSpeedCopy < 0) driverSpeedCopy = -driverSpeedCopy;

	// As speed increases, turn rate decreases
	iVar14 =  VehCalc_MapToRange
	(
		driverSpeedCopy,
		0x300,	d->const_Speed_ClassStat / 2,
		0x40,	iVar14
	);

UseTurnRate:

	// Steer, based on strength, and LeftStickX
	iVar14 =  VehPhysJoystick_GetStrengthAbsolute(driverSpeedOrSmth, iVar14, ptrgamepad->rwd);

	// no desired steer
	if (-iVar14 == 0)
	{
		d->numFramesSpentSteering = 10000;
	}

	// desired steer
	else
	{
		// desired steer left, or active steer left
		if ((iVar14 < 1) || (d->simpTurnState < 0))
		{
			// desired steer right, or active steer right
			if ((-1 < iVar14) || (0 < d->simpTurnState))
			{
				// active steer has not changed
				goto SkipSetSteer;
			}

			// active steer left
			uVar20 = uVar20 | 0x10;
		}

		else
		{
			// active steer right
			uVar20 = uVar20 & 0xffffffef;
		}
		d->numFramesSpentSteering = 0;
	}

SkipSetSteer:

	*(u_char*)&d->simpTurnState = (char)-iVar14;

	// Change wheel rotation based on StickLX
	driverSpeedOrSmth =  VehPhysJoystick_GetStrengthAbsolute(driverSpeedOrSmth, 0x40, ptrgamepad->rwd);
	driverBaseSpeedUshort =  VehCalc_InterpBySpeed((int)d->wheelRotation, FPS_HALF(0x18), -driverSpeedOrSmth);
	*(u_short*)&d->wheelRotation = driverBaseSpeedUshort;

	driverSpeedOrSmth = (int)d->fireSpeed;
	if (driverSpeedOrSmth < 0) driverSpeedOrSmth = -driverSpeedOrSmth;

	if (((d->actionsFlagSetPrevFrame & 1) == 0) || (kartState == KS_DRIFTING))
	{
		driverSpeedOrSmth = driverSpeedOrSmth + 0xf00;
	}
	else
	{
		driverSpeedOrSmth = driverSpeedOrSmth + approximateSpeed >> 1;
	}

	sVar13 = ((driverSpeedOrSmth * 0x89 + d->unkSpeedValue2 * 0x177) * 8) >> 0xc;
	d->unkSpeedValue2 = sVar13;

	if ((d->actionsFlagSetPrevFrame & 8) == 0)
	{
		//prevent Basic Speed from being negative
		if (approximateSpeed2 < 0) approximateSpeed2 = -approximateSpeed2;

		//if Base Speed > 0x200 or approximateSpeed = 0x200 (?)
		if ((approximateSpeed2 > 0x200) || (approximateSpeed > 0x200))
		{
			d->unkSpeedValue1 -= sVar13;
		}
	}

	// alternate tire colors each frame,
	// if 2e808080 is detected (&1==0),
	// if not RevEngine, and if unkSpeedVal
	if
	(
		(d->unkSpeedValue1 < 1) &&
		((d->tireColor & 1) == 0) &&
		(kartState != KS_ENGINE_REVVING)
	)
	{
		//reset 0x3BC
		d->unkSpeedValue1 = 0x1e00;

		d->tireColor = 0x2e606061;
	}

	// default tire color
	else
	{
		d->tireColor = 0x2e808080;
	}

	d->actionsFlagSet = uVar20;
	return;
}

#define DriverPhysLinear_OFFSETOF(TYPE, ELEMENT) (short)((unsigned int)&(((TYPE *)0)->ELEMENT))
short PhysLinear_DriverOffsets[14] =
{
	DriverPhysLinear_OFFSETOF(struct Driver, reserves),
	DriverPhysLinear_OFFSETOF(struct Driver, set_0xF0_OnWallRub),
	DriverPhysLinear_OFFSETOF(struct Driver, burnTimer),
	DriverPhysLinear_OFFSETOF(struct Driver, squishTimer),
	DriverPhysLinear_OFFSETOF(struct Driver, turbo_outsideTimer),
	DriverPhysLinear_OFFSETOF(struct Driver, VehFire_AudioCooldown),
	DriverPhysLinear_OFFSETOF(struct Driver, jump_ForcedMS),
	DriverPhysLinear_OFFSETOF(struct Driver, jump_CooldownMS),
	DriverPhysLinear_OFFSETOF(struct Driver, jump_unknown),
	DriverPhysLinear_OFFSETOF(struct Driver, StartDriving_0x60),
	DriverPhysLinear_OFFSETOF(struct Driver, StartRollback_0x280),
	DriverPhysLinear_OFFSETOF(struct Driver, clockReceive),
	DriverPhysLinear_OFFSETOF(struct Driver, mashingXMakesItBig),
	DriverPhysLinear_OFFSETOF(struct Driver, invincibleTimer)
};