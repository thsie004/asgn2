SHELL=/bin/sh
CC=g++
CFLAGS=-Wall -Werror -ansi -pedantic

all: bin/rshell

rshell: bin/rshell

bin/rshell:
	mkdir bin
	$(CC) $(CFLAGS) src/main.cpp -o bin/rshell

clean:
	rm -r bin
