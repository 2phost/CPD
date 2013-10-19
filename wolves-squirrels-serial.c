#include "wolves-squirrels-serial.h"

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
					
						break;
					case squirrel:
						
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
