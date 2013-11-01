serial:
	gcc -c wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out

serial-v:
	gcc -c -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out

parallel:
	gcc -o wolves-squirrels-parallel.out -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

parallel-v:
	gcc -o wolves-squirrels-parallel.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

clean:
	rm *.o *.out
