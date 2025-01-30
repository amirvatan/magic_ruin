
run : main 
	./main.out
main : main.c 
	gcc -lncurses -o main.out main.c

