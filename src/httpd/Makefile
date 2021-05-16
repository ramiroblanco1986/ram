CC=gcc
CFLAGS=-Wall
PATHLIB=lib

all: default 

default: clean request.o
	$(CC) $(CFLAGS) -o httpd request.o httpd.c 

request.o: 
	$(CC) $(CFLAGS) -c request.c

clean:
	rm -f request.o httpd
