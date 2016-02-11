CC=g++
CFLAGS=-Wall -Werror -ansi -pedantic
VPATH=src:/src

all: rshell

rshell: main.o input.o words.o
	$(CC) main.o input.o words.o -o rshell

main.o: main.cpp
	$(CC) $(CFLAGS) src/main.cpp

input.o: input.cpp
	$(CC) $(CFLAGS) src/input.cpp

words.o: words.cpp words.h
	$(CC) $(CFLAGS) src/words.cpp

clean:
	rm *o rshell
