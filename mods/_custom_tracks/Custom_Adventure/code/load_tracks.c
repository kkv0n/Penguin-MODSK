#include <common.h>
#include "adventure.h"

extern int force;

void CDSYS_XAPauseRequest();
void MEMPACK_PopToState(int id);
void LOAD_Callback_PatchMem(struct LoadQueueSlot* lqs);
int MEMPACK_PushState();
void MM_JumpTo_Title_FirstTime();
void MM_JumpTo_Characters();
void MM_JumpTo_TrackSelect();
void MM_JumpTo_BattleSetup();
void CS_Garage_Init();
void MM_JumpTo_Scrapbook();

void CseqMusic_StopAll();
void MEMPACK_NewPack(void* start, int size);
u_int MEMPACK_GetFreeBytes();
void* MEMPACK_AllocHighMem(int allocSize);
u_int RaceFlag_IsFullyOffScreen();
void RaceFlag_BeginTransition(int param_1);

int LOAD_TenStages(struct GameTracker* gGT, int loadingStage, struct BigHeader* bigfile)
{

	u_char numPlyrNextGame;
	short sVar4;
	int iVar5;
	u_int uVar6;
	CdlCB pcVar7;
	u_int* puVar8;
	int iVar9;
	struct Level* lev;
	u_int gameMode1; //-- redundant
	u_char podiumModel;
	int iVar12;
	char* levelNamePtr;
	u_char* moremoredata; //-- redundant
	int* piVar15;
	u_int uVar16;
	u_int uVar17;
	int vramSize;
	int boolDefault1P;
	int levelID;
	int ovrRegion1;
	int ovrRegion3;
	struct Model* m;

	// pointer to LEV
	iVar9 = (int)sdata->ptrLevelFile;

	// if game is loading
	if (sdata->load_inProgress != 0)
	{
		return loadingStage;
	}

	levelID = gGT->levelID;



	unsigned char vrm_array[MAX_TRACKS] = { COVE_VRM, TUBES_VRM, MCAVES_VRM, SKROCK_VRM, SEWER_VRM, SLIDEC_VRM, TURBOT_VRM,
	TEMPLE_VRM, CPARK_VRM, PPYRAMID_VRM, RAMPAGER_VRM, DCANYON_VRM, BBLUFF_VRM, DMINES_VRM, POLARP_VRM, ROCKYR_VRM, TINYA_VRM,
	NGLABS_VRM, CTCASTLE_VRM, HTSKYWAY_VRM, NITRO_COURT_VRM, OXSTATION_VRM, BOSS1_VRM, BOSS2_VRM, BOSS3_VRM, BOSS4_VRM, BOSS5_VRM };

	unsigned char lev_array[MAX_TRACKS] = { COVE_LEV, TUBES_LEV, MCAVES_LEV, SKROCK_LEV, SEWER_LEV, SLIDEC_LEV, TURBOT_LEV,
	TEMPLE_LEV, CPARK_LEV, PPYRAMID_LEV, RAMPAGER_LEV, DCANYON_LEV, BBLUFF_LEV, DMINES_LEV, POLARP_LEV, ROCKYR_LEV, TINYA_LEV,
	NGLABS_LEV, CTCASTLE_LEV, HTSKYWAY_LEV, NITRO_COURT_LEV, OXSTATION_LEV, BOSS1_LEV, BOSS2_LEV, BOSS3_LEV, BOSS4_LEV, BOSS5_LEV };

	switch (loadingStage)
	{
	case 0:
	{

		// if level is not AdvGarage or Naughty Dog Box Scene
		if ((levelID != ADVENTURE_GARAGE) && (levelID != NAUGHTY_DOG_CRATE))
		{
			Cutscene_VolumeBackup();
		}

		CDSYS_XAPauseRequest();

		// if first boot (SCEA + Copyright + ND Box)
		if (sdata->boolFirstBoot != 0)
		{
			sdata->boolFirstBoot = 0;

			gGT->db[0].drawEnv.isbg = 0;
			gGT->db[1].drawEnv.isbg = 0;
		}

		// if not first boot (dont do this for ND Box)
		else
		{
			// change active allocation system to #1
			// used for whole game (except adventure arena)
			MEMPACK_SwapPacks(0);

			// erase all memory loaded after first boot
			MEMPACK_PopToState(sdata->bookmarkID);
		}

		// pop back here for every load, after first load,
		// this permanently reserves LNG, bigfile header, etc
		sdata->bookmarkID = MEMPACK_PushState();

		// Turn off HUD
		gGT->hudFlags &= 0xfe;
		gGT->hudFlags &= 0xf7;

		// disable all rendering except loading screen
		gGT->renderFlags &= 0x1000;

		gGT->level1 = 0;
		gGT->level2 = 0;

		DrawSync(0);

		// no overlay transition
		gGT->overlayTransition = 0;

		// disable certain game mode flags
		gGT->gameMode1 &= ~(GAME_CUTSCENE | END_OF_RACE | ADVENTURE_ARENA | MAIN_MENU);
		gGT->gameMode2 &= ~(LEV_SWAP | CREDITS | NO_LEV_INSTANCE);

		gGT->visMem1 = 0;
		gGT->visMem2 = 0;

		boolDefault1P = 1;

		if (levelID >= SCRAPBOOK)
		{
			// both in main menu, and after credits,
			// checked and confirmed in retail version
			gGT->gameMode1 |= MAIN_MENU;
		}

		// credits
		else if (levelID >= CREDITS_CRASH)
		{
			// enable cutscene flag
			gGT->gameMode1 |= GAME_CUTSCENE;

			// lev swap (&20) and credits (&80)
			gGT->gameMode2 |= (LEV_SWAP | CREDITS);
		}

		// If you're in Naughty Dog Box Scene,
		// Oxide Any% ending
		// Oxide 101% ending
		else if (levelID >= NAUGHTY_DOG_CRATE)
		{
			// Enable cutscene flag
			gGT->gameMode1 |= GAME_CUTSCENE;
		}

		// main menu or garage
		else if (levelID >= MAIN_MENU_LEVEL)
		{
			// enable flag that shows you are in main menu
			gGT->gameMode1 |= MAIN_MENU;

			if (levelID == ADVENTURE_GARAGE)
			{
				// Enter Adventure Character Selection
				sdata->mainMenuState = 4;
			}

			else
			{
				// get NextGame from the game you exited
				gGT->numPlyrNextGame = gGT->numPlyrCurrGame;
				gGT->numPlyrCurrGame = 4;
				boolDefault1P = 0;
			}
		}

		// intro cutscenes
		else if (levelID >= INTRO_RACE_TODAY)
		{
			// Enable cutscene flag
			gGT->gameMode1 |= GAME_CUTSCENE;

			// lev swap will be needed
			gGT->gameMode2 |= LEV_SWAP;
		}

		// if you are loading into adventure map:
		// any of the hubs: "hub1", "hub2", etc
		else if (levelID >= GEM_STONE_VALLEY)
		{
			// Change mode to Adventure Arena
			gGT->gameMode1 |= ADVENTURE_ARENA;

			// lev swap will be needed
			gGT->gameMode2 |= LEV_SWAP;


		}

		// driving track
		else
		{
			// get CurrGame from main menu's NextGame
			gGT->numPlyrCurrGame = gGT->numPlyrNextGame;
			boolDefault1P = 0;
		}

		if (boolDefault1P != 0)
		{
			gGT->numPlyrCurrGame = 1;
			gGT->numPlyrNextGame = 1;
		}

		// Enable HUD Instances
		gGT->hudFlags |= 2;

		gGT->Debug_ToggleNormalSpawn = 1;

		// default
		sdata->levelLOD = gGT->numPlyrCurrGame;

		// main menu or adv garage
		if ((gGT->gameMode1 & MAIN_MENU) != 0)
		{
			sdata->levelLOD = 1;
		}

		// if relic, or time trial
		if ((gGT->gameMode1 & (TIME_TRIAL | RELIC_RACE)) != 0)
		{
			sdata->levelLOD = 8;
		}

		// RAM optimization, dont do this OG code


			// OG game
			 //MainInit_PrimMem(gGT);
			 //MainInit_OTMem(gGT);

		MainInit_OTMem(gGT);

		/*
		// disable only for no$psx,
   // cause it wont support 2mb+ Prim/OT
   //needed or lev rendering in big tracks breaks
   for(int i = 0; i < 2; i++)
   {
	   struct PrimMem* primMem = &gGT->db[i].primMem;

	   void* pvVar1 = 0x80600000 + i*0x100000;

	   // 1MB, take away some padding
	   int mbsize = 0x100000 - 0x1000;

	   primMem->size = mbsize;
	   primMem->unk2 = (int)pvVar1;
	   primMem->curr = pvVar1;
	   primMem->start = pvVar1;

	   // skip alignment by & 0xfffffffc,
	   // all possible size inputs are already aligned
	   pvVar1 = (void*)((int)pvVar1 + mbsize);
	   primMem->end = pvVar1;
	   primMem->endMin100 = (void*)((int)pvVar1 - 0x100);
   }
   */


   // use HighMem for RaceFlag, dont use MEMPACK_AllocHighMem,
   // then later this allocation is extended to remainder of heap,
   // this will always be LESS than OG game allocation, so it wont
   // impact any intended loading screens


		int backup = (int)sdata->mempack[0].firstFreeByte;

		sdata->mempack[0].firstFreeByte =
			(void*)((int)sdata->mempack[0].lastFreeByte
				- 0xA000 // primMem needed
				- (0x2200 * 2)); // ghost HighMem

		force = 0xA000;
		MainInit_PrimMem(gGT);
		sdata->mempack[0].firstFreeByte = (void*)backup;





		// RAM Optimization, NEVER do this here,
		// by loading other assets first, ptrMap
		// has more room to load and realloc

		if
			(
				// if cutscene, adventure arena, or credits
				((gGT->gameMode1 & (GAME_CUTSCENE | ADVENTURE_ARENA)) != 0) ||
				((gGT->gameMode2 & CREDITS) != 0)
				)
		{
			// (now, at beginning of mempack)
			 //MainInit_JitPoolsNew(gGT);
		}


		break;
	}
	case 1:
	{
		// if XA has not paused since CDSYS_XAPauseRequest in stage #0,
		// then quit the function and try again next frame
		if (sdata->XA_State == 4) return loadingStage;

		// dont load end-of-race in these modes:
		//	credits, lev swap, cutscene, main menu
		if ((gGT->gameMode2 & (LEV_SWAP | CREDITS)) != 0) break;
		if ((gGT->gameMode1 & (GAME_CUTSCENE | MAIN_MENU)) != 0) break;

		//  load time trial overlay (probably unused anyways)
		ovrRegion1 = 3;

		LOAD_OvrEndRace(ovrRegion1);
		break;
	}
	case 2:
	{

		LOAD_OvrLOD(1);

		break;
	}
	case 3:
	{


		// main menu + scrapbook, 230
		if (
			(levelID != ADVENTURE_GARAGE) &&
			((gGT->gameMode1 & MAIN_MENU) != 0)
			)
		{
			ovrRegion3 = 0;
		}

		// race threads, 231
		else if (levelID <= LAB_BASEMENT)
		{
			ovrRegion3 = 1;
		}

		// advHub, 232
		else if (
			(levelID <= CITADEL_CITY) &&
			(gGT->podiumRewardID == 0)
			)
		{
			ovrRegion3 = 2;
		}

		// Cutscene, Credits, ND, Garage, Podium
		else
		{
			ovrRegion3 = 3;
		}

		LOAD_OvrThreads(ovrRegion3);
		break;
	}
	case 4:
	{
		// if level is not AdvGarage or Naughty Dog Box Scene
		if ((levelID != ADVENTURE_GARAGE) && (levelID != NAUGHTY_DOG_CRATE))
		{
			Music_Restart();
		}

		// If in main menu (character selection, track selection, any part of it)
		if ((gGT->gameMode1 & MAIN_MENU) != 0)
		{

			// all these are 230, except for adv garage in 233
			switch (sdata->mainMenuState)
			{
			case 0:	 MM_JumpTo_Title_FirstTime();	break;
			case 1:	 MM_JumpTo_Characters();		break;
			case 2:	 MM_JumpTo_TrackSelect();		break;
			case 3:	 MM_JumpTo_BattleSetup();		break;
			case 4:	 CS_Garage_Init();			break;
			case 5:	 MM_JumpTo_Scrapbook();		break;
			}
		}

		sdata->ptrMPK = 0;
		sdata->load_inProgress = 1;

		data.driverModelExtras[0] = 0;
		data.driverModelExtras[1] = 0;
		data.driverModelExtras[2] = 0;
		LOAD_DriverMPK((unsigned int)bigfile, sdata->levelLOD, &LOAD_Callback_DriverModels);
		break;
	}
	case 5:
	{
		sdata->PLYROBJECTLIST = (int**)((unsigned int)sdata->ptrMPK + 4);
		if (sdata->ptrMPK == 0) sdata->PLYROBJECTLIST = 0;

		// clear and reset
		LibraryOfModels_Clear(gGT);
		LOAD_GlobalModelPtrs_MPK();

		// clear and reset
		DecalGlobal_Clear(gGT);

		gGT->mpkIcons = 0;
		if (sdata->ptrMPK != 0)
		{
			gGT->mpkIcons = *(int*)sdata->ptrMPK;

			if (gGT->mpkIcons != 0)
				DecalGlobal_Store(gGT, (struct LevTexLookup*)gGT->mpkIcons);
		}

		// if level is not AdvGarage or Naughty Dog Box Scene
		if ((levelID != ADVENTURE_GARAGE) && (levelID != NAUGHTY_DOG_CRATE))
		{
			Music_Stop();
			CseqMusic_StopAll();
			Music_LoadBanks();
		}


		break;
	}
	case 6:
	{
		// if level is not AdvGarage or Naughty Dog Box Scene
		if ((levelID != ADVENTURE_GARAGE) && (levelID != NAUGHTY_DOG_CRATE))
		{
			iVar9 = Music_AsyncParseBanks();

			if (iVar9 == 0)
			{
				// quit and restart stage 6 next frame
				return loadingStage;
			}


			Cutscene_VolumeRestore();

		}

		// == banks are done parsing ===


#define NUM_CHECK 8 // modding: 8 drivers


// loop through models
		piVar15 = &data.driverModelExtras[0];
		for (iVar9 = 0; iVar9 < NUM_CHECK; iVar9++, piVar15++)
		{
			// increment pointer by 4,
			// change pointer to file (starting at pointer map)
			// into a pointer to the model itself
			if (*piVar15 != 0) *piVar15 += 4;
		};

		// If the world you're in is made of multiple LEV files
		if ((gGT->gameMode2 & LEV_SWAP) != 0)
		{
			// Cutscene Packs
			iVar9 = 0x6b000;
			iVar12 = 0x40000;

			// If you're in Adventure Arena
			if ((gGT->gameMode1 & ADVENTURE_ARENA) != 0)
			{
				// Adv Arena Packs
				iVar9 = 0x68800;
				iVar12 = 0x68800;
			}



			// Allocate room for LEV swapping
			iVar5 = (int)MEMPACK_AllocMem(iVar9 + iVar12); // "HUB ALLOC"
			sdata->ptrHubAlloc = (void*)iVar5;

			// Change active allocation system to #2
			// pack = [hubAlloc, hubAlloc+size1]
			MEMPACK_SwapPacks(1);
			MEMPACK_NewPack((void*)iVar5, iVar9);

			// Change active allocation system to #3
			// pack = [hubAlloc+size1, hubAlloc+size1+size2]
			MEMPACK_SwapPacks(2);
			MEMPACK_NewPack((void*)(iVar5 + iVar9), iVar12);

			// Intro cutscene with oxide spaceship and all racers
			if ((gGT->gameMode1 & ADVENTURE_ARENA) == 0)
			{
				// Always start with pool 1
				sVar4 = 1;
			}

			// If you're in Adventure Arena
			else
			{
				// Get 1 or 2, depending on map
				sVar4 = LOAD_GetAdvPackIndex();

				// Then swap:
				// Turn 1 into 2
				// Turn 2 into 1
				sVar4 = 3 - sVar4;
			}

			// keep track of subpack levels
			gGT->activeMempackIndex = sVar4;
			gGT->levID_in_each_mempack[sVar4] = gGT->levelID;
			gGT->levID_in_each_mempack[3 - sVar4] = 0xffff;

			// the rest of memory will load pointer maps,
			// loaded at HighMem in main pack, end of RAM,
			// so the pointer maps dont bloat subpacks
			MEMPACK_SwapPacks(0);

#if 1
			// biggest lev_swap (cutscene/adventure)
			// is 0xBBA0 for gemstone ptr map, align up
			// by 0x800 for 0xc000, and use AllocMem,
			// HighMem is now reserved for PrimMem
			sdata->PatchMem_Size = 0xc000;
			sdata->PatchMem_Ptr = (int)MEMPACK_AllocMem(sdata->PatchMem_Size); //, "Patch Table Memory");
#else
			// original game code
			sdata->PatchMem_Size = MEMPACK_GetFreeBytes();
			sdata->PatchMem_Ptr = MEMPACK_AllocHighMem(sdata->PatchMem_Size); //, "Patch Table Memory");
#endif

			// make all futuere allocations in subpacks
			MEMPACK_SwapPacks(gGT->activeMempackIndex);
		}

		// game is now loading
		sdata->load_inProgress = 1;


		if (gGT->levelID == load_track)
		{
			// adds VRAM to loading queue
			LOAD_AppendQueue(sdata->ptrBigfile1, 3, vrm_array[current_track], 0, 0);
			// adds LEV to loading queue
			LOAD_AppendQueue(sdata->ptrBigfile1, 2, lev_array[current_track], 0, &LOAD_Callback_LEV);

		}
		else
		{


			// add VRAM to loading queue
			uVar16 = LOAD_GetBigfileIndex(gGT->levelID, sdata->levelLOD, LVI_VRAM);
			LOAD_AppendQueue((int)bigfile, 3, (int)uVar16, NULL, NULL);

			// add LEV to loading queue
			uVar16 = LOAD_GetBigfileIndex(gGT->levelID, sdata->levelLOD, LVI_LEV);
			LOAD_AppendQueue((int)bigfile, 2, (int)uVar16, NULL, &LOAD_Callback_LEV);

			// can this be optimized with this?
			// I feel like we had this, then had to remove it for some reason?
			// if ((gGT->gameMode2 & LEV_SWAP) != 0)

			// if level ID is AdvHub or Credits
			if ((gGT->gameMode2 & LEV_SWAP) != 0) {

				if (
					// 25-38 or 44-63
					((u_int)(gGT->levelID - GEM_STONE_VALLEY) < 0xe) ||
					((u_int)(gGT->levelID - CREDITS_CRASH) < 0x14)
					)
				{
					// add PTR file to loading queue
					uVar6 = LOAD_GetBigfileIndex(gGT->levelID, sdata->levelLOD, LVI_PTR);
					LOAD_AppendQueue((int)bigfile, 1, (int)uVar6, (void*)sdata->PatchMem_Ptr, &LOAD_Callback_PatchMem);
				}
			}
		}
		break;
	}
	case 7:
	{
		// get level pointer
		lev = sdata->ptrLevelFile;


		// Set LEV pointer
		gGT->level1 = lev;

		// iVar9 is set to sdata->ptrLevelFile at the top of the function
		gGT->visMem1 = lev->visMem;

		// if LEV is valid
		if (lev != 0)
		{
			// Load Icons and IconGroups from LEV
			DecalGlobal_Store(gGT, lev->levTexLookup);
		}

		DebugFont_Init(gGT);

		// if level is not nullptr
		if (lev != 0)
		{

			// store array of model pointers in GameTracker
			LibraryOfModels_Store(gGT, lev->numModels, lev->ptrModelsPtrArray);



			// == must use RDATA strings ==
			// they have bigger sizes that the 
			// search algorithm depends on

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInLEV(lev, rdata.s_circle);
			gGT->ptrCircle = uVar16;

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInLEV(lev, rdata.s_clod);
			gGT->ptrClod = uVar16;

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInLEV(lev, rdata.s_dustpuff);
			gGT->ptrDustpuff = uVar16;

			// search for icon by string "Smoke Ring"
			uVar16 = (u_int)DecalGlobal_FindInLEV(lev, rdata.s_smokering);
			gGT->ptrSmoking = uVar16;

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInLEV(lev, rdata.s_sparkle);
			gGT->ptrSparkle = uVar16;

		}

		// if linked list of icons exists
		if (gGT->mpkIcons != 0)
		{
			piVar15 = (int*)(*(u_int*)((u_int)gGT->mpkIcons + 4));


			// search for icon by string
			//what even are these first arguments? --Super
			uVar16 = (u_int)DecalGlobal_FindInMPK(piVar15, rdata.s_lightredoff);
			gGT->trafficLightIcon[0] = (struct Icon*)uVar16;

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInMPK(piVar15, rdata.s_lightredon);
			gGT->trafficLightIcon[1] = (struct Icon*)uVar16;

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInMPK(piVar15, rdata.s_lightgreenoff);
			gGT->trafficLightIcon[2] = (struct Icon*)uVar16;

			// search for icon by string
			uVar16 = (u_int)DecalGlobal_FindInMPK(piVar15, rdata.s_lightgreenon);
			gGT->trafficLightIcon[3] = (struct Icon*)uVar16;

		}

		gGT->gameMode1_prevFrame = 1;

		// RAM optimization, always do this here,
		// because now ptrMap already loaded and realloc'd
        
	// Extend bit range of quadblockID,
	// upper bits are never used, but still
	// need AND for alignment with LW instruction
	*(unsigned short*)0x800a0f18 = 0xFFFC;
	*(unsigned short*)0x800a1e80 = 0xFFFC;
	*(unsigned short*)0x800a36d8 = 0xFFFC;
	*(unsigned short*)0x800a4fd0 = 0xFFFC;
	*(unsigned short*)0x800a6f70 = 0xFFFC;
	*(unsigned short*)0x800a8b90 = 0xFFFC;
	
		MEMPACK_SwapPacks(0);
		MainInit_JitPoolsNew(gGT);

		// end of RAM, steal rest of heap
		force = 0;
		MainInit_PrimMem(gGT);

		if ((gGT->gameMode2 & LEV_SWAP) != 0)
			MEMPACK_SwapPacks(gGT->activeMempackIndex);

		if
			(
				// 2 is for cutscene
				// 1 is for If you're in Adventure Arena
				((gGT->gameMode1 & (GAME_CUTSCENE | ADVENTURE_ARENA)) == 0) &&

				// if not going to credits
				((gGT->gameMode2 & CREDITS) == 0)
				)
		{
			// RAM optimization, never do this here,
			// cause the optimized version already happened

			 //MainInit_JitPoolsNew(gGT);


			return loadingStage + 1;
		}

		// podium reward
		if (gGT->podiumRewardID != 0)
		{
			// clear all podium model pointers
			iVar9 = 7;
			puVar8 = &data.podiumModel_podiumStands;
			do
			{
				*puVar8 = 0;
				iVar9--;
				puVar8--;
			} while (iVar9 > -1);

			// Get Memory Allocation System Index
			iVar9 = LOAD_GetAdvPackIndex() - 1;

			// change active allocation system
			// Swap 1 and 2 while on adventure map
			MEMPACK_SwapPacks(3 - gGT->activeMempackIndex);

			// game is now loading
			sdata->load_inProgress = 1;

			// VRAM for podium and all related models
			LOAD_AppendQueue(
				(int)bigfile, 3,
				BI_PODIUMVRMS + iVar9,
				NULL, NULL);

			// podium first place
			podiumModel = gGT->podium_modelIndex_First;

			// Fix for Oxide (faster than OG code that does nothing)
			// If Oxide WIN is requested, add 16 to load Oxide LOSE
			if (podiumModel == 0x8d)
				podiumModel = 0x8d + 16;

			if
				(
					(podiumModel != 0) &&

					// if not 0x7e + 0xF
					// if not oxide
					(podiumModel != 0x8d)
					)
			{
				LOAD_AppendQueue(
					(int)bigfile, 2,
					BI_DANCEMODELWIN + iVar9 + (podiumModel - 0x7e) * 2,
					(void*)&data.podiumModel_firstPlace, (void(*)(struct LoadQueueSlot*))0xfffffffe);
			}

			// podium second place exists
			podiumModel = gGT->podium_modelIndex_Second;

			if (podiumModel != 0)
			{
				LOAD_AppendQueue(
					(int)bigfile, 2,
					BI_DANCEMODELLOSE + iVar9 + (podiumModel - 0x7e) * 2,
					(void*)&data.podiumModel_secondPlace, (void(*)(struct LoadQueueSlot*))0xfffffffe);
			}

			// podium third place exists
			podiumModel = gGT->podium_modelIndex_Third;

			if (podiumModel != 0)
			{
				LOAD_AppendQueue(
					(int)bigfile, 2,
					BI_DANCEMODELLOSE + iVar9 + (podiumModel - 0x7e) * 2,
					(void*)&data.podiumModel_thirdPlace, (void(*)(struct LoadQueueSlot*))0xfffffffe);
			}

			// TAWNA
			podiumModel = gGT->podium_modelIndex_tawna;

			// add TAWNA to loading queue
			LOAD_AppendQueue(
				(int)bigfile, 2,
				BI_DANCETAWNAGIRL + iVar9 + (podiumModel - 0x8f) * 2,
				(void*)&data.podiumModel_tawna, (void(*)(struct LoadQueueSlot*))0xfffffffe);

			// if 0x7e+5 (dingo)
			if (gGT->podium_modelIndex_First == 0x83)
			{
				// add "DingoFire" to loading queue
				LOAD_AppendQueue(
					(int)bigfile, 2,
					BI_DINGOFIRE + iVar9,
					(void*)&data.podiumModel_dingoFire, (void(*)(struct LoadQueueSlot*))0xfffffffe);
			}

			// add Podium
			LOAD_AppendQueue(
				(int)bigfile, 2,
				BI_PODIUM + iVar9,
				NULL, &LOAD_Callback_Podiums);

			// Disable LEV instances on Adv Hub, for podium scene
			gGT->gameMode2 = gGT->gameMode2 | 0x100;
		}

		break;
	}
	case 8:
	{
		// If you're in Adventure Arena
		if
			(
				((gGT->gameMode1 & ADVENTURE_ARENA) != 0) &&
				(
					// loop counter
					iVar9 = 0,

					// podium reward
					gGT->podiumRewardID != 0
					)
				)
		{
			piVar15 = &data.podiumModel_firstPlace;

			// for iVar9 = 0; iVar9 < 8; iVar9++
			do
			{
				iVar12 = *piVar15;
				if (iVar12 != 0)
				{
					if (iVar9 < 7)
					{
						*piVar15 = iVar12 + 4;
						iVar12 = *piVar15;
					}

					m = (struct Model*)iVar12;
					if (m->id != -1)
					{
						gGT->modelPtr[m->id] = m;
					}
				}

				// increment loop counter
				iVar9 = iVar9 + 1;

				piVar15 = piVar15 + 1;
			} while (iVar9 < 8);

			// change active allocation system
			// Swap 1 and 2 while on adventure map
			MEMPACK_SwapPacks((int)gGT->activeMempackIndex);
		}

		// Level ID
		iVar9 = gGT->levelID;

		// Main Menu
		if (iVar9 == 0x27)
		{
			uVar16 = 7;
		LAB_800346b0:
			Audio_SetState_Safe(uVar16);
			return loadingStage + 1;
		}

		// One of the maps on Adventure Arena
		if (iVar9 - 0x19U < 5)
		{
			uVar16 = 6;

			// podium reward
			if (gGT->podiumRewardID == 0)
			{
				uVar16 = 5;
			}
			goto LAB_800346b0;
		}

		// oxide intro
		if (iVar9 == 0x1e)
		{
			uVar16 = 3;
			goto LAB_800346b0;
		}

		// credits
		if (iVar9 == 0x2c)
		{
			uVar16 = 2;
			goto LAB_800346b0;
		}

		// Naughty Dog Box
		if (iVar9 == 0x29)
		{
			uVar16 = 4;
			goto LAB_800346b0;
		}

		// stop/pause cseq music
		uVar16 = 1;

		if (iVar9 - 0x2aU < 2) goto LAB_800346b0;
		break;
	}
	case 9:
	{
		if (sdata->XA_State != 2)
		{
			if
				(

					// If not in main menu (not in 2D character selection, track selection, or any part of it)
					((gGT->gameMode1 & MAIN_MENU) == 0) ||

					// If level ID == 40
					// If you are in Adventure Character Selection
					(gGT->levelID == ADVENTURE_GARAGE)
					)
			{
				// if not going to credits
				if ((gGT->gameMode2 & CREDITS) == 0)
				{
					// enable all flags except loading screen
					gameMode1 = gGT->renderFlags | 0xffffefff;
				}

				// if going to credits
				else
				{
					// disable everything (except loading screen if still there)
					// enable drawing render bucket
					gameMode1 = (gGT->renderFlags & 0x1000) | 0x20;
				}

				// apply desired value
				gGT->renderFlags = gameMode1;
			}
			else
			{
				// disable everything (except loading screen if still there)
				// enable drawing render bucket
				gGT->renderFlags = (gGT->renderFlags & 0x1000) | 0x20;

				iVar9 = RaceFlag_IsFullyOffScreen();
				if (iVar9 == 1)
				{
					// checkered flag, begin transition on-screen
					RaceFlag_BeginTransition(1);
				}
			}
			gGT->hudFlags = gGT->hudFlags | 8;
			gGT->framesInThisLEV = 0;
			gGT->msInThisLEV = 0;

			ElimBG_Deactivate(gGT);

			// signify end of load
			return -2;
		}
		__attribute__((fallthrough));
	}
	default:
		return loadingStage;
	}
	loadingStage++;
switchD_80033660_caseD_a:
	return loadingStage;
}