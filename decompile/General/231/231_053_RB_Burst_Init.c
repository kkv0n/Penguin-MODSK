#include <common.h>

void RB_Burst_CollLevInst();
void RB_Burst_CollThBucket();

void DECOMP_RB_Burst_Init(struct Instance* weaponInst)
{
    struct GameTracker* gGT = sdata->gGT;
    struct ScratchpadStruct* sps = 0x1f800108;

    // put weapon position on scratchpad
    sps->Input1.pos[0] = weaponInst->matrix.t[0];
    sps->Input1.pos[1] = weaponInst->matrix.t[1];
    sps->Input1.pos[2] = weaponInst->matrix.t[2];
    
    struct TrackerWeapon* tw = weaponInst->thread->object;
    int modelID = weaponInst->model->id;

    // Configure hit radius and squared radius based on the model ID
    if (modelID == 0x29) 
    {
        sps->Input1.hitRadius = 0x80;
        sps->Input1.hitRadiusSquared = 0x4000;
    }
    else 
    {
        if ((tw->flags & 1) == 0)
        {
            sps->Input1.hitRadius = 0x140;
            sps->Input1.hitRadiusSquared = 0x19000;
        }
        else 
        {
            sps->Input1.hitRadius = 0x200;
            sps->Input1.hitRadiusSquared = 0x40000;
        }
    }
    
    sps->Input1.modelID = modelID;
    sps->Union.ThBuckColl.thread = weaponInst->thread;
    sps->Union.ThBuckColl.funcCallback = RB_Burst_CollThBucket;

    struct Thread* driverTh = tw->instParent->thread;

    // Check collision with different thread buckets
    PROC_CollideHitboxWithBucket(gGT->threadBuckets[PLAYER].thread, sps, driverTh);
    PROC_CollideHitboxWithBucket(gGT->threadBuckets[ROBOT].thread, sps, driverTh);
    PROC_CollideHitboxWithBucket(gGT->threadBuckets[MINE].thread, sps, 0);
    PROC_CollideHitboxWithBucket(gGT->threadBuckets[TRACKING].thread, sps, 0);

    sps->Union.ThBuckColl.funcCallback = RB_Burst_CollLevInst;
    PROC_StartSearch_Self(sps);

    return;
}
