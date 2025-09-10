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
#include "changecamera.c"
#include "setnextcamera.c"
#include "spectator_icons.c"

#include "Class_stats_Box.c"

// Unlimited Gamemodes //////////////////////////////////////////

// Retro Fueled
#include "GameModes/RetroFueled/retro_fueled_bluefire.c"

//Mirror
#include "GameModes/MirrorMode/mirror.c"
#include "GameModes/MirrorMode/mirror_pad_remap.c"

//Shortcutless
#include "GameModes/Shortcutless/shortcutless.c"

// N_Verted
#include "GameModes/N-Verted/n_verted.c"
#include "GameModes/N-Verted/n_verted_driver_state.c"
#include "GameModes/N-Verted/n_verted_lap_skip_prevention.c"

//Stats Modifiers (Gravity, Speed, etc...)
#include "GameModes/stats_modifiers.c"

//Level Modifiers
#include "GameModes/LevelModifiers/level_modifiers.c"

#include "GameModes/dynamic_light.c"

// Item Chaos
#include "GameModes/ItemChaos/item_chaos.c"

// Survival
#include "GameModes/Survival/survival_mode.c"

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

void DisableL2(bool disabled){       
	// if forced camera mode change L2 to R2 else use L2 as L2
	data.gamepadMapBtn[8].output = disabled ? BTN_R2 : BTN_L2_one; 
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
    if (USE_ITEMLESS) {
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

//
void GhostifyWheels(){
	struct Turbo *turboObj;
	struct Thread *fireThread;
	struct GameTracker *gGT = sdata->gGT;
	struct Icon **ptrIconArray;
	struct Instance *inst;

	for (int driverID = 1; driverID < MAX_NUM_PLAYERS; driverID++)	{
		gGT->drivers[driverID]->wheelSprites = ICONGROUP_GETICONS(gGT->iconGroup[0xC]);
	}
}

void GhostifyDrivers()
{
	struct Turbo *turboObj;
	struct Thread *fireThread;
	struct GameTracker *gGT = sdata->gGT;
	struct Icon **ptrIconArray;
	struct Instance *inst;

	for (int driverID = 1; driverID < MAX_NUM_PLAYERS; driverID++)
	{
		// gGT->drivers[driverID]->wheelSprites = ICONGROUP_GETICONS(gGT->iconGroup[0xC]);
		inst = gGT->drivers[driverID]->instSelf;
		if (!inst) { continue; }
		inst->flags |= 0x60000;
		inst->alphaScale = 0xA00;
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

bool USE_NORMAL;
bool USE_MIRROR;
bool USE_ICY_TRACKS;
bool USE_ITEMLESS;
bool USE_MOON_GRAVITY;
bool USE_RETRO_FUELED;
bool USE_FIRST_PERSON;
bool USE_BOSS_RACE;
bool USE_DEMO_CAMERA;
bool USE_N_VERTED;
bool USE_SHORTCUTLESS;
bool USE_NIGHT_FILTER;
bool USE_ITEM_CHAOS;
bool USE_SURVIVAL;
bool USE_SURVIVAL_TIMER;

bool air_throw;
int activeDriversCount = 0;

char* decalText = (char*)0x1F800000;
struct GameTracker *gGT;

static bool init_initialized = false;
static bool initialized = false;

extern int NightFilterBrightness;
extern int NightFilterBlueTint;

void SetGamemodes() {
	USE_NORMAL = octr->gamemodes[NORMAL];
	USE_MIRROR = octr->gamemodes[MIRROR];
	USE_ICY_TRACKS = octr->gamemodes[ICY_TRACKS];
	USE_ITEMLESS = octr->gamemodes[ITEMLESS];
	USE_MOON_GRAVITY = octr->gamemodes[MOON_MODE];
	USE_RETRO_FUELED = octr->gamemodes[RETRO_FUELED];
	USE_FIRST_PERSON = octr->gamemodes[FIRST_PERSON];
	USE_BOSS_RACE = octr->gamemodes[BOSS_RACE];
	USE_DEMO_CAMERA = octr->gamemodes[DEMO_CAMERA];
	USE_N_VERTED = octr->gamemodes[N_VERTED];
	USE_SHORTCUTLESS = octr->gamemodes[SHORTCUTLESS];
	USE_NIGHT_FILTER = octr->gamemodes[NIGHT] || octr->gamemodes[DARKNESS];
	USE_ITEM_CHAOS = octr->gamemodes[ITEM_CHAOS];
	USE_SURVIVAL = octr->gamemodes[SURVIVAL];
	USE_SURVIVAL_TIMER = octr->gamemodes[SURVIVAL_TIMER];
}

bool ItsOnlyNormalEnabled(){
	for (int i = 0; i < sizeof(octr->gamemodes) / sizeof(octr->gamemodes[0]); i++) {
		if (i != NORMAL && octr->gamemodes[i]) {
			return false;
		}
	}
	return true;
}

// Code to run once on game init
void RunGamemodesInitHook() {
    if (init_initialized) return;
    gGT = sdata->gGT;

    SetGamemodes();

    init_initialized = true;
}

bool room_has_retro_fueled;

// Code to run each frame
void RunGamemodesUpdateHook() {
    // Update with the new array of booleans
    SetGamemodes();

	if(octr->gamemodes[NIGHT]){
		NightFilterBrightness = 64;
		NightFilterBlueTint = 15;
	}else if(octr->gamemodes[DARKNESS]){
		NightFilterBrightness = 5;
		NightFilterBlueTint = 0;
	}else{
		NightFilterBrightness = 255;
		NightFilterBlueTint = 0;
	}

	// Restore d-pad input and L2 button if not in race
    if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING))
        RestoreDpadMapping();
		DisableL2(false);
		
	//Only run if game is not paused
	if ((gGT->gameMode1 & PAUSE_ALL) != 0) return;
		
	struct Driver* driver = sdata->gGT->drivers[0];

	if (gGT->gameMode1 & END_OF_RACE) {

		// if game already ended revert first person
		if(USE_FIRST_PERSON)
			gGT->cameraDC[0].cameraMode = 0;
			driver->instSelf->flags &= ~0x80;
	}

    // if the game is not in a race then quit
	if (sdata->gGT->gameMode1 & (START_OF_RACE | MAIN_MENU | END_OF_RACE | GAME_CUTSCENE | LOADING)){
		initialized = false;
		return;
	}

    // Run once before the race starts (should be replaced with a hook injection)
    // ---------------------------------------------------------------------------------------------
    if (gGT->trafficLightsTimer > 0 && !initialized) {

		int numDead = 0;
		for(int i = 0; i < octr->NumDrivers; i++)
			if(octr->nameBuffer[i][0] == 0)
				numDead++;
				
    	activeDriversCount = octr->NumDrivers - numDead;

		// Reset timers and skip prevention structs
        if(!USE_N_VERTED){
            InitShortcutless(USE_SHORTCUTLESS);
        }

        // Init driver floating states and lap skip prevention
        Init_N_Verted(USE_N_VERTED);

		if(USE_NIGHT_FILTER && NightFilterBrightness < 20){
            InitDynamicLighting(gGT->level1);
        }

		InitItemChaos(USE_ITEM_CHAOS);

		InitBossRace(USE_BOSS_RACE);

		InitSurvivalMode(USE_SURVIVAL);

		if(!USE_SURVIVAL){
			InitTimeBasedSurvivalMode(USE_SURVIVAL_TIMER);
		}

		if(USE_ITEMLESS && !USE_MIRROR)
			GhostifyWheels();

		room_has_retro_fueled = USE_RETRO_FUELED;

		if(!USE_DEMO_CAMERA){
			extern bool DemoCameraSpectator;
			DemoCameraSpectator = false;
		}

		// Stats modifiers
        SetGravity(USE_MOON_GRAVITY ? 369 : 900);
        ApplyModifiers();
		
		//If mirror mode flip wumpa shine
        if(USE_MIRROR){
            data.hud_1P_P1[0xC].x = 0xAA;
        }else{
	        data.hud_1P_P1[0xC].x = 286;
        }

		air_throw = false;

        initialized = true;
    }
    // Reset flag when timer is not > 0
    else if (gGT->trafficLightsTimer <= 0) {
        initialized = false;
    }
    // ---------------------------------------------------------------------------------------------

	if(USE_FIRST_PERSON){
		// *(char*)0x80098052 = 0x10;
		gGT->cameraDC[0].cameraMode = 0x10;

		// Make player invisible
		driver->instSelf->flags |= 0x80;
	}

	DisableL2(USE_DEMO_CAMERA || USE_FIRST_PERSON);

	//Disable super engine if player is in first place (check all drivers)
	for (int i = 0; i < MAX_NUM_PLAYERS; i++) {
		struct Driver* d = sdata->gGT->drivers[i];
		if (!d) continue;
		if (d->driverRank < 1 && d->superEngineTimer > 0) {
			d->superEngineTimer = 0;
		}
	}
	//If using an oxide item enable retro fueled while using it
	if(!room_has_retro_fueled){
		USE_RETRO_FUELED = driver->superEngineTimer > 0;
	}
	//If first place and helding an oxide item remove it
	if (driver->driverRank < 1 && driver->heldItemID == ITEM_SUPER_ENGINE) {
		driver->heldItemID = ITEM_NONE;
	}

	//If using invisibility make player immune
	if(driver->invisibleTimer > 0){
		driver->invincibleTimer = driver->invisibleTimer;
	}

	// Spring item will be replaced by nothing item with custom behavior
	if(driver->heldItemID == ITEM_SPRING){
		driver->heldItemID = ITEM_NOTHING;
	}
    
	// Handle retro fueled bluefire (visuals)
    // Rest of retro fueled logic is on physlinear and vehfire
	if(USE_RETRO_FUELED){
		sdata->gGT->gameMode2 |= CHEAT_TURBOPAD;
	}else{
		sdata->gGT->gameMode2 &= ~CHEAT_TURBOPAD;
	}
	HandleBlueFire(USE_RETRO_FUELED);

	HandleMirrorInput();

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

	//Handle air throw (komodo TNTs)
	if(
		// driver->heldItemID == ITEM_NONE // If no item is held

		// If not holding tnt or beaker
		(driver->heldItemID != ITEM_EXPLOSIVE_CRATE && driver->heldItemID != ITEM_N_BRIO_BEAKER) 
		// If hit a red potion
		|| driver->thCloud != NULL

		// If rolling item
		|| (driver->driverRank > 0 && gGT->gameMode1 & ROLLING_ITEM)
	){
		air_throw = false;
	}
	extern int bossflag;
	bossflag = air_throw ? 2 : 0;

	// Handle item chaos logic
	HandleItemChaos(USE_ITEM_CHAOS);

	// Handle boss race logic
	HandleBossRace(USE_BOSS_RACE);

	// Handle survival mode logic
	HandleSurvivalMode(USE_SURVIVAL);
	if(!USE_SURVIVAL){
		HandleTimeBasedSurvivalMode(USE_SURVIVAL_TIMER);
	}

	// Convert all drivers to ghosts on itemless
	// Doesnt work on mirror mode so skip it if enabled

	// This shouldn't need to be called by frame but otherwise transparency
	// only applies depending on quadblock vcolor
	if (USE_ITEMLESS && !USE_MIRROR)
		GhostifyDrivers();

}

#endif
