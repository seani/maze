
//
//
//These functions are the main top-level maze query functions.
//
//

//
//Returns true if you can go in any of the 4 directions for the given coordinate.
//

void CanGo(int32 x, int32 y, bool &right, bool &down, bool &left, bool &up);

//
//4 functions that return true for one direction each.  Equivalent
//to calling the normal CanGo() function.
//

bool CanGoRight(int32 x, int32 y);
bool CanGoDown(int32 x, int32 y);
bool CanGoLeft(int32 x, int32 y);
bool CanGoUp(int32 x, int32 y);
