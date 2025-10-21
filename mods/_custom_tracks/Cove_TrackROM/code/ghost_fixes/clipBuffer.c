#include <common.h>

// Debug clip space
#define JAL(dest) (((unsigned long)dest & 0x3FFFFFF) >> 2 | 0xC000000)
void RunHook226();

u_int MainDB_GetClipSize(u_int levelID, int numPlyrCurrGame)
{
  return 0x4000;
}

#if 0
void RunHook226()
{
	// ra = 0x8000c000;
	asm("lui $ra, 0x8000");
	asm("ori $ra, $ra, 0xc000");
	
	// Save start(s7) and end(t8)
	asm("sw $s7, 0x0($ra)");
	asm("sw $t8, 0x4($ra)");
	asm("nop");
	
	// ra = 0x800a0eb4;
	asm("lui $ra, 0x800a");
	asm("ori $ra, $ra, 0xeb4");
	
	asm("j 0x800aa790");
	asm("nop");
}
#endif