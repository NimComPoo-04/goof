CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: goof

goof: $(OBJ)
<<<<<<< HEAD
	$(CC) $(CFLAGS) -o $@ $^
=======
>>>>>>> origin/main

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf *.o goof
