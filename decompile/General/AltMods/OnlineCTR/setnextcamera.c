#include <common.h>
#include "global.h"

int currCam;

void SetNextCamera(bool increase)
{
	int driverCount = octr->NumDrivers;
	while (true)
	{
		currCam = increase ? (currCam + 1) % driverCount : (currCam + driverCount - 1) % driverCount;
		if (octr->nameBuffer[currCam][0])
		{
			sdata->gGT->cameraDC[0].driverToFollow = sdata->gGT->drivers[currCam];
			return;
		}
	}
}