CFLAGS = -Wall -g
LIBS = -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives
CC = gcc

	objects =  Ship.o Joystick.o Pistol.o Bullet.o Enemy.o Space.o A4.o 

A4: $(objects)
	$(CC) $(CFLAGS) -o A4 $(objects) $(LIBS)

A4.o: A4.c
	$(CC) $(CFLAGS) -c A4.c

Ship.o: Ship.c
	$(CC) $(CFLAGS) -c Ship.c

Joystick.o: Joystick.c
	$(CC) $(CFLAGS) -c Joystick.c

Pistol.o: Pistol.c
	$(CC) $(CFLAGS) -c Pistol.c

Bullet.o: Bullet.c
	$(CC) $(CFLAGS) -c Bullet.c

Enemy.o: Enemy.c
	$(CC) $(CFLAGS) -c Enemy.c

Space.o: Space.c
	$(CC) $(CFLAGS) -c Space.c

clean:
	rm -rf $(objects)

purge: clean
	rm -rf A4