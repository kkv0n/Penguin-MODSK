#include <common.h>
#include "../../../mods/game_modes/Gamemode_Selection/src/utils.h"

int DECOMP_RB_Hazard_HurtDriver(
    struct Driver* driverVictim, 
    int damageType,
    struct Driver* driverAttacker,
    int reason)
{
    struct GameTracker* gGT = sdata->gGT;
    int result=0;
    
    // Check if boss challenge is active and if AI driver is hitting themselves
    // if (USE_BOSS_CHALLENGE && 
    //     driverAttacker == driverVictim && 
    //     (driverVictim->actionsFlagSet & 0x100000) != 0) {
    //     // AI is immune to their own items in boss challenge mode
    //     return 0;
    // }

    if ((driverVictim->actionsFlagSet & 0x100000) == 0)
    {
        #ifndef REBUILD_PS1 //this ifndef should be removed at some point?
        result = VehPickState_NewState(driverVictim, damageType, driverAttacker, reason);
        #endif
    }
    else
    {
        //decomp attempt 1 had the second condition of this if statement as: ((gGT->gameMode1 & ADVENTURE_BOSS) != 0)
        //currently using ((int)gGT->gameMode1 < 0) bc that's what ghidra says (it might be equivalent idk)
        if ((gGT->levelID == OXIDE_STATION) && (gGT->gameMode1 < 0))
            damageType = 1;

        #ifndef REBUILD_PS1 //this ifndef should be removed at some point?
        result = (int)BOTS_ChangeState(driverVictim, damageType, driverAttacker, reason);
        #endif
    }
    return result;
}