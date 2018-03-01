all: drone
drone:drone.o
	gcc -lpthread -o drone drone.o
drone.o: drone.c
	gcc -c drone.c
clean:
	rm drone.o drone
