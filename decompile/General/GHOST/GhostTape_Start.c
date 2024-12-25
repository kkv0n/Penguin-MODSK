#include <common.h>

#ifdef USE_GASMOXIAN
//spectator mode text
#ifdef GASMOX_ENG
const char specting[] = "SPECTATING...";
const char finish_race[] = "FINISHING RACE!";

	const char s_switchCam[] = "Press R1 or L1 to change the camera";
    const char s_gg[] = "GG Thanks For Playing!";
	
#elif defined(GASMOX_ES)
	const char specting[] = "ESPECTEANDO..";
const char finish_race[] = "ACABANDO CARRERA!";

	const char s_switchCam[] = "Presiona L1 o R1 para cambiar camara";
    const char s_gg[] = "GG Gracias por jugar!";
	
#elif defined(GASMOX_BR)
	const char specting[] = "ESPECTANDO...";
const char finish_race[] = "FINALIZANDO CORRIDA!";

	const char s_switchCam[] = "Pressione R1 ou L1 para alternar cam";
    const char s_gg[] = "Terminado,Boa Corrida!!";
#endif

#else
void DECOMP_GhostTape_Start(void)
{
  struct GhostHeader* gh;
  struct Driver* d;
  struct GameTracker* gGT = sdata->gGT;
  
  d = gGT->drivers[0];

  // v1 - PizzaHut (June), Spyro2 (July)
  // v4 - Aug5, Aug14, Sep3, Retail

  gh = sdata->GhostRecording.ptrGhost;
  gh->version = -4;
  gh->levelID = gGT->levelID;
  gh->characterID = data.characterIDs[d->driverID];

  sdata->GhostRecording.VelX = 0;
  sdata->GhostRecording.VelY = 0;
  sdata->GhostRecording.VelZ = 0;

  sdata->GhostRecording.timeElapsedInRace = 0;
  sdata->boolGhostTooBigToSave = 0;
  sdata->ghostOverflowTextTimer = 0;
  sdata->boolCanSaveGhost = 1;
  
  sdata->GhostRecording.ptrCurrOffset = sdata->GhostRecording.ptrStartOffset;
  
  sdata->GhostRecording.countEightFrames = 0;
  sdata->GhostRecording.countSixteenFrames = 0;
  sdata->GhostRecording.timeOfLast80buffer = 0;
  sdata->GhostRecording.boostCooldown1E = 0;

  sdata->GhostRecording.animFrame = -1;
  sdata->GhostRecording.animIndex = -1;
  sdata->GhostRecording.instanceFlags = 0;

  return;
}
#endif