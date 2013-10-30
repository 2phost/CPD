serial:
	gcc -c -ansi -pedantic -Wall wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels.out

parallel:
	gcc -o wolves-squirrels.out -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

parallel-v:
	gcc -o wolves-squirrels.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

serial-verbose:
	gcc -c -ansi -pedantic -Wall -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels.out

clean:
	rm wolves-squirrels-serial.o wolves-squirrels.out wolves-squirrels-serial.h.gch
