all:
	cd .. && $(MAKE)

test-sqrl-simple-move:
	../wolves-squirrels-serial.out test-squirrel-simple-move.in 1 3 1 1 > out.out
	diff out.out test-squirrel-simple-move.out
	rm out.out

test-sqrl-invalid-move:
	../wolves-squirrels-serial.out test-sqrl-invalid-move.in 1 1 1 3 > out.out

test-sqrl-simple-breed:
	../wolves-squirrels-serial.out test-sqrl-simple-breed.in 1 3 1 3

test-sqrl-breed-tree:
	../wolves-squirrels-serial.out test-sqrl-breed-tree.in 1 1 1 1

test-sqrl-simple-conflict:
	../wolves-squirrels-serial.out test-sqrl-simple-conflict.in 1 3 1 1

test-wlf-simple-move:
	../wolves-squirrels-serial.out test-wlf-simple-move.in 3 3 3 2

test-wlf-invalid-move:
	../wolves-squirrels-serial.out test-wlf-invalid-move.in 3 3 3 2
