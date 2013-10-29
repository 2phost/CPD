serial:
	gcc -c -ansi -pedantic -Wall wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels.out

serial-verbose:
	gcc -c -ansi -pedantic -Wall -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels.out

clean:
	rm wolves-squirrels-serial.o wolves-squirrels.out wolves-squirrels-serial.h.gch
