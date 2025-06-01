#include <common.h>
#include "../utils.h"

// Constants for dynamic lighting
#define MAX_LIGHT_RADIUS 1500    // Max lighting radius
#define MIN_LIGHT_RADIUS 10     // Inner radius for full brightness
#define MAX_BRIGHTNESS 100      // Maximum brightness boost
#define COLOR_MEMORY_SIZE 30000 // Number of vertices we can store colors for

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

// void RestoreOriginalColors(struct Level* level) {
//     if (!colors_saved) return;
    
//     struct mesh_info* mi = level->ptr_mesh_info;
//     if (!mi || !mi->ptrVertexArray) return;
    
//     // Restore all tracked vertices to their original colors
//     for (int i = 0; i < tracked_vertices_count; i++) {
//         short vertex_index = tracked_vertex_indices[i];
//         if (vertex_index >= 0 && vertex_index < mi->numVertex) {
//             struct LevVertex* v = &mi->ptrVertexArray[vertex_index];
            
//             for (int c = 0; c < 4; c++) {
//                 v->color_hi[c] = original_colors_hi[i][c];
//                 v->color_lo[c] = original_colors_lo[i][c];
//             }
//         }
//     }
// }

void ApplyDynamicLighting(struct Level* level, struct Driver* driver) {
    if (!colors_saved) return;
    
    struct mesh_info* mi = level->ptr_mesh_info;
    if (!mi || !mi->ptrVertexArray) return;
    
    // Get player position
    short playerX = driver->posCurr.x >> 8;
    short playerY = driver->posCurr.y >> 8;
    short playerZ = driver->posCurr.z >> 8;
    
    // Create a bounding box around the player's position
    struct BoundingBox lightBox;
    lightBox.min[0] = playerX - MAX_LIGHT_RADIUS;
    lightBox.min[1] = playerY - MAX_LIGHT_RADIUS;
    lightBox.min[2] = playerZ - MAX_LIGHT_RADIUS;
    lightBox.max[0] = playerX + MAX_LIGHT_RADIUS;
    lightBox.max[1] = playerY + MAX_LIGHT_RADIUS;
    lightBox.max[2] = playerZ + MAX_LIGHT_RADIUS;
    
    // Process vertices
    for (int i = 0; i < tracked_vertices_count; i++) {
        short vertex_index = tracked_vertex_indices[i];
        if (vertex_index >= 0 && vertex_index < mi->numVertex) {
            struct LevVertex* v = &mi->ptrVertexArray[vertex_index];
            
            // Bounding box check - only process vertices inside the light box
            if (v->pos[0] >= lightBox.min[0] && v->pos[0] <= lightBox.max[0] &&
                v->pos[1] >= lightBox.min[1] && v->pos[1] <= lightBox.max[1] &&
                v->pos[2] >= lightBox.min[2] && v->pos[2] <= lightBox.max[2]) {
                
                // Calculate actual distance from player to vertex (in 2D plane for efficiency)
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
                        // Linear falloff from inner to outer radius
                        brightness = MAX_BRIGHTNESS * (MAX_LIGHT_RADIUS - distance) / (MAX_LIGHT_RADIUS - MIN_LIGHT_RADIUS);
                    }
                    
                    // Apply brightness boost to RGB components
                    for (int c = 0; c < 3; c++) {
                        int color_hi = original_colors_hi[i][c] + brightness;
                        int color_lo = original_colors_lo[i][c] + brightness;
                        
                        // Clamp to 255
                        v->color_hi[c] = color_hi > 255 ? 255 : color_hi;
                        v->color_lo[c] = color_lo > 255 ? 255 : color_lo;
                    }
                } else {
                    // Restore original colors for vertices outside light radius
                    for (int c = 0; c < 4; c++) {
                        v->color_hi[c] = original_colors_hi[i][c];
                        v->color_lo[c] = original_colors_lo[i][c];
                    }
                }
            } else {
                // Vertex outside bounding box, restore original colors
                for (int c = 0; c < 4; c++) {
                    v->color_hi[c] = original_colors_hi[i][c];
                    v->color_lo[c] = original_colors_lo[i][c];
                }
            }
        }
    }
}

void InitDynamicLighting(struct Level* level) {
    colors_saved = false;
    tracked_vertices_count = 0;
    SaveOriginalColors(level);
}