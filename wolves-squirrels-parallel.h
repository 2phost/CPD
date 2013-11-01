#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX 1000

double start=0.0; 
double end=0.0;

enum entity {
	wolf='w',
	squirrel='s',
	squirrel_on_tree = '$',
	ice='i',
	tree='t',
	empty=' '
};
typedef enum entity entity_types;

struct conflicts {
	entity_types type;
	int breeding_period;
	int starvation_period;
};
typedef struct conflicts conflict; 

struct world {
	conflict *conflicts[5];
	omp_lock_t lock_count;
	int count;
	entity_types type; /* Wolf, Squirrel, etc. */
	int breeding_period;
	int starvation_period;
	int breed;  /*indicates if the entity can breed */
} world[MAX][MAX];

/* Fill all the cells, of the square matrix of size world_size, with the empty entity_types*/
int initWorld(int world_size);

/* Prints the contents of the world, according to the expected output format, which must correspond
 * to the input file format. */
int printWorldFormatted(int world_size);

/* Processes the cell's iteration, depending of what kind of entity is at the current cell
 * specified by the position (x, y) */
int computeCell(int x, int y, int s_breeding, int w_breeding, int w_starvation, int world_size);

/**/
struct world *move(entity_types e, int x, int y, int size);
