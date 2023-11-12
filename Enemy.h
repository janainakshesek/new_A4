#ifndef __ENEMY__
#define __ENEMY__

#include "Space.h"
#include "Joystick.h"
#include "Pistol.h"
#include "Bullet.h"

typedef struct shot shot;
struct shot{
	int position_x;
	int position_y;
	int trajectory;
	shot *next;
	shot *previous;
	int shocked;
};

typedef struct shot_sentinel{
	shot *first;
	shot *last;
} shot_sentinel;

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


shot_sentinel* create_shotlist(void);
int search_shot (shot_sentinel *list, int position_x);
void clean_shots(shot_sentinel *list);
shot* remove_shot(shot* current, shot* previous, shot_sentinel *list);
shot_sentinel* straight_shot(space *board, shot_sentinel *list, enemy *shooter);
void update_shots(space *board, shot_sentinel *list);

int add_enemy(space *board, int position_y, int position_x, int side, int face, int type);
int remove_enemy(space *board, int position_y, int position_x);
void update_enemies_position(space *board);
void enemy_shot(enemy *element);

#endif
