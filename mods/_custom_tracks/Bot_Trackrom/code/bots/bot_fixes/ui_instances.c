#include <common.h>
#include "../../header/bot.h"

void UI_INSTANCE_InitAll(void)
{
  struct GameTracker *gGT;
  struct Instance* crystal;
  struct Instance* token;
  u_int gameMode1;
  u_int relicType;
  int iVar5;

  int i;

  gGT = sdata->gGT;
  sdata->menuReadyToPass &= 0xfffffffe;
  gGT->renderFlags |= 0x8000;

  gameMode1 = gGT->gameMode1;
  
  


	// used for multiplayer wumpa
    sdata->ptrPushBufferUI = (int)NULL;
	
	
	if (ghostMode)
	{
      return;
    }
	

	if (gGT->numPlyrCurrGame > 1)
	{
	  struct PushBuffer* pb = &sdata->pushBuffer_DecalMP;
	  struct PushBuffer* ui = &gGT->pushBuffer_UI;
	  
      sdata->ptrPushBufferUI = (int)pb;
    
	  // second half of pixel-LOD pushBuffer, copy from PushBuffer_UI
	  *(int*)&pb->matrix_ViewProj.m[0][0] = *(int*)&ui->matrix_ViewProj.m[0][0];
	  *(int*)&pb->matrix_ViewProj.m[0][2] = *(int*)&ui->matrix_ViewProj.m[0][2];
	  *(int*)&pb->matrix_ViewProj.m[1][1] = *(int*)&ui->matrix_ViewProj.m[1][1];
	  *(int*)&pb->matrix_ViewProj.m[2][0] = *(int*)&ui->matrix_ViewProj.m[2][0];
	  *(int*)&pb->matrix_ViewProj.m[2][2] = *(int*)&ui->matrix_ViewProj.m[2][2];
	  pb->matrix_ViewProj.t[0] = ui->matrix_ViewProj.t[0];
	  pb->matrix_ViewProj.t[1] = ui->matrix_ViewProj.t[1];
	  pb->matrix_ViewProj.t[2] = ui->matrix_ViewProj.t[2];
	  
	  // first half of pixel-LOD pushBuffer, copy from PushBuffer_UI
	  pb->pos[0] = ui->pos[0];
	  pb->pos[1] = ui->pos[1];
	  pb->pos[2] = ui->pos[2];
	  pb->rect.x = ui->rect.x;
	  pb->rect.y = ui->rect.y;
	  pb->rect.w = ui->rect.w;
	  pb->rect.h = ui->rect.h;
	  
	  pb->ptrOT = ui->ptrOT;
	  pb->distanceToScreen_PREV = ui->distanceToScreen_PREV;
	}


    sdata->ptrFruitDisp =
		(int)  UI_INSTANCE_BirthWithThread(0x37,(int) UI_ThTick_CountPickup,3,1,sdata->ptrPushBufferUI,/*sdata->s_fruitdisp*/0);

    if (
			(gGT->numPlyrCurrGame < 3) &&

			// If you're not in Battle Mode
			((gameMode1 & BATTLE_MODE) == 0)
		)
	  {

       UI_INSTANCE_BirthWithThread(0x38,(int) UI_ThTick_big1,2,0,0,/*sdata->s_big1*/0);

    }



  return;
}