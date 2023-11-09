#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Ship.h"
#include "Bullet.h"
#include "Space.h"
#include "Enemy.h"
#include "Obstacle.h"

#include <allegro5/allegro5.h>														
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#define X_SCREEN 1500
#define Y_SCREEN 800

void update_bullets(ship *player){									
	bullet *previous = NULL;																																											
	for (bullet *index = player->gun->shots; index != NULL;){		
		if (index->shoked) {
			if (previous){																																													
				previous->next = index->next;																																								
				bullet_destroy(index);																																										
				index = (bullet*) previous->next;																																							
			} else {
				player->gun->shots = (bullet*) index->next;											
				bullet_destroy(index);
				index = player->gun->shots;																																									
			}
		} else {
			if (!index->trajectory) index->y -= BULLET_MOVE;																																					
			else if (index->trajectory == 1) index->y += BULLET_MOVE;																																			
		
			if ((index->y < 0) || (index->y > Y_SCREEN)){																																						
				if (previous){																																													
					previous->next = index->next;																																								
					bullet_destroy(index);																																										
					index = (bullet*) previous->next;																																							
				} else {																																															
					player->gun->shots = (bullet*) index->next;																																					
					bullet_destroy(index);																																										
					index = player->gun->shots;																																									
				}
			} else {																																																
				previous = index;																																												
				index = (bullet*) index->next;																																									
			}
		}																															
	}
}

void update_position(ship *player){																																				
	
	if (player->control->left){																																										
		ship_move(player, 1, 0, X_SCREEN, Y_SCREEN);																																				
	}
	
	if (player->control->right){ 																																										
		ship_move(player, 1, 1, X_SCREEN, Y_SCREEN);																																				
	}
	
	if (player->control->fire){		
		if (!player->gun->timer){																																										
			ship_shot(player);																																											
			player->gun->timer = PISTOL_COOLDOWN;																																							
		}
	}
	update_bullets(player);
}	

void draw_enemies (space *board, ALLEGRO_BITMAP *image) {
	enemy *aux;
	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity) {
				if (board->map[i][j].type == ENEMY) { 
					aux = board->map[i][j].entity;
					if (aux->type == WEAK) {
						al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, 15, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, j*70+300, i*50+70, 4.5, 4.5, 0, 0);	
					} else if (aux->type == INTERMEDIARY) { 
						al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 18, 15, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, j*70+300, i*50+70, 4.5, 4.5, 0, 0);	
					} else if (aux->type == STRONG) {
						al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 35, 15, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, j*70+300, i*50+70, 4.5, 4.5, 0, 0);	
					}
				}
			}
		}
	}	
}

void draw_obstacles (space *board, ALLEGRO_BITMAP *image) {
	obstacle *aux;
	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity) {
				if (board->map[i][j].type == OBSTACLE) { 
					aux = board->map[i][j].entity;
					if (aux->life > 7)
						al_draw_tinted_scaled_rotated_bitmap_region(image, 50, 20, 30, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, j*130, i*50+50, 5, 5, 0, 0);
					else if (aux->life <= 7 && aux->life > 5)
						al_draw_tinted_scaled_rotated_bitmap_region(image, 50, 35, 30, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, j*130, i*50+50, 5, 5, 0, 0);
					else if (aux->life <= 5)
						al_draw_tinted_scaled_rotated_bitmap_region(image, 50, 45, 30, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, j*130, i*50+50, 5, 5, 0, 0);
				}
			}
		}
	}	
}

unsigned char check_kill(ship *killer, space *board){					
	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity) {
					if (board->map[i][j].type == ENEMY) {
						if ((index->x >= ((j*70+300)-20)) && (index->x <= ((j*70+300)+20)) && (index->y == (i*50+50))) {
							index->shoked = 1;
							remove_enemy(board, i, j);
						}
					}
				}
			}
		}
	}
}

unsigned char check_obstacle_collision(ship *player, space *board) {
	obstacle *o;
	for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next){
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity) {
					if (board->map[i][j].type == OBSTACLE) {
						if ((index->x >= ((j*130)-10)) && (index->x <= ((j*130)+100)) && (index->y == (i*50+50))) {
							index->shoked = 1;
							o = board->map[i][j].entity;
							if (o->life > 0) {
								o->life--;
							} else 
								removeObstacle(j, i, board);
						}
					}
				}
			}
		}
	}
}

void update_enemies_position (space *board) {
	enemy *aux;
	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
				aux = board->map[i][j].entity;
				if (aux->x < board->max_x) {
					aux->x+=10;
				} else {
					aux->x--;
				}
			}
		}
	}
}


int main(int argc, char** argv){
	int x = 11;
	int y = 10;
	int e = 4;
	int started = 0;

	al_init();																																															
	al_init_image_addon();		
	al_init_primitives_addon();																																									
	
	al_install_keyboard();																																												

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																					
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																				
	ALLEGRO_FONT* font = al_create_builtin_font();																																						
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																																		

	al_register_event_source(queue, al_get_keyboard_event_source());																																	
	al_register_event_source(queue, al_get_display_event_source(disp));																																	
	al_register_event_source(queue, al_get_timer_event_source(timer));																																	


	if (!al_init_image_addon()) {
    	fprintf(stderr, "Falha ao inicializar o módulo de imagens.\n");
   		return -2;
	}				
	
	ALLEGRO_BITMAP *gameIcon = al_load_bitmap("./imgs/GameIcon.png");
	if (!gameIcon) {
		fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
		return -3;
	}
	ALLEGRO_BITMAP *shipIcon = al_load_bitmap("./imgs/SpaceInvaders.png");
	if (!shipIcon) {
		fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
		return -3;
	}

	ship *player = ship_create(50, 0, 550, 650, X_SCREEN, Y_SCREEN);	
	space *board = create_board(y, x, e);

	ALLEGRO_EVENT event;																																												
	al_start_timer(timer);	

	while(1){																																															
		al_wait_for_event(queue, &event);

		if (event.type == 30){
			al_clear_to_color(al_map_rgb(0, 0, 0));	

			if (!started) {
				al_draw_scaled_bitmap(gameIcon, 0, 0, 600, 600, 600, 200, 300, 300, 0);
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 500, ALLEGRO_ALIGN_CENTER, "SPACE INVADERS");
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 700, ALLEGRO_ALIGN_CENTER, "APERTE ENTER PARA JOGAR");
			} else {
				update_position(player);	
				check_kill(player, board);
				check_obstacle_collision(player, board);
				update_enemies_position(board);	
				
				al_draw_tinted_scaled_rotated_bitmap_region(shipIcon, 67, 0, 10, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, player->x, player->y, 5, 5, 0, 0);	
				draw_enemies(board, shipIcon);
				draw_obstacles(board, shipIcon);

				for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next) {
					al_draw_filled_circle((index->x+27), (index->y+10), 5, al_map_rgb(0, 0, 255));
	    			if (player->gun->timer) 
						player->gun->timer--; 	
				}																																						

			}

			al_flip_display();

		} else if ((event.type == 10) || (event.type == 12)){		
			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) started = 1;
			else if (event.keyboard.keycode == 82) joystick_left(player->control);																													
			else if (event.keyboard.keycode == 83) joystick_right(player->control);																													
			else if (event.keyboard.keycode == 84) joystick_up(player->control);																														
			else if (event.keyboard.keycode == 85) joystick_down(player->control);		
			else if (event.keyboard.keycode == 3) joystick_fire(player->control);	
			
		} else if (event.type == 42) break;																																								
	}

	clean_board(board);
	destroy_board(board);

	al_destroy_font(font);																																											
	al_destroy_display(disp);																																											
	al_destroy_timer(timer);																																										
	al_destroy_event_queue(queue);		

	return 0;
}