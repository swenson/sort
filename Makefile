# Copyright (c) 2010-2014 Christopher Swenson.
# Copyright (c) 2012 Google Inc. All Rights Reserved.

CC ?= gcc
CFLAGS ?= -O3 -g -Wall -std=c89 -pedantic -Wno-long-long -Wno-format

default: demo stresstest multidemo test

.PHONY: default clean test format

test: stresstest benchmark
	./benchmark | tee benchmark.txt
	./stresstest

clean:
	rm -f demo multidemo stresstest benchmark

demo: demo.c sort.h
	$(CC) $(CFLAGS) demo.c -o demo

multidemo: multidemo.c sort.h
	$(CC) $(CFLAGS) multidemo.c -o multidemo

stresstest: stresstest.c sort.h
	$(CC) $(CFLAGS) stresstest.c -o stresstest

benchmark: benchmark.c sort.h
	$(CC) $(CFLAGS) benchmark.c -o benchmark

format:
	astyle --options=astyle.options sort.h demo.c multidemo.c stresstest.c benchmark.c