CC=gcc
CFLAGS=-Wall
PATHLIB=lib

all: default 

default: clean parser.o lexer.o
	$(CC) $(CFLAGS) -o ram parser.o lexer.o ram.c 

lexer.o: 
	$(CC) $(CFLAGS) -c lexer.c

parser.o: 
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f parser.o lexer.o ram
