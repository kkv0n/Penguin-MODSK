#include <common.h>

// mk_fueled_UI_DrawSlideMeter.c
// Budget 548 / 628 bytes

force_inline void DrawMeter(POLY_F4 *p, short posX, short posY, short length, short height, u_int color, u_long *ot)
{
	*(u_int *)&p->r0 = color;
	p->x0 = length;
	p->y0 = height;
	p->x1 = posX;
	p->y1 = height;
	p->x2 = length;
	p->y2 = posY;
	p->x3 = posX;
	p->y3 = posY;

	*(int *)p = *ot | 0x5000000;
	*ot = (u_int)p & 0xffffff;
}

void mk_fueled_UI_DrawSlideMeter(short posX, short posY, struct Driver *driver)
{
	u_int colorAndCode[4] = {0x28ffff00, 0x2800ffff, 0x28FF00FF, 0x28808080};
	int maxRoom;
	struct DB *backDB;
	POLY_F4 *p;
	u_long *ot;
	short meterHeight;
	short meterLength;
	RECT box;
	u_int rgb[2];
	int currentRoomRemaining;
	struct GameTracker *gGT = sdata->gGT;

	meterLength = 0;

	// height of bar in 1P or 2P
	meterHeight = (2 < gGT->numPlyrCurrGame) ? 3 : 7;

	// if powerslide meter is not zero
	if (driver->turbo_MeterRoomLeft != 0)
	{
		// current room remaining
		currentRoomRemaining = driver->turbo_MeterRoomLeft * 0x31;

		// max amount of room in turbo
		maxRoom = (u_int)driver->const_turboMaxRoom << 5;

		// length of rectangle is currentRoom / maxRoom
		meterLength = 0x31 - (short)(currentRoomRemaining / maxRoom);
	}
	box.x = posX - 0x31;
	box.y = posY - meterHeight;
	box.w = 0x31;
	box.h = meterHeight;

	backDB = gGT->backBuffer;
	// pointer to OT memory
	ot = gGT->pushBuffer_UI.ptrOT;

	CTR_Box_DrawWireBox(&box, 0, ot, &backDB->primMem);

	meterHeight = posY - meterHeight;

	p = backDB->primMem.curr;

	// if there is room remaining to draw
	if (p > (u_long *)backDB->primMem.endMin100)
		return;

	// increment "curr" for next draw after powerslide meter
	backDB->primMem.curr = p + 1;

	char numBoosts = (driver->KartStates.Drifting.numBoostsSuccess < 3) ? driver->KartStates.Drifting.numBoostsSuccess : 2;

	DrawMeter(p, posX, posY, posX - meterLength, meterHeight, colorAndCode[numBoosts], ot);

	p = backDB->primMem.curr;

	if (p > (u_long *)backDB->primMem.endMin100)
		return;

	backDB->primMem.curr = p + 1;
	DrawMeter(p, posX, posY, posX - 0x31, meterHeight, colorAndCode[3], ot);
}

// mk_fueled_VehFire_Increment.c

void mk_fueled_VehFire_Increment(struct Driver* driver, int reserves, u_int type, int fireLevel)
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

    u_char isPlayer = (driver->instSelf->thread->modelIndex == DYNAMIC_PLAYER);

	if
	(
		// if this is a turbo pad
		(type & 4) &&
		
		// racer is in accel prevention (holding square)
		((driver->actionsFlagSet & 8) != 0)
	)
	{
		// do nothing
		return;
	}

    // Player / AI structure + 0x4a shows driver index (0-7)

	// If this is the first driver (P1) and
	if
	((driver->driverID == 0) &&

		// if modelIndex == "player" of any kind
		(isPlayer)
	)
	{
		// Add Reserves to ghost buffer
		GhostTape_WriteBoosts(reserves, (u_char)type, fireLevel);
	}

	kartState = driver->kartState;

	if (kartState == KS_SPINNING) return;
	if (kartState == KS_MASK_GRABBED) return;
	if (kartState == KS_BLASTED) return;

	//turn off 8th flag, turn on 22nd flag of actions flag set
	//means ? (!(8)) and racer just got an outside turbo (22)
	driver->actionsFlagSet = driver->actionsFlagSet & 0xffffff7f | 0x200000;

	// turbo thread bucket
	turboThread = gGT->threadBuckets[TURBO].thread;

	// check all turbo threads
	while(turboThread != 0)
	{	
		// if this turbo thread is owned by this driver
		if (((struct Turbo*)turboThread->object)->driver == driver)
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

		driver->numTurbos = 1;

		#else

		// some sort of variable related to drifting? check + 0x3be in japanese output
		if (driver->japanTurboUnknown == 0)
		{
			driver->numTurbos = 1;
			if ((driver->numTurbosHighScore < 1) && ((gGT->gameMode1 & END_OF_RACE) == 0))
			{
				driver->numTurbosHighScore = 1;
			}
		}
		else
		{
			driver->numTurbos++;
			if ((driver->numTurbosHighScore < driver->numTurbos) && ((gGT->gameMode1 & END_OF_RACE) == 0))
			{
				driver->numTurbosHighScore = driver->numTurbos;
			}
		}

		#endif
		
		turboInst1 = INSTANCE_BirthWithThread(
			STATIC_TURBO_EFFECT, 				// modelID
			&sdata->s_turbo1[0],	// name
			SMALL, 				// SmallStackPool
			TURBO, 				// ThreadBucket
			VehTurbo_ThTick,	// func
			0x10, 				// object size
			0					// no parent thread
		);
	
		turboObj = NULL;
	
		if (turboInst1 != 0)
		{
			// get thread, ignore all collisions
			turboThread = turboInst1->thread;
			turboThread->flags |= 0x1000;
			turboThread->funcThDestroy = VehTurbo_ThDestroy;
			
			// turbo #2
			turboInst2 = INSTANCE_Birth3D(
				gGT->modelPtr[STATIC_TURBO_EFFECT], 	// model
				&sdata->s_turbo2[0], 		// name
				turboThread					// parent thread
			);
			
			// get object, set essentials
			turboObj = turboThread->object;
			turboObj->inst = turboInst2;
			turboObj->driver = driver;
			turboObj->fireAnimIndex = 0;
			turboObj->fireVisibilityCooldown = 0x60;
			
			// make flame disappear after
			// 	- powerslide: two frames (quick death)
			//	- all others: 255 frames (slowly die out)
			if (type & 2)	count = 2;
			else					count = 0xff;
			turboObj->fireDisappearCountdown = count;
	
			// if modelIndex == "player" of any kind
			if (isPlayer)
			{
				turboObj->fireAudioDistort = 0;
				
				if(driver->kartState != KS_CRASHING)
				{
					VehFire_Audio(driver, fireLevel);
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
		if (type & 4)
		{
			// only increase counter on the first frame of turbo pad
			
			if ((driver->actionsFlagSetPrevFrame & 0x200000) == 0)
			{
				driver->numTurbos++;
				
				#if BUILD == JpnRetail
				// the japanese version of the game keeps track of your highest turbo chain in a race
				if (driver->numTurbosHighScore < driver->numTurbos && (gGT->gameMode1 & END_OF_RACE) == 0) driver->numTurbosHighScore = driver->numTurbos;
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
			driver->numTurbos++;
			#if BUILD == JpnRetail
			// the japanese version of the game keeps track of your highest turbo chain in a race
			if (driver->numTurbosHighScore < driver->numTurbos && (gGT->gameMode1 & END_OF_RACE) == 0) driver->numTurbosHighScore = driver->numTurbos;
			#endif
		}
	
		turboObj->fireDisappearCountdown = 0xff;
		turboInst1->alphaScale = 0;
		turboInst2->alphaScale = 0;
	
		// driver -> instance -> thread -> modelIndex == "player" of any kind
		if(isPlayer)
		{
			if
			(
				// if racer is not getting an Outside turbo (turbo pad or powerup), 
				// this prevents audio-spam from multiple boosts  
				((driver->actionsFlagSet & 0x200000) == 0) ||
				((driver->actionsFlagSetPrevFrame & 0x200000) == 0)
			)
			
			{
				turboObj->fireAudioDistort = 0;
				VehFire_Audio(driver, fireLevel);
			}
		}
	}

	newFireSpeedCap = 
	
		driver->const_SingleTurboSpeed +
		
		// fireLevel * 8
		(
			fireLevel * 
			
			// this can all be simplified to: 8
			(driver->const_SacredFireSpeed - driver->const_SingleTurboSpeed) >> 8
		);

	if
	(
		// any gain in boost,
		// resize to gain boost
		(
			// Reserves are equal to zero
			// OR
			// speed cap has been raised
			(driver->reserves == 0) ||
			(driver->fireSpeedCap < (short)newFireSpeedCap)
		) ||

		// OR

		// you have USF, and boosted on a non-STP,
		// resize fire to lose size
		(
			// Current speed cap is at USF
			// AND
			// You are not on a super turbo pad
			(0x6400 <= driver->fireSpeedCap) &&
			((driver->stepFlagSet & 2) == 0)
		)
	)
	
	{
		driver->fireSpeedCap = (short)newFireSpeedCap;

		if (turboObj != 0)
		{
			// modify, cap, and save the size of the fire
			newFireSize = (fireLevel >> 6) + 5;
			if (newFireSize > 8) newFireSize = 8;
			turboObj->fireSize = (short)newFireSize;
		}
	}

	// boost powerup
	if (type & 8)
	{
		//turn on 14th bit of Actions Flag set (means racer is driving against a wall)
		driver->actionsFlagSet |= 0x200;
	}

	// super engine
	if (type & 0x10)
	{
		// set reserves to reserves parameter instead of incrementing
		if (driver->reserves < reserves)
		{
			driver->reserves = reserves;
		}
	}

	// startline, hang time, powerslide
	else if (!(type & 1))
	{
		// increase reserves BY param2
		driver->reserves += reserves;
	}
	
	// turbo pad, boost powerup
	else
	{
		// this adds reserves on the first frame you touch the turbo pad,
		// then prevent reserves from decreasing until the first frame 
		// you leave the turbo pad
		
		oldOTT = driver->turbo_outsideTimer;
		
		if (oldOTT < reserves)
		{
			driver->turbo_outsideTimer += 	(reserves - oldOTT);
			driver->reserves += 			(reserves - oldOTT);
		}
	}

	// if modelIndex == "player" of any kind
	if (!isPlayer) return;
	
    // CameraDC flag
    gGT->cameraDC[driver->driverID].flags |= 0x80;

    // gamepad vibration
    GAMEPAD_ShockForce1(driver, 8, 127);
	
}

// mk_fueled_VehPhysProc_PowerSlide_Update.c
//MK FUELED
void mk_fueled_VehPhysProc_PowerSlide_Update(struct Thread *t, struct Driver *d)
{
    char turbo_level;
    int meterLeft;
    struct GamepadBuffer *pad = &sdata->gGamepads->gamepad[d->driverID];

    // This is the distance remaining that can be filled
    meterLeft = d->turbo_MeterRoomLeft;
    u_short empty = (d->const_turboMaxRoom << 5);

    if ((pad->buttonsTapped & (BTN_R1 | BTN_L1)) == 0)
    {
        turbo_level = d->KartStates.Drifting.numBoostsSuccess;

        // if still drifting
        if ((pad->buttonsReleased & d->buttonUsedToStartDrift) == 0)
        {
            // if (turbo_level)
            // {
            //     MATRIX *currMatrix = &d->instSelf->matrix;
            //     gte_SetRotMatrix(currMatrix);
            //     gte_SetTransMatrix(currMatrix);

            //     struct ParticleEmitter *emset = (sdata->gGT->timer & 1) ? SparksR[turbo_level - 1] : SparksL[turbo_level - 1];

            //     // Tire Sparks
            //     VehEmitter_Terrain_Ground(d, emset);
            // }

            if (turbo_level < 3)
            {
                short turn = d->turnAngleCurr;
                turn = (turn < 0) ? -turn : turn;
                meterLeft -= VehCalc_MapToRange(turn, 0, 1080, 0, 48);

                // if the bar is full or beyond
                if (meterLeft <= 0)
                {
                    // Make a sound
                    OtherFX_Play_Echo(0xf, 1, d->actionsFlagSet & 0x10000);
                    // mini-turbo level up
                    turbo_level++;
                    // reset bar to beginning
                    meterLeft = empty;
                }
            }
            else
            {
                // stay at full bar
                meterLeft = 1;
            }
        }
        // you released L1 / R1 after drifting.
        else
        {
            // if you haven't filled the turbo at least 1x
            if ((turbo_level == 0) && (meterLeft > 0))
            {
                // reset meter to beginning
                d->unk381 = 8;
            }
            else
            {
                // Mini-Turbo
                VehFire_Increment(
                    // driver
                    d,
                    // amount of reserves
                    1440 * turbo_level,
                    // turbo type
                    TURBO_PAD,
                    // fire level
                    167 * turbo_level);
                // reset turbo_level
                turbo_level = 0;
                meterLeft = empty;
            }
        }
        d->KartStates.Drifting.numBoostsSuccess = turbo_level;
    }

    // If you do press L1 or R1
    else
    {
        // Set drift timer to zero, start the drift
        d->KartStates.Drifting.numFramesDrifting = 0;
    }

    d->turbo_MeterRoomLeft = meterLeft;

    if ( // speed is less than half the driver's speed classStat
        (((d->speed < d->const_Speed_ClassStat >> 1 ||

           ((d->actionsFlagSet & 0x2028) != 0)) ||

          // If the gamepad input is...
          ((pad->buttonsHeldCurrFrame &

            // does not include the jump button that you used to start drifting
            d->buttonUsedToStartDrift) == 0))))
    {
        // Stop drifting, just drive
        VehPhysProc_PowerSlide_Finalize(t, d);
        VehPhysProc_Driving_Init(t, d);
    }
}