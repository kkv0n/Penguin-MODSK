#include <common.h>


force_inline char MetaIndex(char x, struct MetaPhys* metaphys)
{
    //these are metaphys indexes
    static const unsigned char desired_indexes[3] = {0x9, 0xb, 0xc};
	unsigned char m;
    
    static const short accel_no_reserves = 544;
	static const short speed_stat = 13900;
	static const short speed_unk = 15400;
    
    static const short custom_stats[3] = {accel_no_reserves, speed_stat, speed_unk};
    
    
	
    
	//verify if this stat needs a change
	for (m = 0; m < 3; m++)
	{
	 if (x == desired_indexes[m])
     {
         GetRetailMetaPhys(metaphys->value) = custom_stats[m];
		 return 1;
     }
	}
	
	return 0;
};

void VehBirth_SetConsts(struct Driver* driver)
{
	unsigned char metaPhysSize;
	short metaPhysOffset;
	int engineStat;
	unsigned char i;
	

	
	struct MetaPhys* metaPhys;
    
	
	//og engineID
	int engineID = data.MetaDataCharacters[data.characterIDs[driver->driverID]].engineID;
	
	
	

	for(i = 0; i < NUM_PHYS_TYPES; i++)
	{
		
		//get metaphys struct
		metaPhys = &data.metaPhys[i];
		
		//get stat offset from metaphys struct
        metaPhysOffset = GetRetailMetaPhys(metaPhys->offset);

		//get stat size from metaphys struct
		metaPhysSize = GetRetailMetaPhys(metaPhys->size);
        
        //check the stat index
        MetaIndex(i, metaPhys);
        
        engineStat = GetRetailMetaPhys(metaPhys->value[engineID]);
    
		//param_1 = custom_value, param_2 = struct driver*, param_3 = data.metaPhys[index].offset, param_4 = data.metaPhys[index].size
		VehStat_Update(engineStat, driver, metaPhysOffset, metaPhysSize);
	
	}

	

	return;
}