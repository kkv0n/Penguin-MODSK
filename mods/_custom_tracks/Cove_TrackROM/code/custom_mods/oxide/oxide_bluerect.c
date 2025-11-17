#include <common.h>

void DecalF(char* str, int posX, int posY, short fontType, int flags)
{
	
	if (sdata->gGT->levelID == MAIN_MENU_LEVEL && D230.MM_State == 2 && oxide
	&& *(int*)str != *(int*)sdata->lngStrings[73])
	{
		posX = 0x11a + 64;
		posY = 0xab;
	}


	DecalFont_DrawLineStrlen(str, 0xffffffff, posX, posY, fontType, flags);
}

void FUN_80021894(RECT* r, Color* rgb, struct OTMem* otMem, struct PrimMem* primMem)
{

  PolyF4 * polygon = NULL;
  
  RECT rect = *r;

  //move the blue rectangle for oxide
  if (oxide && sdata->gGT->levelID == MAIN_MENU_LEVEL && D230.MM_State == 2)
  {
	  if (rect.x == 0x123 || rect.x == 0x120 || rect.x == 0x151)
			rect.x = 0x160;

	


			if (rect.y == 0x87)
			{
				rect.y = 0xcd;
				rect.x = 0x160;
			}
			else if (rect.y == 0xa6)
			{
				rect.y = 0xae;
				rect.x = 0x160;
			}
			else if (rect.y == 0x89)
			{
				rect.y = 0xb0;
				rect.x = 0x160;
			}
			
  }


  
  // if curr is less or equal than primMem.endMin100
  if ((unsigned int)primMem->curr <= (unsigned int)primMem->endMin100) 
  {
	 // set polygon to old curr location
	 polygon = primMem->curr;
	  
    // set curr six bytes further, that's as much as the primitive will need
    primMem->curr = (char*)(primMem->curr + (sizeof(int) * 6));
	

   
  }
  
  // if polygon was set to old curr location (safety check)
  if (polygon != NULL) 
  {
	
	rgb->code.code = 0x28;
	
	// polyf4->colorCode
    polygon->colorCode.self = rgb->self;

    // v0
    polygon->v[0].pos.x = rect.x;
    polygon->v[0].pos.y = rect.y;

    polygon->v[1].pos.x = rect.x + rect.w;
    polygon->v[1].pos.y = rect.y;

    // v2
    polygon->v[2].pos.x = rect.x;
    polygon->v[2].pos.y = rect.y + rect.h;

    // v3
    polygon->v[3].pos.x = rect.x + rect.w;
    polygon->v[3].pos.y = rect.y + rect.h;
				
    polygon->tag.addr = otMem->size & 0xffffff;
	polygon->tag.size = 0x05;
	
    otMem->size = (u_int)polygon & 0xffffff;
  }
  
  return;
  
}