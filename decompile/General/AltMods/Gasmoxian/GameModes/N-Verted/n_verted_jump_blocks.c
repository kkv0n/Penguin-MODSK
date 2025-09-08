#include <common.h>
#include "../../utils.h"

// Define structure for block and its timer
typedef struct {
    short blockId;
    unsigned char frameCount;
    unsigned char frameCountDelay; // delay before starting jump
} JumpBlock;

//(0.5s at 30FPS ≈ 15 frames)

JumpBlock BLIZZARD_BLUFF_blocks[] = {
    // Big jump 1
    {1555, 12, 0},
    {1556, 12, 0},
    {1558, 12, 0},
    {1559, 12, 0},
    {1557, 12, 0},
    {1316, 12, 0},
    {1314, 12, 0},

    // Big jump 2
    {1482, 13, 0},
    {1526, 13, 0},
    {1481, 13, 0},
    {1473, 13, 0},
    {1472, 13, 0},
    {1476, 13, 0},
    {1485, 13, 0},
    {1489, 13, 0},
    {1475, 13, 0}    
};

JumpBlock DINGO_CANYON_blocks[] = {
    {957, 13, 0},
    {881, 13, 0},
};

JumpBlock MYSTERY_CAVES_blocks[] = {
    //Jump 1
    {2313, 4, 0},
    {1789, 4, 0},
    {2311, 0, 1},
    {2355, 0, 1},
    {1772, 0, 1},
    {1764, 0, 1},

    //Jump 2
    {2427, 2, 0},
    {2424, 0, 1},
    {2408, 0, 1},
    
    //Jump 3
    {2485, 3, 0},
    {2480, 3, 0},
    {2539, 3, 0},
    {2520, 0, 1},
    {2481, 0, 1},

    //Turtle ponds
    {768, 1, 1},
    {769, 1, 1},

    {1225, 1, 1},
    {1226, 1, 1},

    {1392, 1, 1},
    {1348, 1, 1},

    {713, 1, 1},
    {763, 1, 1},

    {607, 1, 1},
    {505, 1, 1},
};

JumpBlock HOT_AIR_SKYWAY_blocks[] = {
    //Big jump 1
    {1612, 28, 4},
    {1611, 28, 4},

    //SC
    {466, 20, 0},

    // Big jump 2
    {430, 28, 0},
    {431, 28, 0},

    //Disable respawns here
    {1623, 0, 0},
    {1622, 0, 0},

    {467, 0, 0},
    {433, 0, 0},
    {432, 0, 0},

};

JumpBlock PAPU_PYRAMID_blocks[] = {
    //Bottom stairs 
    {472, 33, 0}, // Increased from 3 to 33
    {518, 33, 0}, // Increased from 3 to 33

    // {469, 3, 8},
    // {517, 3, 8},

    //Middle stairs
    // {542, 17, 0},
    // {433, 17, 0},
    {432, 19, 0},
    {541, 19, 0},

    //Top stairs
    // {434, 6, 0},
    // {543, 6, 0},
    {140, 10, 0},
    {222, 10, 0}
};

JumpBlock SEWER_SPEEDWAY_blocks[] = {
    // Small jump at the end
    {142, 1, 0},
    {141, 1, 0},

    // Waterfall jump
    {1188, 9, 0},
    {1197, 9, 0},
    {1234, 9, 0},
    {1233, 9, 0},

    // Sewer entrance
    {1541, 4, 0},
    {1542, 4, 0},
    {1543, 4, 0},
    {1544, 4, 0},

    {1537, 5, 4},
    {1538, 5, 4},
    {1539, 5, 4},
    {1540, 5, 4},

    //Disable respawns here)
    {1213, 0, 0},
    {1192, 0, 0},
    {1193, 0, 0},
    {1230, 0, 0},
};

JumpBlock CORTEX_CASTLE_blocks[] = {
    //Big fall
    {627, 65, 9},
    {616, 0, 1},
    {615, 0, 1},

    //Stairs bottom
    {1401, 34, 0},
    {1402, 34, 1},
    {1403, 34, 0},
    {1407, 34, 1},

    {1345, 35, 3},
    {1406, 35, 3},

    {1368, 3, 0},
    {1366, 3, 0},

    {1373, 3, 0},
    {1357, 3, 0},

    {1280, 3, 0},
    {1282, 3, 0},

    {1365, 3, 3},
    {1367, 3, 3},
    {1356, 3, 3},
    {1372, 3, 3},
    {1281, 3, 3},
    {1343, 3, 3},

    //Wood ramp
    {1501, 9, 4},
    {1518, 0, 1},
    {1500, 0, 1},
};

JumpBlock POLAR_PASS_blocks[] = {
    // Big fall
    {832, 37, 0},
    {836, 37, 0},
    {959, 37, 0},
    {958, 37, 0},

    // Small jump
    {1136, 11, 0},
    {1137, 11, 0},

    {522, 1, 1},
    {304, 2, 1},
    {303, 2, 5},

    // {567, 1, 1},
    {566, 2, 2},
    // {551, 2, 1},
    {550, 2, 1},
};

JumpBlock N_GIN_LABS_blocks[] = {
    //Big jump
    {705, 29, 0},
    {763, 0, 1},
    {764, 0, 1},

    // small jump (barrels)
    {1101, 5, 6},
    {1098, 0, 1},
    {1145, 0, 1},
    {1096, 0, 1},
    {1097, 0, 1},


    //small jump (intersection)
    {910, 6, 1},

    {519, 9, 16},
    {548, 0, 1},
    {547, 0, 1},
    {550, 0, 1},
    {549, 0, 1},


    {1821, 4, 13},
    {2094, 0, 1},
    {2107, 0, 1},
    {1819, 0, 1},
    {1818, 0, 1}

};

JumpBlock OXIDE_STATION_blocks[] = {
    //Just to disabble respawns here
    {1738, 0, 0},
    {620, 0, 0},
    {619, 0, 0},
    {629, 0, 0},
    {612, 0, 0},
    {611, 0, 0},
    {682, 0, 0},
    {613, 0, 0},
    {684, 0, 0},
    {683, 0, 0},
    {243, 0, 0},
    {239, 0, 0},
    {2245, 0, 0},

    //Set these as fake turbo pads
    {1736, 0, 8},
    {1705, 0, 8},
    {1703, 0, 8},
    {1704, 0, 8},
    
    //Big jump
    {581, 52, 1},
    {1737, 52, 5},

    //space 1
    {244, 44, 6},
    {240, 41, 6},

    //space 2
    {84, 1, 0},
    {85, 1, 0},

    //medium jump
    {423, 30, 0},
    {424, 30, 0},
    {422, 30, 0},
    {421, 30, 0},

    //small jump 1
    {2176, 4, 0},
    {2177, 4, 0},
    {2343, 4, 0},
    {2344, 4, 0},

    //small jump 2
    {2246, 4, 3}
};

// Check if a quadblock is a jump block, also return the frame count and delay
bool isJumpBlock(short levelID, short blockID, unsigned char* outFrames, unsigned char* outDelay) {
    JumpBlock* blocksArray = NULL;
    unsigned char blockCount = 0;

    // Select the appropriate blocks array based on levelId
    switch (levelID) {
        case BLIZZARD_BLUFF:
            blocksArray = BLIZZARD_BLUFF_blocks;
            blockCount = sizeof(BLIZZARD_BLUFF_blocks) / sizeof(JumpBlock);
            break;
        case DINGO_CANYON:  
            blocksArray = DINGO_CANYON_blocks;
            blockCount = sizeof(DINGO_CANYON_blocks) / sizeof(JumpBlock);
            break;
        case MYSTERY_CAVES:
            blocksArray = MYSTERY_CAVES_blocks;
            blockCount = sizeof(MYSTERY_CAVES_blocks) / sizeof(JumpBlock);
            break;
        case HOT_AIR_SKYWAY:
            blocksArray = HOT_AIR_SKYWAY_blocks;
            blockCount = sizeof(HOT_AIR_SKYWAY_blocks) / sizeof(JumpBlock);
            break;
        case PAPU_PYRAMID:
            blocksArray = PAPU_PYRAMID_blocks;
            blockCount = sizeof(PAPU_PYRAMID_blocks) / sizeof(JumpBlock);
            break;
        case SEWER_SPEEDWAY:
            blocksArray = SEWER_SPEEDWAY_blocks;
            blockCount = sizeof(SEWER_SPEEDWAY_blocks) / sizeof(JumpBlock);
            break;
        case CORTEX_CASTLE:
            blocksArray = CORTEX_CASTLE_blocks;
            blockCount = sizeof(CORTEX_CASTLE_blocks) / sizeof(JumpBlock);
            break;
        case POLAR_PASS:
            blocksArray = POLAR_PASS_blocks;
            blockCount = sizeof(POLAR_PASS_blocks) / sizeof(JumpBlock);
            break;
        case N_GIN_LABS:
            blocksArray = N_GIN_LABS_blocks;
            blockCount = sizeof(N_GIN_LABS_blocks) / sizeof(JumpBlock);
            break;
        case OXIDE_STATION:
            blocksArray = OXIDE_STATION_blocks;
            blockCount = sizeof(OXIDE_STATION_blocks) / sizeof(JumpBlock);
            break;
        default:
            // Level ID not found, variables already set to NULL/0
            break;
    }

    // If blockID is specified, search for it
    if (blockID != -1 && blocksArray != NULL) {
        for (unsigned char i = 0; i < blockCount; i++) {
            if ((blocksArray)[i].blockId == blockID) {
                if (outFrames) *outFrames = FPS_DOUBLE((blocksArray)[i].frameCount);
                if (outDelay) *outDelay = FPS_DOUBLE((blocksArray)[i].frameCountDelay);
                return true;
            }
        }
        return false;
    }

    // Block not found or not searching for a block
    return (blockID == -1 && blocksArray != NULL);
}

// Helper function to get the appropriate blocks array and count for a level
// void getLevelBlocksData(short levelID, JumpBlock** blocksArray, short* blockCount) {
//     *blocksArray = NULL;
//     *blockCount = 0;
    
//     // Select the appropriate blocks array based on levelId
//     switch (levelID) {
//         case BLIZZARD_BLUFF:
//             *blocksArray = BLIZZARD_BLUFF_blocks;
//             *blockCount = sizeof(BLIZZARD_BLUFF_blocks) / sizeof(JumpBlock);
//             break;
//         case DINGO_CANYON:  
//             *blocksArray = DINGO_CANYON_blocks;
//             *blockCount = sizeof(DINGO_CANYON_blocks) / sizeof(JumpBlock);
//             break;
//         case MYSTERY_CAVES:
//             *blocksArray = MYSTERY_CAVES_blocks;
//             *blockCount = sizeof(MYSTERY_CAVES_blocks) / sizeof(JumpBlock);
//             break;
//         case HOT_AIR_SKYWAY:
//             *blocksArray = HOT_AIR_SKYWAY_blocks;
//             *blockCount = sizeof(HOT_AIR_SKYWAY_blocks) / sizeof(JumpBlock);
//             break;
//         case PAPU_PYRAMID:
//             *blocksArray = PAPU_PYRAMID_blocks;
//             *blockCount = sizeof(PAPU_PYRAMID_blocks) / sizeof(JumpBlock);
//             break;
//         case SEWER_SPEEDWAY:
//             *blocksArray = SEWER_SPEEDWAY_blocks;
//             *blockCount = sizeof(SEWER_SPEEDWAY_blocks) / sizeof(JumpBlock);
//             break;
//         case CORTEX_CASTLE:
//             *blocksArray = CORTEX_CASTLE_blocks;
//             *blockCount = sizeof(CORTEX_CASTLE_blocks) / sizeof(JumpBlock);
//             break;
//         case POLAR_PASS:
//             *blocksArray = POLAR_PASS_blocks;
//             *blockCount = sizeof(POLAR_PASS_blocks) / sizeof(JumpBlock);
//             break;
//         case N_GIN_LABS:
//             *blocksArray = N_GIN_LABS_blocks;
//             *blockCount = sizeof(N_GIN_LABS_blocks) / sizeof(JumpBlock);
//             break;
//         case OXIDE_STATION:
//             *blocksArray = OXIDE_STATION_blocks;
//             *blockCount = sizeof(OXIDE_STATION_blocks) / sizeof(JumpBlock);
//             break;
//     }
// }