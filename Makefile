sync_car: sync_car.c sync_car-runner.c sync_car.h
	gcc -g -pthread sync_car.c sync_car-runner.c -o sync_car

.PHONY: clean
clean:
	rm -f sync_car