#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wolves-squirrels-serial.h"

int main(int argc, char **argv){
	
	const char *const name = (argc && argv[1]) ? argv[1] : "";
	char *file_name = (char *)malloc(strlen(name) + 1);
	if (file_name != NULL) {
		strncpy(file_name, name, sizeof(file_name)-1);
		file_name[sizeof(file_name) - 1] = '\0';
		
	} else {
		/* Handle error */
	}
	
	/* file manage */
	free(file_name);
	
	int x, y;
	entity_types type;
	int w_breeding, s_breeding, w_starvation, gen_num;
	
	scanf("%d %d %c", &x, &y, &type);
	scanf("%d", &w_breeding);
	scanf("%d", &s_breeding);
	scanf("%d", &w_starvation);
	scanf("%d", &gen_num);
	
	return 0;
}
