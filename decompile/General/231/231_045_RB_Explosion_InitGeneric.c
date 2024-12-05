#include <common.h>

void RB_Explosion_ThTick();

void DECOMP_RB_Explosion_InitGeneric(struct Instance* inst)
{
  struct Instance* explosion;
  unsigned short color;
  
  
  // set funcThDestroy to remove instance from instance pool
  explosion->thread->funcThDestroy = PROC_DestroyInstance;
  return;
}