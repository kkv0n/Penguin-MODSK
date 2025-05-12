#include <common.h>
#include "utils.h"
#include "jump_blocks.c"

// Per-driver floating state
typedef struct {
    unsigned char frameTimer;          // Frames remaining for floating
    unsigned char frameTimerDelay;     // Delay before floating begins
    short lastBlockID;         // Last block ID touched
} DriverFloatState;

// State for each driver
DriverFloatState driverState[4] = {0};

void initDriverStates() {
    for (unsigned char i = 0; i < 4; i++) {
        resetDriverFloatState(i);
    }
}

void resetDriverFloatState(unsigned char driverIndex) {
    driverState[driverIndex].frameTimer = 0;
    driverState[driverIndex].frameTimerDelay = 0;
    driverState[driverIndex].lastBlockID = -1;
}

bool updateDriverFloatState(unsigned char driverIndex, struct Driver* driver) {
    struct GameTracker* gGT = sdata->gGT;
    DriverFloatState* state = &driverState[driverIndex];
    
    // First, handle delay timer
    if (state->frameTimerDelay > 0) {
        state->frameTimerDelay--;
        
        // Give player sacred fire while waiting for jump
        if (driver) {
            int fire = 0x100;
            VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), fire);
        }

        // Play sound when delay timer expires and we're about to start floating
        if (state->frameTimerDelay == 0 && state->frameTimer > 0) {
            OtherFX_Play_Echo(0x09, 0, 1); // Play spring sound
        }
        
        // Don't float yet
        return false;
    }
    
    // Process active float timer
    if (state->frameTimer > 0) {
        state->frameTimer--;
        return true; // Should float
    }
    
    // Check for new jump block
    if (driver && driver->currBlockTouching) {
        short currentBlockID = driver->currBlockTouching->blockID;
        
        // Only set the timer when we move to a new block
        if (currentBlockID != state->lastBlockID) {
            unsigned char frames = 0, delay = 0;
            
            if (isJumpBlock(gGT->levelID, currentBlockID, &frames, &delay)) {
                state->frameTimer = frames;
                state->frameTimerDelay = delay;
                
                // Save the block ID
                if (currentBlockID != 0 && 
                    (driver->currBlockTouching->quadFlags & (Q_WALL | Q_MASK_GRAB)) == 0) {
                    state->lastBlockID = currentBlockID;
                }
                
                // Play sound at start of jump
                if (delay == 0 && frames > 0) {
                    OtherFX_Play_Echo(0x09, 0, 1); // Play spring sound
                    
                    // Give sacred fire on first frame
                    int fire = 0x100;
                    VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), fire);
                }
                
                // Return false because we want to wait for the next frame to start floating
                return false;
            }
            
            // Remember this block to avoid rechecking
            if (currentBlockID != 0 && 
                (driver->currBlockTouching->quadFlags & (Q_WALL | Q_MASK_GRAB)) == 0) {
                state->lastBlockID = currentBlockID;
            }
        }
    }
    
    return false;
}

// void DrawFloatTimer(short posX, short posY, unsigned char remainingFrames, unsigned char maxFrames)
// {
// 	u_int colorAndCode;
// 	short timerMeter;
// 	struct DB* backDB;
// 	u_long *primmemCurr;
// 	POLY_F4 *p;
// 	short bottomY;
// 	RECT box2;
// 	u_int auStack48 [2];
// 	int timerMeterWidth;

// 	// Only draw if there's time remaining
// 	if (remainingFrames <= 0)
// 		return;

// 	timerMeter = posY - 0x2d;

// 	backDB = sdata->gGT->backBuffer;
// 	primmemCurr = backDB->primMem.curr;
// 	p = 0;

// 	// if there is room left for more
// 	if (primmemCurr <= (u_long *)backDB->primMem.endMin100)
// 	{
// 		// add primitives
// 		backDB->primMem.curr = &primmemCurr[6];
// 		p = (POLY_F4 *)primmemCurr;
// 	}

// 	if (p != 0)
// 	{
// 		box2.y = posY - 3;
// 		box2.w = 0x31;
// 		box2.h = 3;
// 		box2.x = posX - 0x31;
// 		memset(auStack48, 0, 4);

// 		CTR_Box_DrawWireBox(&box2, auStack48, sdata->gGT->pushBuffer_UI.ptrOT, &sdata->gGT->backBuffer->primMem);

// 		// Color depends on remaining time
// 		// Red for low time, yellow for medium, green for high
// 		if (remainingFrames < maxFrames / 3)
// 		{
// 			colorAndCode = 0x280000ff; // red
// 		}
// 		else if (remainingFrames < (maxFrames * 2) / 3)
// 		{
// 			colorAndCode = 0x2800ffff; // yellow
// 		}
// 		else
// 		{
// 			colorAndCode = 0x2800ff00; // green
// 		}
		
// 		*(u_int *)&p->r0 = colorAndCode;
		
// 		// Calculate width based on remaining frames relative to max frames
// 		timerMeterWidth = (remainingFrames * 0x31) / maxFrames;
		
// 		bottomY = posY - 3;
// 		p->y0 = posY;
// 		p->y1 = posY;
// 		p->x1 = posX;
// 		p->y2 = bottomY;
// 		p->x3 = posX;
// 		p->y3 = bottomY;
		
// 		timerMeter = posX - timerMeterWidth;
// 		if(timerMeter < (posX - 0x31))
// 		{
// 			timerMeter = posX - 0x31;
// 		}
// 		p->x0 = timerMeter;
// 		p->x2 = timerMeter;

// 		primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

// 		*(int*)p = *primmemCurr | 0x5000000;
// 		*primmemCurr = (u_int)p & 0xffffff;

// 		backDB = sdata->gGT->backBuffer;
// 		primmemCurr = backDB->primMem.curr;
// 		p = 0;

// 		// If there is room to add more
// 		if (primmemCurr <= (u_long *)backDB->primMem.endMin100)
// 		{
// 			// Add more primitives
// 			backDB->primMem.curr = &primmemCurr[6];
// 			p = (POLY_F4 *)primmemCurr;
// 		}

// 		if (p != 0)
// 		{
// 			*(u_int *)&p->r0 = 0x28808080;
// 			p->y0 = posY;
// 			p->y1 = posY;
// 			p->x1 = posX;
// 			p->y2 = bottomY;
// 			p->x3 = posX;
// 			p->y3 = bottomY;
// 			p->x0 = posX - 0x31;
// 			p->x2 = posX - 0x31;

// 			// pointer to OT memory
// 			primmemCurr = sdata->gGT->pushBuffer_UI.ptrOT;

// 			*(int*)p = *primmemCurr | 0x5000000;
// 			*primmemCurr = (u_int)p & 0xffffff;
// 		}
// 	}

// 	return;
// }
