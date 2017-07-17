CC = gcc
CFLAGS = -Wall -Wpedantic --std=c99 -g

all : dice

dice : main.o
	$(CC) $^ -o $@

clean :
	rm -rf *.o dice

.PHONY : all clean