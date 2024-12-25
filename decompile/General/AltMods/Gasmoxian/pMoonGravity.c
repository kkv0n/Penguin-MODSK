//moon mode
#include "global.h"



void moongravity() { 

	        // low gravity event
		// space gravity (369) in og ctr is 41% of normal gravity (900)
	    if (octr->special != 4) {
        for (int i = 0; i < 5; i++) {
            data.metaPhys[0].value[i] = 900; 
        }
		}
 

	
	else
	{

        for (int i = 0; i < 5; i++) {
            data.metaPhys[0].value[i] = 369;
           			
		}
		
    }
	return;
}
