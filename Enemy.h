#ifndef __ENEMY__
#define __ENEMY__

#include "Space.h"
#include "Joystick.h"
#include "Pistol.h"
#include "Bullet.h"

typedef struct enemy{
	unsigned short x;																											
	unsigned short y;	
	unsigned char side;	
	unsigned char face;		
	joystick *control;	
	int trajectory;	
	int img;
	int up;
	pistol *gun;																									
	enum {WEAK, INTERMEDIARY, STRONG} type;
} enemy;

int add_enemy(space *board, int position_y, int position_x, int side, int face, int type);
int remove_enemy(space *board, int position_y, int position_x);
void update_enemies_position(space *board);
void enemy_shot(enemy *element);

#endif
