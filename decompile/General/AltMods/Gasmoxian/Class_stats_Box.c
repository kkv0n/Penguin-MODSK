#include <common.h>
#include "utils.h"
#include "global.h"

RECT DrawStatBar(Point pos, u_char curr_stat, short value, short* barLen, int* barColors);

// Draws a single stat bar with animation
RECT DrawStatBar(Point pos, u_char curr_stat, short value, short* barLen, int* barColors)
{
    struct GameTracker *gGT = sdata->gGT;
    struct PrimMem *primMem = &gGT->backBuffer->primMem;
    RECT r;
    
    // animate growth of stat bar
    short stat = value; //new stat value

    #ifdef USE_16BY9
    stat = WIDE_34(stat);
    #endif

    // half bar length, half speed per frame, just add 1
    #if (defined(USE_60FPS) && defined(USE_16BY9))
        #define BAR_RATE 1
    #else
        #define BAR_RATE 3
    #endif

    if (*barLen < stat)  *barLen = *barLen + BAR_RATE;
    if (stat < *barLen)  *barLen = stat;
    
    u_int statBarPosX = pos.x + 9;
    
    // Calculate consistent Y positions
    u_short statBarStart_Y = pos.y + (15 * curr_stat);
    u_short statBarEnd_Y = statBarStart_Y + 7;
    u_short statBarShadows_Y = statBarStart_Y + 1;

    // bar outline
    r.x = statBarPosX;
    r.y = statBarStart_Y;
    r.w = *barLen;
    r.h = 7;

    // outline color white
    DECOMP_CTR_Box_DrawWireBox(
        &r, MakeColor(0xFF, 0xFF, 0xFF),
        gGT->pushBuffer_UI.ptrOT);

    // bar shadows
    r.x = statBarPosX + 1;
    r.y = statBarShadows_Y;
    r.w = *barLen - 2;
    r.h = 5;

    // outline color black (shadows)
    DECOMP_CTR_Box_DrawWireBox(
        &r, MakeColor(0,0,0),
        gGT->pushBuffer_UI.ptrOT);

    int segmentLen = WIDE_PICK(13, 10);
    int segmentStart = 0;
    int segmentEnd = segmentLen;

    for (unsigned char segmentIndex = 0; segmentIndex < 7; segmentIndex++)
    {
        // color data of bars (blue green yellow red)
        u_int *barColor = &barColors[segmentIndex];
        short currSegmentLen = (short)segmentLen;

        if (*barLen <= segmentEnd)
        {
            currSegmentLen = *barLen - segmentStart;
        }

        if ((int)currSegmentLen << 0x10 < 0)
        {
            currSegmentLen = 0;
        }

        if (segmentStart + currSegmentLen <= *barLen)
        {
            // primMem curr
            POLY_G4* p = primMem->curr;

            // quit if prim mem runs out
            if (p + 2 >= primMem->end)
                return r;

            primMem->curr = p + 1;

            // color data
            *(int *)&p->r0 = barColor[0] | 0x38000000;
            *(int *)&p->r1 = barColor[1] | 0x38000000;
            *(int *)&p->r2 = barColor[0] | 0x38000000;
            *(int *)&p->r3 = barColor[1] | 0x38000000;

            short segmentX = statBarPosX + segmentStart;

            // top left
            p->x0 = segmentX;
            p->y0 = statBarStart_Y;

            // top right
            p->x1 = segmentX + currSegmentLen;
            p->y1 = statBarStart_Y;

            // bottom left
            p->x2 = segmentX;
            p->y2 = statBarEnd_Y;

            // bottom right
            p->x3 = segmentX + currSegmentLen;
            p->y3 = statBarEnd_Y;

            // pointer to OT memory
            void *ot = gGT->pushBuffer_UI.ptrOT;

            *(int *)p = (*(int *)ot & 0xffffff) | 0x8000000;
            *(int *)ot = (int)p & 0xffffff;
        }

        segmentStart += segmentLen;
        segmentEnd += segmentLen;
    }
    
    return r;
}

void DrawClassStats(Point pos, u_char engineID) {
    struct GameTracker *gGT = sdata->gGT;
    RECT box;
    RECT bars[3];
    
    //these are for the bar fill animation
    static short barLen[3] = {0, 0, 0};
    
    //change this for a custom array if you want custom stat
    u_char barStat = (engineID * 3);

    unsigned int barColors[7] = {
        0xc80000,      // blue
        0xA8700,       // blue-green
        0xb428,        // green-yellow
        0xb4b4,        // yellow
        0x64dc,        // orange
        0x28dc,        // dark orange
        0xeb           // red
    };

    //max is 0x50 per value (accel, speed, turn)
    u_char custom_stats[12] = 
    {
        // balanced
        // 0x37, 0x37, 0x37,
        0x50, 0x50, 0x37,
        
        // speed
        // 0x30, 0x50, 0x20,
        0x50, 0x50, 0x20,
        
        // accel
        // 0x50, 0x20, 0xA,
        0x50, 0x50, 0xA,
        
        // turn
        // 0x1c, 0x30, 0x50
        0x50, 0x50, 0x50
    }; //3 per engine
    
    // Draw stat labels first
    u_int statNamePosX = pos.x + 1;
    
    // "Speed"
    DECOMP_DecalFont_DrawLine(sdata->lngStrings[0x245], statNamePosX, pos.y, FONT_BIG, JUSTIFY_RIGHT | ORANGE_RED);

    // "Accel"
    DECOMP_DecalFont_DrawLine(sdata->lngStrings[0x246], statNamePosX, pos.y + 15, FONT_BIG, JUSTIFY_RIGHT | 0x4021);

    // "Turn"
    DECOMP_DecalFont_DrawLine(sdata->lngStrings[0x247], statNamePosX, pos.y + 30, FONT_BIG, JUSTIFY_RIGHT | BLUE);

    // Draw all three stat bars
    for (unsigned char b = 0; b < 3; b++) {
        bars[b] = DrawStatBar(pos, b, (custom_stats[barStat + b]), &barLen[b], &barColors[0]);
    }

    #if 0 // This has no use on current menu setup

    //if player pressed d-pad then reset the bar animation
    if ((sdata->AnyPlayerHold & 0xc) != 0)
    {
            // erase animated bars
            for (char l = 2; l > -1; l--)
            {
                barLen[l] = 0;
            }
    }
    
    #endif
    
    #if 0 // Needs size and position adjustements

    // Calculate rectangle size
    // Since we're removing the class name, make the box smaller
    box.x = pos.x - 6;
    box.y = pos.y - 4;
    box.w = 95; // Adjust width based on your needs
    box.h = 45; // Smaller height now that we removed class name
    
    // Draw 2D Menu rectangle background
    DECOMP_RECTMENU_DrawInnerRect(&box, 4, gGT->backBuffer->otMem.startPlusFour);

    # endif
}