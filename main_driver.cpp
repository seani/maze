#include "maze3.h"

#include <stdio.h>
#include "bitmapwindows.h"
#include "maze_query.h"


#define MAZE_BOUND 170
#define NUM_SQUARES ((MAZE_BOUND * 2 + 1) * 2 + 1)
#define PIXEL_SIZE 2
#define BITMAP_SIZE (NUM_SQUARES * PIXEL_SIZE)

#define MAP(maze_coord) ((maze_coord + MAZE_BOUND) * 2 + 1)

//a bitmap we generate.
bitmap_windows24 bitmap(BITMAP_SIZE, BITMAP_SIZE);

inline void setpixel(int32 x, int32 y, uint8 r = 58, uint8 g = 110, uint8 b = 165);
inline void setpixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b) {
    #if PIXEL_SIZE == 1
    bitmap.fill(x, y, r, g, b);
    #elif PIXEL_SIZE == 2
    bitmap.fill(x * 2, y * 2, r, g, b);
    bitmap.fill(x * 2 + 1, y * 2, r, g, b);
    bitmap.fill(x * 2, y * 2 + 1, r, g, b);
    bitmap.fill(x * 2 + 1, y * 2 + 1, r, g, b);
    #endif
}

int32 main(int32 argc, char **argv) {
    bool right, down, left, up;
//    CanGo(-472, -246, right, down, left, up);

    int32 x, y;

    //fill in all the diagonal bits.
    for (x = 0; x < NUM_SQUARES; x += 2) {
        //get x coord of space just sw of us.
        int32 coord_x = (x / 2) - MAZE_BOUND - 1;

        for (y = 0; y < NUM_SQUARES; y += 2) {
            //get y coord of space just sw of us
            int32 coord_y = (y / 2) - MAZE_BOUND - 1;

            //check if we can go all around this diagonal bit.
            if (CanGoRight(coord_x, coord_y) && CanGoUp(coord_x, coord_y) &&
                CanGoLeft(coord_x + 1, coord_y + 1) && CanGoDown(coord_x + 1, coord_y + 1))
            {
                //we can go around this bit, dont fill it in.
            }
            else {
                //fill it in.
                setpixel(x, y);
            }
        }
    }

    //go across top and get "up" values for each square.
    for (x = -MAZE_BOUND; x <= MAZE_BOUND; x++) {
        //check which directions we can go.
        CanGo(x, MAZE_BOUND, right, down, left, up);

        //see if we can go up.
        if (up == false) {
            //fill in a wall.
            setpixel(MAP(x), MAP(MAZE_BOUND) + 1);
        }
    }

    //go across left and get "left" values for each square
    for (y = -MAZE_BOUND; y <= MAZE_BOUND; y++) {
        //check which directions we can go.
        CanGo(-MAZE_BOUND, y, right, down, left, up);

        //see if we can go left.
        if (left == false) {
            //fill in a wall.
            setpixel(MAP(-MAZE_BOUND) - 1, MAP(y));
        }
    }

    //fill in "right" and "down" walls for all squares.
    for (x = -MAZE_BOUND; x <= MAZE_BOUND; x++) {
        for (y = -MAZE_BOUND; y <= MAZE_BOUND; y++) {
            //check which directions we can go.
            CanGo(x, y, right, down, left, up);

            //check if we can go right
            if (right == false) {
                setpixel(MAP(x) + 1, MAP(y));
            }

            //check if we can go down
            if (down == false) {
                setpixel(MAP(x), MAP(y) - 1);
            }
        }
    }

    setpixel(MAP(-472), MAP(-246), 255, 0, 0);

    //save the bitmap.
    bitmap.save("output2.bmp");

    return 0;
}


