CC := gcc
CFLAGS := -std=c99
OBJ := io.o main.o 

all: client.c server.c
	$(CC) client.c -o client.out
	$(CC) server.c -o server.out

c: client.c
	$(CC) client.c -o client.out

s: server.c
	$(CC) server.c -o server.out