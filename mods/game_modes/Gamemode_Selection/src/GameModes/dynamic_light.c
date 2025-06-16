#include <common.h>
#include "../utils.h"

// Constants for dynamic lighting
#define MAX_LIGHT_RADIUS 1200    // Max lighting radius
#define MIN_LIGHT_RADIUS 10     // Inner radius for full brightness
#define MAX_BRIGHTNESS 100      // Maximum brightness boost
// !WARNING: THIS MAKES THIS FILE EXTREMELY OVERBUDGET
#define COLOR_MEMORY_SIZE 40000 // Number of vertices we can store colors for
int lightRadius[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // Store light radius for each driver

// Original color storage
static unsigned char original_colors_hi[COLOR_MEMORY_SIZE][4];
static unsigned char original_colors_lo[COLOR_MEMORY_SIZE][4];
static int tracked_vertices_count = 0;
static short tracked_vertex_indices[COLOR_MEMORY_SIZE];

// Flag to track if colors have been saved
static bool colors_saved = false;

void SaveOriginalColors(struct Level* level) {
    if (colors_saved) return;
    
    struct mesh_info* mi = level->ptr_mesh_info;
    if (!mi || !mi->ptrVertexArray) return;
    
    // Start with a fresh count
    tracked_vertices_count = 0;
    
    // Save colors of vertices (up to our buffer limit)
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

char prev_level_id = -1;
bool prev_level_was_hi = true;

void ResetDynamicLighting(struct Level* level) {
    // Always reset color data when a level change is detected
    bool current_level_is_hi = gGT->numPlyrCurrGame <= 2;
    bool level_changed = (prev_level_id != gGT->levelID || prev_level_was_hi != current_level_is_hi);
    
    // Only restore colors if we're in the same level and colors were saved
    if (!level_changed && colors_saved) {
        RestoreOriginalColors(level);
    }

    // Reset tracking data
    colors_saved = false;
    tracked_vertices_count = 0;
    
    // Clear any stored data
    for (int i = 0; i < COLOR_MEMORY_SIZE; i++) {
        tracked_vertex_indices[i] = -1;
        for (int c = 0; c < 4; c++) {
            original_colors_hi[i][c] = 0;
            original_colors_lo[i][c] = 0;
        }
    }
    
    // Update level tracking after reset
    if (level_changed) {
        prev_level_id = gGT->levelID;
        prev_level_was_hi = current_level_is_hi;
    }
}

void HandleDynamicLighting(struct Level* level) {
    // Check if we've changed levels since initialization
    bool current_level_is_hi = gGT->numPlyrCurrGame <= 2;
    if (prev_level_id != gGT->levelID || prev_level_was_hi != current_level_is_hi) {
        // Level changed, reset and initialize
        ResetDynamicLighting(level);
        SaveOriginalColors(level);
        prev_level_id = gGT->levelID;
        prev_level_was_hi = current_level_is_hi;
        return;
    }
    
    if (!colors_saved) return;
    
    struct mesh_info* mi = level->ptr_mesh_info;
    if (!mi || !mi->ptrVertexArray) return;
    
    // First reset all vertices to their original colors
    RestoreOriginalColors(level);

    // Create driver bounding boxes first
    struct BoundingBox lightBoxes[8]; // Maximun of 8 drivers
    unsigned char activeDrivers = 0;
    short driverPositions[8][3];

    // Prepare all driver positions and bounding boxes
    for (unsigned char d = 0; d < gGT->numPlyrCurrGame; d++) {
        struct Driver* driver = gGT->drivers[d];
        if (!driver) continue;
        // If its an AI driver skip
        if ((driver->actionsFlagSet & 0x100000) != 0) continue;
        
        // Get driver position
        driverPositions[activeDrivers][0] = driver->posCurr.x >> 8;
        driverPositions[activeDrivers][1] = driver->posCurr.y >> 8;
        driverPositions[activeDrivers][2] = driver->posCurr.z >> 8;

        // Light radius for this driver based on the number of players
        lightRadius[activeDrivers] = MAX_LIGHT_RADIUS / (gGT->numPlyrCurrGame);
        
        // Create a bounding box around the driver's position
        lightBoxes[activeDrivers].min[0] = driverPositions[activeDrivers][0] - lightRadius[activeDrivers];
        lightBoxes[activeDrivers].min[1] = driverPositions[activeDrivers][1] - lightRadius[activeDrivers];
        lightBoxes[activeDrivers].min[2] = driverPositions[activeDrivers][2] - lightRadius[activeDrivers];
        lightBoxes[activeDrivers].max[0] = driverPositions[activeDrivers][0] + lightRadius[activeDrivers];
        lightBoxes[activeDrivers].max[1] = driverPositions[activeDrivers][1] + lightRadius[activeDrivers];
        lightBoxes[activeDrivers].max[2] = driverPositions[activeDrivers][2] + lightRadius[activeDrivers];
        
        activeDrivers++;
    }
    
    if (activeDrivers == 0) return;
    
    // Process vertices first, then check each driver's contribution
    for (int i = 0; i < tracked_vertices_count; i++) {
        short vertex_index = tracked_vertex_indices[i];
        if (vertex_index < 0 || vertex_index >= mi->numVertex) continue;
        
        struct LevVertex* v = &mi->ptrVertexArray[vertex_index];
        int maxBrightness = 0;
        
        // Check against each driver
        for (unsigned char d = 0; d < activeDrivers; d++) {
            // Bounding box check - only process vertices inside the light box
            if (v->pos[0] >= lightBoxes[d].min[0] && v->pos[0] <= lightBoxes[d].max[0] &&
                v->pos[1] >= lightBoxes[d].min[1] && v->pos[1] <= lightBoxes[d].max[1] &&
                v->pos[2] >= lightBoxes[d].min[2] && v->pos[2] <= lightBoxes[d].max[2]) {
                
                // Calculate actual distance from driver to vertex (in 2D plane for efficiency)
                int dx = v->pos[0] - driverPositions[d][0];
                int dz = v->pos[2] - driverPositions[d][2];
                int distance = MATH_FastSqrt((dx * dx) + (dz * dz), 0);
                
                // Calculate brightness contribution from this driver
                if (distance < lightRadius[d]) {
                    int brightness = 0;
                    
                    if (distance < MIN_LIGHT_RADIUS) {
                        brightness = MAX_BRIGHTNESS; // Full brightness in inner radius
                    } else {
                        // Linear falloff from inner to outer radius
                        brightness = MAX_BRIGHTNESS * (lightRadius[d] - distance) / (lightRadius[d] - MIN_LIGHT_RADIUS);
                    }
                    
                    // Keep the maximum brightness contribution from any driver
                    if (brightness > maxBrightness) {
                        maxBrightness = brightness;
                    }
                    
                    // Performance optimization: If we've hit max brightness, no need to check other drivers
                    if (maxBrightness >= MAX_BRIGHTNESS) {
                        break;
                    }
                }
            }
        }
        
        // Apply the brightness directly if there's any contribution
        if (maxBrightness > 0) {
            // Apply brightness boost to RGB components
            for (int c = 0; c < 3; c++) {
                int color_hi = original_colors_hi[i][c] + maxBrightness;
                int color_lo = original_colors_lo[i][c] + maxBrightness;
                
                // Clamp to 255
                v->color_hi[c] = color_hi > 255 ? 255 : color_hi;
                v->color_lo[c] = color_lo > 255 ? 255 : color_lo;
            }
        }
    }
}

void InitDynamicLighting(struct Level* level) {
    ResetDynamicLighting(level);
    SaveOriginalColors(level);
    prev_level_id = gGT->levelID;
    prev_level_was_hi = gGT->numPlyrCurrGame <= 2;
}
