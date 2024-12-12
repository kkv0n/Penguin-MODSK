#include <common.h>
#ifdef USE_ONLINE
#include "../AltMods/OnlineCTR/global.h"
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
	
	#ifdef USE_ONLINE
	d = gGT->drivers[0];
	#endif
	
    u_char driverID = d->driverID;
	//change engine sound per enginetype
	
int engine;

if (octr->enginetype[driverID] <= 3){
	engine = data.MetaDataCharacters
	[data.characterIDs[driverID]].engineID = octr->enginetype[driverID];


}
else
{					
// client.exe sets the OG engine for each character	 
	 engine = data.MetaDataCharacters
	[data.characterIDs[driverID]].engineID = octr->OGengine[driverID]; 
}


		


	#ifndef REBUILD_PS1
    EngineAudio_InitOnce((engine * 4) + driverID, 0x8080);
	#endif

	#ifdef USE_ONLINE
	return;
	#endif
  }
}
