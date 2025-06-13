#include <common.h>

void AH_WarpPad_ThTick(struct Thread* t);
void AH_WarpPad_ThDestroy(struct Thread* t);

extern bool unlock_track;
extern bool ttrack_finished;
extern bool slidec_finished;


void AH_WarpPad_LInB(struct Instance* inst)
{
	int i;
	int levelID;
	struct Thread* t;
	struct WarpPad* warppadObj;

	struct GameTracker* gGT;

	int unlockItem_numOwned;
	int unlockItem_numNeeded;
	int unlockItem_modelID;

	int* arrTokenCount;
	struct Instance* newInst;


    // the last level of every adventure hub
	char last_level[4] = { SEWER_SPEEDWAY, DINGO_CANYON, TINY_ARENA, OXIDE_STATION };

	gGT = sdata->gGT;

	t =
		PROC_BirthWithObject
		(
			SIZE_RELATIVE_POOL_BUCKET
			(
				sizeof(struct WarpPad),
				NONE,
				MEDIUM,
				WARPPAD
			),

			AH_WarpPad_ThTick,	// behavior
			0,					// debug name
			0					// thread relative
		);

	if (t == 0) return;
	inst->thread = t;
	t->inst = inst;

	t->funcThDestroy = AH_WarpPad_ThDestroy;

	// 0 - locked
	// 1 - open for trophy
	// 2 - unlocked all
	// 3 - open for relic/token
	// 4 - purple token or SlideCol/TurboTrack

	// locked
	t->modelIndex = 0;

	// make invisible
	// this is the red triangle 
	// instance from DCxDemo's LEV Viewer
	inst->flags |= 0x80;

	warppadObj = t->object;
	warppadObj->levelID = 0;
	warppadObj->boolEnteredWarppad = 0;
	warppadObj->framesWarping = 0;

	for (i = 0; i < WPIS_NUM_INSTANCES; i++)
		warppadObj->inst[i] = 0;

	// each warppad has a name "warppad#xxx"
	// "warppad#0" is dingo canyon, level ID 0
	// "warppad#16" is slide col, level ID 16
	// "warppad#102" is gem cup 2
	// "warppad#104" is gem cup 4
	// etc

	levelID = 0;
	for (i = 8; inst->name[i] != 0; i++)
	{
		levelID = levelID * 10 + inst->name[i] - '0';
	}

	warppadObj->levelID = levelID;

	unlockItem_numNeeded = -1;

	// Trophy Track
	if (levelID < SLIDE_COLISEUM)
	{
		// optimization idea:
		// instead of data.metaDataLEV[levelID].hubID
		// can we just do gGT->levelID-0x19?

		// if trophy owned
		if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 6)) != 0)
		{
		GetKeysRequirement:

			// keys needed to unlock track again
			unlockItem_modelID = 99;
			unlockItem_numOwned = gGT->currAdvProfile.numKeys;
			unlockItem_numNeeded = D232.arrKeysNeeded[data.metaDataLEV[levelID].hubID];
		}

#define block_until_turbo_slide (levelID == COCO_PARK || levelID == TIGER_TEMPLE)

		// if trophy not owned
		else
		{
			//open coco park and tiger temple
			bool open_tracks;

			open_tracks = false; // set this as false by default
            
            //if we already own 5 trophies then enable coco park and tiger temple (this will reduce my headaches testing)			
			if (ttrack_finished && slidec_finished || gGT->currAdvProfile.numTrophies >= 5) open_tracks = true;


			// number trophies needed to open
			unlockItem_modelID = 0x62;
			unlockItem_numOwned = gGT->currAdvProfile.numTrophies;
			
			//block the 4th warp pad until you finish battle track and block lost ruins until you finish turbo track/slide col
			unlockItem_numNeeded = (levelID == last_level[data.metaDataLEV[levelID].hubID - 1] && !unlock_track ||
				!open_tracks && block_until_turbo_slide) ?
				gGT->currAdvProfile.numTrophies + 1 : data.metaDataLEV[levelID].numTrophiesToOpen;
		}
	}

	// Slide Col
	else if (levelID == SLIDE_COLISEUM)
	{
		//swap between trophy and key model
		unlockItem_modelID = (!slidec_finished && gGT->currAdvProfile.numTrophies < 5) ? 99 : 0x62;
		//the number we need of this item to unlock the track
		unlockItem_numNeeded = (!slidec_finished) ? 4 : 2;
		//the number we own of this item
		unlockItem_numOwned = (!slidec_finished && gGT->currAdvProfile.numTrophies < 5) ?
		gGT->currAdvProfile.numKeys : gGT->currAdvProfile.numTrophies;
	}

	// Turbo Track
	else if (levelID == TURBO_TRACK)
	{
		// swap between trophy and key model
		unlockItem_modelID = (!ttrack_finished && gGT->currAdvProfile.numTrophies < 5) ? 99 : 0x62;
		//the number we need of this required item to unlock the track
		unlockItem_numNeeded = (!slidec_finished && gGT->currAdvProfile.numTrophies < 5) ? gGT->currAdvProfile.numTrophies + 1 :
			(ttrack_finished) ? 2 : 4;
	    //the number we own of this required item
		unlockItem_numOwned = (!ttrack_finished || gGT->currAdvProfile.numTrophies >= 5) ? gGT->currAdvProfile.numTrophies : gGT->currAdvProfile.numKeys ;
	}

	// battle maps
	else if (levelID < GEM_STONE_VALLEY)
	{


        //num trophies required to unlock battle tracks
		unsigned char battle_requirements[4] = { 1, 7, 10, 15 };

        //swap between key and trophy model in the warp pad
		unlockItem_modelID = (unlock_track) ? 99 : 0x62;
        
		//swap between trophy and key requirement to unlock this warp pad
		unlockItem_numNeeded = (unlock_track) ? D232.arrKeysNeeded[data.metaDataLEV[levelID].hubID] :
			battle_requirements[data.metaDataLEV[levelID].hubID - 1];
        
		//the number we own of this required thing
		unlockItem_numOwned = (unlock_track) ? gGT->currAdvProfile.numKeys : gGT->currAdvProfile.numTrophies;
		//goto GetKeysRequirement;
	}

	// gem cups
	else
	{

		unlockItem_modelID = 99;
		unlockItem_numNeeded = 4;
		unlockItem_numOwned = gGT->currAdvProfile.numKeys - 1;
	}

	// if unlocked
	if (unlockItem_numOwned >= unlockItem_numNeeded)
	{
		warppadObj->digit1s = 0;

		// if beam model exists
		if (gGT->modelPtr[0x7B] != 0)
		{
			newInst = INSTANCE_Birth3D(gGT->modelPtr[0x7B], 0, t);

			// copy matrix
			*(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
			*(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
			*(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
			*(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
			*(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
			newInst->matrix.t[0] = inst->matrix.t[0];
			newInst->matrix.t[1] = inst->matrix.t[1];
			newInst->matrix.t[2] = inst->matrix.t[2];

			newInst->alphaScale = 0xc00;

			warppadObj->inst[WPIS_OPEN_BEAM] = newInst;
		}

		// if spiral ring exists
		if (gGT->modelPtr[0x7C] != 0)
		{
			for (i = 0; i < 2; i++)
			{
				newInst = INSTANCE_Birth3D(gGT->modelPtr[0x7C], 0, t);

				// copy matrix
				*(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
				*(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
				*(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
				*(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
				*(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
				newInst->matrix.t[0] = inst->matrix.t[0];
				newInst->matrix.t[1] = inst->matrix.t[1] + i * 0x400;
				newInst->matrix.t[2] = inst->matrix.t[2];

				newInst->alphaScale = 0x400;

				warppadObj->inst[WPIS_OPEN_RING1 + i] = newInst;
			}
		}

		for (i = 0; i < 3; i++)
			warppadObj->thirds[i] = 0x555 * i;

		warppadObj->spinRot_Prize[0] = 0;
		warppadObj->spinRot_Prize[1] = 0;
		warppadObj->spinRot_Prize[2] = 0;

		warppadObj->spinRot_Beam[0] = 0;
		warppadObj->spinRot_Beam[1] = 0;
		warppadObj->spinRot_Beam[2] = 0;

		for (i = 0; i < 2; i++)
		{
			warppadObj->spinRot_Wisp[i][0] = 0;
			warppadObj->spinRot_Wisp[i][1] = 0;
			warppadObj->spinRot_Wisp[i][2] = 0;
		}

		if (levelID < SLIDE_COLISEUM)
		{
			// unlocked all
			t->modelIndex = 2;

			// if trophy not owned
			if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 6)) == 0)
			{
				// open for trophy
				t->modelIndex = 1;

				newInst = INSTANCE_Birth3D(gGT->modelPtr[0x62], 0, t);

				newInst->scale[0] = 0x2800;
				newInst->scale[1] = 0x2800;
				newInst->scale[2] = 0x2800;

				warppadObj->inst[WPIS_OPEN_PRIZE1] = newInst;
			}

			// if token not owned
			if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x4c)) == 0)
			{
				// not open for trophy
				if (t->modelIndex != 1)
				{
					// open for relic/token
					t->modelIndex = 2;
				}
			BattleTrack:
				newInst = INSTANCE_Birth3D(gGT->modelPtr[0x7D], 0, t);

				// specular lighting
				newInst->flags |= 0x30000;

				newInst->scale[0] = 0x2000;
				newInst->scale[1] = 0x2000;
				newInst->scale[2] = 0x2000;

				i = data.metaDataLEV[levelID].ctrTokenGroupID;

				// token color
				newInst->colorRGBA =
					((unsigned int)data.AdvCups[i].color[0] << 0x14) |
					((unsigned int)data.AdvCups[i].color[1] << 0xc) |
					((unsigned int)data.AdvCups[i].color[2] << 0x4);

				// === Naughty Dog Bug ===
				// They made an array where every token color
				// could have it's own specular light, but they're
				// all the same specLight, so just use the first one

				// And because they're all identical, dont bother storing
				// it, just give a pointer to the global, instead of a 
				// pointer to warppadObj->specLight (ThTick and SpinRewards)
#if 0
				warppadObj->specLightToken[0] = D232.specLightToken[0];
				warppadObj->specLightToken[1] = D232.specLightToken[1];
				warppadObj->specLightToken[2] = D232.specLightToken[2];
#endif

				warppadObj->inst[WPIS_OPEN_PRIZE2] = newInst;
			}

		SlideColTurboTrack:

			// if relic not owned
			if (levelID < NITRO_COURT) // check this cause of "goto BattleTrack"
				if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x16)) == 0)
				{
					// SlideCol/TurboTrack
					//if(levelID>=0x10)
					//	t->modelIndex = 4;

					// open for token/relic
					if (t->modelIndex != 1)
						t->modelIndex = 2;

					newInst = INSTANCE_Birth3D(gGT->modelPtr[0x61], 0, t);

					// relic blue
					newInst->colorRGBA = 0x20a5ff0;

					// specular lighting
					newInst->flags |= 0x20000;

					newInst->scale[0] = 0x1800;
					newInst->scale[1] = 0x1800;
					newInst->scale[2] = 0x1800;

					warppadObj->inst[WPIS_OPEN_PRIZE3] = newInst;
				}

			for (i = 0; i < 3; i++)
			{
				newInst = warppadObj->inst[WPIS_OPEN_PRIZE1 + i];

				if (newInst == 0) continue;

				// copy matrix
				*(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
				*(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
				*(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
				*(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
				*(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
				newInst->matrix.t[0] = inst->matrix.t[0];
				newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
				newInst->matrix.t[2] = inst->matrix.t[2];
			}
		}

		// slide col, turbo track
		else if (levelID < 0x12)
		{
			// already unlocked
			t->modelIndex = 2;

			//goto SlideColTurboTrack;
		}

		// battle tracks
		else if (levelID < GEM_STONE_VALLEY)
		{
			i = R232.battleTrackArr[levelID - 0x12] + 0x6f;

			// already unlocked
			t->modelIndex = 2;

			if (CHECK_ADV_BIT(sdata->advProgress.rewards, i) == 0)
			{
				// rainbow
				t->modelIndex = 2;

				goto BattleTrack;
			}
		}

		// gemstone valley
		else
		{
			// bit index of gem
			i = (levelID - 100) + 0x6a;

			// if gem is already unlocked, quit
			if (levelID >= SLIDE_COLISEUM && levelID <= TURBO_TRACK)
			{
				// beaten
				t->modelIndex = 2;

				return;
			}

			// rainbow color
			t->modelIndex = 4;

			newInst = INSTANCE_Birth3D(gGT->modelPtr[0x5f], 0, t);

			// specular lighting
			newInst->flags |= 0x20000;

			i = levelID - 100;

			// token color
			newInst->colorRGBA =
				((unsigned int)data.AdvCups[i].color[0] << 0x14) |
				((unsigned int)data.AdvCups[i].color[1] << 0xc) |
				((unsigned int)data.AdvCups[i].color[2] << 0x4);

			warppadObj->inst[WPIS_OPEN_PRIZE1] = newInst;

#if 0
			// store in Gem array
			warppadObj->specLightGem[0] = D232.specLightGem[0];
			warppadObj->specLightGem[1] = D232.specLightGem[1];
			warppadObj->specLightGem[2] = D232.specLightGem[2];
#endif

			// for matrix copy
			goto SlideColTurboTrack;
		}

		return;
	}

	// === if locked ===

	if (unlockItem_numNeeded < 10)
	{
		warppadObj->digit10s = 0;
		warppadObj->digit1s = unlockItem_numNeeded;
	}

	else
	{
		warppadObj->digit10s = 1;
		warppadObj->digit1s = unlockItem_numNeeded - 10;
	}

	// ====== Item ========

	// WPIS_CLOSED_ITEM
	newInst = INSTANCE_Birth3D(gGT->modelPtr[unlockItem_modelID], 0, t);

	// copy matrix
	*(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
	*(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
	*(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
	*(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
	*(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
	newInst->matrix.t[0] = inst->matrix.t[0];
	newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
	newInst->matrix.t[2] = inst->matrix.t[2];

	newInst->scale[0] = 0x2000;
	newInst->scale[1] = 0x2000;
	newInst->scale[2] = 0x2000;

	// no specular for trophy
	if (unlockItem_modelID != 0x62)
	{
		// specular lighting
		newInst->flags |= 0x20000;

		// relic
		if (unlockItem_modelID == 0x61)
		{
			// Relic blue color
			newInst->colorRGBA = 0x20a5ff0;

#if 0
			warppadObj->specLightRelic[0] = D232.specLightRelic[0];
			warppadObj->specLightRelic[1] = D232.specLightRelic[1];
			warppadObj->specLightRelic[2] = D232.specLightRelic[2];
#endif
		}

		// Key
		else if (unlockItem_modelID == 99)
		{
			// Key color
			newInst->colorRGBA = 0xdca6000;

#if 0
			// store in Gem array (intended by ND, not a bug)
			warppadObj->specLightGem[0] = D232.specLightGem[0];
			warppadObj->specLightGem[1] = D232.specLightGem[1];
			warppadObj->specLightGem[2] = D232.specLightGem[2];
#endif
		}

		// Gem
		else if (unlockItem_modelID == 0x5f)
		{
			// dont set color, that gets set in ThTick

#if 0
// store in Gem array
			warppadObj->specLightGem[0] = D232.specLightGem[0];
			warppadObj->specLightGem[1] = D232.specLightGem[1];
			warppadObj->specLightGem[2] = D232.specLightGem[2];
#endif
		}

		// assume token
		else
		{
			i = levelID - 100;

			// token color
			newInst->colorRGBA =
				((unsigned int)data.AdvCups[i].color[0] << 0x14) |
				((unsigned int)data.AdvCups[i].color[1] << 0xc) |
				((unsigned int)data.AdvCups[i].color[2] << 0x4);

			// === Naughty Dog Bug ===
			// They made an array where every token color
			// could have it's own specular light, but they're
			// all the same specLight, so just use the first one

#if 0
			warppadObj->specLightToken[0] = D232.specLightToken[0];
			warppadObj->specLightToken[1] = D232.specLightToken[1];
			warppadObj->specLightToken[2] = D232.specLightToken[2];
#endif
		}
	}

	warppadObj->inst[WPIS_CLOSED_ITEM] = newInst;

	// ====== "X" ========

	// WPIS_CLOSED_X
	newInst = INSTANCE_Birth3D(gGT->modelPtr[0x6F], 0, t);

	// copy matrix
	*(int*)((int)&newInst->matrix + 0x0) = 0x1000;
	*(int*)((int)&newInst->matrix + 0x4) = 0;
	*(int*)((int)&newInst->matrix + 0x8) = 0x1000;
	*(int*)((int)&newInst->matrix + 0xC) = 0;
	*(short*)((int)&newInst->matrix + 0x10) = 0x1000;
	newInst->matrix.t[0] = inst->matrix.t[0];
	newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
	newInst->matrix.t[2] = inst->matrix.t[2];

	newInst->scale[0] = 0x2000;
	newInst->scale[1] = 0x2000;
	newInst->scale[2] = 0x2000;

	// always face camera
	newInst->model->headers[0].flags |= 1;

	warppadObj->inst[WPIS_CLOSED_X] = newInst;

	// ====== "10s" ========

	if (warppadObj->digit10s != 0)
	{
		// WPIS_CLOSED_10S
		newInst = INSTANCE_Birth3D(gGT->modelPtr[0x38], 0, t);

		// copy matrix
		*(int*)((int)&newInst->matrix + 0x0) = 0x1000;
		*(int*)((int)&newInst->matrix + 0x4) = 0;
		*(int*)((int)&newInst->matrix + 0x8) = 0x1000;
		*(int*)((int)&newInst->matrix + 0xC) = 0;
		*(short*)((int)&newInst->matrix + 0x10) = 0x1000;
		newInst->matrix.t[0] = inst->matrix.t[0];
		newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
		newInst->matrix.t[2] = inst->matrix.t[2];

		newInst->scale[0] = 0x2000;
		newInst->scale[1] = 0x2000;
		newInst->scale[2] = 0x2000;

		// always face camera
		for (i = 0; i < newInst->model->numHeaders; i++)
			newInst->model->headers[i].flags |= 1;

		warppadObj->inst[WPIS_CLOSED_10S] = newInst;
	}

	// ====== "1s" ========

	// STATIC_BIG (1-8)
	i = 0x38;
	if (warppadObj->digit1s == 0) i = 0x6d; // '0'
	if (warppadObj->digit1s == 9) i = 0x6e; // '9'

	// WPIS_CLOSED_1S
	newInst = INSTANCE_Birth3D(gGT->modelPtr[i], 0, t);

	// copy matrix
	*(int*)((int)&newInst->matrix + 0x0) = 0x1000;
	*(int*)((int)&newInst->matrix + 0x4) = 0;
	*(int*)((int)&newInst->matrix + 0x8) = 0x1000;
	*(int*)((int)&newInst->matrix + 0xC) = 0;
	*(short*)((int)&newInst->matrix + 0x10) = 0x1000;
	newInst->matrix.t[0] = inst->matrix.t[0];
	newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
	newInst->matrix.t[2] = inst->matrix.t[2];

	newInst->scale[0] = 0x2000;
	newInst->scale[1] = 0x2000;
	newInst->scale[2] = 0x2000;

	// always face camera
	for (i = 0; i < newInst->model->numHeaders; i++)
		newInst->model->headers[i].flags |= 1;

	warppadObj->inst[WPIS_CLOSED_1S] = newInst;
}