#include <common.h>

#ifdef USE_ONLINE
#include "../AltMods/OnlineCTR/global.h"
#endif

int GetEngineID() {
    switch (octr->enginetype) {
        case 0:
            return 0;  // balanced
        case 1:
            return 1;  // speed
        case 2:
            return 2;  // accel
        case 3:
            return 3;  // turning
        case 4:
            return 4;  // unlimited

            default:
         return 0;	// balanced as default
    }
}
void DECOMP_VehBirth_SetConsts(struct Driver* driver)
{
	u_int metaPhysSize;
	u_int i;
	struct MetaPhys* metaPhys;
	u_char* d;

	d = (u_char*)driver;
	
	int engineID;
	
engineID = data.MetaDataCharacters[data.characterIDs[driver->driverID]].engineID;
	

	
	#ifdef USE_ONLINE
	
engineID = GetEngineID();

	#endif
	
	for(i = 0; i < 65; i++)
	{
		metaPhys = &data.metaPhys[i];

		metaPhysSize = metaPhys->size;

		void* src = &metaPhys->value[engineID];
		void* dst = &d[metaPhys->offset];

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

	return;
}