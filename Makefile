all:
	gcc -c -ansi -pedantic -Wall wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out

run-test:
	./wolves-squirrels-serial a.in 1 1 1 1

clean:
	rm wolves-squirrels-serial.o wolves-squirrels-serial.out