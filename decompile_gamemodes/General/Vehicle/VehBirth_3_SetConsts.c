#include <common.h>

#ifdef USE_ONLINE
#include "../AltMods/OnlineCTR/global.h"
#endif

void DECOMP_VehBirth_SetConsts(struct Driver* driver)
{
    u_int metaPhysSize;
    u_int i;
    struct MetaPhys* metaPhys;
    u_char* d;
    
    #ifdef USE_BETTER_STATS
    int stats[4] = {544, 1152, 13900, 15400}; //accel, something, speed, speedometer offset. Divide speed by 256 to know its kph value
    #endif

    d = (u_char*)driver;

    int engineID = data.MetaDataCharacters[data.characterIDs[driver->driverID]].engineID;

    for(i = 0; i < 65; i++)
    {
        metaPhys = &data.metaPhys[i];
        metaPhysSize = metaPhys->size;

        void* src = &metaPhys->value[engineID];
        void* dst = &d[metaPhys->offset];
        
        #ifdef USE_BETTER_STATS
        // Apply custom stats for indices 9-12
        if (i < 13 && 8 < i) 
        {
            // Use custom stats instead of metaPhys values
            int customValue = stats[i-9];
            
            if (metaPhysSize == 1)
            {
                *(char*)dst = (char)customValue;
                continue;
            }

            if (metaPhysSize == 2)
            {
                *(short*)dst = (short)customValue;
                continue;
            }

            *(int*)dst = customValue;
            continue;
        }
        #endif

        // Regular metaPhys values
        if (metaPhysSize == 1)
        {
            *(char*)dst = *(char*)src;
            continue;
        }

        if (metaPhysSize == 2)
        {
            *(short*)dst = *(short*)src;
            continue;
        }

        *(int*)dst = *(int*)src;
    }
}