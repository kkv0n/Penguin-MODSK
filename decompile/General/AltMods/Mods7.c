#ifdef USE_ONLINE
#include "OnlineCTR/endOfRaceUI2.c"
#include "OnlineCTR/endOfRaceUI3.c"
#include "OnlineCTR/global.h"

void Online_CollidePointWithBucket(struct Thread* th, short* vec3_pos)
{
	    // Si `octr->special` es igual a 3, salir de la función
if (octr->special != 1 && octr->special != 2)
	{ 
return;
	}
	while (th != 0)
	{
		DECOMP_PROC_CollidePointWithSelf(th, vec3_pos);
		// next
		th = th->siblingThread;
	}
}
unsigned int checkpointTimes[(MAX_LAPS * CPS_PER_LAP) + 1];
#endif