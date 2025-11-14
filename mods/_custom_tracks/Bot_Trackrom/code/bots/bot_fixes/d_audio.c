#include <common.h>

static void ddd_Audio(struct Thread* t, struct Driver* d)
{

	if (d->driverID != 0)
		return;

	

	EngineSound_Player(d);
	
	return;

}