CC = gcc
CFLAGS = -O3 -g -Wall

default: demo stresstest

.PHONY: default

demo: demo.c sort.h
	$(CC) $(CFLAGS) demo.c -o demo

stresstest: stresstest.c sort.h
	$(CC) $(CFLAGS) stresstest.c -o stresstest