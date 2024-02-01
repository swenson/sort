# Copyright (c) 2010-2024 Christopher Swenson.
# Copyright (c) 2012 Google Inc. All Rights Reserved.

CC ?= gcc
CFLAGS ?= -O3 -g -Wall -std=c89 -pedantic -Wno-long-long -Wno-format
EXTRA = -DSET_SORT_EXTRA

default: benchmark demo multidemo stresstest test benchmark_extra demo_extra multidemo_extra stresstest_extra

.PHONY: default clean test format

test: stresstest_extra benchmark_extra
	./benchmark_extra | tee benchmark.txt
	./stresstest_extra

clean:
	rm -f demo multidemo stresstest benchmark demo_extra multidemo_extra stresstest_extra benchmark_extra

demo: demo.c sort.h
	$(CC) $(CFLAGS) demo.c -o $@

demo_extra: demo.c sort.h sort_extra.h
	$(CC) $(CFLAGS) demo.c -o $@ $(EXTRA)

multidemo: multidemo.c sort.h
	$(CC) $(CFLAGS) multidemo.c -o $@

multidemo_extra: multidemo.c sort.h sort_extra.h
	$(CC) $(CFLAGS) multidemo.c -o $@ $(EXTRA)

stresstest: stresstest.c sort.h
	$(CC) $(CFLAGS) stresstest.c -o $@

stresstest_extra: stresstest.c sort.h sort_extra.h
	$(CC) $(CFLAGS) stresstest.c -o $@ $(EXTRA)

benchmark: benchmark.c sort.h
	$(CC) $(CFLAGS) benchmark.c -o $@

benchmark_extra: benchmark.c sort.h sort_extra.h
	$(CC) $(CFLAGS) benchmark.c -o $@ $(EXTRA)

format:
	astyle --options=astyle.options sort.h sort_extra.h demo.c multidemo.c stresstest.c benchmark.c