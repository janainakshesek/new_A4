#ifndef __SHIP__ 		
#define __SHIP__			

#include "Joystick.h"		
#include "Pistol.h"		

#define SHIP_STEP 10																	

typedef struct {																												
	unsigned char side;	
	unsigned char face;																										
	unsigned short x;																						
	unsigned short y;		
	pistol *gun;																									
	joystick *control;	
	int life;																										

} ship;																														

ship* ship_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		
void ship_move(ship *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);	
void ship_shot(ship *element);			
void ship_destroy(ship *element);		

#endif																															