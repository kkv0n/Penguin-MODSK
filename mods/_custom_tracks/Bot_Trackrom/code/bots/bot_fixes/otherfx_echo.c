#include <common.h>

// param_3:
// 0 - normal
// 1 - echo
void OtherFX_Play_Echo(unsigned int soundID, int flags, int echoFlag)
{
	if (echoFlag != sdata->gGT->drivers[0]->actionsFlagSet & 0x10000)
		return;
	
	// ff8080:
	// 0x00 - no echo
	// 0xff - volume
	// 0x80 - distortion (none)
	// 0x80 - LR (center of left and right)
	int otherFlags = 0xff8080;
	
	if(echoFlag != 0) otherFlags |= 0x1000000;
	
	OtherFX_Play_LowLevel(soundID & 0xfff, flags & 0xff, otherFlags);
}