#include "global.h"
void StatsUpgrade(struct Driver* driver)
{
    if (octr->enginetype[driver->driverID] == 5)
    {
        /*
            Stat 9 is acceleration,
            Stats 11 and 12 are speed related
        */
        for (int i = 9; i < 13; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                data.metaPhys[i].value[j] = data.metaPhys[i].value[4]; // only speed and accel are at max value
            }
        }
    }
	else
	{
		return;
	}
}