// Gasmox(part 1)
#include <common.h>
#include "utils.h"

#ifdef USE_GASMOXIAN


#include "global.h"
#include "lap1.c"
#include "lap2.c"
#include "hooks.c"
#include "menu.c"
#include "states.c"
#include "thread.c"
#include "getoverheadlen.c"
#include "drawoverheadnames.c"
#include "drawoverheadcalibration.c"
#include "updatetimes.c"
#include "bluefire.c"
#include "changecamera.c"
#include "setnextcamera.c"
#include "zMirrorMode.c"
#include "spectator_icons.c"
#include "pMoonGravity.c"

// Unlimited Gamemodes //////////////////////////////////////////

//Shortcutless
#include "GameModes/Shortcutless/shortcutless.c"

// N_Verted
#include "GameModes/N-Verted/n_verted.c"
#include "GameModes/N-Verted/n_verted_driver_state.c"
#include "GameModes/N-Verted/n_verted_lap_skip_prevention.c"

//Level Modifiers
#include "GameModes/LevelModifiers/level_modifiers.c"

#include "GameModes/dynamic_light.c"

unsigned int checkpointTimes[(MAX_LAPS * CPS_PER_LAP) + 1];

int shouldExecuteSpecText = 0;

const char* spec_mode;

//laps visual number
    const char* options[16] = { "1", "3", "5", "7", "10", "15", "20", "25", 
                                "30", "35", "40", "50", "69", "80", "90", "127" };
								


#ifdef GASMOX_ENG
const char specting[] = "SPECTATING...";
const char finish_race[] = "FINISHING RACE!";

	const char s_switchCam[] = "Press R1 or L1 to change the camera";
    const char s_gg[] = "GG Thanks For Playing!";
	
#elif defined(GASMOX_ES)
	const char specting[] = "ESPECTEANDO..";
const char finish_race[] = "ACABANDO CARRERA!";

	const char s_switchCam[] = "Presiona L1 o R1 para cambiar camara";
    const char s_gg[] = "GG Gracias por jugar!";
	
#elif defined(GASMOX_BR)
	const char specting[] = "ESPECTANDO...";
const char finish_race[] = "FINALIZANDO CORRIDA!";

	const char s_switchCam[] = "Pressione R1 ou L1 para alternar cam";
    const char s_gg[] = "Terminado,Boa Corrida!!";
#endif
								
void spec_text() {
        spec_mode = shouldExecuteSpecText ? specting : finish_race;
        static unsigned frameCounter = 0;
        int spec_color = frameCounter++ & FPS_DOUBLE(1) ? ORANGE : WHITE;
        DECOMP_DecalFont_DrawLine(spec_mode, 0x100, 0x74, FONT_SMALL, JUSTIFY_CENTER | spec_color);

    
}

void ban_demo_skip()
{       

if (octr->special == DEMO_CAMERA) {
    data.gamepadMapBtn[8].output = BTN_R2; // if demo camera mode change L2 to R2
}

	if (octr->CurrState < LOBBY_WAIT_FOR_LOADING)
	{
	  data.gamepadMapBtn[8].output = BTN_L2_one; // if not in demo camera mode use L2 as L2

	} 
	
	return;
}

void queuetojoin(){
	
	if (octr->autoRetryJoinRoomIndex != -1)
	{
		#ifdef GASMOX_ENG
		char* wtj = "Joining, please wait...";
		#elif defined(GASMOX_ES)
		char* wtj = "entrando, espera.. ->";
		#elif defined(GASMOX_BR)
		char* wtj = "na fila, aguarde. ->";
		
		#endif
		int lineInd = octr->autoRetryJoinRoomIndex % 8;
		int pageNum = octr->autoRetryJoinRoomIndex / 8;
		if (pageNum == octr->PageNumber)
			DecalFont_DrawLine(wtj, 0xE0 -120, 0x6b + (lineInd * 8), FONT_SMALL, JUSTIFY_CENTER | PURA_VIOLET);
		if (octr->clientCount[octr->autoRetryJoinRoomIndex] <= 7)
		{
			octr->serverRoom = octr->autoRetryJoinRoomIndex;
			octr->serverLockIn2 = 1;
			octr->autoRetryJoinRoomIndex = -1;
			//0 = cursor move noise
			//1 = cursor select noise
			//2 = triangle noise
			//3 = Ding!
			//4 = different ding!
			//5 = womp
			//6 = slammed into wall sfx
			//...
			DECOMP_OtherFX_Play(3, 1); //play SFX to notify user
		}
	}	
}


void Online_CollidePointWithBucket(struct Thread* th, short* vec3_pos)
{
    // disable collisions in special 3 
    if (octr->special == ITEMLESS) { 
        return;
    }
	else 
	{
	while (th != 0)
	{
		DECOMP_PROC_CollidePointWithSelf(th, vec3_pos);
		// next
		th = th->siblingThread;
	}
  }
}




typedef void (*VehicleFuncPtr)(struct Thread* thread, struct Driver* driver);

void RunVehicleThread(VehicleFuncPtr func, struct Thread* thread, struct Driver* driver)
{
    UpdateCheckpointTracker(driver->driverID);
    if (func == nullptr) { return; }

    bool restore = false;
    if ((sdata->gGT->gameMode1 & END_OF_RACE) && (checkpointTracker[driver->driverID].raceFinished == 0))
    {
        sdata->gGT->gameMode1 &= ~(END_OF_RACE);
        restore = true;
    }
    func(thread, driver);
    if (restore) { sdata->gGT->gameMode1 |= END_OF_RACE; }
}

void RunVehicleSet13(struct Thread* dThread, struct Driver* dOnline)
{
	if(dThread->funcThTick != 0)
		return;

	// This only works because we have NO collision
	// and therefore NO bounce physics, update P1
	// first, then update other drivers later
	
	// We can still "eventually" have mask-weapon
	// collision, but no "bounce" physics with this logic
		
	for(int iVar11 = 0; iVar11 < 13; iVar11++)
	{
		int pcVar5 = dOnline->funcPtrs[iVar11];
		
		if(pcVar5 == 0)
			continue;
		
		RunVehicleThread(pcVar5, dThread, dOnline);
		
		#ifdef USE_60FPS
			#ifndef REBUILD_PS1
				// if this function just ran
				if(pcVar5 == VehFrameProc_Driving)
				{
					// only if jumping animation,
					// otherwise wheelie gets bugged
					if(dOnline->instSelf->animIndex == 3)
					{
						dOnline->matrixIndex =
						dOnline->matrixIndex >> 1;
					}
				}
			#endif
		#endif
	}
}


void ITEMLESS_MAIN(struct GameTracker* gGT)
{
	
 for (unsigned char i = 1; i < 8; i++)
 {
	 
	 if (gGT->drivers[i] == 0 || gGT->drivers[i] == NULL) continue;
	
		gGT->drivers[i]->instSelf->flags |= GHOST_DRAW_TRANSPARENT;
		
		gGT->drivers[i]->instSelf->alphaScale = 0xA00;
		gGT->drivers[i]->wheelSprites = ICONGROUP_GETICONS(gGT->iconGroup[0xc]);
 }
	
}


extern Color HsvToRgb(int h, int s, int v);

Color HsvToRgb(int h, int s, int v)
{
    Color rgb;
	h = h & 0xFF; // modulo 256
    int region, remainder, p, q, t;

    if (s == 0)
    {
        rgb.r = v;
        rgb.g = v;
        rgb.b = v;
        return rgb;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v;
            break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

void AssignMeterGrade(struct Driver * driver, int meterLeft)
{
	const int gradeTreshold[] = {SECONDS(1) * FP(0.50), SECONDS(1) * FP(0.65),
						SECONDS(1) * FP(0.80), SECONDS(1) * FP(0.90),
						SECONDS(1) * FP(0.95)};
	const int gradeColors[] = {CORTEX_RED, ROO_ORANGE, CRASH_BLUE, TROPY_LIGHT_BLUE, PAPU_YELLOW};

	driver->meterGradeTimer = SECONDS(0.5);
	driver->meterGrade[1] = '\0';

	char grades[] = "FDCBA";
	for (int i = 0; i < len(gradeTreshold); i++)
	{
		if (meterLeft > SECONDS(1) - FP_INT(gradeTreshold[i]))
		{
			driver->meterGrade[0] = grades[i];
			driver->gradeColor = gradeColors[i];
			return;
		}
	}
	driver->meterGrade[0] = 'S';
	driver->gradeColor = SILVER;
}


void FixReservesIncrement(struct Driver * driver, int reserves)
{
	if (driver->reserves > 30000) { driver->uncappedReserves += reserves; }
	else { driver->reserves += reserves; }
}




//saffi available in room 15
void saffi_fire(struct Driver * driver, int reserves) {
	int oldOTT;
	
	oldOTT = driver->turbo_outsideTimer;
	
	if (oldOTT < reserves)
		{
			if (octr->serverRoom == 15) {
				driver->reserves += 			(reserves - oldOTT);
			}
			else
			{
				FixReservesIncrement(driver, reserves - oldOTT);
			}
			driver->turbo_outsideTimer += 	(reserves - oldOTT);
}
}

//saffi available in room 15
void saffi_fire2(struct Driver * driver, int reserves) {
		if (octr->serverRoom == 15) {
		driver->reserves += reserves;
		}
		else
		{
		FixReservesIncrement(driver, reserves);
		}
}

// GAMEMODES HANDLING (From Unlimited)

bool USE_N_VERTED;
bool USE_SHORTCUTLESS;
bool USE_NIGHT_FILTER;
bool USE_ITEM_CHAOS;

bool air_throw;

char* decalText = (char*)0x1F800000;
struct GameTracker *gGT;

static bool init_initialized = false;
static bool initialized = false;

extern int NightFilterBrightness;
extern int NightFilterBlueTint;

// Code to run once on game init
void RunGamemodesInitHook() {
    if (init_initialized) return;
    gGT = sdata->gGT;

	//TODO: octr->special will be deprecated as we need to be able to combine gamemodes and not just 1 enabled
    USE_N_VERTED = octr->special == N_VERTED;
	USE_SHORTCUTLESS = octr->special == SHORTCUTLESS;
	USE_NIGHT_FILTER = octr->special == NIGHT;

    init_initialized = true;
}

//Code to run each frame
void RunGamemodesUpdateHook() {

    USE_N_VERTED = octr->special == N_VERTED;
	USE_SHORTCUTLESS = octr->special == SHORTCUTLESS;
	USE_NIGHT_FILTER = octr->special == NIGHT || octr->special == DARKNESS;
	USE_ITEM_CHAOS = octr->special == ITEM_CHAOS;

	if(octr->special == NIGHT){
		NightFilterBrightness = 64;
		NightFilterBlueTint = 15;
	}else if(octr->special == DARKNESS){
		NightFilterBrightness = 5;
		NightFilterBlueTint = 0;
	}else{
		NightFilterBrightness = 255;
		NightFilterBlueTint = 0;
	}

    //Only run if game is not paused
    if ((gGT->gameMode1 & PAUSE_ALL) != 0) return;

    // if the game is not in a race then quit
	if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING)){
		initialized = false;
		return;
	}

    struct Driver* driver = sdata->gGT->drivers[0];

    // Run once before the race starts (should be replaced with a hook injection)
    // ---------------------------------------------------------------------------------------------
    if (gGT->trafficLightsTimer > 0 && !initialized) {

		// Reset timers and skip prevention structs
        if(!USE_N_VERTED){
            InitShortcutless(USE_SHORTCUTLESS);
        }

        // Init driver floating states and lap skip prevention
        Init_N_Verted(USE_N_VERTED);

		if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
            InitDynamicLighting(gGT->level1);
        }

        initialized = true;
    }
    // Reset flag when timer is not > 0
    else if (gGT->trafficLightsTimer <= 0) {
        initialized = false;
    }
    // ---------------------------------------------------------------------------------------------
    
	// Handle shortcutless logic, detect and prevent shortcuts
    if(!USE_N_VERTED){
        HandleShortcutless(USE_SHORTCUTLESS);
    }

    // Handle N-VERTED logic, which involves jump blocks and prevent lap skips
    Handle_N_Verted(USE_N_VERTED);

	// Handle dynamic lighting
	if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
        HandleDynamicLighting(gGT->level1);
    }

	if(driver->heldItemID == ITEM_NONE
		|| gGT->gameMode1 & ROLLING_ITEM
		|| driver->thCloud != NULL // Hit a red potion
	){
		air_throw = false;
	}

	// if(air_throw){
	// 	sprintf(decalText, "AIR THROW");
	// 	DecalFont_DrawLine(decalText, 0x100, 0xc8, FONT_SMALL, (JUSTIFY_CENTER | RED));
	// }

	extern int bossflag;
	extern int bossrace;
	bossflag = bossrace || air_throw ? 2 : 0;

	if (USE_ITEM_CHAOS && driver != NULL && driver->lapIndex == gGT->numLaps - 1) {
		driver->numWumpas = 99;
	}

}

//Rank 0
char ItemChaos_RNG_itemSetRace1[] = {
	// 1/20 Bomb (1)
	1,
	// 9/20 Crate (3)
	3,3,3,3,3,3,3,3,3,
	// 6/20 Beaker (4)
	4,4,4,4,4,4,
	// 4/20 Shield (6)
	6,6,6,6
};

// Rank 1,2
char ItemChaos_RNG_itemSetRace2[] = {
    // 5/52 Turbo (0)
    0,0,0,0,0,
    // 3/52 Bomb (1)
    1,1,1,
    // 5/52 Missile (2)
    2,2,2,2,2,
    // 14/52 Crate (3)
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,
    // 9/52 Beaker (4)
    4,4,4,4,4,4,4,4,4,
    // 7/52 Shield (6)
    6,6,6,6,6,6,6,6,
    // 3/52 Mask (7)
    7,7,7,
    // 3/52 Warp Orb (9)
    9,9,9,
    // 2/52 Invisibility (12)
    12,12
};

// Rank 3,4
char ItemChaos_RNG_itemSetRace3[] = {
    // 2/20 Turbo (0)
    0,0,
    // 3/20 Missile (2)
    2,2,2,
    // 4/20 Crate (3)
    3,3,3,3,
    // 2/20 Beaker (4)
    4,4,
    // 3/20 Shield (6)
    6,6,6,
    // 2/20 Mask (7)
    7,7,
	// 1/20 Clock (8)
    8,
    // 3/20 Warp Orb (9)
    9,9,9
};

// Rank 5,6
char ItemChaos_RNG_itemSetRace4[] = {
    // 2/20 Turbo (0)
    0,0
    // 1/20 Bomb (1)
    1,
    // 2/20 Missile (2)
    2,2,
    // 1/20 Crate (3)
    3,
    // 6/20 Mask (7)
    7,7,7,7,7,7,
    // 2/20 Clock (8)
    8,8,
    // 5/20 Warp Orb (9)
    9,9,9,9,9,
    // 1/20 Super Engine (13)
    13
};

// Rank 7
char ItemChaos_RNG_itemSetBattleDefault[] = {
    // 1/20 Turbo (0)
    0,
    // 7/20 Mask (7)
    7,7,7,7,7,7,7
    // 2/20 Clock (8)
    8,8,
    // 7/20 Warp Orb (9)
    9,9,9,9,9,9,9,
    // 3/20 Super Engine (13)
    13,13,13
};

extern char* charPtr[7];
void ItemChaosItemSets(void) {

	if(USE_ITEM_CHAOS){
		// Directly modify each element of the global charPtr array
		charPtr[0] = &ItemChaos_RNG_itemSetRace1[0]; // Rank 0
		charPtr[1] = &ItemChaos_RNG_itemSetRace2[0]; // Rank 1,2
		charPtr[2] = &ItemChaos_RNG_itemSetRace3[0]; // Rank 3,4
		charPtr[3] = &ItemChaos_RNG_itemSetRace4[0]; // Rank 5,6
		charPtr[4] = &ItemChaos_RNG_itemSetBattleDefault[0]; // Rank 7
		
		// Leave these as default (unused)
		charPtr[5] = (char*)&sdata_static.gameTracker.battleSetup.RNG_itemSetCustom[0];
		charPtr[6] = &data.RNG_itemSetBossrace[0];
	}else{
		charPtr[0] = &data.RNG_itemSetRace1[0]; // Rank 0
		charPtr[1] = &data.RNG_itemSetRace2[0]; // Rank 1,2
		charPtr[2] = &data.RNG_itemSetRace3[0]; // Rank 3,4
		charPtr[3] = &data.RNG_itemSetRace4[0]; // Rank 5,6
		charPtr[4] = &data.RNG_itemSetBattleDefault[0]; // Rank 7
		charPtr[5] = (char*)&sdata_static.gameTracker.battleSetup.RNG_itemSetCustom[0];
		charPtr[6] = &data.RNG_itemSetBossrace[0];
	}
}

int CountActiveWarpOrbs() {
    int count = 0;
    struct Thread* warpballThread;
    struct Instance* warpballInst;

    // Loop through all threads in the TRACKING bucket
    for (
        warpballThread = gGT->threadBuckets[TRACKING].thread;
        warpballThread != 0;
        warpballThread = warpballThread->siblingThread
    ) {
        warpballInst = warpballThread->inst;

        //If its a warpball       
        if(warpballInst->model->id == DYNAMIC_WARPBALL){
            count++;
        }
    }
    
    return count;
}

void ItemChaosSetHeldItem(struct Driver * driver){

	// if(
	// 	driver->driverRank == octr->NumDrivers - 1
	// 	&&
	// 	(
	// 	driver->heldItemID == ITEM_TRACKING_MISSILE_X3
	// 	|| driver->heldItemID == ITEM_BOWLING_BOMB_X3
	// 	)
	// ){
	// 	driver->heldItemID = ITEM_WARP_ORB;
	// }

	//Increment of item amount
	char increment = driver->lapIndex == gGT->numLaps - 1 ? 1 : 0;

	// Check if there are already maximum warp orbs active
	char maxWarpOrbs = octr->NumDrivers < 6 ? 4 : 3;
	maxWarpOrbs += increment;
	if (driver->heldItemID == ITEM_WARP_ORB && CountActiveWarpOrbs() >= maxWarpOrbs) {
		driver->heldItemID = ITEM_TURBO_BOOST;
	}

	if(driver->heldItemID == ITEM_EXPLOSIVE_CRATE){
		driver->numHeldItems = rand() % 3 + 1 + increment;

		if (rand() % 2 == 0) {
			air_throw = true;
		} else {
			air_throw = false;
		}
	}

	else if(driver->heldItemID == ITEM_N_BRIO_BEAKER){
		driver->numHeldItems = rand() % 5 + 1;
	}

	else if(driver->heldItemID == ITEM_TRACKING_MISSILE){
		driver->numHeldItems = rand() % 5 + 1;
	}

	else if(driver->heldItemID == ITEM_POWER_SHIELD){
		if(octr->NumDrivers > 5){
			// 75% chance of 1, 15% chance of 2, 5% chance of 3
			driver->numHeldItems = (rand() % 100 < 75) ? 1 : (rand() % 100 < 90) ? 2 : 3;
		}else{
			//85% chance of 1, 15% chance of 2
			driver->numHeldItems = (rand() % 100 < 85) ? 1 : 2;
		}
	}

	else if(driver->heldItemID == ITEM_MASK){
		// 85% chance of 1, 10% chance of 2, 5% chance of 3
		driver->numHeldItems = (rand() % 100 < 85) ? 1 : (rand() % 100 < 95) ? 2 : 3 + increment;
	}

	if(octr->NumDrivers < 6){
		if(driver->heldItemID == ITEM_WARP_ORB){
			// 80% chance of 1, 20% chance of 2
			driver->numHeldItems = (rand() % 5 == 0) ? 2 : 1;
		}
	}
}



#endif
