#include <stdlib.h>
#include <stdio.h>
#include "Enemy.h"

#define ENEMY_STEP 1
#define X_SCREEN 1500
#define Y_SCREEN 800

//Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
int add_enemy(space *board, int position_y, int position_x, int side, int face, int type){
	enemy *new_enemy = malloc(sizeof(enemy));

	if (new_enemy == NULL) 
		return 0;
	else {
		new_enemy->y = position_y*50+50;
		new_enemy->x = position_x*70;
		new_enemy->side = side;
		new_enemy->face = face;
		new_enemy->control = joystick_create();
        new_enemy->type = type;
		new_enemy->trajectory = 1;
		new_enemy->img = 0;
		new_enemy->up = 0;
		new_enemy->gun = pistol_create();																								
		board->map[position_y][position_x].entity = new_enemy;
		board->map[position_y][position_x].type = ENEMY;

		return 1;
	}
}

enemy *extreme_right (space *board) {
	enemy *aux, *extreme = malloc(sizeof(enemy));
	int x = 0;

	extreme->x = 0;
	extreme->y = 0;
	extreme->side = 0;
	extreme->face = 0;
	extreme->control = NULL;
	extreme->type = 0;
	extreme->trajectory = 0;


	if (!extreme)
		return NULL;

	for (int i = 0; i < board->max_y; i++) {
        for (int j = 0; j < board->max_x; j++) {
            if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
                aux = board->map[i][j].entity;
				if (aux->x > x) {
					x = aux->x;
					extreme = board->map[i][j].entity;
				}
            }
        }
    }

	return extreme;
}

enemy *extreme_left (space *board) {
	enemy *aux, *extreme = malloc(sizeof(enemy));
	int x = 1500;

	extreme->x = 1500;
	extreme->y = 0;
	extreme->side = 0;
	extreme->face = 0;
	extreme->control = NULL;
	extreme->type = 0;
	extreme->trajectory = 0;

	if (!extreme)
		return NULL;

	for (int i = 0; i < board->max_y; i++) {
		for (int j = 0; j < board->max_x; j++) {
			if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
				aux = board->map[i][j].entity;
				if (aux->x < x) {
					x = aux->x;
					extreme = board->map[i][j].entity;
				}
			}
		}
	}

	return extreme;
}

void update_enemies_position(space *board) {
    enemy *right = extreme_right(board), *left = extreme_left(board), *aux;

	if (!right || !left) {
		printf("parei aqui");
		return;
	}

	if (right->x >= X_SCREEN-100) {
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
					aux = board->map[i][j].entity;
					aux->trajectory = -1;
					aux->x -= ENEMY_STEP;
					aux->img++;
					if (aux->img % 20 == 0)
						aux->up = 1;
					else if (aux->img % 120 == 9)
						aux->up = 0;
				}
			}
		}
	} else if (left->x <= 10) {
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
					aux = board->map[i][j].entity;
					aux->trajectory = 1;
					aux->x += ENEMY_STEP;
					aux->img++;
						if (aux->img % 20 == 0)
						aux->up = 1;
					else if (aux->img % 20 == 9)
						aux->up = 0;
				}
			}
		}
	} else {
		for (int i = 0; i < board->max_y; i++) {
			for (int j = 0; j < board->max_x; j++) {
				if (board->map[i][j].entity && board->map[i][j].type == ENEMY) {
					aux = board->map[i][j].entity;
					aux->x += aux->trajectory*ENEMY_STEP;
					aux->img++;
						if (aux->img % 20 == 0)
						aux->up = 1;
					else if (aux->img % 20 == 9)
						aux->up = 0;
				}
			}
		}
	}

}

int valid_shot (enemy *element) {
	if (!element) return 0;

	bullet *aux = element->gun->shots;

	while (aux) {
		if (aux->x == element->x) return 0;
		aux = (bullet*) aux->next;
	}
	return 1;
}

void enemy_shot(enemy *element) {
	bullet *shot;

	if (!element) return;

	if (!valid_shot(element)) return;
	else {
    if (element->face == 0) 
		shot = pistol_shot(element->x, element->y, 1, element->gun);							
	if (shot)
		 element->gun->shots = shot;
	}

}


//Remove um inimigo do tabuleiro. Essa tarefa inclui a desalocação do mesmo
int remove_enemy(space *board, int position_y, int position_x){
	if (board->map[position_y][position_x].entity == NULL) 
		return 0;
	else {
		free(board->map[position_y][position_x].entity);
		board->map[position_y][position_x].entity = NULL;
		return 1;
	}
}