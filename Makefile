CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

RAYLIB = ~/opt/raylib-5.0_linux_amd64

all: goof

goof: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -L $(RAYLIB)/lib -lraylib -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ -I $(RAYLIB)/include

clean:
	rm -rf *.o goof
