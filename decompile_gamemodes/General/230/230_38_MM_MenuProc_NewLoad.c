#include <common.h>

// Add external references for the new functions
extern bool showingModMenuInNewLoad;
extern bool continueToAdventure;
extern bool justCanceledModMenuNewLoad;
extern void ShowModMenuInNewLoad();

void DECOMP_MM_MenuProc_NewLoad(struct RectMenu* menu)
{
  short row;
  
  // row number
  row = menu->rowSelected;
  
  // If we've just canceled the mod menu, we need to ignore input for this frame
  // to avoid accidentally processing the Triangle button twice
  if (justCanceledModMenuNewLoad) {
    justCanceledModMenuNewLoad = false;
    DECOMP_RECTMENU_ClearInput();
    
    // Cancel out early but keep the menu visible
    return;
  }
  
  // If mod menu is showing, don't process any input in this function
  // The mod menu handles its own input
  if (showingModMenuInNewLoad) {
    // Only act on the input if we're ready to continue
    if (continueToAdventure) {
      // Reset the mod menu flags
      showingModMenuInNewLoad = false;
      continueToAdventure = false;
      
      // MM_Title transitioning out
      D230.MM_State = 2;
      
      menu->state |= 4;
    }
    
    // Clear any input to prevent it from being processed when mod menu is closed
    DECOMP_RECTMENU_ClearInput();
    return;
  }
  
  if(row == -1)
    menu->ptrPrevBox_InHierarchy->state &= ~(ONLY_DRAW_TITLE | DRAW_NEXT_MENU_IN_HIERARCHY);
  
  if((unsigned char)row > 1) return;
  
  // Store the row selection for later use
  D230.desiredMenuIndex = row;
  
  // Show the mod menu before proceeding
  ShowModMenuInNewLoad();
    
  // Don't transition yet, wait for mod menu confirmation
  // Clear any input to prevent it from being buffered
  DECOMP_RECTMENU_ClearInput();
  return;
}
