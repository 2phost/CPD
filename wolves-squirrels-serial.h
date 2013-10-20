#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50

enum entity {
	wolf='w',
	squirrel='s',
	ice='i',
	tree='t',
	empty=' '
};

typedef enum entity entity_types;

struct world {
	entity_types type; /* Wolf, Squirrel, etc. */
	int breeding_period;
	int starvation_period;
} world[MAX][MAX];

typedef struct point * coord; 
struct point{
	int x;
	int y;
};

/* Prints the content of the world, which corresponds to a square matriz of size world_size */
int printWorld(int world_size);

/* Fill all the cells, of the square matrix of size world_size, with the empty entity_types*/
int initWorld(int world_size);

/* Clears a specified cell in the world */
int clearWorldCell(int x, int y);

/* Given the entity previous position and it's current position, creates an heir at the previous position
   with the specified breeding and starvation periods, and restarts the entity's breeding period */
int makeBabies(entity_types type, int prev_x, int prev_y, int curr_x, int curr_y, int breeding_period, int starvation_period);

/**/
coord move(entity_types e, int x, int y, int size);
