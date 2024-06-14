all: main.o structs.o
	gcc main.o structs.o -pthread -o study

main.o: main.c structs.h
	gcc  -c main.c -o main.o

structs.o: structs.c structs.h
	gcc -c structs.c -o structs.o

clean:
	rm -f *.o study