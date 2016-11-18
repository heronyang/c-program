CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -pthread

all: server

wrapper.o: wrapper.c wrapper.h
	$(CC) $(CFLAGS) -c wrapper.c

server.o: server.c wrapper.h
	$(CC) $(CFLAGS) -c server.c

server: server.o wrapper.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o server
