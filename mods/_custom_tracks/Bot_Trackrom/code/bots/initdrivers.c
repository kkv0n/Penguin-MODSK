#include <common.h>

void MainInit_Drivers(struct GameTracker *gGT)
{
    char i;
    char numHumans = 1;
	char numCustomBots = 1;
	
	char numPlyrCurrGame = numHumans + numCustomBots;
	
    int gameMode = gGT->gameMode1;
    struct Driver* d;

    for (i = 0; i < 8; i++)
        gGT->drivers[i] = NULL;

    gGT->numBotsNextGame = 0;

    if ( LOAD_IsOpen_RacingOrBattle())
    {
         RB_MinePool_Init();
    }

    // Spawn all players,
	// This MUST be in reverse order,
	// because of threadBucket linked list order
    for (i = numPlyrCurrGame-1; i >= 0; i--)
    {
		gGT->drivers[i] = VehBirth_Player(i);
    }
	


    // if this is main menu
    if ((gameMode & MAIN_MENU) != 0)
    {
		// fill up 4 players
        for (i = numPlyrCurrGame; i < 4; i++)
        {
            gGT->drivers[i] =  VehBirth_Player(i);
        }
    }
	
	return;
}
