#include <common.h>
#include "utils.h"

// param1 - driver
// param2 - reserves to add
// param3 - add type
// param4 - fire level
void  VehFire_Increment(struct Driver* d, int reserves, u_int type, int fireLevel)
{
	char kartState;
	u_char count;

	int newFireSpeedCap;
	int newFireSize;
	int oldOTT;

	u_int addFlags;
	struct Turbo* turboObj;
	struct Thread* turboThread;
	struct Instance* turboInst1;
	struct Instance* turboInst2;

	struct GameTracker* gGT = sdata->gGT;
	if
	(
		// if this is a turbo pad
		((type & 4) != 0) &&

		// racer is in accel prevention (holding square)
		((d->actionsFlagSet & 8) != 0)
	)
	{
		// do nothing
		return;
	}


	kartState = d->kartState;

	if (kartState == KS_SPINNING) return;
	if (kartState == KS_MASK_GRABBED) return;
	if (kartState == KS_BLASTED) return;

	//turn off 8th flag, turn on 22nd flag of actions flag set
	//means ? (!(8)) and racer just got an outside turbo (22)
	d->actionsFlagSet = d->actionsFlagSet & 0xffffff7f | 0x200000;

	// turbo thread bucket
	turboThread = gGT->threadBuckets[TURBO].thread;

	// check all turbo threads
	while(turboThread != 0)
	{
		// if this turbo thread is owned by this driver
		if (((struct Turbo*)turboThread->object)->driver == d)
		{
			// quit, turboThread is now this driver's turbo thread
			break;
		}

		// next turbo thread in bucket
		turboThread = turboThread->siblingThread;
	}

	// if no turbo exists, create one
	if(turboThread == 0)
	{
		#if BUILD < JpnRetail

		d->numTurbos = 1;

		#else

		// some sort of variable related to drifting? check + 0x3be in japanese output
		if (d->japanTurboUnknown == 0)
		{
			d->numTurbos = 1;
			if ((d->numTurbosHighScore < 1) && ((gGT->gameMode1 & END_OF_RACE) == 0))
			{
				d->numTurbosHighScore = 1;
			}
		}
		else
		{
			d->numTurbos++;
			if ((d->numTurbosHighScore < d->numTurbos) && ((gGT->gameMode1 & END_OF_RACE) == 0))
			{
				d->numTurbosHighScore = d->numTurbos;
			}
		}

		#endif


		turboInst1 = INSTANCE_BirthWithThread(
			0x2c, 0, SMALL, TURBO,
			 VehTurbo_ThTick,
			sizeof(struct Turbo), 0
		);

		turboObj = 0;

		if (turboInst1 != 0)
		{
			// get thread, ignore all collisions
			turboThread = turboInst1->thread;
			turboThread->flags |= 0x1000;
			turboThread->funcThDestroy =  VehTurbo_ThDestroy;

			// turbo #2
			turboInst2 = INSTANCE_Birth3D(
				gGT->modelPtr[0x2C], 	// model
				&sdata->s_turbo2[0], 		// name
				turboThread					// parent thread
			);

			// get object, set essentials
			turboObj = turboThread->object;
			turboObj->inst = turboInst2;
			turboObj->driver = d;
			turboObj->fireAnimIndex = 0;
			turboObj->fireVisibilityCooldown = 0x60;

			// make flame disappear after
			// 	- powerslide: two frames (quick death)
			//	- all others: -1 frames (255 = 'no' death)
			if (type & 2)	count = FPS_DOUBLE(2);
			else			count = -1;
			turboObj->fireDisappearCountdown = count;

			// if modelIndex == "player" of any kind
			if (d->instSelf->thread->modelIndex == 0x18)
			{
				turboObj->fireAudioDistort = 0;

				if(d->kartState != KS_CRASHING)
				{
					 VehFire_Audio(d, fireLevel);
				}
			}

			// 2P 3P 4P flags
			addFlags = 0;

			// 1P flags
			if (gGT->numPlyrCurrGame == 1)
			{
				addFlags = 0x2000000;
			}

			// make turbos invisible, and transparent.
			// why does numPlyrCurrGame alter pause-menu invisible toggle?
			turboInst1->flags = turboInst1->flags | addFlags | 0x1040080;
			turboInst2->flags = turboInst2->flags | addFlags | 0x1040080;
		}

	}

	// if turbo exists, modify it
	else
	{
		// get the turbo's object
		turboObj = turboThread->object;

		// get the turbo's instances
		turboInst1 = turboThread->inst;
		turboInst2 = turboObj->inst;

		// remove "dead thread" flag
		turboThread->flags &= 0xfffff7ff;

		// turbo pad
		if ((type & 4) != 0)
		{
			// only increase counter on the first frame of turbo pad

			if ((d->actionsFlagSetPrevFrame & 0x200000) == 0)
			{
				d->numTurbos++;

				#if BUILD == JpnRetail
				// the japanese version of the game keeps track of your highest turbo chain in a race
				if (d->numTurbosHighScore < d->numTurbos && (gGT->gameMode1 & END_OF_RACE) == 0) d->numTurbosHighScore = d->numTurbos;
				#endif
			}
		}

		// all other boosts
		else
		{
			// make fire invisible for the sake of the visibility cooldown as explained in common.h
			turboInst1->flags |= 0x1000080;
			turboInst2->flags |= 0x1000080;

			turboObj->fireVisibilityCooldown = 0x60;
			d->numTurbos++;
			#if BUILD == JpnRetail
			// the japanese version of the game keeps track of your highest turbo chain in a race
			if (d->numTurbosHighScore < d->numTurbos && (gGT->gameMode1 & END_OF_RACE) == 0) d->numTurbosHighScore = d->numTurbos;
			#endif
		}

		turboObj->fireDisappearCountdown = -1;
		turboInst1->alphaScale = 0;
		turboInst2->alphaScale = 0;

		// driver -> instance -> thread -> modelIndex == "player" of any kind
		if(*(short *)&d->instSelf->thread->modelIndex == 0x18)
		{
			if
			(
				// if racer is not getting an Outside turbo (turbo pad or powerup),
				// this prevents audio-spam from multiple boosts
				((d->actionsFlagSet & 0x200000) == 0) ||
				((d->actionsFlagSetPrevFrame & 0x200000) == 0)
			)

			{
				turboObj->fireAudioDistort = 0;
				 VehFire_Audio(d, fireLevel);
			}
		}
	}

	newFireSpeedCap =

		(int)d->const_SingleTurboSpeed +

		// fireLevel * 8
		(
			fireLevel *

			#if 1

			8

			#else

			// this can all be simplified to: 8
			((int)d->const_SacredFireSpeed - (int)d->const_SingleTurboSpeed) >> 8

			#endif
		);

	if
	(
		// any gain in boost,
		// resize to gain boost
		(
			// Reserves are equal to zero
			// OR
			// speed cap has been raised
			(d->reserves == 0) ||
			(d->fireSpeedCap < (short)newFireSpeedCap)
		) ||

		// OR

		// you have USF, and boosted on a non-STP,
		// resize fire to lose size
		(
			// Current speed cap is greater than 0x1000
			// AND
			// You are not on a super turbo pad
			(int)d->const_SacredFireSpeed < (int)d->fireSpeedCap &&
			((d->stepFlagSet & 2) == 0)
			&& !USE_RETRO_FUELED
		)
	)

	{
		d->fireSpeedCap = (short)newFireSpeedCap;

		if (turboObj != 0)
		{
			// modify, cap, and save the size of the fire
			newFireSize = (fireLevel >> 6) + 5;
			if(newFireSize > 8) newFireSize = 8;
			turboObj->fireSize = (short)newFireSize;
		}
	}

	// boost powerup
	if (type & 8)
	{
		//turn on 14th bit of Actions Flag set (means racer is driving against a wall)
		d->actionsFlagSet |= 0x200;
	}

	// super engine
	if (type & 0x10)
	{
		// set reserves to reserves parameter instead of incrementing
		if (d->reserves < reserves)
		{
			d->reserves = reserves;
		}
	}

	// startline, hang time, powerslide
	else if (!(type & 1))
	{

		d->reserves += reserves;

	}

	// turbo pad, boost powerup
	else
	{
		// this adds reserves on the first frame you touch the turbo pad,
		// then prevent reserves from decreasing until the first frame
		// you leave the turbo pad

		oldOTT = d->turbo_outsideTimer;

		if (oldOTT < reserves)
		{

			d->reserves += 			(reserves - oldOTT);

			d->turbo_outsideTimer += 	(reserves - oldOTT);
		}
	}




	// if modelIndex == "player" of any kind
	if (d->instSelf->thread->modelIndex == 0x18)
	{
		// CameraDC flag
		gGT->cameraDC[d->driverID].flags |= 0x80;

		// gamepad vibration
		 GAMEPAD_ShockForce1(d, 8, 0x7f);
	}

}