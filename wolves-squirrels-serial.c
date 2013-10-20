#include "wolves-squirrels-serial.h"

/* Return 1 if it moves */
coord move(entity_types e, int x, int y, int size){
	int cell_number = 0;
	int cell_select = 0;
	int p = 0;

	/*  */
	if(e == wolf){
		/* Search for Squirrels */
		p += world[x-1][y].type == squirrel ? 1 : 0;
		p += world[x][y+1].type == squirrel ? 1 : 0;
		p += world[x+1][y].type == squirrel ? 1 : 0;
		p += world[x-1][y-1].type == squirrel ? 1 : 0;
		/* If there are not Squirrels search for empty cells*/
		if(p>0){
			p += world[x-1][y].type == empty ? 1 : 0;
			p += world[x][y+1].type == empty ? 1 : 0;
			p += world[x+1][y].type == empty ? 1 : 0;
			p += world[x-1][y-1].type == empty ? 1 : 0;
		}
	} else if(e == squirrel) {
		/* Search for Trees */
		p += world[x-1][y].type == tree ? 1 : 0;
		p += world[x][y+1].type == tree ? 1 : 0;
		p += world[x+1][y].type == tree ? 1 : 0;
		p += world[x-1][y-1].type == tree ? 1 : 0;
		/* If there are not Trees search for empty cells*/
		if(p>0){
			p += world[x-1][y].type == empty ? 1 : 0;
			p += world[x][y+1].type == empty ? 1 : 0;
			p += world[x+1][y].type == empty ? 1 : 0;
			p += world[x-1][y-1].type == empty ? 1 : 0;
		}
	}
	
	cell_number = x*size + y;
	cell_select = cell_number % p;
	
	printf("%d\n", cell_select);
	
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

int clearWorldCell(int x, int y){

	world[x][y].type = empty;
	world[x][y].breeding_period = 0;
	world[x][y].starvation_period = 0;

	return 1;
}

int makeBabies(entity_types type, int prev_x, int prev_y, int curr_x, int curr_y, int breeding_period, int starvation_period){

	/*Create Baby*/
	world[prev_x][prev_y].type = type;
	world[prev_x][prev_y].breeding_period = breeding_period;
	world[prev_x][prev_y].starvation_period = starvation_period;

	/* Restart entity breeding period */
	world[curr_x][curr_y].breeding_period = breeding_period;

	return 0;
}

int main(int argc, char **argv){

	int i, j;	
	int x, y, size;
	int w_breeding, s_breeding, w_starvation, gen_num;
	entity_types type = empty;
	char type_code;
	FILE * input_file;
	coord move_motion = NULL;
	
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
	
	
	/* Generate */
	while(gen_num != 0){
		
		/* 1st sub-generation - RED */
		for(i=0; i<size; i++){
			for(j = i%2==0 ? 0 : 1 ; j<size; j+=2){
				switch(world[i][j].type){

					case wolf:
						/* Updates breeding and starvation periods */
						world[i][j].breeding_period--;
						world[i][j].starvation_period--;

						/* neighboring cells has a squirel? true:eating and increase starvation period*/
						/* if multiple neighboring use method*/
						/* if no neighboring: moves to empty cell or use method */
						move_motion = move(wolf, i, j, size);

						if(move_motion != NULL) /* wolf moved */
							;

								
							
						/* if complete breeding, leave a wolf at beginning of stavation and breeding period*/
						/* if move false, cannot breed */
						if(world[i][j].breeding_period && move_motion != NULL)
							makeBabies(wolf, i, j, move_motion->x, move_motion->y, w_breeding, w_starvation);

						/* if stavation: it dies */
						if(world[i][j].starvation_period){
							if(move_motion == NULL){
								clearWorldCell(i, j);
							}else
								clearWorldCell(move_motion->x, move_motion->y);
						}
					
						break;
					case squirrel: 

						/* Updates breeding period */
						world[i][j].breeding_period--;

						move_motion = move(squirrel, i, j, size);

						/*if move false, cannot breed otherwise
						  leave behind a squirel at the beginning of the breeding period
						  starts a new breeding period*/	
						if(!world[i][j].breeding_period && move_motion != NULL)
							makeBabies(squirrel, i, j, move_motion->x, move_motion->y, s_breeding, 0);

						/*Squirrels never starve*/

						break;
					default:
						break;
				}
			}
		}
			
		/* 2nd sub-generation */
		for(i=0; i<size; i++){
			for(j = i%2==0 ? 1 : 0 ; j<size; j+=2){
			
			}
		}
		
		
		gen_num--;
	}
	
	/* Output */
	for(i=0; i<size; i++)
		for(j=0; j<size; j++){

		}
			
	return 0;
}
