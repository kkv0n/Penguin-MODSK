#include <common.h>
extern void moongravity();
extern void StatsUpgrade(struct Driver* driver);
#ifdef USE_ONLINE
#include "../AltMods/OnlineCTR/global.h"
#endif

void DECOMP_VehBirth_SetConsts(struct Driver* driver)
{
	u_int metaPhysSize;
	u_int i;
	struct MetaPhys* metaPhys;
	u_char* d;

	d = (u_char*)driver;
	
	
	unsigned char driverID = driver->driverID;

#ifdef USE_ONLINE

int engineID;
//engine selection
if (octr->enginetype[driverID] < 5){
engineID = octr->enginetype[i];

}
else
{
	engineID = data.MetaDataCharacters
	[data.characterIDs[driverID]].engineID;
	
	// if online stats are selected
	 StatsUpgrade(driverID);
}

	

	 
	 //moon mode event
moongravity();






#else
	int engineID = data.MetaDataCharacters[data.characterIDs[driver->driverID]].engineID

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