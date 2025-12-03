#include <common.h>
#include "adventure.h"

unsigned char curr_page;

void AH_Pause_Draw(int pageID, int posX)
{
	RECT r;
	
	int buttonTap = sdata->gGamepads->gamepad[0].buttonsTapped;
	
	if (buttonTap & BTN_LEFT)
		curr_page = (curr_page == 0) ? 6 : curr_page - 1;
	
	if (buttonTap & BTN_RIGHT)
		curr_page = (curr_page == 6) ? 0 : curr_page + 1;





	

	int bitIndex;
	struct AdvProgress *adv;
	adv = &sdata->advProgress;

	char* str = HUB_NAMES[curr_page];

	 DecalFont_DrawLine(
		str,
		posX + 0x100, 0xf, FONT_BIG,
		0xffff8000);

	int len =
		 DecalFont_GetLineWidth(str, FONT_BIG);

	int half = len >> 1;

	// orange/red
	int colorIndex = 0;
	if ((sdata->frameCounter & FPS_DOUBLE(4)) == 0)
		colorIndex = 3;

	int* ptrColor = data.ptrColor[colorIndex];

    struct GameTracker *gGT = sdata->gGT;
	struct PrimMem* primMem = &gGT->backBuffer->primMem;

	struct Icon** iconPtrArray =
		ICONGROUP_GETICONS(gGT->iconGroup[4]);

    // Draw arrow pointing Left
     DecalHUD_Arrow2D(
        iconPtrArray[0x38],
        (posX - half) + 0xec,
        0x16,

        primMem,
        gGT->pushBuffer_UI.ptrOT,

        ptrColor[0], ptrColor[1],
		ptrColor[2], ptrColor[3],

		0, 0x1000, 0x800);

    // Draw arrow pointing Right
     DecalHUD_Arrow2D(
        iconPtrArray[0x38],
        (posX + half) + 0x112,
        0x16,

        primMem,
		gGT->pushBuffer_UI.ptrOT,

        ptrColor[0], ptrColor[1],
		ptrColor[2], ptrColor[3],

		0, 0x1000, 0);

	struct PauseObject* ptrPauseObject =
		D232.ptrPauseObject;

    // loop through 14 instances
    for (int i = 0; i < 0xe; i++)
    {
		// assume no awards won
        ptrPauseObject->PauseMember[i].unlockFlag &= ~(1);

		// dont draw instance
		ptrPauseObject->PauseMember[i].indexAdvPauseInst = -1;
    }

	int type = D232.advPausePages[curr_page].type;



		// gemstone
		if(curr_page == 6)
		{

			 DecalFont_DrawLine(
				track_names[CUSTOM_BOSS5],
				posX + 0x6e, 2*0x10 + 4 + 0x26,
				FONT_BIG, 4);

			// === Draw Star ===

			// black
			int color = 0x15;

			// set to grey (if beaten oxide at least once)
			if(adv_progress[CUSTOM_BOSS5])
				color = 1;

			u_int *starColor;
            starColor = data.ptrColor[color];

			struct Icon** iconPtrArray =
				ICONGROUP_GETICONS(gGT->iconGroup[5]);

			 DecalHUD_DrawPolyGT4
			(
				iconPtrArray[0x37],

				posX + 0x16d + 0x18,
				2*0x10 + 4 + 0x2a,

				&gGT->backBuffer->primMem,
				gGT->pushBuffer_UI.ptrOT,

				starColor[0], starColor[1],
				starColor[2], starColor[3],

				0, 0x1000
			);


		}

		// any other hub
		else
		{
				 
			    unsigned char menuIndex[6] = {adv_order[HUB_1 + 0], adv_order[HUB_2 + 0],
				adv_order[HUB_3 + 0], adv_order[HUB_4 + 0], adv_order[HUB_5 + 0], adv_order[HUB_FINAL + 0]};
				
				
				unsigned char bossID = CUSTOM_BOSS_0 + curr_page;
				
				unsigned char prize_index;
				char* level_LNG;
				unsigned char instanceIndex;
				unsigned char advPauseInst;
				unsigned char textColor;
				int NewCoords[2];
				


			// 6 custom tracks per hub
			for(int i = 0; i < 5; i++)
			{
				//dont show more tracks in page 2 or last page
				if ((curr_page == 1) &&
					(i > 1) || (curr_page == 5 && i > 3))
					{break;}
					
				
				//BOSSES
				if ((i == 4) || (curr_page == 1 && i == 1))
				{
					


                     instanceIndex = 12;
					 advPauseInst = 5;
					 textColor = WHITE;
					 
					 
					 
					level_LNG = track_names[bossID];
					prize_index = bossID;

			
					NewCoords[0] = posX + 0x50; 
					NewCoords[1] = i*0x10 + 0 + 0x26;
			
		
		
				}

				else
				{
					level_LNG = track_names[menuIndex[curr_page] + i];
					prize_index = menuIndex[curr_page] + i;
												
					

					instanceIndex = i;
					advPauseInst = 6;
					textColor = ORANGE;
					NewCoords[0] = posX + 0x50; 
					NewCoords[1] = i*0x10 + 0 + 0x26;
					
				}
				

				 DecalFont_DrawLine(
					level_LNG,
					NewCoords[0], NewCoords[1],
					FONT_BIG, textColor);


					struct Instance* inst =
						ptrPauseObject->PauseMember[instanceIndex].inst;
						

						NewCoords[0] = posX + 0x15e + 0*0x1e;
						NewCoords[1] =  i*0x10 + 0 + 0x2f;						
					
					// Remove SelectProfile with regular UI variant
					inst->matrix.t[0] =
						 UI_ConvertX_2(NewCoords[0] + 80, 0x100);

					inst->matrix.t[1] =
						 UI_ConvertY_2(NewCoords[1], 0x100);
					
					
				

					// set default
					ptrPauseObject->PauseMember[instanceIndex].indexAdvPauseInst = advPauseInst;
				
				

					if (adv_progress[prize_index])
					ptrPauseObject->PauseMember[instanceIndex].unlockFlag |= 1;
				    else
					ptrPauseObject->PauseMember[instanceIndex].unlockFlag |= 0;


					
				
			}



		}
	

	int iVar7 = len;

	int iVar11 = iVar7 + 0x14;
	if ((short)iVar7 < 0x20b) {
		iVar11 = 0x21e;
	}

	half = iVar11 >> 1;

	r.x = 0x10a - half;
	r.y = 0x20;
	r.w = (short)iVar11 + -0x14;
	r.h = 2;


	u_long* ot = gGT->backBuffer->otMem.startPlusFour;
	 RECTMENU_DrawOuterRect_Edge(&r, (u_int)&sdata->battleSetup_Color_UI_1, 0x20, ot);

	r.x = 0x100 - half;
	r.y = 10;
	r.w = (short)iVar11;
	r.h = 0x82;

	// Draw 2D Menu rectangle background
	 RECTMENU_DrawInnerRect(
		&r, 4, &ot[3]);

	for(int i = 0; i < 0xe; i++)
	{
		int index =
			ptrPauseObject->PauseMember[i].indexAdvPauseInst;

		struct Instance* inst =
			ptrPauseObject->PauseMember[i].inst;

		if(index < 0)
		{
			// make invisible
			inst->flags |= 0x80;
			continue;
		}

		inst->flags &= 0xfff8ff7f;
		inst->flags |= D232.advPauseInst[index].instFlags;

		if(ptrPauseObject->PauseMember[i].unlockFlag == 0)
		{
			inst->flags &= 0xfff8ff7f;
			inst->colorRGBA = 0;
			inst->alphaScale = 0x1000;
		}

		else
		{
			unsigned char* ptrColor =
				(unsigned char*)&D232.advPauseInst[index].color;
				
				inst->alphaScale = 0;
							
				if (i != 12)
				{
					inst->colorRGBA = instanceColor(BGR_TO_RGB(CTR_FontColor(relic_color)));
				}
				else
				{
				
			inst->colorRGBA =
				(ptrColor[0] << 0x14) |
				(ptrColor[1] << 0xc) |
				(ptrColor[2] << 0x4);
				}
		}

		int scale =
			D232.advPauseInst[index].scale;

		inst->scale[0] = scale;
		inst->scale[1] = scale;
		inst->scale[2] = scale;

		int modelID =
			D232.advPauseInst[index].modelID;

		inst->model = gGT->modelPtr[modelID];

		short* rotArr =
			&ptrPauseObject->PauseMember[i].rot[0];

		ConvertRotToMatrix(&inst->matrix, rotArr);

		// if using specular light
		if((inst->flags & 0x70000) == 0x20000)
		{
			short* specArr =
				&D232.advPauseInst[index].specLight[0];

			#ifndef REBUILD_PS1
			Vector_SpecLightSpin2D(inst, rotArr, specArr);
			#endif
		}

		else
		{
			inst->colorRGBA = 0;
		}

		rotArr[1] =
			inst->matrix.t[0] * 0x10 +
			inst->matrix.t[1] * 0x20 +
			sdata->frameCounter * FPS_HALF(0x40);

		rotArr[1] &= 0xfff;
	}
}