#include "wolves-squirrels-serial.h"

int main(int argc, char **argv){
	
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
	}
	
	
	fclose(input_file);

	return 0;
}
