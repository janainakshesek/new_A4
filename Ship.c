#include <stdlib.h>
#include "Ship.h"
#include "Pistol.h"
#include "Joystick.h"
#include "Bullet.h"

ship* ship_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){						

	ship *new_ship = (ship*) malloc(sizeof(ship));																						
	new_ship->side = side;		
	new_ship->face = face;																											
	new_ship->x = x;																															
	new_ship->y = y;			
	new_ship->life = 3;																												
	new_ship->control = joystick_create();	
	new_ship->gun = pistol_create();																								
	return new_ship;																														
}

void ship_move(ship *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){							

	if (!trajectory){ if ((element->x - steps*SHIP_STEP) - element->side/2 >= 0) element->x = element->x - steps*SHIP_STEP;} 				
	else if (trajectory == 1){ if ((element->x + steps*SHIP_STEP) + element->side/2 <= max_x) element->x = element->x + steps*SHIP_STEP;}	
}

void ship_shot(ship *element){																														
	bullet *shot;

	if (!element->face) 
		shot = pistol_shot(element->x, element->y, element->face, element->gun);								
	else if (element->face == 1) 
		shot = pistol_shot(element->x, element->y, 0, element->gun);								
	if (shot)
		 element->gun->shots = shot;
}


void ship_destroy(ship *element){																
	joystick_destroy(element->control);																											
	free(element);																																
}