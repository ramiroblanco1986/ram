CC=gcc
CFLAGS=-Wall
CSSLFLAGS=-lssl -lcrypto -lpthread
PATHLIB=lib

all: default 

default: clean base64.o request.o websocket.o
	$(CC) $(CFLAGS) $(CSSLFLAGS) -o httpd base64.o request.o websocket.o httpd.c 

request.o: 
	$(CC) $(CFLAGS) $(CSSLFLAGS) -c request.c

websocket.o: 
	$(CC) $(CFLAGS) $(CSSLFLAGS) -c websocket.c

base64.o: 
	$(CC) $(CFLAGS) $(CSSLFLAGS) -c base64.c
clean:
	rm -f base64.o request.o websocket.o httpd
