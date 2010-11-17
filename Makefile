CC = gcc
CFLAGS = -O3 -g -Wall

demo: demo.c sort.h
	$(CC) $(CFLAGS) demo.c -o demo