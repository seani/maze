#include "maze3.h"

#include <stdio.h>
#include "maze_tables.h"

//
//For each clipped corner bit, this array gives the 2 associated wall bits.
//

struct clipped_corner_associated_walls {
    uint32 wall1, wall2;
};

clipped_corner_associated_walls walls_for_clipped_corner[4] = {
    2, 9,
    5, 11,
    3, 8,
    0, 6,
};


//
//This function computes the subset of valid mazes for a specific clipped corner pattern.
//

void ComputeClippedCornerValidMazeSubset(uint32 clipped_corner_pattern) {
    //print array name.
    printf("uint32 clipped_corner_%d_valid_mazes[] = {\n", clipped_corner_pattern);

    //true or false for each square maze pattern.
    bool square_pattern_valid[NUM_VALID_SQUARE_MAZES];

    //assume all patterns are valid for now
    for (uint32 i = 0; i < NUM_VALID_SQUARE_MAZES; i++) {
        square_pattern_valid[i] = true;
    }

    //go through each corner bit.
    for (uint32 corner = 0; corner < 4; corner++) {
        //check if this corner clips into the maze.
        if ((clipped_corner_pattern & (1 << corner)) == 0) {
            //this corner doesnt clip into the maze.
            continue;
        }

        //check each pattern and see if we can clip out this corner without invalidating the maze.
        for (uint32 maze_pattern = 0; maze_pattern < NUM_VALID_SQUARE_MAZES; maze_pattern++) {
            //clipping the corner out of this maze pattern will invalidate it unless
            //this corner square is a "dead end" in the maze.  The corner is a dead end
            //if and only if 1 (cant be both) of the 2 edges that forms the corner is a
            //wall in this particular maze pattern.
            if (is_wall_present(walls_for_clipped_corner[corner].wall1, valid_square_mazes[maze_pattern]) == false &&
                is_wall_present(walls_for_clipped_corner[corner].wall2, valid_square_mazes[maze_pattern]) == false)
            {
                //There is no wall in either position, so this is not a dead end.
                //Therefore clipping out this corner of this maze pattern makes
                //the maze pattern invalid.
                square_pattern_valid[maze_pattern] = false;
            }
        }
    }

    //how many different maze patterns are valid for these clipped corners.
    uint32 num_valid_mazes = 0;

    //print out all the valid mazes that can safely have these corners clipped out.
    for (uint32 pattern = 0; pattern < NUM_VALID_SQUARE_MAZES; pattern++) {
        //check if this is pattern is valid
        if (square_pattern_valid[pattern] == false) continue;

        //print out the maze pattern.
        printf("0x%x, ", valid_square_mazes[pattern]);

        num_valid_mazes++;
    }

    //close the array definition.
    printf("\n};\n");
}

void ComputeClippedCornerValidMazeSubsets() {
    //do each of the 16 clipped corner subsets.
    for (uint32 i = 0; i < 16; i++) {
        //compute the subset for this corner config.
        ComputeClippedCornerValidMazeSubset(i);
    }
}
