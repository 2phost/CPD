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

/* Prints the content of the world, which corresponds to a square matriz of size world_size */
int printWorld(int world_size);

/* Fill all the cells, of the square matrix of size world_size, with the empty entity_types*/
int initWorld(int world_size);
