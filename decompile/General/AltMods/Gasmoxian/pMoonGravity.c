//moon mode
#include "global.h"
#include "utils.h"


void moongravity() { 

	        // low gravity event
		// space gravity (369) in og ctr is 41% of normal gravity (900)
	    if (!USE_MOON_GRAVITY) {
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
