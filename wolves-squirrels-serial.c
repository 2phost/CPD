#include "wolves-squirrels-serial.h"

void move(entity_types e, int x, int y, int size){
	int cell_number = 0;
	int cell_select = 0;
	int p = 0;

	p += world[x-1][y].type == ' ' ? 1 : 0;
	p += world[x][y+1].type == ' ' ? 1 : 0;
	p += world[x+1][y].type == ' ' ? 1 : 0;
	p += world[x-1][y-1].type == ' ' ? 1 : 0;
		
	cell_number = x*size + y;
	cell_select = cell_number % p;
	
	printf("%d\n", cell_select);
	
}


int main(int argc, char **argv){

	int i, j;	
	int x, y, size;
	int w_breeding, s_breeding, w_starvation, gen_num;
	entity_types type;
	FILE * input_file;
	
	w_breeding = atoi(argv[2]);
	s_breeding = atoi(argv[3]);
	w_starvation = atoi(argv[4]);
	gen_num = atoi(argv[5]);
	
	if(argc < 5)
		return -1;
		
	input_file = fopen(argv[1], "r");
	if(input_file == NULL)
		return -1;
		
	fscanf(input_file, "%d", &size);
	
	while(fscanf(input_file, "%d %d %c", &x, &y, &type) != EOF){
		world[x][y].type = type;
		if(type == w){
			world[x][y].breeding_period = w_breeding;
			world[x][y].starvation_period = w_starvation;
		} else if(type == s){
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
					case w: 
						/* neighboring cells has a squirel? true:eating and increase starvation period*/
						/* if multiple neighboring use method*/
						/* if no neighboring: moves to empty cell or use method */
						
						/* if complete breeding, leave a wolfat beginning of stavation and breeding period*/
						// if move false, cannot breed
						/* if stavation: it dies */
						
						
						break;
					case s: /*Squirrels never starve*/
						//move()
						// if move false, cannot breed
						if(!world[i][j].breeding_period /*&& move true*/){
							/*leave behind a squirel at the beginning of the breeding period*/
							/*starts a new breeding period*/	
						} else {
							
						}
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
