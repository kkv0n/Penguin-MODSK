#include <common.h>

void  VehBirth_EngineAudio_AllPlayers(void)
{
  struct Thread* th;
  struct GameTracker* gGT;
  gGT = sdata->gGT;

  for(
		th = gGT->threadBuckets[PLAYER].thread;
		th != 0;
		th = th->siblingThread
	)
  {
	struct Driver* d = th->object;
	

	d = gGT->drivers[0];

	
    u_char driverID = d->driverID;

	int engine = data.MetaDataCharacters
		[data.characterIDs[driverID]].engineID;


    EngineAudio_InitOnce((engine * 4) + driverID, 0x8080);



	return;

  }
}
