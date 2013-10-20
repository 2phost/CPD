#include "wolves-squirrels-serial.h"

/* Move */
struct world *move(entity_types e, int x, int y, int size){
	int cell_number = 0;
	int cell_select = 0;
	int p = 0;
	struct world *pos[4];

	/* Search */
	switch(world[x][y].type){
		case wolf:
			/* Search for Squirrels */
			if(x-1>0 && world[x-1][y].type == squirrel)
				pos[p++] = &world[x-1][y];
			if(y+1<size && world[x][y+1].type == squirrel)
				pos[p++] = &world[x][y+1];
			if(x+1<size && world[x+1][y].type == squirrel)
				pos[p++] = &world[x+1][y];
			if(y-1>0 && world[x][y-1].type == squirrel)
				pos[p++] = &world[x][y-1];
				
			if(p==0){
				if(x-1>0 && world[x-1][y].type == empty)
					pos[p++] = &world[x-1][y];
				if(y+1<size world[x][y+1].type == empty)
					pos[p++] = &world[x][y+1];
				if(x+1<size world[x+1][y].type == empty)
					pos[p++] = &world[x+1][y];
				if(y-1>0 world[x][y-1].type == empty)
					pos[p++] = &world[x][y-1];
			}
			break;
		case squirrel:
			/* Search for Trees */
			if(x-1>0 && world[x-1][y].type == tree || world[x-1][y].type == empty)
				pos[p++] = &world[x-1][y];
			if(y+1<size world[x][y+1].type == tree || world[x][y+1].type == empty)
				pos[p++] = &world[x][y+1];
			if(x+1<size world[x+1][y].type == tree || world[x+1][y].type == empty)
				pos[p++] = &world[x+1][y];
			if(y-1>0 world[x][y-1].type == tree || world[x][y-1].type == empty)
				pos[p++] = &world[x][y-1];
			break;
		default:
			return NULL;
	}	
	
	if(p>0){
		cell_number = x*size + y;
		cell_select = cell_number % p;
		
		printf("----->%d\n", cell_select);
		
		return pos[cell_select];		
	}
		
	return NULL;
}


int initWorld(int world_size){
	
	int i, j;

	for(i=0; i < world_size; i++)
		for(j=0; j < world_size; j++)
			world[i][j].type = empty;

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
			printf(" %c |", world[i][j].type);		
		}
		printf("\n");
	}

	return 0;
}

int printWorldFormatted(int world_size){

	int i, j;

	printf("%d\n", world_size);
	
	for(i=0; i < world_size; i++){
		for(j=0; j < world_size; j++){
			if(world[i][j].type != empty)			
				printf("%d %d %c\n", i, j, world[i][j].type);	
		}
	}

	return 0;
}

int correctWorld(entity_types type, int prev_x, int prev_y, int curr_x, int curr_y, int world_size){
	
	switch(type){
		case wolf:
			break;
		case squirrel:
			break;
		default:
			break;
	}

	return 0;
}

int clearWorldCell(int x, int y){

	world[x][y].type = empty;
	world[x][y].breeding_period = 0;
	world[x][y].starvation_period = 0;

	return 1;
}

int makeBabies(entity_types type, struct world* prev_cell, struct world* curr_cell, int breeding_period, int starvation_period){

	/*Create Baby*/
	prev_cell->type = type;
	prev_cell->breeding_period = breeding_period;
	prev_cell->starvation_period = starvation_period;

	/* Restart entity breeding period */
	curr_cell->breeding_period = breeding_period;

	return 0;
}

int computeCell(int x, int y, int s_breeding, int w_breeding, int w_starvation, int world_size){
	
	struct world * move_motion = NULL;
	int ent_breeding, ent_starvation;
		
	switch(world[x][y].type){

		/* At each iteration the wolf trie to move to a cell with a squirrel
		 * in order to eat it, but he can also move to empty cells.
		 * If it's breeding period is 0, and he ate a squirrel, he creates an
		 * heir, at his previous position, otherwise waits until he is able to
		 * eat a squirrel to create the heir.
		 * At each iteration the wolf starvation period decrements, if it reaches
		 * 0 before the wolf is able to eat a squirrel, the wolf dies. But if he
		 * is able to eat a squirrel, his starvation period restarts. */
		case wolf:
			/* Updates breeding and starvation periods and moves the wolf*/
			ent_breeding = --world[x][y].breeding_period;
			ent_starvation = --world[x][y].starvation_period;
			move_motion = move(wolf, x, y, world_size);

			/* if complete breeding : leave a wolf at beginning of stavation and breeding period
			 * otherwise : cannot breed 
			if(!ent_breeding && move_motion != NULL && move_motion->ate)
				makeBabies(wolf, x, y, move_motion->x, move_motion->y, w_breeding, w_starvation);
			*/

			/* if the wolf ate a squirrel : it's starvation period is incremented 
			if(move_motion->ate)
				world[move_motion->x][move_motion->y].starvation_period = w_starvation;
			*/

			/* if stavation: it dies 
			if(ent_starvation == 0){
				if(move_motion == NULL){
					clearWorldCell(x, y);
				}else
					clearWorldCell(move_motion->x, move_motion->y);
			}
			*/

			break;

		/* At each iteration the squirrel tries to move to an empty cell
         * he can also move to a cell with a tree.
         * If it's breeding period is 0 and he moved e creates an heir at
         * is previous position, otherwise waits until he is able to move
         * to create the heir.
         * Squirrels never starve. */
		case squirrel: 

			move_motion = move(squirrel, x, y, world_size);

			if(move_motion != NULL){
				world[x][y].type = empty;
				world[x][y].breeding_period = 0;
				world[x][y].starvation_period = 0;

				move_motion->type = (move_motion->type == tree) ? squirrel_on_tree : squirrel;
				move_motion->breeding_period = (move_motion->breeding_period == 0) ? 0 : move_motion->breeding_period-1;
				move_motion->starvation_period = 0;
			}

			/* if breeding period and moved : he leaves behing a squirrel at the beginning of the breeding period
			 * otherwise: he cannot breed */
			if(move_motion != NULL && move_motion->breeding_period==0)
				makeBabies(squirrel, &world[x][y], move_motion, s_breeding, 0);
			else
				printf("-------------------->bp = %d\n", move_motion->breeding_period);
			
			
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
	entity_types type = empty;
	char type_code;
	FILE * input_file;


	if(argc < 5){
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
		world[x][y].type = type_code;
		if(type == wolf){
			world[x][y].breeding_period = w_breeding;
			world[x][y].starvation_period = w_starvation;
		} else if(type == squirrel){
			world[x][y].breeding_period = s_breeding;
		}
	}
	
	fclose(input_file);
	

	/*DEBUG*/
	printf("INITIAL WORLD\n");	
	printWorld(size);
	printf("\n");
	
	/* Generate */
	while(gen_num != 0){
		
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

		/*DEBUG*/
		printf("Iteration %d:\n", gen_num);		
		printWorld(size);
		printf("\n");

		gen_num--;
	}
	
	/* Output */
	printWorldFormatted(size);
			
	return 0;
}
