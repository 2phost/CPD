#include <stdlib.h>
#include "wolves-squirrels-mpi.h"
#include <mpi.h>

MPI_Status status; 

/* Move */
struct world *move(entity_types e, int x, int y, int size, int rows, struct world **world, int rank){
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
			if(x+1 < rows && world[x+1][y].type == squirrel)
				pos[p++] = &world[x+1][y];
			if(y-1 >= 0 && world[x][y-1].type == squirrel)
				pos[p++] = &world[x][y-1];
				
			if(p==0){
				if(x-1 >= 0 && world[x-1][y].type == empty)
					pos[p++] = &world[x-1][y];
				if(y+1 < size && world[x][y+1].type == empty)
					pos[p++] = &world[x][y+1];
				if(x+1 < rows && world[x+1][y].type == empty)
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
			if(x+1 < rows && (world[x+1][y].type == tree || world[x+1][y].type == empty))
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

	for(i=0; i < world_size; i++)
		for(j=0; j < world_size; j++){
			for(z=0; z < 5; z++)
				world_global[i][j].conflicts[z]=NULL;
			world_global[i][j].type = empty;
			world_global[i][j].breed = 0;
			world_global[i][j].count=0;
		}

	return 0;
}

int printWorldFormatted(int world_size){

	int i, j;

	printf("%d\n", world_size);
	
	for(i=0; i < world_size; i++){
		for(j=0; j < world_size; j++){
			if(world_global[i][j].type != empty)			
				printf("%d %d %c\n", i, j, world_global[i][j].type);	
		}
	}

	return 0;
}


int printProcWorldFormatted(int size, int rank, struct world** proc_world){

	int i, j;
	int aditional;

	if(rank == 0 || rank == 1)
		aditional = 1;
	else
		aditional = 2;
	
	printf("RANK %d\n", rank);

	printf("%d\n", size);
	
	for(i=0; i < size+aditional; i++){
		for(j=0; j < size; j++){
			if(proc_world[i][j].type != empty)			
				printf("%d %d %c\n", i, j, proc_world[i][j].type);	
		}
	}

	return 0;
}


int computeCell(int x, int y, int s_breeding, int w_breeding, int w_starvation, int world_size, int rows, struct world **world, int rank){

	
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
			
			

			move_motion = move(wolf, x, y, world_size, rows, world, rank);


			if(move_motion != NULL){
				
				/* If pregnant? */
				if(world[x][y].breed){
					world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
					world[x][y].conflicts[world[x][y].count]->type = wolf;
					world[x][y].conflicts[world[x][y].count]->breeding_period = w_breeding;
					world[x][y].conflicts[world[x][y].count]->starvation_period = w_starvation;
					world[x][y].count += 1;
				}
								
				move_motion->conflicts[move_motion->count] = (conflict*)malloc(sizeof(struct conflicts));
				move_motion->conflicts[move_motion->count]->type = wolf;
				move_motion->conflicts[move_motion->count]->breeding_period = world[x][y].breed ? w_breeding : world[x][y].breeding_period-1;
				move_motion->conflicts[move_motion->count]->starvation_period = world[x][y].starvation_period-1;
				move_motion->count += 1;
				world[x][y].breed=0;
				
			}else{
				world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
				world[x][y].conflicts[world[x][y].count]->type = wolf;
				world[x][y].conflicts[world[x][y].count]->breeding_period = world[x][y].breeding_period-1;
				world[x][y].conflicts[world[x][y].count]->starvation_period = world[x][y].starvation_period-1;
				world[x][y].count += 1;
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

		  move_motion = move(squirrel, x, y, world_size, rows, world, rank);
		  

			if(move_motion != NULL){

				/* if breeding period is 0 or lower : he leaves behing a squirrel at the beginning of the breeding period
				 * otherwise: he cannot breed */
				if(world[x][y].breeding_period <= 0){
					world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
					world[x][y].conflicts[world[x][y].count]->type = squirrel;
					world[x][y].conflicts[world[x][y].count]->breeding_period = s_breeding;
					world[x][y].conflicts[world[x][y].count]->starvation_period = 0;
					world[x][y].count += 1;
					sot = 0;
				}

				move_motion->conflicts[move_motion->count] = (conflict*)malloc(sizeof(struct conflicts));
				move_motion->conflicts[move_motion->count]->type = squirrel;
				move_motion->conflicts[move_motion->count]->breeding_period = world[x][y].breeding_period <= 0 ? s_breeding : world[x][y].breeding_period-1;
				move_motion->conflicts[move_motion->count]->starvation_period = 0;
				move_motion->count += 1;
				
				if(sot){
					world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
					world[x][y].conflicts[world[x][y].count]->type = tree;
					world[x][y].count +=1;
				}

			}else{
				world[x][y].conflicts[world[x][y].count] = (conflict*)malloc(sizeof(struct conflicts));
				world[x][y].conflicts[world[x][y].count]->type = sot ? squirrel_on_tree : squirrel;
				world[x][y].conflicts[world[x][y].count]->breeding_period = world[x][y].breeding_period-1;
				world[x][y].conflicts[world[x][y].count]->starvation_period = 0;
				world[x][y].count += 1;
			}
			
			/*Squirrels never starve*/
			break;

		default:
			break;
	}

	return 0;
}

int fixWorld(int size, int w_starvation, int w_breeding, int rows, struct world **world, int rank){
	int x,y;
	int aux;
	int ate;
	int lowest_breeding;
	
	for(x = rank==0 ? 0:1; x<rank==0 ? rows : rows+1; x++){
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

  int i, j, z;	
	int x, y, size;
	int w_breeding, s_breeding, w_starvation, gen_num;
	char type_code;
	char *file_name;
	FILE * input_file;
	double secs;
	int averow, extra, rank, n_processes, offset, rows;
	int mtype, dest, rowsAux, source;
	int aditional, start_line;

	struct world **proc_world;

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); //Rank, an integer, is used in MPI to be a process identifier associated with a communicator
	MPI_Comm_size (MPI_COMM_WORLD, &n_processes); //where size is the total number of processes in the MPI_Comm_world.

	/* Processo Master */
	if (rank == MASTER) {
	  secs =  MPI_Wtime();

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

  /* Talvez dê para melhorar */
	fscanf(input_file, "%d", &size);

	initWorld(size);


	while(fscanf(input_file, "%d %d %c", &x, &y, &type_code) != EOF){
		world_global[x][y].type = type_code;
		if(world_global[x][y].type == wolf){
			world_global[x][y].breeding_period = w_breeding;
			world_global[x][y].starvation_period = w_starvation;
		} else if(world_global[x][y].type == squirrel){
			world_global[x][y].breeding_period = s_breeding;
		}
	}

	fclose(input_file);

	  
	/* Envia parcela a cada processo */
	averow = size / n_processes;
	extra = size % n_processes;
	offset = 0;
	mtype = FROM_MASTER;


	for(dest=1; dest<n_processes; dest++){
		
		rowsAux = dest <= extra ? averow+1 : averow;
		MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
		MPI_Send(&rowsAux, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
		MPI_Send(&size, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);

		for(i=0; i < rowsAux; i++)
			MPI_Send(world_global[offset+i], sizeof(struct world)*size, MPI_BYTE, dest, mtype, MPI_COMM_WORLD);

		offset = offset + rowsAux;
  	}

	proc_world = (struct world**) malloc(sizeof(struct world*)*(averow+1));
	for(i = 0; i < averow+1; i++){
	  proc_world[i] = (struct world*) malloc(sizeof(struct world)*size);
	  for(j = 0; j < size; j ++){
		proc_world[i][j].type = empty;
		proc_world[i][j].count = 0;
		proc_world[i][j].breeding_period = 0;
		proc_world[i][j].starvation_period = 0;
		proc_world[i][j].breed = 0;
		for(z=0; z < 5; z++)
		  world_global[i][j].conflicts[z]=NULL;
	  }
	}
	

	for(i=1; i< averow+1; i++)
		proc_world[i] = world_global[offset+i]; 				
	 
#ifdef MPIVERBOSE
	  printf("Distribuição: %lf\n", MPI_Wtime() - secs);
	  secs = MPI_Wtime();
#endif
	  aditional = 1;
	} else if(rank > 0) {
		
		mtype = FROM_MASTER;
		source = MASTER;
		MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&rowsAux, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&size, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);

		proc_world = (struct world**) malloc(sizeof(struct world*)*(rowsAux+aditional));
		

		if(rank == 1){
			aditional = 1;
			start_line = 0;
		} else{
			aditional = 2;
			start_line = 1;
		}

		for(i = 0; i < rowsAux+aditional; i++){
			proc_world[i] = (struct world*) malloc(sizeof(struct world)*size);
			for(j = 0; j < size; j ++){
			  proc_world[i][j].type = empty;
			  proc_world[i][j].count = 0;
			  proc_world[i][j].breeding_period = 0;
			  proc_world[i][j].starvation_period = 0;
			  proc_world[i][j].breed = 0;
			  for(z=0; z < 5; z++)
				world_global[i][j].conflicts[z]=NULL;
			}
		}
			
		for(i = start_line; i < rowsAux+start_line; i++){
			MPI_Recv(proc_world[i], sizeof(struct world)*size, MPI_BYTE, source, mtype, MPI_COMM_WORLD, &status);
		}
	}


#ifdef VERBOSE
//	start = clock();
#endif

	/* Comum a todos os processos */
	MPI_Bcast(&w_breeding, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s_breeding, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&w_starvation, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gen_num, 1, MPI_INT, 0, MPI_COMM_WORLD);




#ifdef MPIVERBOSE
//	printf("Leitura: %lf\n", MPI_Wtime() - secs);
//	secs =  MPI_Wtime();
#endif

	/* Generate */
	MPI_Status	*statuses[2];
	MPI_Request *request[2];
	struct world *worldaux1, *worldaux2;
		
	//worldaux1 = (struct world*) malloc(sizeof(struct world)*size);
	//worldaux2 = (struct world*) malloc(sizeof(struct world)*size);

	while(gen_num != 0){
		
		if(rank == 1){
			for(i = 0; i < size ; i++){
				proc_world[rowsAux][i].type = empty;
				proc_world[rowsAux][i].count = 0;
				proc_world[rowsAux][i].breeding_period = 0;
				proc_world[rowsAux][i].starvation_period = 0;
				proc_world[rowsAux][i].breed = 0;
				for(z=0; z < 5; z++)
					world_global[i][j].conflicts[z]=NULL;
			}
		}else if(rank == MASTER){
			for(i = 0; i < size ; i++){
				proc_world[0][i].type = empty;
				proc_world[0][i].count = 0;
				proc_world[0][i].breeding_period = 0;
				proc_world[0][i].starvation_period = 0;
				proc_world[0][i].breed = 0;
				for(z=0; z < 5; z++)
					world_global[i][j].conflicts[z]=NULL;
			}
		}else{
			for(i = 0; i < size ; i++){
				proc_world[0][i].type = empty;
				proc_world[0][i].count = 0;
				proc_world[0][i].breeding_period = 0;
				proc_world[0][i].starvation_period = 0;
				proc_world[0][i].breed = 0;
				for(z=0; z < 5; z++)
					world_global[i][j].conflicts[z]=NULL;
				proc_world[rowsAux][i].type = empty;
				proc_world[rowsAux][i].count = 0;
				proc_world[rowsAux][i].breeding_period = 0;
				proc_world[rowsAux][i].starvation_period = 0;
				proc_world[rowsAux][i].breed = 0;
				for(z=0; z < 5; z++)
					world_global[i][j].conflicts[z]=NULL;
			}
		}

	  for(i = (rank==1? 0 : 1); i< (rank==1 ? rowsAux : rowsAux+1) ; i++){
		for(j=0; j<size; j++){
		  computeCell(i, j, s_breeding, w_breeding, w_starvation, size, rowsAux+aditional, proc_world, rank);
		}
	  }
	  

	  /*Enviar a  linha de coisas estranhas para o(s) processo(s) da fronteira */
	  if(rank == 1){
		MPI_Irecv(worldaux1, sizeof(struct world)*size, MPI_BYTE, rank+1, 2, MPI_COMM_WORLD, request[0]);
		MPI_Send(proc_world[rowsAux], sizeof(struct world)*size, MPI_BYTE, (rank+1)%n_processes, rank, MPI_COMM_WORLD);
		MPI_Wait(request[0], statuses[0]);

		/* Juntar aos conflictos*/
		for(i=0; i<size; i++){
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count] = (conflict*)malloc(sizeof(struct conflicts));
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count]->type = worldaux1[i].type;
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count]->breeding_period = worldaux1[i].breeding_period;
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count]->starvation_period = worldaux1[i].starvation_period;
		  proc_world[rowsAux][i].count += 1;
		}
	  } else if(rank == MASTER){
		MPI_Irecv(worldaux1, sizeof(struct world)*size, MPI_BYTE, n_processes-1, n_processes-1, MPI_COMM_WORLD, request[0]);
		MPI_Send(proc_world[averow], sizeof(struct world)*size, MPI_BYTE, n_processes-1, rank, MPI_COMM_WORLD);
		MPI_Wait(request[0], statuses[0]);

		for(i=0; i<size; i++){
		  proc_world[0][i].conflicts[proc_world[0][i].count] = (conflict*)malloc(sizeof(struct conflicts));
		  proc_world[0][i].conflicts[proc_world[0][i].count]->type = worldaux1[i].type;
		  proc_world[0][i].conflicts[proc_world[0][i].count]->breeding_period = worldaux1[i].breeding_period;
		  proc_world[0][i].conflicts[proc_world[0][i].count]->starvation_period = worldaux1[i].starvation_period;
		  proc_world[0][i].count += 1;
		}
	  } else {
		MPI_Irecv(worldaux1, sizeof(struct world)*size, MPI_BYTE, rank+1%n_processes, rank+1%n_processes, MPI_COMM_WORLD, request[0]);
		MPI_Irecv(worldaux2, sizeof(struct world)*size, MPI_BYTE, rank-1%n_processes, rank-1%n_processes, MPI_COMM_WORLD, request[1]);
		MPI_Send(proc_world[rowsAux], sizeof(struct world)*size, MPI_BYTE, (rank+1)%n_processes, rank, MPI_COMM_WORLD);
		MPI_Send(proc_world[rowsAux], sizeof(struct world)*size, MPI_BYTE, (rank-1)%n_processes, rank, MPI_COMM_WORLD);
		MPI_Wait(request[0], statuses[0]);
		MPI_Wait(request[1], statuses[1]);

		for(i=0; i<size; i++){
		  proc_world[0][i].conflicts[proc_world[0][i].count] = (conflict*)malloc(sizeof(struct conflicts));
		  proc_world[0][i].conflicts[proc_world[0][i].count]->type = worldaux1[i].type;
		  proc_world[0][i].conflicts[proc_world[0][i].count]->breeding_period = worldaux1[i].breeding_period;
		  proc_world[0][i].conflicts[proc_world[0][i].count]->starvation_period = worldaux1[i].starvation_period;
		  proc_world[0][i].count += 1;
		}

		for(i=0; i<size; i++){
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count] = (conflict*)malloc(sizeof(struct conflicts));
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count]->type = worldaux1[i].type;
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count]->breeding_period = worldaux1[i].breeding_period;
		  proc_world[rowsAux][i].conflicts[proc_world[rowsAux][i].count]->starvation_period = worldaux1[i].starvation_period;
		  proc_world[rowsAux][i].count += 1;
		}
	  }
	  
	  /* fix do pequeno mundo do processo */
	  fixWorld(size, w_starvation, w_breeding, rowsAux, proc_world, rank);
	  
	  gen_num--;
	}


	/* Receber tudo no master */

#ifdef VERBOSE
	end = clock();
	printf("Elapsed Time : %lf\n", (double)(end-start) / CLOCKS_PER_SEC);
#endif
	
	MPI_Finalize();

	/* Output */
	//printWorldFormatted(size);
			
	return 0;
}
