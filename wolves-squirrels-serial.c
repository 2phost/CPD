#include <stdlib.h>
#include "wolves-squirrels-serial.h"

/* Move */
struct world *move(entity_types e, int x, int y, int size){

	int cell_number = 0;
	int cell_select = 0;
	int p = 0;
	struct world *pos[4];
	int d_world = (w_number+1) % 2;

	/* Search */
	switch(world[w_number][x][y].type){

		case wolf:
			/* Search for Squirrels */
			if(x-1 >= 0 && world[w_number][x-1][y].type == squirrel)
				pos[p++] = &world[d_world][x-1][y];
			if(y+1 < size && world[w_number][x][y+1].type == squirrel)
				pos[p++] = &world[d_world][x][y+1];
			if(x+1 < size && world[w_number][x+1][y].type == squirrel)
				pos[p++] = &world[d_world][x+1][y];
			if(y-1 >= 0 && world[w_number][x][y-1].type == squirrel)
				pos[p++] = &world[d_world][x][y-1];
				
			if(p==0){
				if(x-1 >= 0 && world[w_number][x-1][y].type == empty)
					pos[p++] = &world[d_world][x-1][y];
				if(y+1 < size && world[w_number][x][y+1].type == empty)
					pos[p++] = &world[d_world][x][y+1];
				if(x+1 < size && world[w_number][x+1][y].type == empty)
					pos[p++] = &world[d_world][x+1][y];
				if(y-1 >= 0 && world[w_number][x][y-1].type == empty)
					pos[p++] = &world[d_world][x][y-1];
			}
			break;

		case squirrel:
		case squirrel_on_tree:
			/* Search for Trees */
			if(x-1 >= 0 && (world[w_number][x-1][y].type == tree || world[w_number][x-1][y].type == empty))
				pos[p++] = &world[d_world][x-1][y];
			if(y+1 < size && (world[w_number][x][y+1].type == tree || world[w_number][x][y+1].type == empty))
				pos[p++] = &world[d_world][x][y+1];
			if(x+1 < size && (world[w_number][x+1][y].type == tree || world[w_number][x+1][y].type == empty))
				pos[p++] = &world[d_world][x+1][y];
			if(y-1 >= 0 && (world[w_number][x][y-1].type == tree || world[w_number][x][y-1].type == empty))
				pos[p++] = &world[d_world][x][y-1];
			break;
		default:
			return NULL;
	}	
	
	if(p>0){
		cell_number = x*size + y;
		cell_select = cell_number % p;
		
		return pos[cell_select];		
	}
		
	return NULL;
}


int initWorld(int world_size){
	
	int i, j;

	for(i=0; i < world_size; i++)
		for(j=0; j < world_size; j++){
			world[i][j].coord.x=i;
			world[i][j].coord.y=j;
			world[i][j].coord.x=i;
			world[i][j].coord.y=j;
			world[i][j].type = empty;
			world[i][j].type = empty;
		}

	return 0;
}

int clearWorldCell(struct world* cell){
	cell->type = empty;
	cell->breeding_period = 0;
	cell->starvation_period = 0;

	return 0;
}

int cleanWorld(int world_size){
	
	int i, j;
	int d_world = (w_number+1)%2;

	for(i=0; i < world_size; i++)
		for(j=0; j < world_size; j++){
			if(world[d_world][i][j].type == squirrel_on_tree)
				world[d_world][i][j].type = tree;
			if(world[d_world][i][j].type != tree && world[d_world][i][j].type != ice)
				clearWorldCell(&world[d_world][i][j]);
		}

	return 0;
}

int printWorld(int world_size){

	int i, j;

    printf(" |");
	for(j=0; j<world_size; j++)
		printf(" %d |", j);
	printf("\n");

	for(i=0; i < world_size; i++){
		printf("%d|", i);
		for(j=0; j< world_size; j++){
			printf(" %c |", world[w_number][i][j].type);		
		}
		printf("\n");
	}

	return 0;
}

int printWorldDetailed(int world_size){
	int i, j;
	int d_world = (w_number+1) % 2;

	printf("=== printWorldDetailed ===\n");
	printf("reading from world[%d] ...\n", w_number);
	printf(" |");
	for(j=0; j<world_size; j++)
		printf(" %d     |", j);
	printf("\n");

	for(i=0; i < world_size; i++){
		printf("%d|", i);
		for(j=0; j< world_size; j++){
			printf(" %c %d %d |", world[w_number][i][j].type, world[w_number][i][j].breeding_period, world[w_number][i][j].starvation_period);		
		}
		printf("\n");
	}

	printf("\n\nwriting to world[%d] ...\n", d_world);
	printf(" |");
	for(j=0; j<world_size; j++)
		printf(" %d     |", j);
	printf("\n");

	for(i=0; i < world_size; i++){
		printf("%d|", i);
		for(j=0; j< world_size; j++){
			printf(" %c %d %d |", world[d_world][i][j].type, world[d_world][i][j].breeding_period, world[d_world][i][j].starvation_period);		
		}
		printf("\n");
	}
	printf("==========================\n");

	return 0;
}

int printWorldFormatted(int world_size){

	int i, j;

	printf("%d\n", world_size);
	
	for(i=0; i < world_size; i++){
		for(j=0; j < world_size; j++){
			if(world[w_number][i][j].type != empty)			
				printf("%d %d %c %d %d\n", i, j, world[w_number][i][j].type, world[w_number][i][j].breeding_period, world[w_number][i][j].starvation_period);	
		}
	}

	return 0;
}


int makeBabies(entity_types type, struct world* prev_cell, struct world* curr_cell, int breeding_period, int starvation_period){
	
	switch(type){
		case squirrel:
			if(prev_cell->type != wolf){
				/*Create Baby*/
				prev_cell->type = squirrel;
				prev_cell->breeding_period = breeding_period;
				prev_cell->starvation_period = 0;
				/* Restart entity breeding period */
				curr_cell->breeding_period = breeding_period;
			}else{
				prev_cell->starvation_period = starvation_period;
				prev_cell->breed = prev_cell->breeding_period <= 0 ? 1 : 0; 
			}
			break;

		case wolf:
			/*Create Baby*/
			prev_cell->type = wolf;
			prev_cell->breeding_period = breeding_period;
			prev_cell->starvation_period = starvation_period;
			prev_cell->breed = 0;
			/* Restart entity breeding period */
			curr_cell->breeding_period = breeding_period;
			curr_cell->breed = 0;
			break;

		case squirrel_on_tree:
			/*Create Baby*/
			prev_cell->type = squirrel_on_tree;
			prev_cell->breeding_period = breeding_period;
			prev_cell->starvation_period = 0;
			/* Restart entity breeding period */
			curr_cell->breeding_period = breeding_period;
			break;

		default:
			break;
	}

	return 0;
}

int computeCell(int x, int y, int s_breeding, int w_breeding, int w_starvation, int world_size){
	
	struct world * move_motion = NULL;
	int d_world = (w_number+1) % 2;
	int sot = 0, starv=0;

	switch(world[w_number][x][y].type){

		/* At each iteration the wolf trie to move to a cell with a squirrel
		 * in order to eat it, but he can also move to empty cells.
		 * If it's breeding period is 0, and he ate a squirrel, he creates an
		 * heir, at his previous position, otherwise waits until he is able to
		 * eat a squirrel to create the heir.
		 
		 * UPDATES (21-10-13):
		 *	wolves only die after making number of moves equivalent to the starvation
		 *	period (if they dont' eat a squirrel in the mean time). For example, if the
		 *	starvation period is 1 and the number of generations is 1, the output should
		 *	contain one wolf at the new position. This wolf would die here if there were
		 *	more generations.
		
		 * For the wolves starvation instead of adding one starvation period per squirrel
		 * eaten, it's starvation period should restart */
		case wolf:

			/* if starvation : it dies */
			if(world[w_number][x][y].starvation_period == 0)
				break;
			
			move_motion = move(wolf, x, y, world_size);

			if(move_motion != NULL){
				
				/* If pregnant?*/
				if(world[w_number][x][y].breed)
					makeBabies(wolf, &world[d_world][x][y], move_motion, w_breeding, w_starvation);

				/* Conflict Resolution */
				switch(move_motion->type){

					/* case a wolf ends up in a cell with another wolf, the wolf farthest from startvation wins.
					 * In case the starvation periods are equal, the wolf with the lowest breeding period wins. */
					case wolf:

						/* starv is the difference between the starvation levels of the moving wolf, and the
						 * wolf already on the cell */
						starv = (world[w_number][x][y].starvation_period-1) - move_motion->starvation_period;
						
						if(starv == 0){ /* Their starvation levels is tied */
							move_motion->breeding_period = 
								world[w_number][x][y].breeding_period <= move_motion->breeding_period ?
									world[w_number][x][y].breeding_period-1 : move_motion->breeding_period;						
	
						}else if(starv > 0){ /* The moving wolf has a bigger starvation level and wins */
							move_motion->starvation_period = world[w_number][x][y].starvation_period-1;
							move_motion->breeding_period = world[w_number][x][y].breeding_period-1;
						} /* The original wolf has a bigger starvation level, and nothing needs to change */
							
						break;

					/* case a wolf ends up in a cell with a squirrel, he eats the squirrel and it's
					 * starvation period restarts. */
					case squirrel:
						move_motion->type = wolf;
						move_motion->starvation_period = w_starvation;
						move_motion->breed = move_motion->breeding_period <= 0 ? 1 : 0;	
						move_motion->breeding_period = world[w_number][x][y].breeding_period-1;
						break;

					default:
						move_motion->type = wolf;
						move_motion->breeding_period = world[w_number][x][y].breeding_period-1;
						move_motion->starvation_period = world[w_number][x][y].starvation_period-1;
				}

			}else{
				world[d_world][x][y].type = wolf;
				world[d_world][x][y].breeding_period = world[w_number][x][y].breeding_period-1;
				world[d_world][x][y].starvation_period = world[w_number][x][y].starvation_period-1;
			}

			break;

		/* At each iteration the squirrel tries to move to an empty cell
         * he can also move to a cell with a tree.
         * If it's breeding period is 0 and he moved e creates an heir at
         * is previous position, otherwise waits until he is able to move
         * to create the heir.
         * Squirrels never starve. */
		case squirrel_on_tree:
			sot = 1;
		case squirrel:

			move_motion = move(squirrel, x, y, world_size);

			if(move_motion != NULL){

				/* Conflict Resolution */
				switch(move_motion->type){
					/* case a squirrels ends up in a tree, he becomes a squirrel_on_tree. */
					case tree:
						move_motion->type = squirrel_on_tree;
						move_motion->breeding_period = world[w_number][x][y].breeding_period-1;
						move_motion->starvation_period = 0;
						break;
					/* case two squirrels end up in the same cell, the squirrel with the lowest
					 * breeding period wins. */
					case squirrel: 
						move_motion->breeding_period = 
							world[w_number][x][y].breeding_period <= move_motion->breeding_period ? 
								world[w_number][x][y].breeding_period-1 : move_motion->breeding_period-1;
						break;
					/* case a squirrel ends up in a cell with a wolf, he dies and the wolf's
					 * starvation period restarts. */
					case wolf:
						if(move_motion->starvation_period > 0){
							move_motion->starvation_period = w_starvation;
							if(move_motion->breeding_period < 0)
								move_motion->breed = 1;
					
						}
						
						break;

					default:
						move_motion->type = squirrel;
						move_motion->breeding_period = world[w_number][x][y].breeding_period-1;
						move_motion->starvation_period = 0;
				}

				/* if breeding period is 0 or lower : he leaves behing a squirrel at the beginning of the breeding period
				 * otherwise: he cannot breed */
				if(move_motion->type != wolf && move_motion->breeding_period <= 0)
					makeBabies(sot ? squirrel_on_tree: squirrel, &world[d_world][x][y], move_motion, s_breeding, w_starvation);

			}else{
				if(world[d_world][x][y].type != wolf){				
					world[d_world][x][y].type = sot ? squirrel_on_tree : squirrel;
					world[d_world][x][y].breeding_period = world[w_number][x][y].breeding_period-1;;
					world[d_world][x][y].starvation_period = 0;
				}else{
					world[d_world][x][y].starvation_period = w_starvation;
				}
			}
			
			/*Squirrels never starve*/
			break;

		default:
			break;
	}

	return 0;
}


int main(int argc, char **argv){

	int i, j;	
	int x, y, size;
	int w_breeding, s_breeding, w_starvation, gen_num;
	char type_code;
	FILE * input_file;
	
	w_number = 0;
	
	if(argc <= 5){
		printf("ERROR: Expected 5 arguments provided %d.\n", argc);
		printf("Expected:\n./wolves-squirrels-serial <InputFile> <WolfBreedingPeriod> <SquirrelBreedingPeriod> <WolfStarvationPerior> <Generations>\n");	
		return -1;
    }

	w_breeding = atoi(argv[2]);
	s_breeding = atoi(argv[3]);
	w_starvation = atoi(argv[4]);
	gen_num = atoi(argv[5]);
	
	input_file = fopen(argv[1], "r");
	if(input_file == NULL){
		printf("ERROR: A valid input file is expected. %s is not a valid file.\n", argv[1]);	
		return -1;
	}
		
	fscanf(input_file, "%d", &size);

	initWorld(size);

	while(fscanf(input_file, "%d %d %c", &x, &y, &type_code) != EOF){
		world[0][x][y].type = type_code;
		world[1][x][y].type = type_code;
		if(world[0][x][y].type == wolf){
			world[0][x][y].breeding_period = w_breeding;
			world[0][x][y].starvation_period = w_starvation;
		} else if(world[0][x][y].type == squirrel || world[0][x][y].type == squirrel_on_tree){
			world[0][x][y].breeding_period = s_breeding;
		}
	}
	
	fclose(input_file);

#ifdef VERBOSE
	printf("INITIAL WORLD - w_number = %d\n", w_number);	
	printWorldDetailed(size);
	printf("\n");
#endif
	
	/* Generate */
	while(gen_num != 0){
		cleanWorld(size);

		/* 1st sub-generation - RED */
		for(i=0; i<size; i++){
			for(j = i%2 == 0 ? 0 : 1 ; j<size; j+=2){
				computeCell(i, j, s_breeding, w_breeding, w_starvation, size);
			}		
		}

		/* 2nd sub-generation */
		for(i=0; i<size; i++){
			for(j = i%2 == 0 ? 1 : 0 ; j<size; j+=2){
				computeCell(i, j, s_breeding, w_breeding, w_starvation, size);
			}
		}


#ifdef VERBOSE
		printf("\n\nIteration %d:\n", gen_num);		
		printWorld(size);
		printf("\n");
#endif

		gen_num--;
		w_number = (w_number+1) % 2;
	}
	
	/* Output */
	printWorldFormatted(size);
			
	return 0;
}
