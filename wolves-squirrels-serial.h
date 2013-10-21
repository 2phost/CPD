#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50

enum entity {
	wolf='w',
	squirrel='s',
	squirrel_on_tree = '$',
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
	int ate;
};

/* Fill all the cells, of the square matrix of size world_size, with the empty entity_types*/
int initWorld(int world_size);

/* Prints the content of the world, which corresponds to a square matriz of size world_size */
int printWorld(int world_size);

/* Copies the contents of the real world to the fake world */
int copyWorldTo(struct world real_world[MAX][MAX], struct world fake_world[MAX][MAX]);

/* Prints the contents of the world, according to the expected output format, which must correspond
 * to the input file format. */
int printWorldFormatted(int world_size);

/* Corrects the world, according to the performed move */
int correctWorld(entity_types type, int prev_x, int prev_y, int curr_x, int curr_y, int world_size);

/* Clears a specified cell in the world */
int clearWorldCell(struct world* cell);

int killWolf(struct world* wolf);

/* Processes the cell's iteration, depending of what kind of entity is at the current cell
 * specified by the position (x, y) */
int computeCell(int x, int y, int s_breeding, int w_breeding, int w_starvation, int world_size);

/* Given the entity previous position and it's current position, creates an heir at the previous position
   with the specified breeding and starvation periods, and restarts the entity's breeding period */
int makeBabies(entity_types type, struct world* prev_cell, struct world* curr_cell, int breeding_period, int starvation_period);

/**/
struct world *move(entity_types e, int x, int y, int size);
