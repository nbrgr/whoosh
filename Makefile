flags = -Wall

all: whoosh

whoosh: main parse execute builtin
	gcc parse.o builtin.o execute.o main.o -o whoosh

main: main.c
	gcc -c main.c -o main.o

parse: parse.c
	gcc -c parse.c -o parse.o

execute: execute.c
	gcc -c execute.c -o execute.o

builtin: builtin.c
	gcc -c builtin.c -o builtin.o

clean:
	rm *.o --force
