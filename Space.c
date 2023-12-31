#include <stdlib.h>

#include "Space.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Ship.h"
#include "Pistol.h"

// Biblioteca que cria e gerencia um tabuleiro dado seu tamanho
// O tabuleiro tem índices [0,X+1] e [0,Y+1] - ou seja, tamanho, em linhas, de X+2; e em colunas, de Y+2,
// onde as bordas/limites do tabuleiro estão definidos.

space* generate_board(int max_y, int max_x){
	space *new_board = (space*) malloc (sizeof(space));
	new_board->map = (sqm**) malloc ((max_y + 1) * sizeof(sqm*));
	for (int i = 0; i <= max_y; i++) 
		new_board->map[i] = (sqm*) calloc (max_x + 1, sizeof(sqm));
	new_board->max_x = max_x;
	new_board->max_y = max_y;
	return new_board;
}

space* create_board(int max_y, int max_x, int enemy_lines){
	if ((max_x <= 0) || (max_y <= 0)) return 0;
	if (enemy_lines > max_y) return 0;
	
	space *board = generate_board(max_y, max_x);
	for (int j = 0; j < max_x; j++) {
		add_enemy(board, 0, j, 0, 0, 2);
	}
	for (int j = 0; j < max_x; j++) {
		add_enemy(board, 1, j, 0, 0, 1);
	}
	for (int j = 0; j < max_x; j++) {
		add_enemy(board, 2, j, 0, 0, 0);
	}
	for (int j = 0; j < max_x; j++) {
		add_enemy(board, 3, j, 0, 0, 0);
	}
    
    addObstacle(1, 9, board);
    addObstacle(4, 9, board);
    addObstacle(7, 9, board);
    addObstacle(10, 9, board);


	return board;
}

void clean_board(space *board){
	for (int i = 0; i <= board->max_y; i++){
		for (int j = 0; j <= board->max_x; j++){
			if (!board->map[i][j].entity) continue;
			if (board->map[i][j].type == ENEMY) {
				pistol_destroy(((enemy*) board->map[i][j].entity)->gun);
				joystick_destroy(((enemy*) board->map[i][j].entity)->control);
			}
			free(board->map[i][j].entity);
			board->map[i][j].entity = NULL;
		}
	}
}

void destroy_board(space* board){
	for (int i = 0; i <= board->max_y; i++)
		free(board->map[i]);
	free(board->map);
	free(board);
}
