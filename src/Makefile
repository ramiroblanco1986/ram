CC=gcc
CFLAGS=-Wall
PATHLIB=lib

DATABASE=db.bin

all: default 

default: lexer.o
	$(CC) $(CFLAGS) -o ram ram.c lexer.o 

lexer.o: 
	$(CC) $(CFLAGS) -c lexer.c

clean:
	rm -f lexer.o ram
