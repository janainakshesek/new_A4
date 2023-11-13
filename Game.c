#include "Game.h"

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

void update_enemy_bullets(enemy *enemy){									
	bullet *previous = NULL;	

  if (!enemy)
    return;

	for (bullet *index = enemy->gun->shots; index != NULL;){		
		if (index->shoked) {
			if (previous){																																													
				previous->next = index->next;																																								
				bullet_destroy(index);																																										
				index = (bullet*) previous->next;																																							
			} else {
				enemy->gun->shots = (bullet*) index->next;											
				bullet_destroy(index);
				index =enemy->gun->shots;																																									
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
					enemy->gun->shots = (bullet*) index->next;																																					
					bullet_destroy(index);																																										
					index = enemy->gun->shots;																																									
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
						if (!aux->up)
							al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 0, 15, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 4.5, 4.5, 0, 0);	
						else 
							al_draw_tinted_scaled_rotated_bitmap_region(image, 15, 0, 15, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 4.5, 4.5, 0, 0);
					} else if (aux->type == INTERMEDIARY) { 
						if (!aux->up)
							al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 18, 15, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 4.5, 4.5, 0, 0);	
						else
							al_draw_tinted_scaled_rotated_bitmap_region(image, 15, 18, 15, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 4.5, 4.5, 0, 0);	
					} else if (aux->type == STRONG) {
						if (!aux->up)
							al_draw_tinted_scaled_rotated_bitmap_region(image, 0, 35, 15, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 4.5, 4.5, 0, 0);	
						else
							al_draw_tinted_scaled_rotated_bitmap_region(image, 15, 35, 15, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 4.5, 4.5, 0, 0);	
					}
				}
			}
		}
	}	
}

void draw_obstacles (space *board, ALLEGRO_BITMAP *image, ALLEGRO_FONT* font) {
	obstacle *aux;
	char lifeText[20];

	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity) {
				if (board->map[i][j].type == OBSTACLE) { 
					aux = board->map[i][j].entity;
					sprintf(lifeText, "%d", aux->life);
					al_draw_text(font, al_map_rgb(154, 217, 65), aux->x, aux->y, ALLEGRO_ALIGN_CENTER, lifeText);
					if (aux->life > 7){
						al_draw_tinted_scaled_rotated_bitmap_region(image, 50, 20, 30, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 5, 5, 0, 0);
					}
					else if (aux->life <= 7 && aux->life > 5)
						al_draw_tinted_scaled_rotated_bitmap_region(image, 50, 35, 30, 10, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 5, 5, 0, 0);
					else if (aux->life <= 5)
						al_draw_tinted_scaled_rotated_bitmap_region(image, 50, 55, 30, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, aux->x, aux->y, 5, 5, 0, 0);
				}
			}
		}
	}	
}

void draw_explosion(int x, int y, ALLEGRO_BITMAP *image) {
    al_draw_tinted_scaled_rotated_bitmap_region(image, 35, 45, 10, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, x + 27, y, 3, 3, 0, 0);
    al_draw_tinted_scaled_rotated_bitmap_region(image, 35, 55, 10, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, x + 27, y, 3, 3, 0, 0);
    al_draw_tinted_scaled_rotated_bitmap_region(image, 35, 45, 10, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, x + 27, y, 3, 3, 0, 0);
}


void check_kill(ship *killer, space *board, ALLEGRO_BITMAP *shipIcon){
	enemy *aux;					
	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity) {
					if (board->map[i][j].type == ENEMY) {
						aux = board->map[i][j].entity;
						if ((index->x >= (aux->x-20)) && (index->x <= (aux->x+20)) && ((index->y >= aux->y) && (index->y <= aux->y+20))) {
							index->shoked = 1;
              				draw_explosion(aux->x, aux->y, shipIcon);
              				if (aux->type == WEAK)
								killer->score += 10;
              				else if (aux->type == INTERMEDIARY)
                				killer->score += 20;
              				else if (aux->type == STRONG)
                				killer->score += 40;
							remove_enemy(board, i, j);
						}
					}
				}
			}
		}
	}
}

void check_obstacle_collision(ship *player, space *board) {
	obstacle *o;
	for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next){
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity) {
					if (board->map[i][j].type == OBSTACLE) {
						o = board->map[i][j].entity;	
						if ((index->x >= (o->x-100)) && (index->x <= (o->x+100) && (index->y >= (o->y-50) && (index->y <= o->y+50)))) {
							index->shoked = 1;
							if (o->life > 1) {
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

void check_obstacle_enemy (space *board) {
	enemy *aux; obstacle *o;
	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity) {
				if (board->map[i][j].type == ENEMY) {
					aux = board->map[i][j].entity;	
					for (bullet *index = aux->gun->shots; index != NULL; index = (bullet*) index->next){
						for (int k = 0; k < board->max_y; k++) {
							for (int l = 0; l < board->max_x; l++) {
								if (board->map[k][l].entity && board->map[k][l].type == OBSTACLE) {
									o = board->map[k][l].entity;
								  if ((index->x >= (o->x-100)) && (index->x <= (o->x+100) && (index->y >= (o->y-50) && (index->y <= o->y+50)))) {
									  index->shoked = 1;
									  if (aux->type == WEAK) {
										  if (o->life > 1) {
											  o->life--;
										  } else 
											  removeObstacle(j, i, board);
									  } else if (aux->type == INTERMEDIARY || aux->type == STRONG) {
										    if (o->life > 1) {
											  o->life -= 2;
										  } else 
											  removeObstacle(j, i, board);
									  }
								  }
								}
							}
						}
					}
				}
			}
		}
	}
}

enemy *closer(space *board, ship *player) {
    enemy *aux, *closer = NULL;
    int minDistance = 100000;

    for (int i = 0; i < board->max_y; i++) {
        for (int j = 0; j < board->max_x; j++) {
            if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
                aux = board->map[i][j].entity;

                int distanceX = abs(player->x - aux->x);
                int distanceY = abs(player->y - aux->y);

                if (distanceX + distanceY < minDistance) {
                    minDistance = distanceX + distanceY;
                    closer = aux;
                }

            }
        }
    }		
    return closer;
}

int shot_screen (space *board) {
	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity) {
				if (board->map[i][j].type == ENEMY) {
					enemy *aux = board->map[i][j].entity;
					for (bullet *index = aux->gun->shots; index != NULL; index = (bullet*) index->next) {
						if ((index->x >= 0) && (index->x <= X_SCREEN) && ((index->y >= 0) && (index->y <= Y_SCREEN))) {
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

void draw_ship(ship *player, ALLEGRO_BITMAP *image) {
    if (!player->blinking) {
        al_draw_tinted_scaled_rotated_bitmap_region(image, 67, 0, 10, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, player->x, player->y, 5, 5, 0, 0);
    } else {
        if (player->blinkCounter % 10 < 5) {
            al_draw_tinted_scaled_rotated_bitmap_region(image, 67, 0, 10, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, player->x, player->y, 5, 5, 0, 0);
        } else {
            al_draw_tinted_scaled_rotated_bitmap_region(image, 67, 0, 10, 20, al_map_rgba_f(0, 0, 0, 0), 0, 0, player->x, player->y, 5, 5, 0, 0);
        }

        if (player->blinkCounter > 0) {
            player->blinkCounter--;
        } else {
            player->blinking = 0;
        }
    }
}

int check_ship_kill(ship *player, space *board, ALLEGRO_BITMAP *shipIcon) {
    enemy *aux;
    for (int i = 0; i < board->max_y; i++) {
        for (int j = 0; j < board->max_x; j++) {
            if (board->map[i][j].entity) {
                if (board->map[i][j].type == ENEMY) {
                    aux = board->map[i][j].entity;
                    for (bullet *index = aux->gun->shots; index != NULL; index = (bullet *)index->next) {
                        if ((index->x >= (player->x - 20)) && (index->x <= (player->x + 20)) && ((index->y >= player->y) && (index->y <= player->y + 20))) {
                            index->shoked = 1;
                            if ((player->life > 1) && (!player->blinking)) {
                                player->life--;

                                player->blinking = 1;
                                player->blinkCounter = 50;  

                                draw_ship(player, shipIcon);
                            } else if (player->life == 1) {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
  return 0;
}

void verify_shots (space *board, ship *player) {
  enemy *aux;
  for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next){
    for (int i = 0; i < board->max_y; i++) {
      for (int j = 0; j < board->max_x; j++) {
        if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
          aux = board->map[i][j].entity;
          for (bullet *aux_index = aux->gun->shots; aux_index != NULL; aux_index = (bullet*) aux_index->next) {
            if ((index->x >= (aux_index->x-10)) && (index->x <= (aux_index->x+10)) && ((index->y >= aux_index->y) && (index->y <= aux_index->y+20))) {
              index->shoked = 1;
              aux_index->shoked = 1;
            }
          }
        }
      }
    }
  }
}

void draw_enemies_shots (space *board, ALLEGRO_BITMAP *image) {
  enemy *aux;

	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity) {
				if (board->map[i][j].type == ENEMY) {
					aux = board->map[i][j].entity;
					for (bullet *index = aux->gun->shots; index != NULL; index = (bullet*) index->next) {
            if (!index->shoked) {
            if (aux->type == WEAK)
				      al_draw_tinted_scaled_rotated_bitmap_region(image, 30, 0, 10, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, index->x, index->y+30, 3, 3, 0, 0);	
            else if (aux->type == INTERMEDIARY) 
				      al_draw_tinted_scaled_rotated_bitmap_region(image, 30, 20, 10, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, index->x, index->y+30, 3, 3, 0, 0);	
            else if (aux->type == STRONG)
				      al_draw_tinted_scaled_rotated_bitmap_region(image, 30, 30, 20, 15, al_map_rgba_f(1, 1, 1, 1), 0, 0, index->x, index->y+30, 3, 3, 0, 0);	
            } else {
              draw_explosion(index->x, index->y+30, image);
            }
        	}
					update_enemy_bullets(aux);
				}
			}
	  }
	}		
}

int count_enemies (space *board) {
  int count = 0;

  for (int i = 0; i < board->max_y; i++) {
    for (int j = 0; j < board->max_x; j++) {
      if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
        count++;
      }
    }
  }

  return count;
}

int create_cenary (ship *player,  ALLEGRO_BITMAP *shipIcon, ALLEGRO_FONT *font) {
	int x = 11;
	int y = 10;
	int e = 4;
	int aux = 1;
	space *board = create_board(y, x, e);

	while (aux) aux = start_game(board, player, shipIcon, font); 

	return aux;
}

int start_game (space *board, ship *player, ALLEGRO_BITMAP *shipIcon, ALLEGRO_FONT *font) {
    char lifeText[20];
	enemy *aux;
    int end = 0, enemies = -1;

    sprintf(lifeText, "SCORE: %d", player->score);
	al_draw_text(font, al_map_rgb(154, 217, 65), X_SCREEN/2, 20, ALLEGRO_ALIGN_CENTER, lifeText);

	al_draw_text(font, al_map_rgb(154, 217, 65), 40, 20, ALLEGRO_ALIGN_CENTER, "LIFE: ");
	for (int i = 0; i < player->life; i++) {
		al_draw_tinted_scaled_rotated_bitmap_region(shipIcon, 67, 0, 10, 20, al_map_rgba_f(1, 1, 1, 1), 0, 0, i*40+70, 5, 2, 2, 0, 0);	
	}

	update_position(player);	
	check_kill(player, board, shipIcon);
	check_obstacle_collision(player, board);
    check_obstacle_enemy(board);
	end = check_ship_kill(player, board, shipIcon);
    if (end == 1) {
        return 0;
    }

	draw_enemies(board, shipIcon);
	update_enemies_position(board);	
    verify_shots(board, player);
				
    draw_ship(player, shipIcon);
	draw_obstacles(board, shipIcon, font);
    draw_enemies_shots(board, shipIcon);

	for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next) {
	al_draw_filled_circle(index->x+27, index->y+30, 5, al_map_rgb(255, 0, 0));            
	if (player->gun->timer) 
		player->gun->timer--; 	
	}	

	aux = closer(board, player);

	enemies = count_enemies(board);
	if (enemies == 0) return -3;

    if (aux) {
        if (!shot_screen(board))
		    enemy_shot(aux);
    }
    return 1;
}
