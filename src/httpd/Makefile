CC=gcc
CFLAGS=-Wall
CSSLFLAGS=-lssl -lcrypto
PATHLIB=lib

all: default 

default: clean base64.o request.o
	$(CC) $(CFLAGS) $(CSSLFLAGS) -o httpd base64.o request.o httpd.c 

request.o: 
	$(CC) $(CFLAGS) $(CSSLFLAGS) -c request.c

base64.o: 
	$(CC) $(CFLAGS) $(CSSLFLAGS) -c base64.c
clean:
	rm -f base64.o request.o httpd
