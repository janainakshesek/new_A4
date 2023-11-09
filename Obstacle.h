#ifndef __OBSTACLE__
#define __OBSTACLE__

#include "Space.h"

typedef struct obstacle {
    int x;
    int y;
    int life;
} obstacle;

void addObstacle(int x, int y, space *board);
void removeObstacle(int x, int y, space *board);


#endif