#include "Obstacle.h"
#include <stdlib.h>

void addObstacle(int x, int y, space *board) {
    obstacle *o = (obstacle*) malloc(sizeof(obstacle));

    o->x = x;
    o->y = y;
    o->life = 10;

    if (!board->map[y][x].entity) {
        board->map[y][x].entity = o;
        board->map[y][x].type = OBSTACLE;
    }
}

void removeObstacle(int x, int y, space *board) {
    if (board->map[y][x].entity) {
        free(board->map[y][x].entity);
        board->map[y][x].entity = NULL;
        board->map[y][x].type = 0;
    }
}