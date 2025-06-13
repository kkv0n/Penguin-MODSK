#include <common.h>

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

  // For most of the function

  // If you're not in Crystal Challenge (in adventure mode)
  if ((gameMode1 & CRYSTAL_CHALLENGE) == 0)
  {
	// If you're in Adventure Arena
    if ((gameMode1 & ADVENTURE_ARENA) != 0)
	{

	  //is ignoring the return value of these calls intentional?
       UI_INSTANCE_BirthWithThread(0x61,	(int) UI_ThTick_Reward,0xe,1,0,/*sdata->s_relic1*/0);
       UI_INSTANCE_BirthWithThread(99,	(int) UI_ThTick_Reward,0xf,1,0,/*sdata->s_key1*/0);
       UI_INSTANCE_BirthWithThread(0x62,	(int) UI_ThTick_Reward,0x10,0,0,/*sdata->s_trophy1*/0);


      GAMEPROG_AdvPercent(&sdata->advProgress);


	  return;
    }

    if ((gameMode1 & (RELIC_RACE | ADVENTURE_ARENA | TIME_TRIAL)) != 0)
	{



	  // If you're not in a Relic Race
      if ((gameMode1 & RELIC_RACE) == 0) {
        return;
      }

	  // The rest of this block only happens in Relic Mode
      sdata->ptrRelic = 	 UI_INSTANCE_BirthWithThread(0x61,(int) UI_ThTick_Reward, 0xe,1,0,/*sdata->s_relic1*/0);
      sdata->ptrTimebox1 = 	 UI_INSTANCE_BirthWithThread(0x5c,(int) UI_ThTick_CountPickup, 0x13,1,0,/*sdata->s_timebox1*/0);

	  // if instance
      if (sdata->ptrRelic != 0)
	  {
		// set scale to zero
        sdata->ptrRelic->scale[0] = 0;
        sdata->ptrRelic->scale[1] = 0;
        sdata->ptrRelic->scale[2] = 0;
      }

	  // Get Relic Time to put in HUD
	  if(
			// no platinum and no gold
			(CHECK_ADV_BIT(sdata->advProgress.rewards, (gGT->levelID + 0x3a)) == 0) &&
			(CHECK_ADV_BIT(sdata->advProgress.rewards, (gGT->levelID + 0x28)) == 0)
		)
	  {
		  // 0 if sapphire not unlocked, (show sapphire)
		  // 1 if sapphire is unlocked (show gold)
		  relicType = CHECK_ADV_BIT(sdata->advProgress.rewards, (gGT->levelID + 0x16));
	  }

	  // if unlocked gold or unlocked platinum
      else
	  {
		// put platinum time on screen
        relicType = 2;
      }

	  // get relic time on this track, for this relic type (sapphire, gold, platinum)
	  unsigned int relicTime = data.RelicTime[gGT->levelID*3 + relicType];

	  // store globally for HUD to access later
      sdata->relicTime_1min = relicTime / 0xe100;
      sdata->relicTime_10sec = (relicTime / 0x2580) % 6;
      sdata->relicTime_1sec = (relicTime / 0x3c0) % 10;
      sdata->relicTime_10ms = ((relicTime * 100) / 0x3c0) % 10;
      sdata->relicTime_1ms = ((relicTime * 1000) / 0x3c0) % 10;

      return;
    }

	// used for multiplayer wumpa
    sdata->ptrPushBufferUI = (int)NULL;

    if (
			(gGT->numPlyrCurrGame < 3) &&

			// If you're not in Battle Mode
			((gameMode1 & BATTLE_MODE) == 0)
		)
	  {

    }

	// If you're not in Adventure Mode
    if ((gameMode1 & ADVENTURE_MODE) == 0) {
      return;
    }

    sdata->ptrHudC->flags |= 0x80;
    sdata->ptrHudT->flags |= 0x80;
    sdata->ptrHudR->flags |= 0x80;
  }

  // If you're in Crystal Challenge
  else
  {
    sdata->ptrMenuCrystal =  UI_INSTANCE_BirthWithThread(0x60,(int) UI_ThTick_Reward,0x11,0,0,/*sdata->s_crystal1*/0);
	sdata->ptrHudCrystal =  UI_INSTANCE_BirthWithThread(0x60,(int) UI_ThTick_Reward,0x11,0,0,/*sdata->s_crystal1*/0);
	sdata->ptrHudCrystal->flags |= 0x80;
  }

  // Make a token
  sdata->ptrToken =  UI_INSTANCE_BirthWithThread(0x7d,(int) UI_ThTick_Reward,0x12,0,0,/*sdata->s_token*/0);

  // make copy of Token pointer
  token = sdata->ptrToken;

  // set Token scale (x, y, z) to zero
  token->scale[0] = 0;
  token->scale[1] = 0;
  token->scale[2] = 0;

  // make Token invisible
  token->flags |= 0x80;
  return;
}