#include <common.h>

void VehPhysProc_SlamWall_Animate(struct Thread* t, struct Driver* d)
{
	struct Instance* inst;
	inst = t->inst;

	inst->animFrame++;


	d->matrixIndex++;

	// If crashing animation is not finished, quit function
	if(
		// oxide has no animation
		(data.characterIDs[d->driverID] != 0xF) &&

		// animation is not over
		((inst->animFrame+1) < 15)
	  )
	{
		return;
	}

	// == Initialize Driving ==

	d->matrixArray = 0;
	d->matrixIndex = 0;
	inst->animIndex = 0;

	// start halfway into steer animation
	inst->animFrame = FPS_DOUBLE(10);


	d->funcPtrs[0] = VehPhysProc_Driving_Init;
}