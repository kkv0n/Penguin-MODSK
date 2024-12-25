#include <common.h>

#ifdef USE_GASMOXIAN

#include "../../General/AltMods/Gasmoxian/global.h"
#include "../../General/AltMods/Gasmoxian/changecamera.c"

void queuetojoin(){
	
	if (octr->autoRetryJoinRoomIndex != -1)
	{
		#ifdef GASMOX_ENG
		char* wtj = "joinning,pls wait ->";
		#elif defined(GASMOX_ES)
		char* wtj = "entrando.. espera ->";
		#elif defined(GASMOX_BR)
		char* wtj = "na fila, aguarde. ->";
		
		#endif
		int lineInd = octr->autoRetryJoinRoomIndex % 8;
		int pageNum = octr->autoRetryJoinRoomIndex / 8;
		if (pageNum == octr->PageNumber)
			DecalFont_DrawLine(wtj, 0xE0 -120, 0x6b + (lineInd * 8), FONT_SMALL, JUSTIFY_CENTER | PURA_VIOLET);
		if (octr->clientCount[octr->autoRetryJoinRoomIndex] <= 7)
		{
			octr->serverRoom = octr->autoRetryJoinRoomIndex;
			octr->serverLockIn2 = 1;
			octr->autoRetryJoinRoomIndex = -1;
			//0 = cursor move noise
			//1 = cursor select noise
			//2 = triangle noise
			//3 = Ding!
			//4 = different ding!
			//5 = womp
			//6 = slammed into wall sfx
			//...
			DECOMP_OtherFX_Play(3, 1); //play SFX to notify user
		}
	}	
}


#else
void DECOMP_GhostReplay_Init2()
{
	// combined into GhostReplay_Init1
	return;
}
#endif