#include <common.h>

#ifdef USE_GASMOXIAN
#include "../AltMods/Gasmoxian/global.h"
#endif

void DECOMP_VehBirth_EngineAudio_AllPlayers(void)
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
	
	#ifdef USE_GASMOXIAN
	d = gGT->drivers[0];
	#endif
	
    u_char driverID = d->driverID;
	
#ifdef USE_GASMOXIAN

	//change engine sound per enginetype
	
	data.MetaDataCharacters
	[data.characterIDs[driverID]].engineID = octr->enginetype[driverID];

	int engine = data.MetaDataCharacters
		[data.characterIDs[driverID]].engineID;


#else
	int engine = data.MetaDataCharacters
		[data.characterIDs[driverID]].engineID;
#endif

	#ifndef REBUILD_PS1
    EngineAudio_InitOnce((engine * 4) + driverID, 0x8080);
	#endif

	#ifdef USE_GASMOXIAN
	return;
	#endif
  }
}
