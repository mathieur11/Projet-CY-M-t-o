CC=gcc
CFLAGS=-c
all: tri

tri: main.o header.h
	$(CC) main.o -o tri

main.o: main.c header.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *o main

