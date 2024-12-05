#include <common.h>

void RB_Burst_CollLevInst();
void RB_Burst_CollThBucket();

void DECOMP_RB_Blowup_Init(struct Instance* weaponInst)
{
  struct GameTracker* gGT = sdata->gGT;
  struct ScratchpadStruct* sps = 0x1f800108;


  sps->Input1.pos[0] = weaponInst->matrix.t[0];
  sps->Input1.pos[1] = weaponInst->matrix.t[1];
  sps->Input1.pos[2] = weaponInst->matrix.t[2];


  if (gGT->gameMode1 < 0) 
  {
    // hitRadius and hitRadiusSquared
    sps->Input1.hitRadius = 0x100;
    sps->Input1.hitRadiusSquared = 0x10000;
  }
  else 
  {
    // hitRadius and hitRadiusSquared
    sps->Input1.hitRadius = 0x140;
    sps->Input1.hitRadiusSquared = 0x19000;
  }

  sps->Input1.modelID = weaponInst->model->id;
  sps->Union.ThBuckColl.thread = weaponInst->thread;
  sps->Union.ThBuckColl.funcCallback = RB_Burst_CollThBucket;


  PROC_StartSearch_Self(sps);


  PROC_CollideHitboxWithBucket(gGT->threadBuckets[ROBOT].thread, sps, 0);
  PROC_CollideHitboxWithBucket(gGT->threadBuckets[MINE].thread, sps, 0);


  if (weaponInst->model->id != 0x27) 
  {
    sps->Input1.hitRadius = 0x80;
    sps->Input1.hitRadiusSquared = 0x4000;
  }


  PROC_CollideHitboxWithBucket(gGT->threadBuckets[PLAYER].thread, sps, 0);

  sps->Union.ThBuckColl.funcCallback = RB_Burst_CollLevInst;
  return;
}
