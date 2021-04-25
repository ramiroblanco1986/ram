CC=gcc
CFLAGS=-Wall
PATHLIB=lib

DATABASE=db.bin

all: default 

default: clean lexer.o
	$(CC) $(CFLAGS) -o ram lexer.o ram.c 

lexer.o: 
	$(CC) $(CFLAGS) -c lexer.c

clean:
	rm -f lexer.o ram
