#include <common.h>
#include "global.h"

// forward declaration
extern int bestCourseIndex;
extern int bestLapIndex;


void UpdateBestTimes()
{
    int bestCourse = HOURS(100);
    int bestLap = MINUTES(99);
    for (int i = 0; i < octr->NumDrivers; i++)
    {
        int index = octr->raceStats[i].slot;
        if ((octr->nameBuffer[index][0] == 0) ||
            (octr->raceStats[i].finalTime == 0)) { continue; }

        if (octr->raceStats[i].finalTime < bestCourse)
        {
            bestCourse = octr->raceStats[i].finalTime;
            bestCourseIndex = index;
        }

        if (octr->raceStats[i].bestLap < bestLap)
        {
            bestLap = octr->raceStats[i].bestLap;
            bestLapIndex = index;
        }
    }
}