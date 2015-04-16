#include "maze3.h"

#include <stdio.h>
#include "maze_unit.h"

//
//These 12 arrays tell us which bits are set in the different corner configs.
//
//"left", "right", "up", and "down" refer to the 4 walls that can make a plus
//in the center of the corner config.
//
//"left_up", and "left_down" refer to the 2 vertical edges on the left side,
//"up_left", and "up_right" refer to the 2 horizontal edges on the top side, etc.
//

bool cc_bit_left[21] = {
    true, true, true, true,
    false, false, true, true,
    false, true, true, false,
    false, false, true, true,
    true, true, false, false, true,
};

bool cc_bit_right[21] = {
    true, true, true, true,
    true, true, false, false,
    true, false, false, true,
    false, false, true, true,
    true, true, false, false, true,
};

bool cc_bit_up[21] = {
    true, true, false, false,
    true, true, true, true,
    true, true, false, false,
    true, true, false, false,
    false, false, true, true, true,
};

bool cc_bit_down[21] = {
    false, false, true, true,
    true, true, true, true,
    false, false, true, true,
    true, true, false, false, 
    false, false, true, true, true,
};

bool cc_bit_left_down[21] = {
    true, false, false, false, 
    false, true, false, false, 
    false, true, false, true,
    false, true, false, true, 
    true, false, true, false, false,
};

bool cc_bit_left_up[21] = {
    false, false, true, false,
    true, false, false, false,
    true, false, true, false,
    true, false, false, true,
    false, true, false, true, false,
};

bool cc_bit_up_left[21] = {
    false, false, true, false, 
    true, false, false, false,
    true, false, true, false,
    true, false, false, true, 
    false, true, false, true, false,
};

bool cc_bit_up_right[21] = {
    false, false, false, true,
    false, false, true, false,
    false, true, false, true,
    true, false, true, false, 
    true, false, true, false, false, 
};

bool cc_bit_right_up[21] = {
    false, false, false, true, 
    false, false, true, false, 
    false, true, false, true, 
    true, false, true, false, 
    true, false, true, false, false, 
};

bool cc_bit_right_down[21] = {
    false, true, false, false, 
    false, false, false, true, 
    true, false, true, false, 
    false, true, true, false, 
    false, true, false, true, false,
};

bool cc_bit_down_right[21] = {
    false, true, false, false, 
    false, false, false, true, 
    true, false, true, false, 
    false, true, true, false,
    false, true, false, true, false,
};

bool cc_bit_down_left[21] = {
    true, false, false, false, 
    false, true, false, false,
    false, true, false, true, 
    false, true, false, true, 
    true, false, true, false, false,
};

#define hc(val) ((val) ? "--" : "  ")
#define vc(val) ((val) ? "|" : " ")
#define hw(num) hc(is_wall_present(num, walls))
#define vw(num) vc(is_wall_present(num, walls))

void CMazeUnit::Print() {
    printf("Level %d   X: %d (%d %d) Y: %d (%d %d)\n", level, coord_x, coord_x * 3 - 1, coord_x * 3 + 1, coord_y, coord_y * 3 - 1, coord_y * 3 + 1);
    printf("L: %d  T: %d  R: %d  B: %d\n", edge_left, edge_top, edge_right, edge_bottom);
    
    printf(" %s+%s    %s+%s \n", hc(cc_bit_up_left[corner_nw]), hc(cc_bit_up_right[corner_nw]), 
                                 hc(cc_bit_up_left[corner_ne]), hc(cc_bit_up_right[corner_ne]));
    printf("%s  %s  %s  %s  %s  %s\n", vc(cc_bit_left_up[corner_nw]), vc(cc_bit_up[corner_nw]), vc(cc_bit_right_up[corner_nw]),
                                       vc(cc_bit_left_up[corner_ne]), vc(cc_bit_up[corner_ne]), vc(cc_bit_right_up[corner_ne]));
    printf("+%so%s+%s+%so%s+\n", hc(cc_bit_left[corner_nw]), 
                                 hw(21), hw(22), hw(23),
                                 hc(cc_bit_right[corner_ne]));
    printf("%s  %s  %s  %s  %s  %s\n", vc(cc_bit_left_down[corner_nw]), 
                                       vw(14), vw(2), vw(5), vw(17),
                                       vc(cc_bit_right_down[corner_ne]));
    printf(" %s+%s+%s+%s+%s\n", hc(cc_bit_down_left[corner_nw]),
                                hw(9), hw(10), hw(11),
                                hc(cc_bit_down_right[corner_ne]));
    printf("   %s  %s  %s  %s\n", vw(13), vw(1), vw(4), vw(16));
    printf(" %s+%s+%s+%s+%s\n", hc(cc_bit_up_left[corner_sw]),
                                hw(6), hw(7), hw(8),
                                hc(cc_bit_up_right[corner_se]));
    printf("%s  %s  %s  %s  %s  %s\n", vc(cc_bit_left_up[corner_sw]),
                                       vw(12), vw(0), vw(3), vw(15),
                                       vc(cc_bit_right_up[corner_se]));
    printf("+%so%s+%s+%so%s+\n", hc(cc_bit_left[corner_sw]),
                                 hw(18), hw(19), hw(20),
                                 hc(cc_bit_right[corner_se]));
    printf("%s  %s  %s  %s  %s  %s\n", vc(cc_bit_left_down[corner_sw]), vc(cc_bit_down[corner_sw]), vc(cc_bit_right_down[corner_sw]),
                                       vc(cc_bit_left_down[corner_se]), vc(cc_bit_down[corner_se]), vc(cc_bit_right_down[corner_se]));
    printf(" %s+%s    %s+%s \n", hc(cc_bit_down_left[corner_sw]), hc(cc_bit_down_right[corner_sw]), 
                                 hc(cc_bit_down_left[corner_se]), hc(cc_bit_down_right[corner_se]));

    printf("\n");
}
