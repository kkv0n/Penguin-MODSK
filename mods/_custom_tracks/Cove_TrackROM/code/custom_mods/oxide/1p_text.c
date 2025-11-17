#include <common.h>
#include "../../header/cove_rom.h"

void DecalF(char* str, int posX, int posY, short fontType, int flags);

void DecalFont_DrawLine(char* str, int posX, int posY, short fontType, int flags)
{
	
	DecalF(str, posX, posY, fontType, flags);
	//DecalFont_DrawLineStrlen(str, 0xffffffff, posX, posY, fontType, flags);
}



