serial-v:
	gcc -c -DVERBOSE=1 wolves-squirrels-serial.h wolves-squirrels-serial.c
	gcc wolves-squirrels-serial.o -o wolves-squirrels-serial.out
	./wolves-squirrels-serial.out prof-test/world_1000.in 20 50 30 10000 > serial.out
	cat serial.out | grep Elapsed

parallel:
	gcc -o wolves-squirrels-p.out -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

parallel-v:
	gcc -o wolves-squirrels.out -DVERBOSE=1 -fopenmp wolves-squirrels-parallel.c wolves-squirrels-parallel.h

clean:
	rm wolves-squirrels-serial.o wolves-squirrels.out wolves-squirrels-serial.h.gch *.out
