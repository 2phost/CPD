#define MAX 50

enum entity {
	WOLVE,
	SQUIRREL,
	ICE,
	TREE
};

typedef enum entity entity_types;

struct world {
	int type; /* Wolf, Squirrel, etc. */
	int breeding_period;
	int starvation_period;
} world[MAX][MAX];
	
