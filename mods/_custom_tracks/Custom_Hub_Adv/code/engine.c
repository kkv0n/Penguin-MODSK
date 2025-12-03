#include <common.h>
extern uint16_t engineFlags;


	
	
force_inline char MetaIndex(char x)
{
    //these are metaphys indexes
    static const unsigned char desired_indexes[6] = {0x9, 0xb, 0xc, 0x12, 0x14, 0x15};
	unsigned char m;
	
    
	//verify if this is a custom stat
	for (m = 0; m < 6; m++)
	{
	 if (x == desired_indexes[m])
		 return m;
	}
	
	return -1;
};

force_inline char GetStatIndex(char x)
{
	//the stat we are modifying
	static const unsigned char stat_for_engine[6] = {ACCEL, SPEED, SPEED, TURN, TURN, TURN};
	 
	 
	 if (x >= 0 && x < 6)
		 return stat_for_engine[x] - 1; //return - 1 for the sake of maths (bruh)
	
	return -1;
}

//return the new stat
force_inline short getvehicleStats(char curr_stat, short* custom_stat)
{

	unsigned char s;
	

	   for (s = 0; s < MAX; s++)
	    {
			if (curr_stat >= MAX || curr_stat < 0) break;
			
			//if this engineFlag is active then return this stat
		if ((engineFlags & ((uint32_t)1 << ((curr_stat * 4) + s))) != 0)
		  {
			  printf("custom stat: %hd , curr_engine %d\n",custom_stat[s], curr_stat);
			  return custom_stat[s];
		  }
			  
	    }
	
	return 0;
	
	
};

void Custom_VehBirth_SetConsts(struct Driver* driver)
{
	unsigned char metaPhysSize;
	short metaPhysOffset;
	int engineStat;
	unsigned char i;
	char customStat_index;
	

	
	struct MetaPhys* metaPhys;
    
	
	//og engineID
	int engineID = data.MetaDataCharacters[data.characterIDs[driver->driverID]].engineID;
	
	//custom stats
	static const short accel_no_reserves[4] = {480, 544, 448, 512};
	static const short speed_stat[4] = {13140, 13520, 13900, 12950};
	static const short speed_unk[4] = {14640, 15020, 15400, 14450};
	static const short turnRate[4] = {28, 26, 24, 30};
	static const short turnDecreaseRate[4] = {4080, 9080, 5666, 7252};
	static const short turnInputDelay[4] = {5000, 4500, 4000, 5500};
	
	//array ptr to the stats
	static short* newStats[6] = {accel_no_reserves, speed_stat, speed_unk, turnRate, turnDecreaseRate, turnInputDelay};
	
	

	for(i = 0; i < NUM_PHYS_TYPES; i++)
	{
		
		//get metaphys struct
		metaPhys = &data.metaPhys[i];
		
		//get stat offset from metaphys struct
        metaPhysOffset = GetRetailMetaPhys(metaPhys->offset);

		//get stat size from metaphys struct
		metaPhysSize = GetRetailMetaPhys(metaPhys->size);
		
		//if this is not the stat we want to change it returns -1, otherwise it returns the metaphys index
		customStat_index = MetaIndex(i);
		
		//get custom engine stat
		engineStat = getvehicleStats(GetStatIndex(customStat_index), newStats[customStat_index]);
		
		//if cant find a custom stat for the current stat
		engineStat = (customStat_index == -1 || engineStat == 0) ? GetRetailMetaPhys(metaPhys->value[engineID]) : engineStat;
    
		//param_1 = custom_value, param_2 = struct driver*, param_3 = data.metaPhys[index].offset, param_4 = data.metaPhys[index].size
		VehStat_Update(engineStat, driver, metaPhysOffset, metaPhysSize);
	
	}

	

	return;
}