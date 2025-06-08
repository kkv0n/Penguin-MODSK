#include <common.h>

int DECOMP_LOAD_GetBigfileIndex(unsigned int levelID, int lod)
{
	// === Wow we need a bigfile enum ===

#ifdef USE_HIGHMP

#ifdef USE_CUSTOM_TRACKS
// if custom track use lod parameter
// Note that this wont allow custom tracks on time trial
if (
	levelID >= NITRO_COURT && levelID <= LAB_BASEMENT
	&& (sdata->gGT->gameMode1 & (BATTLE_MODE | ADVENTURE_MODE)) == 0
) {
	lod = lod;
}

else if ((sdata->gGT->gameMode1 & (TIME_TRIAL | RELIC_RACE)) != 0){
	lod = 8;
}

else {
	lod = 1;
}
#else

// Always load lod 1 except for time trial/relic race which have special lod 8
if ((sdata->gGT->gameMode1 & (TIME_TRIAL | RELIC_RACE)) != 0){
	lod = 8;
}

else {
	lod = 1;
}

#endif
#endif

#ifdef USE_REAL60PS1
	lod = 4;
#endif

	if(levelID <= LAB_BASEMENT)
		return BI_ARCADETRACKS + levelID*8 + sdata->levBigLodIndex[lod-1];
	
	// adv hubs
	if(levelID <= CITADEL_CITY)
		return BI_ADVENTUREHUB + (levelID-GEM_STONE_VALLEY)*3;
	
	if(levelID <= INTRO_OXIDE)
		return BI_CUTSCENES_INTRO + (levelID-INTRO_RACE_TODAY)*3;
	
	if(levelID <= ADVENTURE_GARAGE)
		return BI_MAINMENUFILE + (levelID-MAIN_MENU_LEVEL)*2;
	
	if(levelID == NAUGHTY_DOG_CRATE)
		return BI_NDBOX;
	
	if(levelID <= OXIDE_TRUE_ENDING)
		return BI_CUTSCENES_OUTRO + (levelID-OXIDE_ENDING)*2;
	
	if(levelID <= CREDITS_MEGUMI)
		return BI_CREDITS + (levelID-CREDITS_CRASH)*3;
	
	// SCRAPBOOK
	return BI_SCRAPBOOK;
}