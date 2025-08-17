// N-Verted by Anfrost

#include <common.h>
#include "../../utils.h"

extern bool isJumpBlock(short levelID, short blockID, unsigned char* outFrames, unsigned char* outDelay);

void RotateSpawns(struct Level *level){
    for (int i = 0; i < 8; i++)
    {
        // Move backwards
        short angle = (level->DriverSpawn[i].rot[1] + 0x400) & 0xfff;
        int dist = 0x300;
        level->DriverSpawn[i].pos[0] += (MATH_Sin(angle) * dist) >> 12;
        level->DriverSpawn[i].pos[2] += (MATH_Cos(angle) * dist) >> 12; 
        // Add 0x800 to Y rotation (180 degrees)
        level->DriverSpawn[i].rot[1] = (level->DriverSpawn[i].rot[1] + 0x800) & 0xfff;
    }
}

void InvertCheckpoints(struct Level *level) {
    int numCheckpoints = level->cnt_restart_points;
    struct CheckpointNode* checkpoints = level->ptr_restart_points;

    struct GameTracker *gGT = sdata->gGT;
    short levelID = gGT->levelID;

    if (numCheckpoints <= 0 || checkpoints == NULL)
        return;
    
    // Save original distance to finish at checkpoint 0 (startline)
    unsigned short originalStartDistToFinish = checkpoints[0].distToFinish;
    
    // Allocate temporary memory for checkpoints
    struct CheckpointNode* tempCheckpoints = (struct CheckpointNode*)MEMPACK_AllocHighMem(sizeof(struct CheckpointNode) * numCheckpoints);
    for (int i = 0; i < numCheckpoints; i++) {
        tempCheckpoints[i] = checkpoints[i];
    }
    
    // Helper function to map indices
    unsigned char mapIndex(unsigned char idx) {
        if (idx == 0xFF)
            return 0xFF; // invalid checkpoint index
        
        if (idx == 0)
            return 0; // keep checkpoint 0 as is
            
        // Invert the index
        return (numCheckpoints - idx);
    }
    
    // Invert the order of checkpoints
    for (int i = 1; i < numCheckpoints; i++) {
        tempCheckpoints[i] = checkpoints[numCheckpoints - i];
    }
    
    // Reassign the nextIndex values and recalculate distToFinish
    for (int i = 0; i < numCheckpoints; i++) {
        // Swap nextIndex_forward and nextIndex_backward
        unsigned char tempForward = tempCheckpoints[i].nextIndex_forward;
        tempCheckpoints[i].nextIndex_forward = mapIndex(tempCheckpoints[i].nextIndex_backward);
        tempCheckpoints[i].nextIndex_backward = mapIndex(tempForward);
        
        unsigned char tempLeft = tempCheckpoints[i].nextIndex_left;
        tempCheckpoints[i].nextIndex_left = mapIndex(tempCheckpoints[i].nextIndex_right);
        tempCheckpoints[i].nextIndex_right = mapIndex(tempLeft);
        
        // Recalculate distToFinish
        if (i == 0) {
            // Checkpoint 0 (startline) should keep its original distance to finish
            tempCheckpoints[i].distToFinish = originalStartDistToFinish;
        } else {
            // Recalculate distance to finish for other checkpoints
            tempCheckpoints[i].distToFinish = originalStartDistToFinish * 
                (numCheckpoints - i) / numCheckpoints;
        }
    }
    
    // Make sure the first checkpoint points to the second one and the last one points to the first one
    if (numCheckpoints > 1) {
        tempCheckpoints[0].nextIndex_forward = 1;
        tempCheckpoints[0].nextIndex_backward = numCheckpoints - 1;
        
        tempCheckpoints[numCheckpoints - 1].nextIndex_forward = 0;
    }
    
    // Copy the inverted checkpoints back to the original array
    for (int i = 0; i < numCheckpoints; i++) {
        checkpoints[i] = tempCheckpoints[i];
    }
    
    // free the temporary memory
    MEMPACK_ClearHighMem();

    // Invert the checkpointIndex of each QuadBlock
    struct mesh_info* mi = level->ptr_mesh_info;
    struct QuadBlock* quadBlocks = mi->ptrQuadBlockArray;
    
    for (int i = 0; i < mi->numQuadBlock; i++) {
        struct QuadBlock* qb = &quadBlocks[i];

        if (qb->checkpointIndex != -1) {
            if (qb->checkpointIndex != 0) {
                qb->checkpointIndex = numCheckpoints - qb->checkpointIndex;
                
                // Avoid respawns on top of jump blocks
                if (isJumpBlock(levelID, qb->blockID, NULL, NULL)) {
                    if (qb->checkpointIndex > 2) {
                        qb->checkpointIndex = qb->checkpointIndex - 2; // Move back 2 checkpoints
                    }
                }
            }
        }
    }

    //Solidify walls (This is just used on cortex castle to avoid a weird lap skip)
    //{265, 266, 601, 602, 603, 604, 267, 268, 586, 587, 588, 589}

    if(levelID == CORTEX_CASTLE) {
        int solidifyWallIDs[] = {
            265, 266, 601, 602, 603, 604, 267, 268, 586, 587, 588, 589,
            696, 689, 693, 1143, 697, 690, 694, 1097, 698, 695, 688, 1095
        };

        int numIDs = sizeof(solidifyWallIDs) / sizeof(solidifyWallIDs[0]);

        extern void SolidifyWalls(struct Level *level, int* quadBlockIDs, int numIDs);
        SolidifyWalls(level, solidifyWallIDs, numIDs);
    }
}

void ReverseTrack(struct Level *level){
    if (level->cnt_restart_points > 0 && level->ptr_restart_points != NULL)
    {   
        // Rotate each driver spawn by 180 degrees
        RotateSpawns(level);

        // Invert Checkpoints
        InvertCheckpoints(level);
    }
}

