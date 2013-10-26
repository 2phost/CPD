all:
	cd .. && $(MAKE)
	cat test.log | less

help:
	cat test.log | less

#Generic World Tests
test-static-scene:
	../wolves-squirrels-serial.out test-static-scene.in 1 1 1 100 > out.out
	diff out.out ./expected_outputs/test-static-scene.out
	rm out.out

#Squirrel Related Tests
test-sqrl-simple-move:
	../wolves-squirrels-serial.out test-squirrel-simple-move.in 1 3 1 1 > out.out
	diff out.out ./expected_outputs/test-squirrel-simple-move.out
	rm out.out

test-sqrl-invalid-move:
	../wolves-squirrels-serial.out test-sqrl-invalid-move.in 1 1 1 3 > out.out
	diff out.out ./expected_outputs/test-sqrl-invalid-move.out
	rm out.out

test-sqrl-simple-breed:
	../wolves-squirrels-serial.out test-sqrl-simple-breed.in 1 3 1 3 > out.out
	diff out.out ./expected_outputs/test-sqrl-simple-breed.out 
	rm out.out

test-sqrl-breed-tree:
	../wolves-squirrels-serial.out test-sqrl-breed-tree.in 1 1 1 1 > out.out
	diff out.out ./expected_outputs/test-sqrl-breed-tree.out
	rm out.out

test-sqrl-simple-conflict:
	../wolves-squirrels-serial.out test-sqrl-simple-conflict.in 1 3 1 1 > out.out
	diff out.out ./expected_outputs/test-sqrl-simple-conflict.out
	rm out.out

test-wlf-simple-move:
	../wolves-squirrels-serial.out test-wlf-simple-move.in 3 3 3 2 > out.out
	diff out.out ./expected_outputs/test-wlf-simple-move.out
	rm out.out

test-wlf-invalid-move:
	../wolves-squirrels-serial.out test-wlf-invalid-move.in 3 3 3 2 > out.out
	diff out.out ./expected_outputs/test-wlf-invalid-move.out
	rm out.out
