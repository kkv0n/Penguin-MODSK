#ifndef COVE_ROM_H
#define COVER_ROM_H

#define FALSE 0
#define TRUE 1

enum WEATHER
{
	CLEAR,
	RAIN,
	SNOW
};

extern unsigned char CUSTOM_LEVEL_ID;
extern u_char reserves_bar;
extern u_char weather_type;
extern u_char RETRO_FUELED;
extern char* track_name;
extern int force;



void LOAD_Custom_LOD_Driver(struct BigHeader* bigfile, unsigned char levelLOD, void* callback);
void Cove_InitTrack();
void Cove_Main();
void TT_End(void);
void Custom_Weather(unsigned char weatherIndex);
void bluefire();
void ghost_OnBoot();
void get_ghostIDs(struct SpawnType1* ptrSpawnType1);
extern char reload_Ghosts(struct GameTracker* gGT);

#endif