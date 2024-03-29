mpi:
	mpicc -g -o wolves-squirrels-mpi.out -DMPIVERBOSE=1 wolves-squirrels-mpi.c wolves-squirrels-mpi.h
	mpirun -np 4 wolves-squirrels-mpi.out prof-test/world_10.in 20 5 30 5
	
serial-v:
	gcc -c -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out
	./wolves-squirrels-serial.out prof-test/world_1000.in 20 50 30 10000 > serial.out
	cat serial.out | grep Elapsed

parallel:
	gcc -o wolves-squirrels-p.out -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

parallel-v:
	gcc -o wolves-squirrels.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

dynamic-v:
	gcc -o wolves-squirrels-dynamic.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel-dynamic.c wolves-squirrels-parallel.h
	./wolves-squirrels-dynamic.out prof-test/world_1000.in 20 50 30 10000 > dynamic.out
	cat dynamic.out | grep Elapsed
guided-v:
	gcc -o wolves-squirrels-guided.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel-guided.c wolves-squirrels-parallel.h
	./wolves-squirrels-guided.out prof-test/world_1000.in 20 50 30 10000 > guided.out
	cat guided.out | grep Elapsed

static-v:
	gcc -o wolves-squirrels-static.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel-static.c wolves-squirrels-parallel.h
	./wolves-squirrels-static.out prof-test/world_1000.in 20 50 30 10000 > static.out
	cat static.out | grep Elapsed

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
	./wolves-squirrels-s.out prof-test/world_1000.in 20 50 30 200 > 1.out
	./wolves-squirrels-p.out prof-test/world_1000.in 20 50 30 200 > 2.out
	diff 1.out 2.out
	
clean:
	rm wolves-squirrels-serial.o wolves-squirrels.out wolves-squirrels-serial.h.gch *.out
