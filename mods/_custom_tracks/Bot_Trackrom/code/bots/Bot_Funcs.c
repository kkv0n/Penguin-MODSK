 #include <common.h>
 #include "../header/bot.h"
 #include "../unlimited/itemChaos.c"
 #include "../../mod_settings.c"
 
 
struct custom_bots* curr_path;

static struct curr_PosButtons curr_inputs;

bool botPaused;

void BotPos(Vec3* pos, short* kAngleplusfire, char kartState, struct Driver* bot)
{
   
   short angle = kAngleplusfire[0];
   short reserves = kAngleplusfire[1];
   short fireSpeedCap = kAngleplusfire[2];
   
   
	GAMEPAD_CUSTOM_INPUT(&sdata->gGamepads->gamepad[bot->driverID], true);
   
    //current pos
    bot->posCurr.x = pos->x;
	bot->posCurr.y = pos->y;
	bot->posCurr.z = pos->z;
    
	
	
    
    //ConvertRotToMatrix(&sdata->gGT->drivers[1]->instSelf->matrix.m, &sdata->gGT->drivers[1]->rotCurr.x);
    
    //ConvertRotToMatrix(&sdata->gGT->drivers[1]->instSelf->matrix.m, &sdata->gGT->drivers[1]->rotCurr.x);
    
    //gte_SetRotMatrix(m);
	//gte_SetTransMatrix(m);
    

   
    //prev frame pos
    bot->posPrev.x = pos->x;
	bot->posPrev.y = pos->y;
	bot->posPrev.z = pos->z;
     

    bot->angle = angle;
    bot->reserves = reserves;
    bot->fireSpeedCap = fireSpeedCap;
	
	if (kartState != -1)
	bot->kartState = kartState; 
}

//store this outside
short curr_frame;
static bool show_info = false;


void CleanBotInputs(unsigned char driverID)
{
	sdata->gGamepads->gamepad[driverID].buttonsTapped = 0;
	sdata->gGamepads->gamepad[driverID].buttonsReleased = 0;
	sdata->gGamepads->gamepad[driverID].buttonsHeldCurrFrame = 0;
}

void ShowBotPathInfo(struct Driver* bot)
{
	if (bot == NULL || !RaceFlag_IsFullyOffScreen())
	{
		show_info = false;

		goto resetCamera;
	}
	
	if ((sdata->gGamepads->gamepad[0].buttonsTapped & BTN_SELECT) != 0)
	{
		show_info ^= true;
	}
			
	
	if (show_info)
	{
		if (gGT->cameraDC[0].driverToFollow == NULL) return;
		
		char frames[9];
	
		sprintf(frames, "%hd", curr_frame);
		
		if (gGT->cameraDC[0].driverToFollow != bot)
			gGT->cameraDC[0].driverToFollow = bot;
		
		DecalFont_DrawLine(frames, 150, 188, FONT_SMALL, JUSTIFY_CENTER | ORANGE);
	}
	else
	{
		resetCamera:
		
		if (gGT->cameraDC[0].driverToFollow != gGT->drivers[0])
			gGT->cameraDC[0].driverToFollow = gGT->drivers[0];
	}
}


int Decompress_Pos(short value)
{
	int pos = value << 8;
	
	return pos;
};

int Decompress_Btn(unsigned char buttonsHeld)
{
	int currButtons;
	unsigned char i;
	
	int buttonFlags[8] = {
		BTN_UP,
		BTN_DOWN,
		BTN_LEFT,
		BTN_RIGHT,
		BTN_CROSS,
		BTN_SQUARE,
		BTN_L1,
		BTN_R1};
				
				
		for (i = 0; i < 8; i++)
		{
			if ((buttonsHeld & (1 << i)) != 0)
				currButtons |= buttonFlags[i];
		}
		
	return currButtons;
}



void UpdateCurrInputs()
{
	//Decompress
	curr_inputs.pos.x = Decompress_Pos(curr_path[curr_frame].pos.x);
	curr_inputs.pos.y = Decompress_Pos(curr_path[curr_frame].pos.y);
	curr_inputs.pos.z = Decompress_Pos(curr_path[curr_frame].pos.z);		
	curr_inputs.buttonsHeldCurrFrame = Decompress_Btn(curr_path[curr_frame].buttonsHeldCurrFrame);
}

void ExplodeMines()
{
	struct Thread* thread = gGT->threadBuckets[MINE].thread;
	
	if (thread == NULL) return;

	for (
			struct Thread* mines = thread->siblingThread;
			mines != NULL; mines = mines->siblingThread
		)
	{
		RB_GenericMine_ThDestroy(mines, mines->inst, (struct MineWeapon*)mines->object);
	}

}

void RunPath()
{
    struct GameTracker* gGT = sdata->gGT;
    struct Driver* bot = gGT->drivers[1];
    struct QuadBlock* respawn = bot->lastValid;
    struct QuadBlock* currBlock = bot->currBlockTouching;
    int trafficLights = gGT->trafficLightsTimer;
    int gameMode = gGT->gameMode1; 
	unsigned short currQuadFlags = currBlock->quadFlags;
    unsigned char levelID = gGT->levelID;
    unsigned char kartState = bot->kartState;
    static short lastValidFrame;
    
    static int prevElapsed;
	
	static char prevLap;
	
	static bool Fall;
	

	unsigned short NUM_FRAMES = *(int*)(0x703720 ADD_PSX_ADDRESS);
	unsigned char currLap = bot->lapIndex;
	
	
	if (NUM_FRAMES == 0 || curr_path == NULL) return;
	
         
   //fps count for the bot path
    unsigned char FPS_COUNT = FPS_DOUBLE(1);
	unsigned char FPS_SUM = FPS_DOUBLE(1);
	


	
	
    
    //reset everything
	if (trafficLights > 0)
	{
		curr_frame = 0;
		prevElapsed = 0;
        lastValidFrame = 0;
        prevElapsed = 0;
		prevLap = 0;
		Fall = false;
		
		if (!ghostMode)
			ItemChaos_Init(true);
		
		//decompress
		UpdateCurrInputs();
	}
	

	
	

         
       //quit if conditions dont meet
	if (SKIP_BOT_PATH(trafficLights, levelID, gameMode, bot->actionsFlagSet))
	{
		CleanBotInputs(bot->driverID);
		return;
	}
	
	
	
    
    if (!ghostMode) //only if item mode
    {
		
		if (bot->invincibleTimer != 0)
			bot->instSelf->thread->flags |= 0x1000;
		else
			bot->instSelf->thread->flags &= ~(0x1000);
	
        //if mask grab animation or being hit by items/hazards
        if (PAUSE_BOT_PATH(kartState))
        {
			
			botPaused = true; // avoid item chaos bugs
			
			bot->invincibleTimer = SECONDS(1);
			
			
			
			
			if (kartState == KS_BLASTED)
			{	
				bot->posCurr.x = curr_inputs.pos.x;
				bot->posCurr.z = curr_inputs.pos.z;
				CleanBotInputs(bot->driverID);
				return;
			}
			else if (kartState == KS_SPINNING)
			{
				FPS_COUNT = FPS_DOUBLE(2);
			}
			

			
        }
        
		
        //if(bot->NoInputTimer > 0)
		//	return;
	



	
    //if mask grab then wait until the bot touch the ground and go back to last valid frame before you fell to the void
    if (BOT_MASK_GRABBED(kartState) || bot->underDriver == NULL)
    {
        curr_frame = lastValidFrame;
		bot->invincibleTimer = SECONDS(2);
		botPaused = true;
		UpdateCurrInputs();
		CleanBotInputs(bot->driverID);
        return;
    }
    else if (respawn != NULL && currBlock->blockID == respawn->blockID) //if this quadblock have a valid checkpoint then save this for future mask grabs
    {
        lastValidFrame = curr_frame;
    }
	
	if ((bot->actionsFlagSet & ACTION_TOUCH_GROUND) == 0)
	{
		if (kartState == KS_SPINNING || bot->clockReceive > 0)
		{
			if (!Fall)
			{
			
				Fall = true;
			}
		}
		
		if (botPaused)
		{
			CleanBotInputs(bot->driverID);
			return;
		}

	}
	else
	{

		
		if (Fall)
		{
			Fall = false;
		}
		

			
	}
	
    botPaused = false; //avoid itemchaos bugs
    
    if (bot->clockReceive > 0)
	{
        FPS_COUNT = FPS_DOUBLE(3); //if clock is active then make the bot slower
		
		if (bot->clockReceive > SECONDS(6)) 
			bot->clockReceive = SECONDS(6);
		
	}
		
		//if (bot->lapIndex == (gGT->numLaps - 1) && bot->clockReceive < 1) //too hard for simple mortals
		//	FPS_SUM = FPS_DOUBLE(2);
		
		//if (bot->clockReceive > 4800) return;
		
		
		
		if (Fall)
		{

			bot->posCurr.x = curr_inputs.pos.x;
			bot->posCurr.z = curr_inputs.pos.z;	
			CleanBotInputs(bot->driverID);
			return;
		}
		
			

			
	}
	
	
    
	//30 fps per second
	int ElapsedFrame = (gGT->timer / FPS_COUNT);
	
	if (prevElapsed != ElapsedFrame && curr_frame > 0)
	{
		prevElapsed = ElapsedFrame; //store elapsedframes
        
		

		curr_frame = (FPS_DOUBLE(curr_frame + FPS_SUM)) % NUM_FRAMES; //custom timer count
		
		
		//decompress
		UpdateCurrInputs();
	}
	    
    short kAngleplusfire[3] = {curr_path[curr_frame].angle, curr_path[curr_frame].reserves, curr_path[curr_frame].fireSpeedCap};
	
	


    //run bot path
    BotPos(&curr_inputs.pos, &kAngleplusfire, curr_path[curr_frame].kartState, bot);
	

	
	if (curr_frame == 0)
	{
		if (kAngleplusfire[1] > 0)
			VehFire_Increment(bot, 0x2d0, 1, 0x180);
	}
	
	
	
	
	if (!ghostMode && ((bot->actionsFlagSet & ACTION_TOUCH_GROUND) != 0))
	{

		//explode all mine weapons from last lap
		if (prevLap != bot->lapIndex)
		{
			ExplodeMines();
			prevLap = bot->lapIndex;
		}
		
		void HandleItemChaos(bool enabled);
		HandleItemChaos(true);
		

	}
	
    //0 counts as a frame
    if (curr_frame == 0 && prevElapsed != ElapsedFrame)
	{
        curr_frame++;
		UpdateCurrInputs();
	}


}



void GAMEPAD_CUSTOM_INPUT(struct GamepadBuffer *pad, bool isBot) //this one is used for humans too
{
    struct GameTracker* gGT = sdata->gGT;
    short trafficLights;
    unsigned char levelID;
    int gameMode = gGT->gameMode1;
	unsigned char i;
    
    levelID = gGT->levelID;
    trafficLights = gGT->trafficLightsTimer;
    
	
	if((gameMode & LOADING) == 0)
	{
		
		//current button pressed by the bot
		int currButton;
        
		
        //if player 1
		if(!isBot)
		{
			// value pressed
			currButton = pad->buttonsHeldCurrFrame;
			
			
			if (pad->ptrControllerPacket == NULL)
			{
				// erase tap and release
				pad->buttonsTapped = 0;
				pad->buttonsReleased = 0;
			}
			else
			{
				// tapped
				pad->buttonsTapped = ~pad->buttonsHeldPrevFrame & pad->buttonsHeldCurrFrame;

				// released
				pad->buttonsReleased = pad->buttonsHeldPrevFrame & ~pad->buttonsHeldCurrFrame;
			}
		
		}
		
		// if bots
		else
		{
				pad->ptrControllerPacket = sdata->gGamepads->gamepad[0].ptrControllerPacket;

                
            
				// button Pressed by the bot
				currButton = curr_inputs.buttonsHeldCurrFrame;
            
		

				pad->buttonsHeldCurrFrame = currButton;
			
				// In this order: Up, Down, Left, Right
				if ((currButton & BTN_UP) != 0) pad->stickLY = 0;
				if ((currButton & BTN_DOWN) != 0) pad->stickLY = 0xFF;
				if ((currButton & BTN_LEFT) != 0) pad->stickLX = 0;
				if ((currButton & BTN_RIGHT) != 0) pad->stickLX = 0xFF;
				
				
				// tapped
				pad->buttonsTapped = ~pad->buttonsHeldPrevFrame & pad->buttonsHeldCurrFrame;

				// released
				pad->buttonsReleased = pad->buttonsHeldPrevFrame & ~pad->buttonsHeldCurrFrame;
			
        }

	}
      
}


void BOT_TIME_TRIAL(struct Driver* bot)
{
    struct Icon** transparentTires;
    struct Instance* botInst = bot->instSelf;
	
	//disable collision for this driver
	bot->instSelf->thread->flags |= 0x1000;
    
	//set transparent tires
	if (sdata->gGT->iconGroup[0xC] != NULL)
	{
		transparentTires = ICONGROUP_GETICONS(sdata->gGT->iconGroup[0xC]);
		
    if (bot->wheelSprites != transparentTires)
        bot->wheelSprites = transparentTires;
	}
    
	//set ghost transparency flag
	if ((botInst->flags & GHOST_DRAW_TRANSPARENT) == 0)
		botInst->flags |= GHOST_DRAW_TRANSPARENT;
	
	//change instance alpha
	if (botInst->alphaScale != 0xA00)
		botInst->alphaScale = 0xA00;
}

void LOAD_BOT_PATH(bool ghost) //dynamically swap files
{
	static bool loaded[2];
	
	int ramSize;
	
	if (!loaded[0] && ghost) //if ghost is required and was not loaded
	{
		LOAD_XnfFile("\\CGHOST.BIN;1", (void*)(0x703720 ADD_PSX_ADDRESS), &ramSize);
		loaded[0] = true;
		loaded[1] = false;
	}
    
	if (!loaded[1] && !ghost) //if bot is required and was not loaded
	{
		LOAD_XnfFile("\\CBOT.BIN;1", (void*)(0x703720 ADD_PSX_ADDRESS), &ramSize);
		loaded[1] = true;
		loaded[0] = false;
	}
}

void SET_BOT_SETTINGS(bool ghost, struct GameTracker* gGT)
{
    struct Driver* bot = gGT->drivers[1];
    struct Data* CTR = &data;
	
	
	//set AI exhaust and skip noises from this kart
	bot->instSelf->thread->modelIndex = (ghost) ? DYNAMIC_GHOST : DYNAMIC_ROBOT_CAR;
	
	if (sdata->Loading.stage == -1 && gGT->levelID < GEM_STONE_VALLEY)
	{
		DrawOverheadNames(CTR);
				
		if (ghost)
		{
			if (bot != NULL)
				BOT_TIME_TRIAL(bot);
		}
		
		return;
	}
	

	gGT->numPlyrCurrGame = 1;
	gGT->numPlyrNextGame = 1;
	gGT->bossID = 1;
	
	CTR->characterIDs[bot->driverID] = *(short*)((0x703720 + 4) ADD_PSX_ADDRESS) % 16;
	
	if (CTR->characterIDs[bot->driverID] < 0)
		CTR->characterIDs[bot->driverID] = 0;

	curr_path = (struct custom_bots*)((0x703720 + 8) ADD_PSX_ADDRESS);

	


}



struct WorldPos
{
	short World_posX;
	short World_posY;
	short World_posZ;
	short World_posW;

	short Screen_posX;
	short Screen_posY;

	int Screen_posZ;
};

int GetOverheadLen(struct Driver* bot)
{
	struct WorldPos* ptrDest = (struct WorldPos*)0x1f800108;
	ptrDest->World_posX = bot->posCurr.x >> 8;
	ptrDest->World_posY = (bot->posCurr.y >> 8) + 75;
	ptrDest->World_posZ = bot->posCurr.z >> 8;
	ptrDest->World_posW = 0;

	gte_ldv0(&ptrDest->World_posX);

	gte_rtps();
	gte_stsxy(&ptrDest->Screen_posX);
	gte_stsz(&ptrDest->Screen_posZ);

	int posZ = ptrDest->Screen_posZ;

	if(posZ < 150) return 0;
	if(posZ < 190) return 9;
	if(posZ < 234) return 8;
	if(posZ < 331) return 7;
	if(posZ < 475) return 6;
	if(posZ < 906) return 5;

return 3;
}

void DrawOverheadNames(struct Data* CTR)
{
	MATRIX* m;
    


	struct GameTracker* gGT = sdata->gGT;
	struct WorldPos* ptrDest = (struct WorldPos*)0x1f800108;
    struct Driver* bot = gGT->drivers[1];
    
    if ((gGT->drivers[0]->actionsFlagSet & ACTION_RACE_FINISHED) != 0) return; //if human player finished

	// pushBuffer offset 0x28
	m = &gGT->pushBuffer[0].matrix_ViewProj;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
	
	unsigned char colorIndex = CTR->characterIDs[bot->driverID] + 5; //characterID + 5

	int color = (JUSTIFY_CENTER | colorIndex);



		int len = GetOverheadLen(bot);
		if(len == 0) return;
        
        
       data.font_charPixWidth[FONT_SMALL] = WIDE_34(len + 4); 
		DecalFont_DrawLineStrlen(
			bot_name,
			9,
			ptrDest->Screen_posX,
			ptrDest->Screen_posY-0x4,
			FONT_SMALL, color);
            data.font_charPixWidth[FONT_SMALL] =  WIDE_34(13);
}


void CustomBot_Thtick(struct GameTracker* gGT)
{

	if ((gGT->gameMode1 & PAUSE_ALL) != 0) return;
	
    SET_BOT_SETTINGS(ghostMode, gGT);
	RunPath();
}