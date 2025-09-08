#include <common.h>
#include "global.h"


extern int bestCourseIndex;
extern int bestLapIndex;
extern void UpdateBestTimes();

void EndOfRace_Icons()
{
    char s_time[15];
    TotalTime tt;
    const int playersPerRow = 4;
    const int rectSpace = 5;
    const int iconSpacing = 126;
    const int iconHeight = 28;
    const int xStart = 0;
    const int yStart = 20;
    const int scale = FP(1);
    int playersFinished = 0;
    int rectHeightMultiplier = 1;
    Point pos = MakePoint(xStart, yStart);
    UpdateBestTimes();
    for (int i = 0; i < octr->NumDrivers; i++)
    {
        int index = octr->raceStats[i].slot;
        if ((octr->nameBuffer[index][0] == 0) ||
            (octr->raceStats[i].finalTime == 0)) { continue; }

        struct Icon * icon = sdata->gGT->ptrIcons[data.MetaDataCharacters[data.characterIDs[index]].iconID];
        char racePos = i + '1';
        DECOMP_DecalFont_DrawLineStrlen(&racePos, 1, pos.x + 27, pos.y, FONT_SMALL, RED);
        DECOMP_DecalFont_DrawLineStrlen(octr->nameBuffer[index], NAME_LEN, pos.x + 38, pos.y + 1, FONT_SMALL, index == 0 ? OXIDE_LIGHT_GREEN : ORANGE);
        ElapsedTimeToTotalTime(&tt, octr->raceStats[i].finalTime);
        if (tt.hours > 0)
        {
            sprintf(s_time, "%d:%02d:%02d", tt.hours, tt.minutes, tt.seconds);
        }
        else
        {
            sprintf(s_time, "%d:%02d.%03d", tt.minutes, tt.seconds, tt.miliseconds);
        }
        DECOMP_DecalFont_DrawLine(s_time, pos.x + 38, pos.y + 8 + 1, FONT_SMALL, index == bestCourseIndex ? SILVER : PINSTRIPE_PALE_DARK_BLUE);
        ElapsedTimeToTotalTime(&tt, octr->raceStats[i].bestLap);
        tt.minutes = min(tt.minutes, 120);
        sprintf(s_time, "%d:%02d.%03d", tt.minutes, tt.seconds, tt.miliseconds);
        DECOMP_DecalFont_DrawLine(s_time, pos.x + 38, pos.y + 16 + 1, FONT_SMALL, index == bestLapIndex ? PURA_VIOLET : PINSTRIPE_PALE_DARK_BLUE);
        DECOMP_UI_DrawDriverIcon(icon, pos, sdata->gGT->pushBuffer_UI.ptrOT, 1, scale, MakeColor(0x80, 0x80, 0x80));

        pos.x += iconSpacing;
        playersFinished++;
        if (playersFinished == playersPerRow)
        {
            pos.x = xStart;
            pos.y += iconHeight;
        }
        else if (playersFinished > playersPerRow) { rectHeightMultiplier = 2; }
    }
    const RECT bgRect = {.x = 0, .y = yStart - rectSpace, .w = 512, .h = rectSpace * 2 + iconHeight * rectHeightMultiplier - 3};
    DECOMP_RECTMENU_DrawInnerRect(&bgRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
}

// First, collect valid drivers and sort them by elimination order
typedef struct {
    int index;          // Driver index
    int elimPosition;   // Position in elimination order (-1 = not eliminated)
} DriverSortData;

extern int GetDriverEliminationPosition(int driverID);
void EndOfRace_Icons_Survival()
{
    const int playersPerRow = 4;
    const int rectSpace = 5;
    const int iconSpacing = 126;
    const int iconHeight = 28;
    const int xStart = 0;
    const int yStart = 20;
    const int scale = FP(1);
    int playersDrawn = 0;
    int rectHeightMultiplier = 1;
    Point pos = MakePoint(xStart, yStart);
    
    DriverSortData driverData[8];
    int validDriverCount = 0;
    
    // Collect driver data
    for (int i = 0; i < octr->NumDrivers; i++) {
        int index = octr->raceStats[i].slot;
        if (octr->nameBuffer[index][0] == 0) continue; // Skip invalid drivers
        
        driverData[validDriverCount].index = index;
        driverData[validDriverCount].elimPosition = GetDriverEliminationPosition(index);
        validDriverCount++;
    }
    
    // Simple bubble sort to order drivers:
    // 1. Non-eliminated drivers first (elimPosition = -1)
    // 2. Then by elimination order (last eliminated to first eliminated)
    for (int i = 0; i < validDriverCount - 1; i++) {
        for (int j = 0; j < validDriverCount - i - 1; j++) {
            bool shouldSwap = false;
            
            // If one is eliminated and one isn't, non-eliminated comes first
            if (driverData[j].elimPosition >= 0 && driverData[j+1].elimPosition == -1) {
                shouldSwap = true;
            }
            // If both are eliminated, sort by elimination order (higher position = eliminated later)
            else if (driverData[j].elimPosition >= 0 && driverData[j+1].elimPosition >= 0 && 
                     driverData[j].elimPosition < driverData[j+1].elimPosition) {
                shouldSwap = true;
            }
            
            if (shouldSwap) {
                DriverSortData temp = driverData[j];
                driverData[j] = driverData[j+1];
                driverData[j+1] = temp;
            }
        }
    }
    
    // Now draw the icons in the sorted order
    for (int i = 0; i < validDriverCount; i++) {
        int index = driverData[i].index;
        
        struct Icon * icon = sdata->gGT->ptrIcons[data.MetaDataCharacters[data.characterIDs[index]].iconID];
        
        // Determine position number to display
        char racePos;
        if (driverData[i].elimPosition == -1) {
            racePos = '1'; // Winner (not eliminated)
        } else {
            // Position based on elimination (last eliminated = 2, second-to-last = 3, etc.)
            racePos = (validDriverCount - driverData[i].elimPosition) + '0';
        }
        
        // Draw position and name
        DECOMP_DecalFont_DrawLineStrlen(&racePos, 1, pos.x + 27, pos.y, FONT_SMALL, RED);
        DECOMP_DecalFont_DrawLineStrlen(octr->nameBuffer[index], NAME_LEN, pos.x + 38, pos.y + 1, FONT_SMALL, 
            index == 0 ? OXIDE_LIGHT_GREEN : ORANGE);
        
        // Add "WINNER" text for the non-eliminated driver
        if (driverData[i].elimPosition == -1 && i == 0) {
            #ifdef GASMOX_ENG
            DECOMP_DecalFont_DrawLine("WINNER", pos.x + 38, pos.y + 10, FONT_SMALL, PAPU_YELLOW);
            #elif defined(GASMOX_ES)
            DECOMP_DecalFont_DrawLine("GANADOR", pos.x + 38, pos.y + 10, FONT_SMALL, PAPU_YELLOW);
            #elif defined(GASMOX_BR)
            DECOMP_DecalFont_DrawLine("VENCEDOR", pos.x + 38, pos.y + 10, FONT_SMALL, PAPU_YELLOW);
            #endif
        } else {
            // Add "ELIMINATED" text for eliminated drivers
            #ifdef GASMOX_ENG
            DECOMP_DecalFont_DrawLine("ELIMINATED", pos.x + 38, pos.y + 10, FONT_SMALL, CORTEX_RED);
            #elif defined(GASMOX_ES)
            DECOMP_DecalFont_DrawLine("ELIMINADO", pos.x + 38, pos.y + 10, FONT_SMALL, CORTEX_RED);
            #elif defined(GASMOX_BR)
            DECOMP_DecalFont_DrawLine("ELIMINADO", pos.x + 38, pos.y + 10, FONT_SMALL, CORTEX_RED);
            #endif
        }

        // Draw driver icon
        DECOMP_UI_DrawDriverIcon(icon, pos, sdata->gGT->pushBuffer_UI.ptrOT, 1, scale, MakeColor(0x80, 0x80, 0x80));

        pos.x += iconSpacing;
        playersDrawn++;
        if (playersDrawn == playersPerRow)
        {
            pos.x = xStart;
            pos.y += iconHeight;
        }
        else if (playersDrawn > playersPerRow) { rectHeightMultiplier = 2; }
    }
    const RECT bgRect = {.x = 0, .y = yStart - rectSpace, .w = 512, .h = rectSpace * 2 + iconHeight * rectHeightMultiplier - 3};
    DECOMP_RECTMENU_DrawInnerRect(&bgRect, 0, sdata->gGT->backBuffer->otMem.startPlusFour);
}