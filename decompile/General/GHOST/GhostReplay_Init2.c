#include <common.h>


#include "../../General/AltMods/OnlineCTR/changecamera.c"



void queuetojoin(){
	
	if (octr->autoRetryJoinRoomIndex != -1)
	{
		char* wtj = "joinning,pls wait ->";
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