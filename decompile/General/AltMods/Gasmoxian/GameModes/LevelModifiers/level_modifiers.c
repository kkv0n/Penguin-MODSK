#include <common.h>

#include "../../utils.h"

void WallRide(struct Level *level){
    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;

    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];

        // Add SpeedImpact to all walls
        // if (qb->quadFlags & Q_WALL) {
        //     qb->speedImpact = -127;
        // }

        //Mark all walls as ground
        if (qb->quadFlags & Q_WALL) {
            qb->quadFlags |= Q_GROUND;
            qb->quadFlags &= ~Q_WALL;
        }

    }
}

void Boundless(struct Level *level){
    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;

    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];

        //ignore Turbo pads
        if (
            qb->quadFlags & Q_TRIGGER_SCRIPT
            // && (qb->terrain_type == TERRAIN_GRASS || qb->terrain_type == TERRAIN_DIRT) 
        ){
            continue;
        }

        //ignore kickers2 (idk why but this break some killplanes in n gin labs)
        if (gGT->levelID == N_GIN_LABS && qb->quadFlags & Q_KICKERS2) {
            continue;
        }

        //Remove Mask Grad and Out of Bounds from all quads
        qb->quadFlags &= ~(Q_MASK_GRAB | Q_OOF_BOUNDS);

        //Remove collition from killplanes and invisible walls
        if (qb->quadFlags & Q_INV_TRIGGERS){
            if(
                qb->terrain_type != TERRAIN_MUD //Avoid holes in tiny arena
                && qb->weather_intensity == 0 //Avoid holes on weather quadblocks
                && !((
                    gGT->levelID == CRASH_COVE
                    || gGT->levelID == MYSTERY_CAVES
                    || gGT->levelID == N_GIN_LABS
                    || gGT->levelID == CORTEX_CASTLE
                ) && (qb->quadFlags & Q_GROUND)) //Don't remove ground flags in these levels, it makes holes
            ){ 
                qb->quadFlags &= ~(Q_WALL | Q_GROUND);
            }     
        }
        //Replace offroad terrains
        if (
            qb->terrain_type == TERRAIN_GRASS
            || qb->terrain_type == TERRAIN_DIRT
            || qb->terrain_type == TERRAIN_SNOW
            || qb->terrain_type == TERRAIN_SLOWGRASS
            || qb->terrain_type == TERRAIN_SLOWDIRT
            || qb->terrain_type == TERRAIN_WATER
            // || qb->terrain_type == TERRAIN_RIVERASPHALT
            // || qb->terrain_type == TERRAIN_OCEANASPHALT
            // || qb->terrain_type == TERRAIN_STEAMASPHALT
            || qb->terrain_type == TERRAIN_MUD
            || qb->terrain_type == TERRAIN_TRACK
        ) {
            qb->terrain_type = TERRAIN_ASPHALT;
        }
    }
}

// Solidify a list of quadblocks as wall if they had no colision flag
//qb->blockID
void SolidifyWalls(struct Level *level, int* quadBlockIDs, int numIDs) {
    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;

    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];
        // If the quadblock is on the list
        for (int j = 0; j < numIDs; j++) {
            if (qb->blockID == quadBlockIDs[j]) {
                qb->quadFlags &= ~Q_NO_COLL;
                qb->quadFlags |= Q_WALL;
                break;  // Exit inner loop once found
            }
        }
    }
}

void SpeedwayPhys(struct Level *level){
    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;

    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];

        // Add SpeedImpact
        if (qb->quadFlags & (Q_GROUND | Q_WALL)) {
            qb->speedImpact = -127;
        }
    }
}

// Helper funcion to conver hex RBG to BGR
unsigned int ConvertHexToBGR(unsigned int hexColor) {
    unsigned char r = (hexColor >> 16) & 0xFF;
    unsigned char g = (hexColor >> 8) & 0xFF;
    unsigned char b = hexColor & 0xFF;
    return (b << 16) | (g << 8) | r; // Return in BGR format
}

bool hasAnimatedTexture(struct QuadBlock* qb) {
    for (int i = 0; i < 4; i++) {
        // Check if lowest bit is set (bitwise AND with 1)
        if ((uintptr_t)qb->ptr_texture_mid[i] & 1) {
            return true;
        }
    }
    return false;
}

// Night filter -> Change skybox, and apply a color filter to the levels, Made by Anfrost
//0-255 (lower = darker) (default: 64)
//Amount of blue to add (0-255) (default: 15)

int NightFilterBrightness = 255;
int NightFilterBlueTint = 15;

void NightSkybox(struct Level *level){
    // if level is caves, sewer or labs dont modify skybox since are indoor levels
    if (gGT->levelID != SEWER_SPEEDWAY && gGT->levelID != MYSTERY_CAVES && gGT->levelID != N_GIN_LABS) {
        // Add stars to the sky
        // If mirror mode is enabled don't add stars
        // if(!USE_MIRROR){
        //     gGT->renderFlags |= 8;
        //     level->stars.numStars = 768;
        //     level->stars.spread = 0;
        //     level->stars.seed = 65535;
        //     level->stars.distance = 1022;
        // }

        gGT->renderFlags |= 8;
        level->unkStarData[0] = 768;
        level->unkStarData[1] = 0;
        level->unkStarData[2] = 65535;
        level->unkStarData[3] = 1022;

        // Remove skybox
        level->ptr_skybox = NULL;
        // Enable the gradient
        level->configFlags |= 1;

        level->clearColorRGBA = 0x000000; // Set clear color to black (This does nothing for some reason)

        // Set gradient colors (format: 0x00BBGGRR)
        level->glowGradient[0].colorFrom = ConvertHexToBGR(0x060025);
        level->glowGradient[0].colorTo = ConvertHexToBGR(0x1A0047);

        level->glowGradient[1].colorFrom = ConvertHexToBGR(0x1A0047);
        level->glowGradient[1].colorTo = ConvertHexToBGR(0x000013);

        level->glowGradient[2].colorFrom = ConvertHexToBGR(0x000013);
        level->glowGradient[2].colorTo = ConvertHexToBGR(0x000000);

        // Set gradient positions
        level->glowGradient[0].pointFrom = 140;
        level->glowGradient[0].pointTo = 90;

        level->glowGradient[1].pointFrom = 90;
        level->glowGradient[1].pointTo = 60;

        level->glowGradient[2].pointFrom = 60;
        level->glowGradient[2].pointTo = -120;
    }
}

void NightFilter(struct Level *level, int brightness, int blueTint) {
    struct GameTracker* gGT = sdata->gGT;

    // Apply night skybox
    NightSkybox(level);

    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;
    int numQB = mi->numQuadBlock;
    
    // Create a bitmap where each bit represents a vertex
    #define BITMAP_SIZE (mi->numVertex / 8 + 1)
    unsigned char vertexBitmap[BITMAP_SIZE];
    
    // Initialize bitmap to all zeros
    for (int i = 0; i < BITMAP_SIZE; i++) {
        vertexBitmap[i] = 0;
    }
    
    // Mark vertices used by quadblocks
    for (int i = 0; i < numQB; i++) {
        struct QuadBlock* qb = &quadBlocks[i];
        
        // Ignore animated textures (Turbo pads, cascades, oxide station "lights", etc.)
        if (hasAnimatedTexture(qb)) {
            continue;
        }
        
        for (int j = 0; j < 9; j++) {
            short vIndex = qb->index[j];
            if (vIndex < 0 || vIndex >= mi->numVertex) continue;
            
            // Set the corresponding bit in the bitmap
            vertexBitmap[vIndex / 8] |= (1 << (vIndex % 8));
        }
    }
    
    // Process vertices that were marked in the bitmap
    for (int i = 0; i < mi->numVertex; i++) {
        // Check if this vertex is marked for processing
        if (!(vertexBitmap[i / 8] & (1 << (i % 8)))) continue;
        
        struct LevVertex* v = &mi->ptrVertexArray[i];
        unsigned char* hi = v->color_hi;
        unsigned char* lo = v->color_lo;

        // Detect green colors (for turbo pads)
        int isGreen_hi = (hi[1] > 80) && (hi[1] > hi[0] + 30) && (hi[1] > hi[2] + 30);

        // If not green, darken the colors and add blue tint
        if (!isGreen_hi) {
            hi[0] = (hi[0] * brightness) >> 8;
            hi[1] = (hi[1] * brightness) >> 8;
            hi[2] = (hi[2] * brightness) >> 8;
            hi[2] = (hi[2] + blueTint > 255) ? 255 : hi[2] + blueTint;

            lo[0] = (lo[0] * brightness) >> 8;
            lo[1] = (lo[1] * brightness) >> 8;
            lo[2] = (lo[2] * brightness) >> 8;
            lo[2] = (lo[2] + blueTint > 255) ? 255 : lo[2] + blueTint;
        }
    }
    
    // Darken skybox (if available)
    // struct Skybox* sb = level->ptr_skybox;
    // if (sb && sb->ptrVertex) {
    //     int count = sb->numVertex;
    //     struct ShortVertex* sk_verts = sb->ptrVertex;
        
    //     for (int i = 0; i < count; i++) {
    //         unsigned char* col = (unsigned char*)&sk_verts[i].Color;
        
    //         col[0] = 0; // R
    //         col[1] = 0; // G
    //         col[2] = 0; // B
    //         // col[3] = 255; // A
    //     }
    // }

}

void SeparateTrackSpawns(struct Level *level) {
    // Get rotation of spawn 1 (our pivot point)
    short baseRot = level->DriverSpawn[1].rot[1];
    short baseAngle = (baseRot + 0x400) & 0xfff;

    // Calculate left/right direction
    short leftRightAngle = (baseAngle - 1000) & 0xfff;

    // Separation distance
    int separationDistance = 0x100;

    // For each spawn point
    for (int i = 0; i < 8; i++) {
        int sideOffset = 0;
        int backOffset = 0;

        // Skip spawn 1 (our reference point)
        if (i == 1) continue;

        // Determine offsets based on spawn index
        switch (i) {
            case 0: sideOffset = -separationDistance; break;
            case 2: sideOffset = separationDistance; break;
            case 3: sideOffset = separationDistance * 2; break;
            case 4: sideOffset = -separationDistance; backOffset = -separationDistance; break;
            case 5: backOffset = -separationDistance; break;
            case 6: sideOffset = separationDistance; backOffset = -separationDistance; break;
            case 7: sideOffset = separationDistance * 2; backOffset = -separationDistance; break;
        }

        // Apply side offset (left/right)
        if (sideOffset != 0) {
            level->DriverSpawn[i].pos[0] += (MATH_Sin(leftRightAngle) * sideOffset) >> 12;
            level->DriverSpawn[i].pos[2] += (MATH_Cos(leftRightAngle) * sideOffset) >> 12;
        }

        // Apply back offset
        if (backOffset != 0) {
            level->DriverSpawn[i].pos[0] += (MATH_Sin(baseAngle) * backOffset) >> 12;
            level->DriverSpawn[i].pos[2] += (MATH_Cos(baseAngle) * backOffset) >> 12;
        }
    }
}

bool NightFilterApplied(struct Level* lev) {
    if (!lev) return false;

    // Trivial check for skybox and stars
    if(
        lev->ptr_skybox == NULL
        && (lev->configFlags & 1)
        // && lev->stars.numStars > 0
        && lev->unkStarData[0] > 0

    ){
        return true;
    }

    return false;
}

// void ApplyLevelModifiers(struct Level* lev) 
// {
//     if (!lev) return;
    
//     if(gGT->numPlyrCurrGame > 1
//         && IS_CUSTOM_TRACK_ID(gGT->levelID)
//         && ((gGT->gameMode1 & (BATTLE_MODE | ADVENTURE_MODE)) == 0)
//     ) {
//         SeparateTrackSpawns(lev);
//     }

//     if (USE_SHORTCUTLESS && gGT->levelID <= TURBO_TRACK) {
//         RemoveOffRoadCHK(lev);
//     }

//     if (USE_N_VERTED && gGT->levelID <= LAB_BASEMENT) {
//         ReverseTrack(lev);
//     }

//     if (
//         USE_NIGHT_FILTER
//         && (gGT->levelID <= INTRO_OXIDE || gGT->levelID == ADVENTURE_GARAGE)
//         && !NightFilterApplied(lev)
//     ){
//         NightFilter(lev, NightFilterBrightness, NightFilterBlueTint);
//     }

//     if (USE_BOUNDLESS && gGT->levelID < INTRO_RACE_TODAY) {
//         Boundless(lev);
//     }

//     if (USE_WALL_RIDE && gGT->levelID < INTRO_RACE_TODAY) {
//         WallRide(lev);
//     }

//     if (USE_SPEEDWAY_PHYSICS && gGT->levelID <= LAB_BASEMENT) {
//         SpeedwayPhys(lev);
//     }
// }