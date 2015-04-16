//
//
//Grid coordinate range functions.
//
//

int32 MapToHigherLevel(int32 coord);
void BoundsOfGrid(int32 coord, int32 &lower, int32 &upper);
void BoundsOfSubgrid(int32 coord, int32 &lower, int32 &upper);


//
//
//Random number picking functions.
//
//

uint8 GetCornerConfiguration(uint32 level, int32 corner_x, int32 corner_y);
uint8 GetEdgeCodeLeft(uint32 level, int32 coord_x, int32 coord_y);
uint8 GetEdgeCodeDown(uint32 level, int32 coord_x, int32 coord_y);
uint16 GetWallRandomNumber(uint32 level, int32 coord_x, int32 coord_y);
bool GetNoWalls(uint32 level, int32 coord_x, int32 coord_y);




