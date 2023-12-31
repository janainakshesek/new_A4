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
#include "Game.h"

#include <allegro5/allegro5.h>														
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#define X_SCREEN 1500
#define Y_SCREEN 800

int main(int argc, char** argv){
	int x = 11;
	int y = 10;
	int e = 4;
	int started = 0, end = 0, enemies = -1, working = 0, createBoard = 0, createPlayer = 0;

	al_init();																																															
	al_init_image_addon();		
	al_init_primitives_addon();																																									
	
	al_install_keyboard();																																												

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																					
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																				
	ALLEGRO_FONT* font =  al_load_bitmap_font("./imgs/fixed_font.tga");	
	if (!font) {
      perror("Falha ao carregar fonte.\n");
	}																																			
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

	ship *player;	

	ALLEGRO_EVENT event;																																												
	al_start_timer(timer);	

    char lifeText[20];
	space *board;
	bullet *aux;

	while(1){																																															
		al_wait_for_event(queue, &event);

		if (event.type == 30){
			al_clear_to_color(al_map_rgb(0, 0, 0));	
			
			if (!started && !end) {
				al_draw_scaled_bitmap(gameIcon, 0, 0, 600, 600, 600, 200, 300, 300, 0);
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 500, ALLEGRO_ALIGN_CENTER, "SPACE INVADERS");
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 700, ALLEGRO_ALIGN_CENTER, "APERTE ENTER PARA JOGAR");

			} else if (started && (end == 1 || end == -3) )  {
				started = 0;
				working = 0;
			} else if (!started && end == 1 && !working) { 
				sprintf(lifeText, "SCORE: %d", player->score);
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 400, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 200, ALLEGRO_ALIGN_CENTER, lifeText);
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 600, ALLEGRO_ALIGN_CENTER, "APERTE ENTER PARA JOGAR");
				al_draw_text(font, al_map_rgb(154, 217, 65), 730, 700, ALLEGRO_ALIGN_CENTER, "APERTE ESC PARA SAIR");

			} else if (end == -3 && !started && !working) { 
				if (player->life < 5)
					player->life++;	
				aux = player->gun->shots;
				while (aux) {
					if (aux->y > 0) 
						aux->y = 0;
					aux = (bullet*) aux->next;
				}
				player->x = X_SCREEN/2;
				player->y = 650;
				started = 1;
				end = 0;
				createBoard = 1;
				createPlayer = 0;
			
      		} else if(started && !end && !working && createPlayer) {
				player = ship_create(50, 0, X_SCREEN/2, 650, X_SCREEN, Y_SCREEN);
				createBoard = 1;
				createPlayer = 0;
			} else if (started && createBoard && !working && !end) {
				board = create_board(y, x, e);

				working = 1;
				createBoard = 0;
			} 
			else if ( started && working && !end) {
				enemies = count_enemies(board);
				end = check_ship_kill(player, board, shipIcon);
				if (enemies == 0)
					end = -3;
				start_game(board, player, shipIcon, font);
			}

			al_flip_display();

		} else if ((event.type == 10) || (event.type == 12)){		
			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {started = 1; end = 0; createPlayer = 1;}
			else if (event.keyboard.keycode == 82 && started && working) joystick_left(player->control);																													
			else if (event.keyboard.keycode == 83 && started && working) joystick_right(player->control);																													
			else if (event.keyboard.keycode == 84 && started && working) joystick_up(player->control);																														
			else if (event.keyboard.keycode == 85 && started && working) joystick_down(player->control);		
			else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && started && working) joystick_fire(player->control);	

			else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

			clean_board(board);
			destroy_board(board);
			pistol_destroy(player->gun);
			ship_destroy(player);
			
			al_destroy_font(font);																																											
			al_destroy_display(disp);																																											
			al_destroy_timer(timer);																																										
			al_destroy_event_queue(queue);	
			al_destroy_bitmap(gameIcon);
			al_destroy_bitmap(shipIcon);

			break;
			} 
			
		} else if (event.type == 42) break;																																				
	}

	return 0;
}