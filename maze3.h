//CRT headers.
#include <stdlib.h>

//pointer template class that auto calls reference count functions.
#include "template_pointer.h"

//
//Integer types.
//
typedef int int32;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


//
//Maze-wall bit manipulation macros.
//

//macro that evaluates to true if the specified wall exists in the given maze pattern.
#define is_wall_present(wall_index, maze_pattern)\
    (((maze_pattern) & (1 << (wall_index))) == 0)

//evaluates to a bit in the pos position.
#define bit(pos) (1 << (pos))

//evaluates to a wall in the pos position.  Will be a number will all 1's 
//except in the specified position.
#define wall(pos) (~bit(pos))


//
//Helper macros for delete
//

#define delc(ptr) if (ptr != NULL) {delete ptr; ptr = NULL;}


