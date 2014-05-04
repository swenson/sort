/* Copyright (c) 2010-2014 Christopher Swenson. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#define _XOPEN_SOURCE
#include <sys/time.h>

/* sort #1 */

#define SORT_NAME sorter
#define SORT_TYPE int64_t
/* You can redefine the comparison operator.
   The default is
#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
   but the one below is often faster for integer types.
*/
#define SORT_CMP(x, y) (x - y)
#include "sort.h"

/*
   We now have the following functions defined
   * sorter_shell_sort
   * sorter_binary_insertion_sort
   * sorter_heap_sort
   * sorter_quick_sort
   * sorter_merge_sort
   * sorter_selection_sort
   * sorter_tim_sort

   Each takes two arguments: int64_t *array, size_t size

*/

/* sort #2 */

#define SORT_NAME sorter2
#define SORT_TYPE int64_t
/* You can redefine the comparison operator.
   The default is
#define SORT_CMP(x, y)  ((x) > (y) ? -1 : ((x) == (y) ? 0 : 1))
   but the one below is often faster for integer types.
*/
#define SORT_CMP(x, y) (y - x)
#include "sort.h"

/*
   We now have the following functions defined
   * sorter2_shell_sort
   * sorter2_binary_insertion_sort
   * sorter2_heap_sort
   * sorter2_quick_sort
   * sorter2_merge_sort
   * sorter2_selection_sort
   * sorter2_tim_sort

   Each takes two arguments: int64_t *array, size_t size

*/


/* Used to control the demo */
#define SEED 123
#define SIZE 10000
#define RUNS 1

/* helper functions */
void verify(int64_t *dst, const int size) {
  int i;

  for (i = 1; i < size; i++) {
    if (dst[i - 1] > dst[i]) {
      printf("Verify failed! at %d\n", i);

      for (i = i - 2; i < SIZE; i++) {
        printf(" %lld", (long long int)dst[i]);
      }

      printf("\n");
      break;
    }
  }
}

void verify2(int64_t *dst, const int size) {
  int i;

  for (i = 1; i < size; i++) {
    if (dst[i - 1] < dst[i]) {
      printf("Verify failed! at %d\n", i);

      for (i = i - 2; i < SIZE; i++) {
        printf(" %lld", (long long int)dst[i]);
      }

      printf("\n");
      break;
    }
  }
}

static __inline double utime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (1000000.0 * t.tv_sec + t.tv_usec);
}

static void fill(int64_t *arr, const int size) {
  int i;

  for (i = 0; i < size; i++) {
    arr[i] = lrand48();
  }
}

/* used for stdlib */
static __inline int simple_cmp(const void *a, const void *b) {
  const int64_t da = *((const int64_t *) a);
  const int64_t db = *((const int64_t *) b);
  return (da < db) ? -1 : (da == db) ? 0 : 1;
}

static __inline int simple_cmp2(const void *a, const void *b) {
  const int64_t da = *((const int64_t *) a);
  const int64_t db = *((const int64_t *) b);
  return (da > db) ? -1 : (da == db) ? 0 : 1;
}

void run_tests(void) {
  int i;
  int64_t arr[SIZE];
  int64_t dst[SIZE];
  double start_time;
  double end_time;
  double total_time;
  printf("Running tests\n");
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    qsort(dst, SIZE, sizeof(int64_t), simple_cmp);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("stdlib qsort time: %.2f us per iteration\n", total_time / RUNS);
#ifndef __linux__
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    heapsort(dst, SIZE, sizeof(int64_t), simple_cmp);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("stdlib heapsort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    mergesort(dst, SIZE, sizeof(int64_t), simple_cmp);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("stdlib mergesort time: %.2f us per iteration\n", total_time / RUNS);
#endif
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_quick_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("quick sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_selection_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("selection sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_merge_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("merge sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_binary_insertion_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("binary insertion sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_heap_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("heap sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_shell_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("shell sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter_tim_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, SIZE);
  }

  printf("tim sort time: %.2f us per iteration\n", total_time / RUNS);
}

void run_tests2(void) {
  int i;
  int64_t arr[SIZE];
  int64_t dst[SIZE];
  double start_time;
  double end_time;
  double total_time;
  printf("Running tests - 2\n");
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    qsort(dst, SIZE, sizeof(int64_t), simple_cmp2);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("stdlib qsort time: %.2f us per iteration\n", total_time / RUNS);
#ifndef __linux__
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    heapsort(dst, SIZE, sizeof(int64_t), simple_cmp2);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("stdlib heapsort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    mergesort(dst, SIZE, sizeof(int64_t), simple_cmp2);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("stdlib mergesort time: %.2f us per iteration\n", total_time / RUNS);
#endif
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_quick_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("quick sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_selection_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("selection sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_merge_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("merge sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_binary_insertion_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("binary insertion sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_heap_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("heap sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_shell_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("shell sort time: %.2f us per iteration\n", total_time / RUNS);
  srand48(SEED);
  total_time = 0.0;

  for (i = 0; i < RUNS; i++) {
    fill(arr, SIZE);
    memcpy(dst, arr, sizeof(int64_t) * SIZE);
    start_time = utime();
    sorter2_tim_sort(dst, SIZE);
    end_time = utime();
    total_time += end_time - start_time;
    verify2(dst, SIZE);
  }

  printf("tim sort time: %.2f us per iteration\n", total_time / RUNS);
}
int main(void) {
  run_tests();
  run_tests2();
  return 0;
}
