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