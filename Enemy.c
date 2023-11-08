#include <stdlib.h>
#include <stdio.h>
#include "Enemy.h"

shot_sentinel* create_shotlist(void){
	shot_sentinel *list = (shot_sentinel*) malloc (sizeof(shot_sentinel));
	if (list == NULL) return NULL;
	list->first = NULL;
	list->last  = NULL;
	return list;
}

int search_shot (shot_sentinel *list, int position_x){
	shot *s = (shot*) list->first;
	while (s) {
		if (s->position_x >= position_x-10 && s->position_x <= position_x+10)
			return 1;
		s = s->next;
	}
	return 0;
}

//	Remove os tiros da lista
shot* remove_shot(shot* current, shot* previous, shot_sentinel *list){
  if (!current)
    return NULL;
  else if (current == list->first && current == list->last) {
    list->first = NULL;
    list->last = NULL;
    current->next = NULL;
    current->previous = NULL;
    free(current);
    return NULL;
  }
  else if (current == list->first) {
    list->first = current->next;
    list->first->next = current->next->next;
    list->first->previous = NULL;
    current->next = NULL;
    free(current);
  }
  else if (current == list->last) {
    list->last = current->previous;
    list->last->previous = current->previous->previous;
    list->last->next = NULL;
    current->previous = NULL;
    free(current);
  } else {
    current->previous->next = current->next;
    current->next->previous = current->previous;
    current->next = NULL;
    current->previous = NULL;
    free(current);
  }

  return previous;
}

void clean_shots(shot_sentinel *list){
	shot *p = (shot*) list->first;
	shot *q = NULL;
	while (p != NULL) {
    q = p->next;
		p = remove_shot(p, q, list);
		p = q;
	}
}

//Os tiros presentes no tabuleiro devem ser atualizados
//  Se o tiro acertar um alvo, ou sair do tabuleiro, ele deve ser removido da lista
//  Caso contrário, ele deve "andar" uma casa (sqm) à frente
void update_shots(space *board, shot_sentinel *list){
	shot *s = (shot*) list->first, *aux;
	if (!s)
    	return;

	while (s) {
		while (s->position_y < 800) {
			s->position_y+=5;
		}
		if (s->position_y == 800) {
			aux = s->next;
			s = remove_shot(s, s->previous, list);
		} 
		s = aux;
	}
}

//Adiciona um novo tiro à lista. Neste momento, todos os tiros se movem apenas para frente
shot_sentinel* straight_shot(space *board, shot_sentinel *list, enemy *shooter){
	shot *new_shot = (shot*) malloc (sizeof(shot));
	new_shot->position_x = shooter->x;
	new_shot->position_y = shooter->y + 1;
	
	if (list->first == NULL) {
		list->first = new_shot;
		list->last = new_shot;
		new_shot->next = NULL;
    	new_shot->previous = NULL;
		new_shot->shocked = 0;
	}  else {
    	new_shot->previous = list->last;
		new_shot->next = NULL;
		list->last->next = new_shot;
		list->last = new_shot;
		new_shot->shocked = 0;
	}

	return list;
}

//Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
int add_enemy(space *board, int position_y, int position_x, int side, int face, int type){
	enemy *new_enemy = malloc(sizeof(enemy));

	if (new_enemy == NULL) 
		return 0;
	else {
		new_enemy->y = position_y*50;
		new_enemy->x = position_x*70+300;
		new_enemy->side = side;
		new_enemy->face = face;
		new_enemy->control = joystick_create();
        new_enemy->type = type;
		board->map[position_y][position_x].entity = new_enemy;
		board->map[position_y][position_x].type = ENEMY;

		return 1;
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