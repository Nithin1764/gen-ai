CC=gcc
CFLAGS=-std=c11 -O2 -Wall -Wextra

all: server

server: server.c
	$(CC) $(CFLAGS) server.c -o server

run: server
	PORT=10000 ./server

clean:
	rm -f server