#include <common.h>
#include "macro.h"

short difficulty[] = {0, 0x50, 0xa0, 0xf0, 0x140, 0x280};

void MainFreeze_Difficulty(struct MenuBox *mb)
{
    MainFreeze_SafeAdvDestroy();
    struct GameTracker *gGT = sdata->gGT;
    u_int gameMode = gGT->gameMode1;
    struct MenuBox *pause = MainFreeze_GetMenuBox();

    if (sdata->AnyPlayerTap & BTN_TRIANGLE)
    {
        sdata->ptrDesiredMenuBox = pause;
        return;
    }

    if (!(sdata->AnyPlayerTap & (BTN_CROSS | BTN_CIRCLE)))
        return;

    char validRows = (gameMode & ARCADE_MODE) ? 5 : 6;
    short row = mb->rowSelected;

    if (row >= validRows)
        return;

    gGT->arcadeDifficulty = (mb->rows[0].stringIndex != 346) ? difficulty[row] : difficulty[row+1];

    if (gameMode & ADVENTURE_ARENA)
    {
        sdata->ptrDesiredMenuBox = pause;
        return;
    }

    gGT->gameMode1 &= ~PAUSE_1;

    if (TitleFlag_IsFullyOffScreen())
        TitleFlag_BeginTransition(1);

    if (sdata->boolPlayGhost || sdata->ptrGhostTapePlaying)
    {
        data.characterIDs[1] = *(short *)((int)sdata->ptrGhostTapePlaying + 6);
    }
    sdata->Loading.stage = -5;
    mb->state |= NEEDS_TO_CLOSE;
}

void Retry_Difficulty(struct MenuBox *mb)
{
    struct GameTracker *gGT = sdata->gGT;
    u_int gameMode = gGT->gameMode1;
    struct MenuBox *endmenu;

    if (sdata->AnyPlayerTap & BTN_TRIANGLE)
    {
        if ((gameMode & ADVENTURE_MODE) != 0)
            endmenu = &data.menuBox_Retry_ExitToMap;
        else
            endmenu = (gGT->numPlyrCurrGame == 1) ? (struct MenuBox *)0x800a0b58 : (struct MenuBox *)0x800a0b84;

        MENUBOX_Show(endmenu);
        return;
    }

    if (!(sdata->AnyPlayerTap & (BTN_CROSS | BTN_CIRCLE)))
        return;

    char validRows = (gameMode & ARCADE_MODE) ? 5 : 6;
    short row = mb->rowSelected;

    if (row >= validRows)
        return;

    gGT->arcadeDifficulty = (mb->rows[0].stringIndex != 346) ? difficulty[row] : difficulty[row+1];
    gGT->hudFlags &= 0xfe;

    if (TitleFlag_IsFullyOffScreen())
        TitleFlag_BeginTransition(1);

    sdata->Loading.stage = -5;
    mb->state |= NEEDS_TO_CLOSE;
}

struct MenuRow rows_advDifficulty[] =
    {
        [0] = MENU_ROW(590, ROW_ADV_CLASSIC, ROW_ADV_EASY, ROW_ADV_CLASSIC, ROW_ADV_CLASSIC),
        [1] = MENU_ROW(346, ROW_ADV_CLASSIC, ROW_ADV_MEDIUM, ROW_ADV_EASY, ROW_ADV_EASY),
        [2] = MENU_ROW(347, ROW_ADV_EASY, ROW_ADV_HARD, ROW_ADV_MEDIUM, ROW_ADV_MEDIUM),
        [3] = MENU_ROW(348, ROW_ADV_MEDIUM, ROW_ADV_SUPER_HARD, ROW_ADV_HARD, ROW_ADV_HARD),
        [4] = MENU_ROW(588, ROW_ADV_HARD, ROW_ADV_ULTRA_HARD, ROW_ADV_SUPER_HARD, ROW_ADV_SUPER_HARD),
        [5] = MENU_ROW(589, ROW_ADV_SUPER_HARD, ROW_ADV_ULTRA_HARD, ROW_ADV_ULTRA_HARD, ROW_ADV_ULTRA_HARD),
        [6] = FINALIZER_ROW};

struct MenuRow rows_extraDifficulty[] =
    {
        [0] = MENU_ROW(346, ROW_EASY, ROW_MEDIUM, ROW_EASY, ROW_EASY),
        [1] = MENU_ROW(347, ROW_EASY, ROW_HARD, ROW_MEDIUM, ROW_MEDIUM),
        [2] = MENU_ROW(348, ROW_MEDIUM, ROW_SUPER_HARD, ROW_HARD, ROW_HARD),
        [3] = MENU_ROW(588, ROW_HARD, ROW_ULTRA_HARD, ROW_SUPER_HARD, ROW_SUPER_HARD),
        [4] = MENU_ROW(589, ROW_SUPER_HARD, ROW_ULTRA_HARD, ROW_ULTRA_HARD, ROW_ULTRA_HARD),
        [5] = FINALIZER_ROW};

struct MenuRow new_advHub[] =
    {
        MENU_ROW(2, 4, 1, 0, 0),
        MENU_ROW(11, 0, 2, 1, 1),
        MENU_ROW(7, 1, 3, 2, 2),
        MENU_ROW(14, 2, 4, 3, 3),
        MENU_ROW(3, 3, 0, 4, 4),
        FINALIZER_ROW};

struct MenuRow new_advRace[] =
    {
        MENU_ROW(2, 4, 1, 0, 0),
        MENU_ROW(1, 0, 2, 1, 1),
        MENU_ROW(7, 1, 3, 2, 2),
        MENU_ROW(14, 2, 4, 3, 3),
        MENU_ROW(13, 3, 0, 4, 4),
        FINALIZER_ROW};

struct MenuRow new_arcadeRace[] =
    {
        MENU_ROW(2, 6, 1, 0, 0),
        MENU_ROW(1, 0, 2, 1, 1),
        MENU_ROW(5, 1, 3, 2, 2),
        MENU_ROW(6, 2, 4, 3, 3),
        MENU_ROW(7, 3, 5, 4, 4),
        MENU_ROW(14, 4, 6, 5, 5),
        MENU_ROW(3, 5, 0, 6, 6),
        FINALIZER_ROW};

struct MenuBox AdvHubDifficulty = {
    .stringIndexTitle = 345,
    .posX_curr = 256,
    .posY_curr = 108,
    .unk1 = 0,
    .state = CENTER_ON_COORDS | ALL_PLAYERS_USE_MENU | USE_SMALL_FONT | BIG_TEXT_IN_TITLE | EXECUTE_FUNCPTR,
    .rows = rows_advDifficulty,
    .funcPtr = MainFreeze_Difficulty,
    .width = 209,
    .height = 74};

struct MenuBox AdvMainFreeze_Difficulty = {
    .stringIndexTitle = 345,
    .posX_curr = 256,
    .posY_curr = 108,
    .unk1 = 0,
    .state = CENTER_ON_COORDS | ALL_PLAYERS_USE_MENU | USE_SMALL_FONT | BIG_TEXT_IN_TITLE | EXECUTE_FUNCPTR,
    .rows = rows_advDifficulty,
    .funcPtr = MainFreeze_Difficulty,
    .width = 209,
    .height = 74};

struct MenuBox arcadeMainFreeze_Difficulty = {
    .stringIndexTitle = 345,
    .posX_curr = 256,
    .posY_curr = 108,
    .unk1 = 0,
    .state = CENTER_ON_COORDS | ALL_PLAYERS_USE_MENU | USE_SMALL_FONT | BIG_TEXT_IN_TITLE | EXECUTE_FUNCPTR,
    .rows = rows_extraDifficulty,
    .funcPtr = MainFreeze_Difficulty,
    .width = 209,
    .height = 74};