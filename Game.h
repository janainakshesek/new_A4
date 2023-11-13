#ifndef __GAME__ 		
#define __GAME__		

#define X_SCREEN 1500
#define Y_SCREEN 800

#include "Ship.h"
#include "Enemy.h"

#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_image.h>

void update_bullets(ship *player);								
void update_enemy_bullets(enemy *enemy);
void update_position(ship *player);
void draw_enemies (space *board, ALLEGRO_BITMAP *image);
void draw_obstacles (space *board, ALLEGRO_BITMAP *image, ALLEGRO_FONT* font);
void draw_explosion(int x, int y, ALLEGRO_BITMAP *image);
void check_kill(ship *killer, space *board, ALLEGRO_BITMAP *shipIcon);
void check_obstacle_collision(ship *player, space *board);
void check_obstacle_enemy (space *board);
enemy *closer(space *board, ship *player);
int shot_screen (space *board);
void draw_ship(ship *player, ALLEGRO_BITMAP *image);
int check_ship_kill(ship *player, space *board, ALLEGRO_BITMAP *shipIcon);
void verify_shots (space *board, ship *player);
void draw_enemies_shots (space *board, ALLEGRO_BITMAP *image);
int count_enemies (space *board);
int create_cenary (ship *player,  ALLEGRO_BITMAP *shipIcon, ALLEGRO_FONT *font);
void start_game (space *board, ship *player, ALLEGRO_BITMAP *image, ALLEGRO_FONT *font);

#endif 