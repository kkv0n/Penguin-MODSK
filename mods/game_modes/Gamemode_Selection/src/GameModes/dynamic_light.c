#include <common.h>
#include "../utils.h"

// Constants for dynamic lighting
#define MAX_LIGHT_RADIUS 1000    // Max lighting radius
#define MIN_LIGHT_RADIUS 10      // Inner radius for full brightness
#define MAX_BRIGHTNESS 100       // Maximum brightness boost
#define COLOR_MEMORY_SIZE 30000  // Number of vertices we can store colors for
#define MAX_QUADBLOCKS_TO_PROCESS 32  // Maximum number of quadblocks to process at once
#define UPDATE_FREQUENCY 1       // Only update lighting every X frames

// Original color storage
static unsigned char original_colors_hi[COLOR_MEMORY_SIZE][4];
static unsigned char original_colors_lo[COLOR_MEMORY_SIZE][4];
static int tracked_vertices_count = 0;
static short tracked_vertex_indices[COLOR_MEMORY_SIZE];

// Quadblock tracking
static struct QuadBlock* tracked_quadblocks[MAX_QUADBLOCKS_TO_PROCESS];
static int num_tracked_quadblocks = 0;
static int last_processed_quadblock_index = 0;

// Optimization variables
static bool colors_saved = false;
static int frame_counter = 0;
static short last_player_x = 0;
static short last_player_z = 0;
static int quadblock_refresh_counter = 0;

// Helper functions
static bool isVertexInRange(struct LevVertex* v, short playerX, short playerZ, int maxDist) {
    int dx = v->pos[0] - playerX;
    int dz = v->pos[2] - playerZ;
    int distance = MATH_FastSqrt((dx * dx) + (dz * dz), 0);
    return distance < maxDist;
}

static void trackQuadblock(struct QuadBlock* qb) {
    // Check if quadblock is already tracked
    for (int i = 0; i < num_tracked_quadblocks; i++) {
        if (tracked_quadblocks[i] == qb) return;
    }
    
    // Add to tracked list if there's room
    if (num_tracked_quadblocks < MAX_QUADBLOCKS_TO_PROCESS) {
        tracked_quadblocks[num_tracked_quadblocks++] = qb;
    }
}

void SaveOriginalColors(struct Level* level) {
    if (colors_saved) return;
    
    struct mesh_info* mi = level->ptr_mesh_info;
    if (!mi || !mi->ptrVertexArray) return;
    
    // Start with a fresh count
    tracked_vertices_count = 0;
    num_tracked_quadblocks = 0;
    
    // Save ALL vertex colors initially (we'll process them selectively later)
    int max_vertices = mi->numVertex < COLOR_MEMORY_SIZE ? mi->numVertex : COLOR_MEMORY_SIZE;
    
    for (int i = 0; i < max_vertices; i++) {
        struct LevVertex* v = &mi->ptrVertexArray[i];
        
        // Store vertex index and colors
        tracked_vertex_indices[tracked_vertices_count] = i;
        
        for (int c = 0; c < 4; c++) {
            original_colors_hi[tracked_vertices_count][c] = v->color_hi[c];
            original_colors_lo[tracked_vertices_count][c] = v->color_lo[c];
        }
        
        tracked_vertices_count++;
    }
    
    colors_saved = true;
}

void RestoreOriginalColors(struct Level* level) {
    if (!colors_saved) return;
    
    struct mesh_info* mi = level->ptr_mesh_info;
    if (!mi || !mi->ptrVertexArray) return;
    
    // Restore all tracked vertices to their original colors
    for (int i = 0; i < tracked_vertices_count; i++) {
        short vertex_index = tracked_vertex_indices[i];
        if (vertex_index >= 0 && vertex_index < mi->numVertex) {
            struct LevVertex* v = &mi->ptrVertexArray[vertex_index];
            
            for (int c = 0; c < 4; c++) {
                v->color_hi[c] = original_colors_hi[i][c];
                v->color_lo[c] = original_colors_lo[i][c];
            }
        }
    }
}

// Find the nearest vertices to the player using quadblocks
static void UpdateTrackedQuadblocks(struct Level* level, struct Driver* driver) {
    // Only update quadblocks periodically or when player moves significantly
    if (++quadblock_refresh_counter < 30) {
        short playerX = driver->posCurr.x >> 8;
        short playerZ = driver->posCurr.z >> 8;
        
        // Check if player has moved enough to warrant a refresh
        int dx = playerX - last_player_x;
        int dz = playerZ - last_player_z;
        int distanceMoved = MATH_FastSqrt((dx * dx) + (dz * dz), 0);
        if (distanceMoved < 100) { // ~100 units of movement
            return;
        }
    }
    
    quadblock_refresh_counter = 0;
    num_tracked_quadblocks = 0;
    
    // Start with the quadblock directly under the player
    struct QuadBlock* underQuad = driver->underDriver;
    if (!underQuad) return;
    
    // Track the current quadblock
    trackQuadblock(underQuad);
    
    // Track previous quadblock for smoother transitions
    if (driver->lastValid && driver->lastValid != underQuad) {
        trackQuadblock(driver->lastValid);
    }
    
    // Remember player position
    last_player_x = driver->posCurr.x >> 8;
    last_player_z = driver->posCurr.z >> 8;
}

void ApplyDynamicLighting(struct Level* level, struct Driver* driver) {
    if (!colors_saved) return;
    
    // Skip frames for performance
    if (++frame_counter < UPDATE_FREQUENCY) return;
    frame_counter = 0;
    
    struct mesh_info* mi = level->ptr_mesh_info;
    if (!mi || !mi->ptrVertexArray) return;
    
    // Update which quadblocks to process
    UpdateTrackedQuadblocks(level, driver);
    
    // If no quadblocks to track, use the underDriver directly
    if (num_tracked_quadblocks == 0 && driver->underDriver) {
        trackQuadblock(driver->underDriver);
    }
    
    // Get player position
    short playerX = driver->posCurr.x >> 8;
    short playerY = driver->posCurr.y >> 8;
    short playerZ = driver->posCurr.z >> 8;
    
    // First, restore all vertices to original colors
    for (int i = 0; i < tracked_vertices_count; i++) {
        short vertex_index = tracked_vertex_indices[i];
        if (vertex_index >= 0 && vertex_index < mi->numVertex) {
            struct LevVertex* v = &mi->ptrVertexArray[vertex_index];
            
            for (int c = 0; c < 4; c++) {
                v->color_hi[c] = original_colors_hi[i][c];
                v->color_lo[c] = original_colors_lo[i][c];
            }
        }
    }
    
    // Process vertices in or near current quadblocks
    for (int qi = 0; qi < num_tracked_quadblocks; qi++) {
        struct QuadBlock* qb = tracked_quadblocks[qi];
        if (!qb) continue;
        
        // Use bounding box to filter vertices
        struct BoundingBox* bbox = &qb->bbox;
        
        // Expand the bounding box by the light radius
        int minX = bbox->min[0] - MAX_LIGHT_RADIUS;
        int maxX = bbox->max[0] + MAX_LIGHT_RADIUS;
        int minZ = bbox->min[2] - MAX_LIGHT_RADIUS;
        int maxZ = bbox->max[2] + MAX_LIGHT_RADIUS;
        
        // For each vertex in tracked list, check if it's close to this quadblock
        for (int i = 0; i < tracked_vertices_count; i++) {
            short vertex_index = tracked_vertex_indices[i];
            if (vertex_index >= 0 && vertex_index < mi->numVertex) {
                struct LevVertex* v = &mi->ptrVertexArray[vertex_index];
                
                // Quick bounding box check before calculating distance
                if (v->pos[0] < minX || v->pos[0] > maxX || 
                    v->pos[2] < minZ || v->pos[2] > maxZ) {
                    continue; // Skip vertices far from this quadblock
                }
                
                // Calculate actual distance to player using MATH_FastSqrt
                int dx = v->pos[0] - playerX;
                int dz = v->pos[2] - playerZ;
                int distance = MATH_FastSqrt((dx * dx) + (dz * dz), 0);
                
                // Apply dynamic lighting based on distance
                if (distance < MAX_LIGHT_RADIUS) {
                    // Calculate brightness boost (closer = brighter)
                    int brightness = 0;
                    
                    if (distance < MIN_LIGHT_RADIUS) {
                        brightness = MAX_BRIGHTNESS; // Full brightness in inner radius
                    } else {
                        // Use actual distances for calculation with FastSqrt
                        brightness = MAX_BRIGHTNESS * (MAX_LIGHT_RADIUS - distance) / (MAX_LIGHT_RADIUS - MIN_LIGHT_RADIUS);
                    }
                    
                    // Apply brightness boost to RGB components (not alpha)
                    for (int c = 0; c < 3; c++) {
                        int color_hi = original_colors_hi[i][c] + brightness;
                        int color_lo = original_colors_lo[i][c] + brightness;
                        
                        // Clamp to 255
                        v->color_hi[c] = color_hi > 255 ? 255 : color_hi;
                        v->color_lo[c] = color_lo > 255 ? 255 : color_lo;
                    }
                }
            }
        }
    }
}

void InitDynamicLighting(struct Level* level) {
    colors_saved = false;
    tracked_vertices_count = 0;
    num_tracked_quadblocks = 0;
    frame_counter = 0;
    quadblock_refresh_counter = 0;
    SaveOriginalColors(level);
}