#include <common.h>
#include "global.h"
#include "utils.h"

extern int GetOverheadLen(struct Driver* d);

void DrawOverheadNames()
{
    int i;
    MATRIX* m;

    struct GameTracker* gGT = sdata->gGT;
    struct MyData* ptrDest = (struct MyData*)0x1f800108;

    // pushBuffer offset 0x28
    m = &gGT->pushBuffer[0].matrix_ViewProj;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);

    // start from P2
    i = 1;

    if((gGT->gameMode1 & START_OF_RACE) != 0)
    {
        // start from P1
        i = 0;
    }

    for(i; i < octr->NumDrivers; i++)
    {
        int len = GetOverheadLen(gGT->drivers[i]);
        if(len == 0) continue;

        // if mirror mode
        if(USE_MIRROR)
        {
            ptrDest->Screen_posX = 0x200 - ptrDest->Screen_posX;
        }

        // Calculate color based on character ID
        int color = JUSTIFY_CENTER;
        if(i == 0)
        {
            // Player 1 gets special color
            color |= OXIDE_LIGHT_GREEN;
        }
        else
        {
            // Use character-based color for other players
            // color |= (data.characterIDs[i] + 5);
			color |= ORANGE;
        }
        
        DECOMP_DecalFont_DrawLineStrlen(
            octr->nameBuffer[i],
            len,
            ptrDest->Screen_posX,
            ptrDest->Screen_posY-0x4,
            FONT_SMALL, color);
    }
}