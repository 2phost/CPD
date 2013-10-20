#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50

enum entity {
	w,
	s,
	i,
	t
};

typedef enum entity entity_types;

struct world {
	entity_types type; /* Wolf, Squirrel, etc. */
	int breeding_period;
	int starvation_period;
} world[MAX][MAX];
	
