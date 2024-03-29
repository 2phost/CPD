#include <stdlib.h>
#include "wolves-squirrels-parallel.h"

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
			if(x-1 >= 0 && world[x-1][y].type == squirrel)
				pos[p++] = &world[x-1][y];
			if(y+1 < size && world[x][y+1].type == squirrel)
				pos[p++] = &world[x][y+1];
			if(x+1 < size && world[x+1][y].type == squirrel)
				pos[p++] = &world[x+1][y];
			if(y-1 >= 0 && world[x][y-1].type == squirrel)
				pos[p++] = &world[x][y-1];
				
			if(p==0){
				if(x-1 >= 0 && world[x-1][y].type == empty)
					pos[p++] = &world[x-1][y];
				if(y+1 < size && world[x][y+1].type == empty)
					pos[p++] = &world[x][y+1];
				if(x+1 < size && world[x+1][y].type == empty)
					pos[p++] = &world[x+1][y];
				if(y-1 >= 0 && world[x][y-1].type == empty)
					pos[p++] = &world[x][y-1];
			}
			break;

		case squirrel:
		case squirrel_on_tree:
			/* Search for Trees */
			if(x-1 >= 0 && (world[x-1][y].type == tree || world[x-1][y].type == empty))
				pos[p++] = &world[x-1][y];
			if(y+1 < size && (world[x][y+1].type == tree || world[x][y+1].type == empty))
				pos[p++] = &world[x][y+1];
			if(x+1 < size && (world[x+1][y].type == tree || world[x+1][y].type == empty))
				pos[p++] = &world[x+1][y];
			if(y-1 >= 0 && (world[x][y-1].type == tree || world[x][y-1].type == empty))
				pos[p++] = &world[x][y-1];
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
	
	int i, j, z;
	
	#pragma omp parallel for private(i, j, z) schedule(dynamic, world_size/(NUM_THREADS*2))
	for(i=0; i < world_size; i++)
		for(j=0; j < world_size; j++){
			for(z=0; z < 5; z++)
				world[i][j].conflicts[z]=NULL;
			world[i][j].type = empty;
			world[i][j].breed = 0;
			world[i][j].count=0;
			omp_init_lock(&(world[i][j].lock_count));
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

int computeCell(int x, int y, int s_breeding, int w_breeding, int w_starvation, int world_size){
	
	struct world * move_motion = NULL;
	int sot = 0, starv=0;

	switch(world[x][y].type){

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
			if(world[x][y].starvation_period == 0){
				break;
			}
			
			move_motion = move(wolf, x, y, world_size);

			if(move_motion != NULL){
				
				/* If pregnant? */
				if(world[x][y].breed){
					omp_set_lock(&(world[x][y].lock_count));
					world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
					world[x][y].conflicts[world[x][y].count]->type = wolf;
					world[x][y].conflicts[world[x][y].count]->breeding_period = w_breeding;
					world[x][y].conflicts[world[x][y].count]->starvation_period = w_starvation;
					world[x][y].count += 1;
					omp_unset_lock(&(world[x][y].lock_count));
				}
				
				omp_set_lock(&(move_motion->lock_count));
				move_motion->conflicts[move_motion->count] = (conflict*)malloc(sizeof(struct conflicts));
				move_motion->conflicts[move_motion->count]->type = wolf;
				move_motion->conflicts[move_motion->count]->breeding_period = world[x][y].breed ? w_breeding : world[x][y].breeding_period-1;
				move_motion->conflicts[move_motion->count]->starvation_period = world[x][y].starvation_period-1;
				move_motion->count += 1;
				world[x][y].breed=0;
				omp_unset_lock(&(move_motion->lock_count));
				
			}else{
				omp_set_lock(&(world[x][y].lock_count));
				world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
				world[x][y].conflicts[world[x][y].count]->type = wolf;
				world[x][y].conflicts[world[x][y].count]->breeding_period = world[x][y].breeding_period-1;
				world[x][y].conflicts[world[x][y].count]->starvation_period = world[x][y].starvation_period-1;
				world[x][y].count += 1;
				omp_unset_lock(&(world[x][y].lock_count));
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

				/* if breeding period is 0 or lower : he leaves behing a squirrel at the beginning of the breeding period
				 * otherwise: he cannot breed */
				if(world[x][y].breeding_period <= 0){
					omp_set_lock(&(world[x][y].lock_count));
					world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
					world[x][y].conflicts[world[x][y].count]->type = squirrel;
					world[x][y].conflicts[world[x][y].count]->breeding_period = s_breeding;
					world[x][y].conflicts[world[x][y].count]->starvation_period = 0;
					world[x][y].count += 1;
					sot = 0;
					omp_unset_lock(&(world[x][y].lock_count));
				}

				omp_set_lock(&(move_motion->lock_count));
				move_motion->conflicts[move_motion->count] = (conflict*)malloc(sizeof(struct conflicts));
				move_motion->conflicts[move_motion->count]->type = squirrel;
				move_motion->conflicts[move_motion->count]->breeding_period = world[x][y].breeding_period <= 0 ? s_breeding : world[x][y].breeding_period-1;
				move_motion->conflicts[move_motion->count]->starvation_period = 0;
				move_motion->count += 1;
				omp_unset_lock(&(move_motion->lock_count));
				
				if(sot){
					omp_set_lock(&(world[x][y].lock_count));
					world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
					world[x][y].conflicts[world[x][y].count]->type = tree;
					world[x][y].count +=1;
					omp_unset_lock(&(world[x][y].lock_count));
				}
				
				

			}else{
				omp_set_lock(&(world[x][y].lock_count));
				world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
				world[x][y].conflicts[world[x][y].count]->type = sot ? squirrel_on_tree : squirrel;
				world[x][y].conflicts[world[x][y].count]->breeding_period = world[x][y].breeding_period-1;
				world[x][y].conflicts[world[x][y].count]->starvation_period = 0;
				world[x][y].count += 1;
				omp_unset_lock(&(world[x][y].lock_count));
			}
			
			/*Squirrels never starve*/
			break;

		default:
			break;
	}

	return 0;
}

int fixWorld(int size, int w_starvation, int w_breeding){
	int x,y;
	int aux;
	int ate;
	int lowest_breeding;
	
#pragma omp parallel for private(x, y, ate, aux, lowest_breeding) schedule(dynamic, size/(NUM_THREADS*2))
	for(x=0; x<size; x++){
		for(y=0 ; y<size; y++){
			if(world[x][y].type == ice || (world[x][y].type == tree && world[x][y].count == 0))
				continue;
			if(world[x][y].count == 0){
				world[x][y].type = empty;
				continue;
			}
			
			ate=0;
			aux=0;
			lowest_breeding = w_breeding; 

			switch(world[x][y].type){	
				case tree:
					world[x][y].type = squirrel_on_tree;
					world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
					aux++;
					while (aux < world[x][y].count){
						if(world[x][y].conflicts[aux]->breeding_period < world[x][y].breeding_period)
							world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
						aux++;
					}
					world[x][y].starvation_period = 0;
					break;
				
				case squirrel:
					if(world[x][y].count == 1 && world[x][y].conflicts[aux]->type == squirrel){
						world[x][y].type = squirrel;
						world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
						world[x][y].starvation_period = 0;
						break;
					}
					world[x][y].type = wolf;
					world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
					aux++;
					while (aux < world[x][y].count){
						if(world[x][y].conflicts[aux]->breeding_period < world[x][y].breeding_period)
							world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
						aux++;
					}
					world[x][y].starvation_period = w_starvation;
					if(world[x][y].breeding_period <= 0)
						world[x][y].breed = 1;
					break;
					
				case squirrel_on_tree:
					if(world[x][y].conflicts[0]->type == tree)
						world[x][y].type = tree;
					else{
						world[x][y].type = squirrel_on_tree;
						world[x][y].breeding_period = world[x][y].conflicts[0]->breeding_period;
						world[x][y].starvation_period = 0;
					}
					break;
					
				case empty:

					lowest_breeding = world[x][y].conflicts[aux]->type == wolf ? world[x][y].conflicts[aux]->breeding_period : lowest_breeding;
					world[x][y].type = world[x][y].conflicts[aux]->type;
					world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
					world[x][y].starvation_period = world[x][y].conflicts[aux]->starvation_period;
					aux++;
					
					while (aux < world[x][y].count){
						if(world[x][y].type != world[x][y].conflicts[aux]->type){
							ate = 1;
							world[x][y].type = wolf;
							world[x][y].starvation_period = w_starvation;
							if(world[x][y].conflicts[aux]->type == wolf){
								world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
								lowest_breeding = lowest_breeding < world[x][y].breeding_period ? lowest_breeding : world[x][y].breeding_period;
							}
							
						} else if(world[x][y].type == wolf){
							if(world[x][y].conflicts[aux]->starvation_period > world[x][y].starvation_period){
								world[x][y].starvation_period = world[x][y].conflicts[aux]->starvation_period;
								world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
							}
							
							lowest_breeding = lowest_breeding < world[x][y].conflicts[aux]->breeding_period ? 
								lowest_breeding : world[x][y].conflicts[aux]->breeding_period;

						} else if(world[x][y].type == squirrel){
							if(world[x][y].conflicts[aux]->breeding_period < world[x][y].breeding_period)
								world[x][y].breeding_period = world[x][y].conflicts[aux]->breeding_period;
						}
						
						aux++;
					}

					if(ate)
						world[x][y].breeding_period = lowest_breeding;

					if(ate == 1 && world[x][y].breeding_period <= 0)
						world[x][y].breed = 1;

					break;
				
			}
			
			/* Free */
			aux=0;
			while(aux < world[x][y].count){
				free(world[x][y].conflicts[aux]);
				aux++;
			}
			world[x][y].count = 0;
		}
	}
	return 0;
}


int main(int argc, char **argv){

	int i, j;	
	int x, y, size;
	int w_breeding, s_breeding, w_starvation, gen_num;
	char type_code;
	FILE * input_file;
	
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
		world[x][y].type = type_code;
		if(world[x][y].type == wolf){
			world[x][y].breeding_period = w_breeding;
			world[x][y].starvation_period = w_starvation;
		} else if(world[x][y].type == squirrel){
			world[x][y].breeding_period = s_breeding;
		}
	}
	
	fclose(input_file);

#ifdef VERBOSE
	start = omp_get_wtime();
#endif	

	/* Generate */
	while(gen_num != 0){
#pragma omp parallel sections
{
	#pragma omp section
	{
		/* 1st sub-generation - RED */
		#pragma omp parallel for private(i, j) schedule(dynamic, size/(NUM_THREADS*2))
		for(i=0; i<size; i++){
			for(j = i%2 == 0 ? 0 : 1 ; j<size; j+=2){
				computeCell(i, j, s_breeding, w_breeding, w_starvation, size);
			}		
		}
	}
		
	#pragma omp section
	{
		/* 2nd sub-generation */
		#pragma omp parallel for private(i, j) schedule(dynamic, size/(NUM_THREADS*2))
		for(i=0; i<size; i++){
			for(j = i%2 == 0 ? 1 : 0 ; j<size; j+=2){
				computeCell(i, j, s_breeding, w_breeding, w_starvation, size);
			}
		}
	}
}
		fixWorld(size, w_starvation, w_breeding);

		gen_num--;
	}

#ifdef VERBOSE
	end = omp_get_wtime();
	printf("Elapsed time: %lf\n", end-start); 
#endif
	
	/* Output */
	printWorldFormatted(size);
			
	return 0;
}
