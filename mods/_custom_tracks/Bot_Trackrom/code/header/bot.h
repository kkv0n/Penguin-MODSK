#ifndef BOTS_H_HEADER
#define BOTS_H_HEADER
#include <common.h>


#define HUMAN_PLAYER 0
#define SKIP_BOT_INPUT(lights, level) (lights > 0 || level >= INTRO_RACE_TODAY)
#define SKIP_BOT_PATH(tf, lvl, gm, act) (SKIP_BOT_INPUT(tf, lvl) || ((gm & END_OF_RACE) != 0) || ((gm & PAUSE_1) != 0) || BOT_FINISHED_RACE(act))
#define BOT_BLASTED(k) (k == KS_BLASTED)
#define BOT_FINISHED_RACE(x) ((x & ACTION_RACE_FINISHED) != 0)
#define BOT_SPIN(k) (k == KS_SPINNING)
#define BOT_MASK_GRABBED(k) (k == KS_MASK_GRABBED)
#define PAUSE_BOT_PATH(k) (BOT_SPIN(k) || BOT_BLASTED(k))


 struct custom_bots
 {
	   Vec3 pos; //pos[3] xyz
       SVec4 rot; //rot[4] xyzw
       short angle; //driver->angle
       short reserves; //driver->reserves
       short fireSpeedCap; //driver->fireSpeedCap
       char kartState; //driver->kartState

       int buttonsHeldCurrFrame; //gamepad[1].buttonsHeldCurrFrame
			 
 };
 

enum BotDiff
{
    BOT_EASY = 1,
    BOT_NORMAL = 2,
    BOT_HARD = 3
};

enum BotWeaponID
{
	I_TURBO,
	I_BOMB,
	I_MISSILE,
	I_TNTNITRO,
	I_POTION,
	I_SPRING,
	I_PSHIELD,
	I_MASK,
	I_CLOCK,
	I_WARPBALL
};

extern int force;
extern unsigned char botItemList[6];
extern unsigned char bot_Itemdifficulty;
extern unsigned char numCustomBots;
extern unsigned char CUSTOM_TRACK_ID;
extern bool ghostMode;
extern bool botPaused;
extern char* bot_name;
extern char* main_menu_options[2];
extern unsigned char characterID_items;
extern unsigned char characterID_ghost;
void LOAD_BOT_PATH(bool ghost);
void ItemChaos_Init(bool enabled);
void CustomBot_Thtick(struct GameTracker* gGT);
void CustomTrack_Init();
void BotMod_Main();
void CBOT_EndEvent_DrawMenu(void);
void Lines_OnMainMenu();
#endif