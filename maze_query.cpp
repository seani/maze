#include "maze3.h"

#include "maze_unit.h"
#include "maze_util.h"

//maze unit cache of one grid per level.
pointer<CMazeUnit> maze_unit_cache[100];

CMazeUnit *CMazeUnit::Get(uint32 level, int32 coord_x, int32 coord_y) {
    //check if there is a maze unit in the cache for this level.
    if (maze_unit_cache[level].IsNull() == false) {
        //there is a maze unit cached, check if it is the correct one.
        if (coord_x == maze_unit_cache[level]->coord_x &&
            coord_y == maze_unit_cache[level]->coord_y)
        {
            //the one in the cache is the one they are asking for.
            return maze_unit_cache[level];
        }
    }

    //make a new maze unit
    CMazeUnit *maze_unit = new CMazeUnit();

    //initialize it.
    maze_unit->Init(level, coord_x, coord_y);

    //put it in the cache
    maze_unit_cache[level] = maze_unit;

    //return it.
    return maze_unit;
}

void CanGo(int32 x, int32 y, bool &right, bool &down, bool &left, bool &up) {
    //get maze unit that owns this coordinate.
    pointer<CMazeUnit> maze_unit = CMazeUnit::Get(1, MapToHigherLevel(x), MapToHigherLevel(y));

    //query which directions we can take.
    maze_unit->CanGo(x, y, right, down, left, up);
}

bool CanGoRight(int32 x, int32 y) {
    //query which directions we can take.
    bool right, down, left, up;
    CanGo(x, y, right, down, left, up);

    //return the correct bool
    return right;
}

bool CanGoDown(int32 x, int32 y) {
    //query which directions we can take.
    bool right, down, left, up;
    CanGo(x, y, right, down, left, up);

    //return the correct bool
    return down;
}

bool CanGoLeft(int32 x, int32 y) {
    //query which directions we can take.
    bool right, down, left, up;
    CanGo(x, y, right, down, left, up);

    //return the correct bool
    return left;
}

bool CanGoUp(int32 x, int32 y) {
    //query which directions we can take.
    bool right, down, left, up;
    CanGo(x, y, right, down, left, up);

    //return the correct bool
    return up;
}



