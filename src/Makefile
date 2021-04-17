CC=gcc
CFLAGS=-Wall
PATHLIB=lib

DATABASE=db.bin

all: default 

default: clean lexer.o
	$(CC) $(CFLAGS) -g -o ram ram.c lexer.o 

lexer.o: 
	$(CC) $(CFLAGS) -c lexer.c

clean:
	rm -f lexer.o ram
