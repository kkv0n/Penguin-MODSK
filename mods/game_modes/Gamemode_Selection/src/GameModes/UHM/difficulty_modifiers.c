// Ultra hard mode by Niko
#include <common.h>
#include "../../utils.h"

//This should be gGT->arcadeDifficulty, but for some reason it doesnt work
unsigned short* superHardAddr = (unsigned short*)0x80012658;

// This should use BOTS_Adv_AdjustDifficulty instead, but its not fully decompiled
void AdvAdjustDifficulty()
{
    u_short add = 0x2008;
    // u_short modifier = sdata->gGT->arcadeDifficulty;
    u_short modifier = *superHardAddr; // Use the superHardAddr value instead

    *(unsigned short *)0x800126b8 = modifier;
    *(unsigned short *)0x800126ba = add;
    *(unsigned short *)0x80012728 = modifier;
    *(unsigned short *)0x8001272a = add;
    *(unsigned short *)0x800127b0 = modifier * 2;
    *(unsigned short *)0x800127b2 = add;
    *(unsigned short *)0x80012844 = modifier;
    *(unsigned short *)0x80012846 = add;

    BOTS_Adv_AdjustDifficulty();
}

void GiveBotsUSF(){
    for (unsigned char i = 0; i <= gGT->numBotsNextGame; i++) {
        struct Driver* driver = gGT->drivers[i];

        if (driver == NULL) continue;

        //If its a bot
        if ((driver->actionsFlagSet & 0x100000) != 0) {

            // If bot has a TNT on their head or is affected by a clock
            if ((driver->instTntRecv != 0) || (driver->clockReceive != 0)) continue;
            // If bot is spinning or blasted
            if ((driver->botData.botFlags & 2) != 0) continue;

            int usf_fire = 0x800;
            VehFire_Increment(driver, 960, (TURBO_PAD | FREEZE_RESERVES_ON_TURBO_PAD), usf_fire);
        }
    }
}

void SetDifficultyLevel(int newLevel)
{
    // Clamp between 4 and 9
    // if (newLevel < 4) newLevel = 4;
    if (newLevel > 9) newLevel = 9;

    *superHardAddr = newLevel * 0x50;

    // Enable/Disable Super Hard mode based on difficulty level
    if (newLevel >= 4) {
        sdata->gGT->gameMode2 |= CHEAT_SUPERHARD;
    } else {
        sdata->gGT->gameMode2 &= ~CHEAT_SUPERHARD;
    }

    AdvAdjustDifficulty();
}