//
//The CMazeUnit class.  Used internally to generate a maze.
//
//Implementation of the CMazeUnit functions are in the files:
//  maze_unit.cpp
//  maze_unit_debug.cpp
//  maze_query.cpp
//

//
//A full description of a maze unit.  A maze unit is a 3x3 grid, with additions
//and deletions at the corners depending on the corner configuration.  
//

class CMazeUnit {
  public:
    CMazeUnit();

    //
    //Retrieves the specified maze unit.  Results are cached so
    //requent calls for common units is relatively quick.
    //

    static CMazeUnit *Get(uint32 level, int32 coord_x, int32 coord_y);

    //inc/dec our reference count.
    void RefInc();
    void RefDec();

    //debug function.
    void Print();

  private:
    //our reference count.
    int32 ref_count;

  private:

    //The level of the maze.  Zero is not a valid level number.
    uint32 level;

    //Coordinate of this maze unit, when it is seen as a 1x1 square in the
    //level above.
    int32 coord_x, coord_y;

    //the maze pattern
    uint32 walls;

    //the 4 corner configurations.
    uint8 corner_nw, corner_ne, corner_se, corner_sw;

    //edge opening codes for each edge.  A value of 0 means you cant
    //go to the adjacent maze unit.  A value of 1 or 2 means the opening
    //is on the left or top 1/3 of the edge.  A value of 3 means the
    //opening is in the center of the edge.  A value of 4 or 5 means the
    //opening is on the right or bottom 1/3 of the edge.  The exact location
    //of the opening with values 1, 2, 4, and 5 depend on the configurations
    //of the 2 corners.  It is possible to have a non-center (non-3) value
    //specified, even though the current corner configuration makes this
    //physically impossible.  When that is the case, the default position
    //of 3 is used.
    //Also, these values will be 0xff until the CanGo function is called
    //for a square other than the center.
    uint8 edge_top, edge_right, edge_bottom, edge_left;

  private:
    //
    //Takes this unit's level and coordinates, and computes all the 
    //random based parameters such as corner configurations, and wall pattern.
    //Edge codes are not computed unless the CanGo function is called.
    //

    void Init(uint32 level, int32 coord_x, int32 coord_y);

    //
    //Comptues the edge codes by getting this unit's parent and calling its CanGo function.
    //

    void ComputeEdgeCodes();

    void ProcessEdgeLeft();
    void ProcessEdgeTop();
    void ProcessEdgeRight();
    void ProcessEdgeBottom();

    //
    //Helper to remove a wall.
    //
    
    inline void RemoveWall(uint8 wall_index);

  public:
    //
    //Returns true if you can go in the specified direction from the
    //given coordinate.  The coordinate is specified at one level lower
    //than "this" unit's level
    //

    void CanGo(int32 x, int32 y, bool &right, bool &down, bool &left, bool &up);
};

