CC = gcc
CFLAGS = -O3 -g -Wall

default: demo stresstest multidemo

.PHONY: default

demo: demo.c sort.h sort_common.h
	$(CC) $(CFLAGS) demo.c -o demo

multidemo: multidemo.c sort.h sort_common.h
	$(CC) $(CFLAGS) multidemo.c -o multidemo

stresstest: stresstest.c sort.h sort_common.h
	$(CC) $(CFLAGS) stresstest.c -o stresstest
