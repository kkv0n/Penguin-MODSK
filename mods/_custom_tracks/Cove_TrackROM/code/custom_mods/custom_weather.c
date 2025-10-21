#include <common.h>

u_char weather_type;

void Custom_Weather(unsigned char weatherIndex) 
{
	struct RainBuffer* rainBuffer = &sdata->gGT->level1->rainBuffer;


	switch(weatherIndex)
	{
		default:
		{
			break;
		}
		case RAIN: 	//TIGER TEMPLE RAIN
		{
			rainBuffer->renderMode[0] = 0xe1000a60; //fillMode
			rainBuffer->renderMode[1] = 1; //offsetOT
			rainBuffer->numParticles_curr = 120;
			rainBuffer->numParticles_max = 120;
			rainBuffer->vanishRate = 9;
			rainBuffer->colorRGBA_top = 0x00404040; //ARGB
			rainBuffer->colorRGBA_bottom = 0x00ffffff; //ARGB
			rainBuffer->unk_4[0] = -80;
			rainBuffer->unk_4[1] = 14;
			rainBuffer->unk_4[2] = -32;
			rainBuffer->unk_4[3] = -89;
			rainBuffer->unk_4[8] = 20;
			rainBuffer->unk_4[10] = -120;
			rainBuffer->unk_4[11] = -1;
			rainBuffer->unk_22 = 0;
		
			break;
		}
		case SNOW: //BLIZZARD BLUFF SNOW
		{
			rainBuffer->renderMode[0] = 0xe1000a20; //fillMode
			rainBuffer->renderMode[1] = 1; //offsetOT
			rainBuffer->numParticles_curr = 240;
			rainBuffer->numParticles_max = 240;
			rainBuffer->vanishRate = 1;
			rainBuffer->colorRGBA_top = 0x00404040; //ARGB
			rainBuffer->colorRGBA_bottom = 0x00ffffff; //ARGB
			rainBuffer->unk_4[0] = 0;
			rainBuffer->unk_4[1] = 0;
			rainBuffer->unk_4[2] = 0;
			rainBuffer->unk_4[3] = -8;
			rainBuffer->unk_4[8] = 0;
			rainBuffer->unk_4[10] = -8;
			rainBuffer->unk_4[11] = -1;
			rainBuffer->unk_22 = 0;
		
			break;
		}
	}
}