CC = gcc
CFLAGS = -std=c99 -O2 -Wall

all: noteapp

noteapp: src/main.c
	$(CC) $(CFLAGS) -o noteapp src/main.c

clean:
	rm -f noteapp
