//
//
//These tables generalize and optimize certain operations needed
//to generate the maze.
//
//For a full description of each one, see the comments by the 
//the array definitions.
//
//

//
//Enums for corners and directions.
//

typedef enum {
    UP_LEFT,
    UP_RIGHT,
    DOWN_RIGHT,
    DOWN_LEFT,

    NUM_CORNERS,
} CORNER_INDEX;

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,

    NUM_DIRECTIONS,
} DIRECTION_INDEX;


//All the valid maze wall patterns for perfectly square 3x3 mazes.
const uint32 NUM_VALID_SQUARE_MAZES = 192;
extern uint32 valid_square_mazes[NUM_VALID_SQUARE_MAZES];

//
//Structure that stores, for a particular clipped corner pattern, a subset 
//of all valid maze patterns and the length of that subset.
//

struct clipped_corner_valid_mazes_struct {
    //the number of valid mazes for this combination of clipped corners.
    uint32 num_valid_mazes;

    //the array of valid mazes.
    uint32 *valid_mazes;
};

//array for each clipped corner pattern.
extern clipped_corner_valid_mazes_struct valid_clipped_mazes[16];

//array mapping general to relative corner configurations.
extern uint8 corner_config_general_to_relative_mapping[21][NUM_CORNERS];

//walls added when the different corner configs are used.
extern uint32 corner_wall_additions[21][NUM_CORNERS];

//walls removed when the different corner configs are used.
extern uint32 corner_wall_removals[21][NUM_CORNERS];

//mapping from coord offset to square number
extern uint32 coord_offset_to_square_index[3][3];

//gives wall indices for each grid square number
extern uint32 square_index_to_wall_index[9][NUM_DIRECTIONS];

