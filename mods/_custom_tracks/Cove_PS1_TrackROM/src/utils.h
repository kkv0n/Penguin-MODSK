#ifndef UTILS_H_TRACKROM
#define UTILS_H_TRACKROM

#include <common.h>

extern unsigned char CUSTOM_LEVEL_ID;


#define GHOST_READY (volatile int*) 0x8000C008
#define CHAR_MODEL_PTRS (struct Model**) 0x8000C010

#define CUSTOM_VRAM_ADDR (char*) 0x80200000
#define GHOST_SIZE_ADDR (int*) 0x80280000
#define DRIVER_ADDR (char*) 0x80290000
#define GHOST_ADDR (char*) 0x80280004
#define CUSTOM_MAP_PTR_ADDR (int*) 0x80300000
#define CUSTOM_LEV_ADDR (char*) 0x80300004


#endif