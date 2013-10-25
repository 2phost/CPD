all:
	gcc -c -ansi -pedantic -Wall wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out

verbose:
	gcc -c -ansi -pedantic -Wall -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out

run-test:
	./wolves-squirrels-serial.out a.in 1 1 1 1

clean:
	rm wolves-squirrels-serial.o wolves-squirrels-serial.out wolves-squirrels-serial.h.gch
