#include "maze3.h"

#include "random.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//
//  Grid Coordinate range functions.
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//
//Every coordinate at every level lies within a 3x3 grid space.  This entire
//3x3 grid is given a single coordinate at the next highest level.
//
//This function takes a coordinate at one level and gives the corresponding
//coordinate that contains it at the next higher level.
//
int32 MapToHigherLevel(int32 coord) {
    //there are 3 regions, less than -1, greater than 1, and in between.
    if (coord < -1) {
        return (coord - 1) / 3;
    }
    else if (coord > 1) {
        return (coord + 1) / 3;
    }
    else {
        return 0;
    }
}

//
//This function takes a coordinate and gives the extents of the 3x3 sub-grid that 
//contains that coordinate.
//

void BoundsOfGrid(int32 coord, int32 &lower, int32 &upper) {
    //get the higher level coordinate.
    int32 higher_coord = MapToHigherLevel(coord);

    //multiply by 3 and add/subtract 1.
    lower = higher_coord * 3 - 1;
    upper = higher_coord * 3 + 1;
}

//
//This function takes a coordinate at one level and gives the extents of the
//3x3 sub-grid encompassed at the next lower level.
//

void BoundsOfSubgrid(int32 coord, int32 &lower, int32 &upper) {
    lower = coord * 3 - 1;
    upper = coord * 3 + 1;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//
//  Random number picking functions.
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//
//Returns the corner configuration for a given coordinate and level.
//The configuration is chosen based on a combination of level, x, and y values
//used as a seed for a deterministic random number generator.
//

uint8 GetCornerConfiguration(uint32 level, int32 corner_x, int32 corner_y) {
    //pick a deterministic random number based on the level and the coordinates
    return (uint8)Rand((uint16)level, uint16(corner_x * 1.345432), uint16(corner_y * 1.67298), (uint16)(corner_x + corner_y)) % 21;
}

//
//Returns a random number between 1 and 5 inclusive.  
//

uint8 GetEdgeCodeLeft(uint32 level, int32 coord_x, int32 coord_y) {
    return (uint8)Rand((uint16)level, uint16(coord_x * 1.345432), uint16(coord_y * 1.67298), 0x1122) % 5 + 1;
}

uint8 GetEdgeCodeDown(uint32 level, int32 coord_x, int32 coord_y) {
    return (uint8)Rand((uint16)level, uint16(coord_x * 1.345432), uint16(coord_y * 1.67298), 0x2211) % 5 + 1;
}

//
//Returns a 16 bit random number to be used to pick a wall pattern.
//

uint16 GetWallRandomNumber(uint32 level, int32 coord_x, int32 coord_y) {
    return Rand((uint16)level, uint16(coord_x * 61.234) ^ 0x1243, uint16(coord_y * -14.6543) ^ 0x2354, 0x4444);
}

//
//Returns true if the maze unit specified should have no walls.
//

bool GetNoWalls(uint32 level, int32 coord_x, int32 coord_y) {
    return false;
    if (level == 1 && GetNoWalls(level + 1, MapToHigherLevel(coord_x), MapToHigherLevel(coord_y))) {
        //we have no walls either, cause of our parent.
        return true;
    }

    if (level <= 2) {
        //pick a random number
        uint16 rand = Rand((uint16)level, uint16(coord_x * 7.7654) ^ 0x5555, uint16(coord_y * -8.3452) ^ 0x7272, 0x8765);

        //check percent chance.
        return rand < 2000;
    }

    return false;
}

