#include <common.h>

unsigned char CUSTOM_TRACK_ID = DINGO_CANYON;

//custom bot nickname on race
char* bot_name = "custombot";

//main menu option names
char* main_menu_options[2] =
{
	"GHOST CHALLENGE",
	"BOSS CHALLENGE"
	
};

void Lines_OnMainMenu()
{
	DecalFont_DrawLine("CUSTOM BOT MOD", 5, 197, FONT_SMALL, SILVER);
	DecalFont_DrawLine(__DATE__, 	5, 206, FONT_SMALL, ORANGE);
	DecalFont_DrawLine(__TIME__, 	170, 206, FONT_SMALL, ORANGE);
	
}


unsigned char botItemList[6] = {
	I_BOMB, I_MISSILE, I_TNTNITRO,
	I_POTION, I_CLOCK, I_WARPBALL
	
};

unsigned char bot_Itemdifficulty = BOT_NORMAL;


//Bot Item Difficulty
/*
    BOT_EASY
    BOT_NORMAL
    BOT_HARD

*/


//BotWeaponID:
/*

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
*/