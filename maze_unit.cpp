#include "maze3.h"

#include "maze_unit.h"
#include "maze_util.h"
#include "maze_tables.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//
//  The CMazeUnit class.
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


CMazeUnit::CMazeUnit() {
    //set our reference count to zero.
    ref_count = 0;
}

void CMazeUnit::RefInc() {
    ref_count++;
}

void CMazeUnit::RefDec() {
    //decrement count.
    ref_count--;

    //check if all references are gone.
    if (ref_count < 1) {
        //delete ourself
        delete this;
    }
}

inline void CMazeUnit::RemoveWall(uint8 wall_index) {
    //remove the wall.  (Set a bit)
    walls = walls | ~(wall(wall_index));
}

void CMazeUnit::Init(uint32 level, int32 coord_x, int32 coord_y) {
    //save the given parameters
    this->level = level;
    this->coord_x = coord_x;
    this->coord_y = coord_y;

    //get the corner configurations.
    corner_sw = GetCornerConfiguration(level, coord_x, coord_y);
    corner_nw = GetCornerConfiguration(level, coord_x, coord_y + 1);
    corner_ne = GetCornerConfiguration(level, coord_x + 1, coord_y + 1);
    corner_se = GetCornerConfiguration(level, coord_x + 1, coord_y);

    //set the edge codes to uninitialized.
    edge_top = edge_right = edge_bottom = edge_left = 0xff;

    //
    //Compute our maze wall pattern given the 4 corner configurations.
    //

    //Form the clipped corner lookup index from the 4 corner patterns.
    //We check each corner and see if it has relative configuration zero.
    uint32 corner_clip_index = 
        ((corner_config_general_to_relative_mapping[corner_nw][UP_LEFT] == 0) ? 1 : 0) +
        ((corner_config_general_to_relative_mapping[corner_ne][UP_RIGHT] == 0) ? 2 : 0) +
        ((corner_config_general_to_relative_mapping[corner_se][DOWN_RIGHT] == 0) ? 4 : 0) +
        ((corner_config_general_to_relative_mapping[corner_sw][DOWN_LEFT] == 0) ? 8 : 0);

    //pick a random number from 0 to the number of valid mazes for this corner clip index.
    uint32 maze_index = GetWallRandomNumber(level, coord_x, coord_y) %
                          valid_clipped_mazes[corner_clip_index].num_valid_mazes;

    //get the maze pattern itself.
    walls = valid_clipped_mazes[corner_clip_index].valid_mazes[maze_index];

    //randomly empty this room if we are level 1.
    if (GetNoWalls(level, coord_x, coord_y) == true) {
        walls = 0xfff;
    }

    //add extra walls for our corner configs.
    walls = walls & corner_wall_additions[corner_nw][UP_LEFT];
    walls = walls & corner_wall_additions[corner_ne][UP_RIGHT];
    walls = walls & corner_wall_additions[corner_se][DOWN_RIGHT];
    walls = walls & corner_wall_additions[corner_sw][DOWN_LEFT];

    //remove walls for our corner configs.
    walls = walls | corner_wall_removals[corner_nw][UP_LEFT];
    walls = walls | corner_wall_removals[corner_ne][UP_RIGHT];
    walls = walls | corner_wall_removals[corner_se][DOWN_RIGHT];
    walls = walls | corner_wall_removals[corner_sw][DOWN_LEFT];
}

void CMazeUnit::CanGo(int32 x, int32 y, bool &right, bool &down, bool &left, bool &up) {
    //get the extents of the grid square in x direction.
    int32 x_lower, x_upper;
    BoundsOfSubgrid(coord_x, x_lower, x_upper);

    //get the extents of the grid square in the y direction.
    int32 y_lower, y_upper;
    BoundsOfSubgrid(coord_y, y_lower, y_upper);

    //make sure we were given a valid x and y.
    if (x < x_lower || x > x_upper) return;
    if (y < y_lower || y > y_upper) return;

    //inc our own reference count.
    RefInc();

    //check if the requested coords are in the center.
    if (x == x_lower + 1 && y == y_lower + 1) {
        //it is in the exact center.  use our maze wall pattern to check for obstructions.
        right = is_wall_present(4, walls) == false;
        down = is_wall_present(7, walls) == false;
        left = is_wall_present(1, walls) == false;
        up = is_wall_present(10, walls) == false;
    }
    else {
        //First check if our edge codes have been computed.
        if (edge_top == 0xff) {
            //edge codes have not been computed.  Compute them now.
            ComputeEdgeCodes();
        }

        //get our square index.
        uint32 square_index = coord_offset_to_square_index[x - x_lower][y - y_lower];

        //check the walls around us to determine if we can move in the 4 directions.
        right = is_wall_present(square_index_to_wall_index[square_index][RIGHT], walls) == false;
        down = is_wall_present(square_index_to_wall_index[square_index][DOWN], walls) == false;
        left = is_wall_present(square_index_to_wall_index[square_index][LEFT], walls) == false;
        up = is_wall_present(square_index_to_wall_index[square_index][UP], walls) == false;
    }

    //decrement our reference count.
    RefDec();
}

void CMazeUnit::ComputeEdgeCodes() {
    //
    //To get the edge codes, we need to find out if we can get to the adjacent
    //maze units.  To do this, ask the maze unit one level higher.
    //

    //get the maze unit one level above us.
    pointer<CMazeUnit> parent = Get(level + 1, MapToHigherLevel(coord_x), MapToHigherLevel(coord_y));

    //ask the parent unit if we can go in the different directions.
    bool can_go_right, can_go_down, can_go_left, can_go_up;
    parent->CanGo(coord_x, coord_y, can_go_right, can_go_down, can_go_left, can_go_up);

    //
    //pick random edge codes for all the directions, if we can go in that direction.
    //

    edge_right = (can_go_right == false) ? 0 : GetEdgeCodeLeft(level, coord_x + 1, coord_y);
    edge_bottom = (can_go_down == false) ? 0 : GetEdgeCodeDown(level, coord_x, coord_y);
    edge_left = (can_go_left == false) ? 0 : GetEdgeCodeLeft(level, coord_x, coord_y);
    edge_top = (can_go_up == false) ? 0 : GetEdgeCodeDown(level, coord_x, coord_y + 1);

    //
    //Process each edge, breaking down the correct walls.
    //

    //right
    ProcessEdgeRight();

    //down
    ProcessEdgeBottom();

    //left
    ProcessEdgeLeft();

    //up
    ProcessEdgeTop();
}

void CMazeUnit::ProcessEdgeRight() {
    //check if we should have an opening on the top
    if (edge_right == 1 || edge_right == 2) {
        //opening should be in top 1/3 of the edge.  Make sure this is valid given our corner.

        //get our relative corner config for the ne corner.
        uint8 ne_relative = corner_config_general_to_relative_mapping[corner_ne][UP_RIGHT];

        //get ne corner config as seen from maze unit to our right.
        uint8 ne_from_right = corner_config_general_to_relative_mapping[corner_ne][UP_LEFT];

        //check if our corner is bulging in.
        if (ne_relative == 0) {
            //it is bulgin in.

            //check if the unit to the right owns our corner.
            if (ne_from_right == 2 || ne_from_right == 3) {
                //we can bust out either of the 2 walls in this corner.
                if (edge_right == 1) {
                    //bust out wall 5.
                    RemoveWall(5);
                }
                else if (edge_right == 2) {
                    //bust out wall 11
                    RemoveWall(11);
                }
            }
            else {
                //switch to edge code 3.
                edge_right = 3;

                //break middle wall.
                RemoveWall(16);
            }
        }
        //check for config where our corner is square.
        else if (ne_relative == 1) {
            //make sure the square on the right owns it's corner.
            if (ne_from_right != 0) {
                //our neighbor owns it's corner square.  There is only 1 wall to break, break it.
                RemoveWall(17);
            }
            else {
                //switch to edge code 3.
                edge_right = 3;

                //break middle wall.
                RemoveWall(16);
            }
        }
        //check if we protrude into unit on the right.
        else if (ne_relative == 3 || ne_relative == 4) {
            //the walls to bust down are in the unit to the right.  no effect on us.
        }
        //check for config 2, where we protrude into the unit above us only.
        else if (ne_relative == 2) {
            //check if the unit on the right also protrudes up.
            if (ne_from_right == 4) {
                //if we have edge code 2, break down wall 17.
                if (edge_right == 2) {
                    RemoveWall(17);
                }
            }
            //check if the unit on the right owns it's corner square at all.
            else if (ne_from_right == 0) {
                //edge code 1 and 2 are invalid since we dont have a common wall at the top 
                //of this edge.  change to code 3.
                edge_right = 3;

                //break down the wall.
                RemoveWall(16);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(17);
            }
        }
    }
    //check if the opening is in the middle.
    else if (edge_right == 3) {
        //remove the wall in the middle of the edge.
        RemoveWall(16);
    }
    //check if the opening is on the bottom of the edge.
    else if (edge_right == 4 || edge_right == 5) {
        //get the relative corner config for our se corner.
        uint8 se_relative = corner_config_general_to_relative_mapping[corner_se][DOWN_RIGHT];

        //get se corner config as seen from the maze unit to our right.
        uint8 se_from_right = corner_config_general_to_relative_mapping[corner_se][DOWN_LEFT];

        //check if we do not own our corner
        if (se_relative == 0) {
            //check if the unit to the right owns our corner.
            if (se_from_right == 3 || se_from_right == 4) {
                //it owns it.  we can remove either wall, check which one.
                if (edge_right == 4) {
                    RemoveWall(8);
                }
                else if (edge_right == 5) {
                    RemoveWall(3);
                }
            }
            else {
                //switch to edge code 3.
                edge_right = 3;

                //break middle wall.
                RemoveWall(16);
            }
        }
        //check if the corner is square.
        else if (se_relative == 1) {
            //check if the unit on the right owns it's corner.
            if (se_from_right != 0) {
                //it owns the square, bust the only wall we can.
                RemoveWall(15);
            }
            else {
                //switch to edge code 3.
                edge_right = 3;

                //break middle wall.
                RemoveWall(16);
            }
        }
        //check if we protrude into the right unit
        else if (se_relative == 3 || se_relative == 2) {
            //the walls to break are in the other unit, no effect on us.
        }
        //check if we protrude into the unit below us only.
        else if (se_relative == 4) {
            //check if the unit on the right also protrudes down.
            if (se_from_right == 2) {
                //if we have edge code 4
                if (edge_right == 4) {
                    //break the corner wall
                    RemoveWall(15);
                }
            }
            //check if the unit on the right owns it's sw corner at all.
            else if (se_from_right == 0) {
                //it does not own it's corner, we have no common edge to break here.
                //switch to code 3.
                edge_right = 3;

                //break the middle wall.
                RemoveWall(16);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(15);
            }
        }
    }

    //check if we should break wall 17 because it is part of an edge opening
    //between the units above us.
    if (corner_ne == 13) {
        //check if we can go right from the unit above us.

        //get the parent of the maze unit above us.
        pointer<CMazeUnit> north_parent = Get(level + 1, MapToHigherLevel(coord_x), MapToHigherLevel(coord_y + 1));

        //call CanGo to get which directions are not blocked.
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        north_parent->CanGo(coord_x, coord_y + 1, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go right.
        if (can_go_right == true) {
            //get the edge code for the unit above us (not the parent) to determine
            //if the opening is down at the bottom where it protrudes into our territority.
            if (GetEdgeCodeLeft(level, coord_x + 1, coord_y + 1) == 5) {
                //yes, break down the wall
                RemoveWall(17);
            }
        }
    }

    //check if we should break down wall 15 cause it is part of an edge opening
    //between the units below us.
    if (corner_se == 12) {
        //check if we can go right from the unit below us.

        //get the parent of the unit below us
        pointer<CMazeUnit> south_parent = Get(level + 1, MapToHigherLevel(coord_x), MapToHigherLevel(coord_y - 1));

        //call CanGo to get which directions are not blocked
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        south_parent->CanGo(coord_x, coord_y - 1, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go right
        if (can_go_right == true) {
            //get the right edge code for the unit below us.  
            if (GetEdgeCodeLeft(level, coord_x + 1, coord_y - 1) == 1) {
                //break the wall down.
                RemoveWall(15);
            }
        }
    }
}

void CMazeUnit::ProcessEdgeTop() {
    //check if we should have an opening on the left
    if (edge_top == 1 || edge_top == 2) {
        //opening should be in left 1/3 of the edge.  Make sure this is valid given our corner.

        //get our relative corner config for the nw corner.
        uint8 nw_relative = corner_config_general_to_relative_mapping[corner_nw][UP_LEFT];

        //get nw corner config as seen from maze unit above us.
        uint8 nw_from_up = corner_config_general_to_relative_mapping[corner_nw][DOWN_LEFT];

        //check if the corner is bulging in.
        if (nw_relative == 0) {
            //it is bulging in.

            //check if the unit above owns our corner.
            if (nw_from_up == 2 || nw_from_up == 3) {
                //we can bust out either of the 2 walls in this corner.
                if (edge_top == 1) {
                    //bust out wall 9.
                    RemoveWall(9);
                }
                else if (edge_top == 2) {
                    //bust out wall 2
                    RemoveWall(2);
                }
            }
            else {
                //switch to edge code 3.
                edge_top = 3;

                //break middle wall.
                RemoveWall(22);
            }
        }
        //check for config where our corner is square.
        else if (nw_relative == 1) {
            //make sure the unit above owns it's corner.
            if (nw_from_up != 0) {
                //our neighbor owns it's corner square.  There is only 1 wall to break, break it.
                RemoveWall(21);
            }
            else {
                //switch to edge code 3.
                edge_top = 3;

                //break middle wall.
                RemoveWall(22);
            }
        }
        //check if we protrude into unit above.
        else if (nw_relative == 3 || nw_relative == 4) {
            //the walls to bust down are in the unit above.  no effect on us.
        }
        //check for config 2, where we protrude into the unit to the left only.
        else if (nw_relative == 2) {
            //check if the unit above also protrudes left.
            if (nw_from_up == 4) {
                //if we have edge code 2, break down wall 21.
                if (edge_top == 2) {
                    RemoveWall(21);
                }
            }
            //check if the unit above owns its corner square at all.
            else if (nw_from_up == 0) {
                //edge code 1 and 2 are invalid since we dont have a common wall at the left
                //of this edge.  change to code 3.
                edge_top = 3;

                //break down the wall.
                RemoveWall(22);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(21);
            }
        }
    }
    //check if the opening is in the middle.
    else if (edge_top == 3) {
        //remove the wall in the middle of the edge.
        RemoveWall(22);
    }
    //check if the opening is on the right of the edge.
    else if (edge_top == 4 || edge_top == 5) {
        //get the relative corner config for our ne corner.
        uint8 ne_relative = corner_config_general_to_relative_mapping[corner_ne][UP_RIGHT];

        //get ne corner config as seen from the maze unit above.
        uint8 ne_from_up = corner_config_general_to_relative_mapping[corner_ne][DOWN_RIGHT];

        //check if we do not own our corner
        if (ne_relative == 0) {
            //check if the unit above owns our corner.
            if (ne_from_up == 3 || ne_from_up == 4) {
                //it owns it.  we can remove either wall, check which one.
                if (edge_top == 4) {
                    RemoveWall(5);
                }
                else if (edge_top == 5) {
                    RemoveWall(11);
                }
            }
            else {
                //switch to edge code 3.
                edge_top = 3;

                //break middle wall.
                RemoveWall(22);
            }
        }
        //check if the corner is square.
        else if (ne_relative == 1) {
            //check if the unit above owns it's corner.
            if (ne_from_up != 0) {
                //it owns the square, bust the only wall we can.
                RemoveWall(23);
            }
            else {
                //switch to edge code 3.
                edge_top = 3;

                //break middle wall.
                RemoveWall(22);
            }
        }
        //check if we protrude into unit above
        else if (ne_relative == 3 || ne_relative == 2) {
            //the walls to break are in the other unit, no effect on us.
        }
        //check if we protrude into the unit to the right only.
        else if (ne_relative == 4) {
            //check if the unit above also protrudes to the right.
            if (ne_from_up == 2) {
                //if we have edge code 4
                if (edge_top == 4) {
                    //break the corner wall
                    RemoveWall(23);
                }
            }
            //check if the unit above owns it's se corner at all.
            else if (ne_from_up == 0) {
                //it does not own it's corner, we have no common edge to break here.
                //switch to code 3.
                edge_top = 3;

                //break the middle wall.
                RemoveWall(22);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(23);
            }
        }
    }

    //check if we should break wall 21 because it is part of an edge opening
    //between the units to the left.
    if (corner_nw == 14) {
        //check if we can go up from the unit to the left.

        //get the parent of the maze unit to the left.
        pointer<CMazeUnit> west_parent = Get(level + 1, MapToHigherLevel(coord_x - 1), MapToHigherLevel(coord_y));

        //call CanGo to get which directions are not blocked.
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        west_parent->CanGo(coord_x - 1, coord_y, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go up.
        if (can_go_up == true) {
            //get the top edge code for the unit to the left (not the parent) to determine
            //if the opening is on the right where it protrudes into our territority.
            if (GetEdgeCodeDown(level, coord_x - 1, coord_y + 1) == 5) {
                //yes, break down the wall
                RemoveWall(21);
            }
        }
    }

    //check if we should break down wall 23 cause it is part of an edge opening
    //between the units to the right.
    if (corner_ne == 15) {
        //check if we can go up from the unit to the right.

        //get the parent of the unit to the right
        pointer<CMazeUnit> east_parent = Get(level + 1, MapToHigherLevel(coord_x + 1), MapToHigherLevel(coord_y));

        //call CanGo to get which directions are not blocked
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        east_parent->CanGo(coord_x + 1, coord_y, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go up
        if (can_go_up == true) {
            //get the top edge code for the unit to the right.  
            if (GetEdgeCodeDown(level, coord_x + 1, coord_y + 1) == 1) {
                //break the wall down.
                RemoveWall(23);
            }
        }
    }
}

void CMazeUnit::ProcessEdgeLeft() {
    //check if we should have an opening on the top
    if (edge_left == 1 || edge_left == 2) {
        //opening should be in top 1/3 of the edge.  Make sure this is valid given our corner.

        //get our relative corner config for the nw corner.
        uint8 nw_relative = corner_config_general_to_relative_mapping[corner_nw][UP_LEFT];

        //get nw corner config as seen from maze unit to our left.
        uint8 nw_from_left = corner_config_general_to_relative_mapping[corner_nw][UP_RIGHT];

        //check if our corner is bulging in.
        if (nw_relative == 0) {
            //it is bulgin in.

            //check if the unit to the left owns our corner.
            if (nw_from_left == 4 || nw_from_left == 3) {
                //we can bust out either of the 2 walls in this corner.
                if (edge_left == 1) {
                    //bust out wall 2.
                    RemoveWall(2);
                }
                else if (edge_left == 2) {
                    //bust out wall 9
                    RemoveWall(9);
                }
            }
            else {
                //switch to edge code 3.
                edge_left = 3;

                //break middle wall.
                RemoveWall(13);
            }
        }
        //check for config where our corner is square.
        else if (nw_relative == 1) {
            //make sure the square on the left owns it's corner.
            if (nw_from_left != 0) {
                //our neighbor owns it's corner square.  There is only 1 wall to break, break it.
                RemoveWall(14);
            }
            else {
                //switch to edge code 3.
                edge_left = 3;

                //break middle wall.
                RemoveWall(13);
            }
        }
        //check if we protrude into unit on the left.
        else if (nw_relative == 3 || nw_relative == 2) {
            //the walls to bust down are in the unit to the left.  no effect on us.
        }
        //check for config 4, where we protrude into the unit above us only.
        else if (nw_relative == 4) {
            //check if the unit on the left also protrudes up.
            if (nw_from_left == 2) {
                //if we have edge code 2, break down wall 14.
                if (edge_left == 2) {
                    RemoveWall(14);
                }
            }
            //check if the unit on the left owns its corner square at all.
            else if (nw_from_left == 0) {
                //edge code 1 and 2 are invalid since we dont have a common wall at the top 
                //of this edge.  change to code 3.
                edge_left = 3;

                //break down the wall.
                RemoveWall(13);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(14);
            }
        }
    }
    //check if the opening is in the middle.
    else if (edge_left == 3) {
        //remove the wall in the middle of the edge.
        RemoveWall(13);
    }
    //check if the opening is on the bottom of the edge.
    else if (edge_left == 4 || edge_left == 5) {
        //get the relative corner config for our sw corner.
        uint8 sw_relative = corner_config_general_to_relative_mapping[corner_sw][DOWN_LEFT];

        //get sw corner config as seen from the maze unit to our left.
        uint8 sw_from_left = corner_config_general_to_relative_mapping[corner_sw][DOWN_RIGHT];

        //check if we do not own our corner
        if (sw_relative == 0) {
            //check if the unit to the left owns our corner.
            if (sw_from_left == 3 || sw_from_left == 2) {
                //it owns it.  we can remove either wall, check which one.
                if (edge_left == 4) {
                    RemoveWall(6);
                }
                else if (edge_left == 5) {
                    RemoveWall(0);
                }
            }
            else {
                //switch to edge code 3.
                edge_left = 3;

                //break middle wall.
                RemoveWall(13);
            }
        }
        //check if the corner is square.
        else if (sw_relative == 1) {
            //check if the unit on the left owns it's corner.
            if (sw_from_left != 0) {
                //it owns the square, bust the only wall we can.
                RemoveWall(12);
            }
            else {
                //switch to edge code 3.
                edge_left = 3;

                //break middle wall.
                RemoveWall(13);
            }
        }
        //check if we protrude into the left unit
        else if (sw_relative == 3 || sw_relative == 4) {
            //the walls to break are in the other unit, no effect on us.
        }
        //check if we protrude into the unit below us only.
        else if (sw_relative == 2) {
            //check if the unit on the right also protrudes down.
            if (sw_from_left == 4) {
                //if we have edge code 4
                if (edge_left == 4) {
                    //break the corner wall
                    RemoveWall(12);
                }
            }
            //check if the unit on the left owns it's se corner at all.
            else if (sw_from_left == 0) {
                //it does not own it's corner, we have no common edge to break here.
                //switch to code 3.
                edge_left = 3;

                //break the middle wall.
                RemoveWall(13);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(12);
            }
        }
    }

    //check if we should break wall 14 because it is part of an edge opening
    //between the units above us.
    if (corner_nw == 13) {
        //check if we can go left from the unit above us.

        //get the parent of the maze unit above us.
        pointer<CMazeUnit> north_parent = Get(level + 1, MapToHigherLevel(coord_x), MapToHigherLevel(coord_y + 1));

        //call CanGo to get which directions are not blocked.
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        north_parent->CanGo(coord_x, coord_y + 1, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go left.
        if (can_go_left == true) {
            //get the left edge code for the unit above us (not the parent) to determine
            //if the opening is down at the bottom where it protrudes into our territority.
            if (GetEdgeCodeLeft(level, coord_x, coord_y + 1) == 5) {
                //yes, break down the wall
                RemoveWall(14);
            }
        }
    }

    //check if we should break down wall 12 cause it is part of an edge opening
    //between the units below us.
    if (corner_sw == 12) {
        //check if we can go left from the unit below us.

        //get the parent of the unit below us
        pointer<CMazeUnit> south_parent = Get(level + 1, MapToHigherLevel(coord_x), MapToHigherLevel(coord_y - 1));

        //call CanGo to get which directions are not blocked
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        south_parent->CanGo(coord_x, coord_y - 1, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go left
        if (can_go_left == true) {
            //get the left edge code for the unit below us.  
            if (GetEdgeCodeLeft(level, coord_x, coord_y - 1) == 1) {
                //break the wall down.
                RemoveWall(12);
            }
        }
    }
}

void CMazeUnit::ProcessEdgeBottom() {
    //check if we should have an opening on the left
    if (edge_bottom == 1 || edge_bottom == 2) {
        //opening should be in left 1/3 of the edge.  Make sure this is valid given our corner.

        //get our relative corner config for the sw corner.
        uint8 sw_relative = corner_config_general_to_relative_mapping[corner_sw][DOWN_LEFT];

        //get sw corner config as seen from maze unit above us.
        uint8 sw_from_down = corner_config_general_to_relative_mapping[corner_sw][UP_LEFT];

        //check if the corner is bulging in.
        if (sw_relative == 0) {
            //it is bulging in.

            //check if the unit below owns our corner.
            if (sw_from_down == 4 || sw_from_down == 3) {
                //we can bust out either of the 2 walls in this corner.
                if (edge_bottom == 1) {
                    //bust out wall 6.
                    RemoveWall(6);
                }
                else if (edge_bottom == 2) {
                    //bust out wall 0
                    RemoveWall(0);
                }
            }
            else {
                //switch to edge code 3.
                edge_bottom = 3;

                //break middle wall.
                RemoveWall(19);
            }
        }
        //check for config where our corner is square.
        else if (sw_relative == 1) {
            //make sure the unit below owns it's corner.
            if (sw_from_down != 0) {
                //our neighbor owns it's corner square.  There is only 1 wall to break, break it.
                RemoveWall(18);
            }
            else {
                //switch to edge code 3.
                edge_bottom = 3;

                //break middle wall.
                RemoveWall(19);
            }
        }
        //check if we protrude into unit below.
        else if (sw_relative == 3 || sw_relative == 2) {
            //the walls to bust down are in the unit below.  no effect on us.
        }
        //check for config 4, where we protrude into the unit to the left only.
        else if (sw_relative == 4) {
            //check if the unit below also protrudes left.
            if (sw_from_down == 2) {
                //if we have edge code 2, break down wall 18.
                if (edge_bottom == 2) {
                    RemoveWall(18);
                }
            }
            //check if the unit below owns its corner square at all.
            else if (sw_from_down == 0) {
                //edge code 1 and 2 are invalid since we dont have a common wall at the left
                //of this edge.  change to code 3.
                edge_bottom = 3;

                //break down the wall.
                RemoveWall(19);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(18);
            }
        }
    }
    //check if the opening is in the middle.
    else if (edge_bottom == 3) {
        //remove the wall in the middle of the edge.
        RemoveWall(19);
    }
    //check if the opening is on the right of the edge.
    else if (edge_bottom == 4 || edge_bottom == 5) {
        //get the relative corner config for our se corner.
        uint8 se_relative = corner_config_general_to_relative_mapping[corner_se][DOWN_RIGHT];

        //get ne corner config as seen from the maze unit above.
        uint8 se_from_down = corner_config_general_to_relative_mapping[corner_se][UP_RIGHT];

        //check if we do not own our corner
        if (se_relative == 0) {
            //check if the unit below owns our corner.
            if (se_from_down == 3 || se_from_down == 2) {
                //it owns it.  we can remove either wall, check which one.
                if (edge_bottom == 4) {
                    RemoveWall(3);
                }
                else if (edge_bottom == 5) {
                    RemoveWall(8);
                }
            }
            else {
                //switch to edge code 3.
                edge_bottom = 3;

                //break middle wall.
                RemoveWall(19);
            }
        }
        //check if the corner is square.
        else if (se_relative == 1) {
            //check if the unit below owns it's corner.
            if (se_from_down != 0) {
                //it owns the square, bust the only wall we can.
                RemoveWall(20);
            }
            else {
                //switch to edge code 3.
                edge_bottom = 3;

                //break middle wall.
                RemoveWall(19);
            }
        }
        //check if we protrude into unit below
        else if (se_relative == 3 || se_relative == 4) {
            //the walls to break are in the other unit, no effect on us.
        }
        //check if we protrude into the unit to the right only.
        else if (se_relative == 2) {
            //check if the unit below also protrudes to the right.
            if (se_from_down == 4) {
                //if we have edge code 4
                if (edge_bottom == 4) {
                    //break the corner wall
                    RemoveWall(20);
                }
            }
            //check if the unit below owns it's ne corner at all.
            else if (se_from_down == 0) {
                //it does not own it's corner, we have no common edge to break here.
                //switch to code 3.
                edge_bottom = 3;

                //break the middle wall.
                RemoveWall(19);
            }
            else {
                //break down the only wall we have in common.
                RemoveWall(20);
            }
        }
    }

    //check if we should break wall 18 because it is part of an edge opening
    //between the units to the left.
    if (corner_sw == 14) {
        //check if we can go down from the unit to the left.

        //get the parent of the maze unit to the left.
        pointer<CMazeUnit> west_parent = Get(level + 1, MapToHigherLevel(coord_x - 1), MapToHigherLevel(coord_y));

        //call CanGo to get which directions are not blocked.
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        west_parent->CanGo(coord_x - 1, coord_y, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go down.
        if (can_go_down == true) {
            //get the bottom edge code for the unit to the left (not the parent) to determine
            //if the opening is on the right where it protrudes into our territority.
            if (GetEdgeCodeDown(level, coord_x - 1, coord_y) == 5) {
                //yes, break down the wall
                RemoveWall(18);
            }
        }
    }

    //check if we should break down wall 20 cause it is part of an edge opening
    //between the units to the right.
    if (corner_se == 15) {
        //check if we can go down from the unit to the right.

        //get the parent of the unit to the right
        pointer<CMazeUnit> east_parent = Get(level + 1, MapToHigherLevel(coord_x + 1), MapToHigherLevel(coord_y));

        //call CanGo to get which directions are not blocked
        bool can_go_right, can_go_down, can_go_left, can_go_up;
        east_parent->CanGo(coord_x + 1, coord_y, can_go_right, can_go_down, can_go_left, can_go_up);

        //check if we can go down
        if (can_go_down == true) {
            //get the bottom edge code for the unit to the right.  
            if (GetEdgeCodeDown(level, coord_x + 1, coord_y) == 1) {
                //break the wall down.
                RemoveWall(20);
            }
        }
    }
}

