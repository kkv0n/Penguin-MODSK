#include <common.h>

#include "../../utils.h"
#include "../../global.h"

// Define maximum sizes for skybox data
#define MAX_SKYBOX_VERTICES 5000
#define MAX_SKYBOX_FACES_PER_SEGMENT 1000

static struct {
    bool initialized;
    int numVertex;
    struct ShortVertex vertices[MAX_SKYBOX_VERTICES];
    short numFaces[NUM_SKYBOX_SEGMENTS];
    struct SkyboxFace faces[NUM_SKYBOX_SEGMENTS][MAX_SKYBOX_FACES_PER_SEGMENT];
} g_Skybox = {0};

// Create a static skybox structure that we'll use to replace the level's skybox
static struct Skybox SkyboxCopy;
// Static arrays to store face pointers
static struct SkyboxFace* facePointers[NUM_SKYBOX_SEGMENTS];

// Function to capture the skybox
bool CaptureSkybox(struct Level* level) {
    if (!level || !level->ptr_skybox || g_Skybox.initialized)
        return false;

    struct Skybox* skybox = level->ptr_skybox;
    
    // Check if we have enough space
    if (skybox->numVertex > MAX_SKYBOX_VERTICES)
        return false;
        
    // Copy vertex data
    g_Skybox.numVertex = skybox->numVertex;
    memcpy(g_Skybox.vertices, skybox->ptrVertex, g_Skybox.numVertex * sizeof(struct ShortVertex));
    
    // Copy face counts and faces
    for (int i = 0; i < NUM_SKYBOX_SEGMENTS; i++) {
        g_Skybox.numFaces[i] = skybox->numFaces[i];
        
        if (g_Skybox.numFaces[i] > 0) {
            if (g_Skybox.numFaces[i] > MAX_SKYBOX_FACES_PER_SEGMENT)
                continue; // Skip if too many faces
                
            memcpy(g_Skybox.faces[i], skybox->ptrFaces[i], 
                   g_Skybox.numFaces[i] * sizeof(struct SkyboxFace));
        }
    }
    
    g_Skybox.initialized = true;
    return true;
}

// Function to apply the stored skybox to another level
bool ApplySkybox(struct Level* level) {
    if (!level || !g_Skybox.initialized)
        return false;
    
    // Set up the static skybox structure
    SkyboxCopy.numVertex = g_Skybox.numVertex;
    SkyboxCopy.ptrVertex = g_Skybox.vertices;
    
    // Set up face pointers
    for (int i = 0; i < NUM_SKYBOX_SEGMENTS; i++) {
        SkyboxCopy.numFaces[i] = g_Skybox.numFaces[i];
        facePointers[i] = g_Skybox.faces[i];
        SkyboxCopy.ptrFaces[i] = facePointers[i];
    }
    
    // Apply the skybox to the level
    level->ptr_skybox = &SkyboxCopy;
    
    return true;
}

// Function to transform skybox colors to green variants
bool GreenSkybox(struct Level* level) {
    if (!level || !level->ptr_skybox)
        return false;
        
    struct Skybox* skybox = level->ptr_skybox;
    
    // Process skybox vertices
    for (int i = 0; i < skybox->numVertex; i++) {
        // Access color as bytes
        unsigned char* color = (unsigned char*)&skybox->ptrVertex[i].Color;
        
        // Extract original RGB values
        unsigned char r = color[0];
        unsigned char g = color[1]; 
        unsigned char b = color[2];
        
        // Calculate luminance (brightness)
        unsigned char luminance = (r + g + b) / 3;
        
        // Make green dominant while preserving some original color variation
        color[0] = luminance / 4;          // Reduce red
        color[1] = luminance + (g / 2);    // Enhance green (but don't overflow)
        if (color[1] > 255) color[1] = 255;
        color[2] = luminance / 3;          // Reduce blue
    }
    
    return true;
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
    #define SHOULD_SKIP_SKYBOX(x) (x != SEWER_SPEEDWAY && x != MYSTERY_CAVES && x != N_GIN_LABS)
    
    if (SHOULD_SKIP_SKYBOX(gGT->levelID)){
        // Add stars to the sky
        // If mirror mode is enabled don't add stars
        if(!USE_MIRROR){
            gGT->renderFlags |= 8;
            level->stars.numStars = 768;
            level->stars.spread = 0;
            level->stars.seed = 65535;
            level->stars.distance = 1022;
        }

        // Apply skybox
        if (g_Skybox.initialized) {
            ApplySkybox(level);

            // Enable a pitch black gradient
            level->configFlags |= 1;
            level->clearColorRGBA = 0x000000;
            level->glowGradient[0].colorFrom = 0x000000;
            level->glowGradient[0].colorTo = 0x000000;
            level->glowGradient[0].pointFrom = 140;
            level->glowGradient[0].pointTo = -120;

            return;
        }

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

// This its only needed if its possible to restart the race, since this is online, it wont happen
# if 0 
bool NightFilterApplied(struct Level* level) {
    if (!level) return false;

    // Trivial check for skybox and stars
    if(
        level->ptr_skybox == NULL
        && (level->configFlags & 1)
        // && lev->stars.numStars > 0
        && level->stars.numStars > 0

    ){
        return true;
    }

    return false;
}
#endif

// About &level->rainBuffer->unk_4

// These 4 are affected by the speed
// 0: Constantly increasing value per frame, looks like a clock  (unknown)
// 1: Constantly increasing value per second, looks like a clock  (unknown)
// 2: Constantly decreasing value per frame, looks like a clock  (unknown)
// 3: Constantly decreasing value per second, looks like a clock  (unknown)

// 4: Changes particles position only when changing value from 0 to -1  (unknown)
// 5: Changes particles position on each value change (unknown)
// 6: Seems to do nothing (unknown)
// 7: Seems to do nothing (unknown)
// 8: Falling angle, based on a world axis (X?)
// 9: Horizontal speed (X?)?  weather disappears outside [-15, 15] range and it freak out each value change, (unknown)
// 10: Falling speed, -1 is the slowest and 0 the fastest (Y?)
// 11: Same as 9 but on the new angle (Y?)  (unknown)
// 12: Falling angle, based on a world axis (Z?)
// 13: Same as 9 but on the new angle (Z?)  (unknown)
// 14: Toggle? weather visible on 0, weather disappears on any other value  (unknown)
// 15: Toggle? weather visible on 0, weather disappears on any other value (Again?)

void AddWeather(struct Level* level, enum WEATHER_TYPE weather_type){
    int fillMode;
    int color_top, color_bottom; //ARGB
    char fallingAngle, fallingSpeed, unk_verticalSpeed;
    char weather_intensity, weather_vanishRate;

    switch(weather_type){
        case WEATHER_RAIN:
            fillMode = 0xe1000a60;
            color_top = 0x00404040;
            color_bottom = 0x00ffffff;
            fallingAngle = 20;
            fallingSpeed = -120;
            unk_verticalSpeed = -1;
            weather_intensity = 90;
            weather_vanishRate = 9;
        break;

        case WEATHER_SNOW:
            fillMode = 0xe1000a20;
            color_top = 0x00ffffff;
            color_bottom = 0x00ffffff;
            fallingAngle = 0;
            fallingSpeed = -8;
            unk_verticalSpeed = -1;
            weather_intensity = 60;
            weather_vanishRate = 1;
            break;

        case WEATHER_NONE:
            return;
        default:
            return;
    }
    //Rain buffer settings
    struct RainBuffer* rainBuffer = &level->rainBuffer;

    //fillMode
	rainBuffer->fillMode = fillMode;
    //offsetOT
	rainBuffer->offsetOT = 1; 

	rainBuffer->colorARGB_top = color_top;
	rainBuffer->colorARGB_bottom = color_bottom;

    rainBuffer->fallAngleX = fallingAngle;
    rainBuffer->fallingSpeed = fallingSpeed;
    rainBuffer->speedY = unk_verticalSpeed;

	rainBuffer->unk_22 = 0;

    // Enable weather on all quad blocks
    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;

    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];

        qb->weather_intensity = weather_intensity;
        qb->weather_vanishRate = weather_vanishRate;
    }
}

// Unused unlimited gamemodes
#if 0
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
#endif

// Unused on online, this its meant to separate track spawns on custom tracks on unlimited mod
# if 0
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
#endif    