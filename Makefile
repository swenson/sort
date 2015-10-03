# Copyright (c) 2010-2014 Christopher Swenson.
# Copyright (c) 2012 Google Inc. All Rights Reserved.

CC ?= gcc
CFLAGS = -O3 -g -Wall -std=c89

default: demo stresstest multidemo test

.PHONY: default clean test format

test: stresstest
	./stresstest

clean:
	rm -f demo multidemo stresstest

demo: demo.c sort.h sort_common.h
	$(CC) $(CFLAGS) demo.c -o demo

multidemo: multidemo.c sort.h sort_common.h
	$(CC) $(CFLAGS) multidemo.c -o multidemo

stresstest: stresstest.c sort.h sort_common.h
	$(CC) $(CFLAGS) stresstest.c -o stresstest

format:
	astyle --options=astyle.options sort.h sort_common.h demo.c multidemo.c stresstest.c
