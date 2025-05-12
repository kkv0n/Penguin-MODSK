#include <common.h>

void VehBirth_SetConsts(struct Driver *d)
{
    unsigned short penta_stats[4] = {544, 1152, 13900, 15400}; //accel, something, speed, speedometer offset. Divide speed by 256 to know its kph value
    unsigned char engineID = data.MetaDataCharacters[data.characterIDs[d->driverID]].engineID;

    for (int i = 0; i < 65; i++)
    {
        int* meta = &data.metaPhys[i];
        int metaOffset = meta[1];
        unsigned char metaSize = meta[2];
        short metaValue = meta[3 + engineID];


            //if current stat is accel/speed related
            if(i > 8 &&  i < 13)
            {
				//use penta max stats
			 if (data.characterIDs[d->driverID] == PENTA_PENGUIN)
			 {				 metaValue = penta_stats[i - 9]; }
						 
			 else { goto normal_stats;  }			 
				
			 
			}
			
			else
			{
				normal_stats:
			  metaValue = meta[3 + engineID]; //if current stat is not accel or speed related
			}			
				
            


        if (metaSize == 2)
        {
            *(short *)((int)d + metaOffset) = (short)(metaValue);
            continue;
        }

        if (metaSize == 1)
        {
            *(char *)((int)d + metaOffset) = (char)(metaValue);
            continue;
        }

        *(int *)((int)d + metaOffset) = (int)(metaValue);
    }
}