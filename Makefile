serial:
	gcc -c wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-s.out

parallel:
	gcc -o wolves-squirrels-p.out -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

parallel-v:
	gcc -o wolves-squirrels.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

serial-verbose:
	gcc -c -ansi -pedantic -Wall -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels.out

test1:
	make serial
	make parallel
	./wolves-squirrels-s.out prof-test/world_10.in 20 5 30 5 > 1.out
	./wolves-squirrels-p.out prof-test/world_10.in 20 5 30 5 > 2.out
	diff 1.out 2.out

test2:
	make serial
	make parallel
	./wolves-squirrels-s.out prof-test/world_10.in 20 50 30 1000000 > 1.out
	./wolves-squirrels-p.out prof-test/world_10.in 20 50 30 1000000 > 2.out
	diff 1.out 2.out
	
test3:
	make serial
	make parallel
	./wolves-squirrels-s.out prof-test/world_100.in 20 50 30 100000 > 1.out
	./wolves-squirrels-p.out prof-test/world_100.in 20 50 30 100000 > 2.out
	diff 1.out 2.out
	
test4:
	make serial
	make parallel
	./wolves-squirrels-s.out prof-test/world_1000.in 20 50 30 100 > 1.out
	./wolves-squirrels-p.out prof-test/world_1000.in 20 50 30 100 > 2.out
	diff 1.out 2.out
	
test5:
	make serial
	make parallel
	./wolves-squirrels-s.out prof-test/world_10000.in 20 50 30 100 > 1.out
	./wolves-squirrels-p.out prof-test/world_10000.in 20 50 30 100 > 2.out
	diff 1.out 2.out
	
clean:
	rm wolves-squirrels-serial.o wolves-squirrels.out wolves-squirrels-serial.h.gch
