all: run
run:
	gcc main.c task.c users.c board.c -o main
	./main

