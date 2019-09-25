/* Copyright (c) 2010-2014 Christopher Swenson. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#define _XOPEN_SOURCE
#include <sys/time.h>
#include <ctype.h>
#include <stdlib.h>

#define SORT_NAME sorter
#define SORT_TYPE int64_t
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#define SORT_CMP(x, y) ((x) - (y))
#define SORT_CSWAP(x, y) {SORT_TYPE _sort_swap_temp = MAX((x), (y)); (x) = MIN((x),(y)); (y) = _sort_swap_temp;}
#include "sort.h"

/* Used to control the stress test */
#define SEED 123
#define FAST_ITERATIONS 1
#define SLOW_ITERATIONS 1
#define SIZES 1

size_t sizes[SIZES] = {100000};

/* used for stdlib */
static __inline int simple_cmp(const void *a, const void *b) {
  const int64_t da = *((const int64_t *) a);
  const int64_t db = *((const int64_t *) b);
  return (da < db) ? -1 : (da == db) ? 0 : 1;
}

static __inline double utime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (1000000.0 * t.tv_sec + t.tv_usec);
}

static void fill_random(int64_t *dst, const int size) {
  int i;
  srand48(SEED);

  for (i = 0; i < size; i++) {
    dst[i] = lrand48();
  }
}

void capitalize(const char *word, char *new_word) {
  int len;
  len = strlen(word);

  if (len < 1) {
    return;
  }

  strcpy(new_word, word);
  new_word[0] = toupper(new_word[0]);
}

int platform_bits() {
#if defined (__amd64__) || defined (__x86_64__)

  if (1) { /* avoid two returns in a row */
    return 64;
  }

#endif
  /* backup case */
  return sizeof(void *) * 8;
}

void platform_name(char *output) {
  char *name;
#if defined (__amd64__) || defined (__x86_64__)
  name = "x86";
#elif defined (__arm__)
  name = "arm";
#else
  name = "unknown";
#endif
  sprintf(output, "%s_%d", name, platform_bits());
}

#define TEST_STDLIB(name) do { \
  capitalize(#name, capital_word); \
  for (test = 0; test < SIZES; test++) { \
    int64_t size = sizes[test]; \
    int64_t *dst = (int64_t *) malloc(sizeof(int64_t) * size); \
    diff = 0; \
    iter = 0; \
    while (1) { \
      fill_random(dst, size); \
      usec1 = utime(); \
      name (dst, size, sizeof(int64_t), simple_cmp); \
      usec2 = utime(); \
      diff += usec2 - usec1; \
      iter++; \
      if (diff >= 1000000.0) { \
        break; \
      } \
    } \
    free(dst); \
    char name_buf[128]; \
    sprintf(name_buf, "%s %lld %s", capital_word, size, platform); \
    printf("%-40s %4d %16.1f ns/op\n", name_buf, iter, diff * 1000.0 / (double) iter); \
  } \
} while (0)

#define TEST_SORT_H(name) do { \
  capitalize(#name, capital_word); \
  for (test = 0; test < SIZES; test++) { \
    int64_t size = sizes[test]; \
    int64_t *dst = (int64_t *) malloc(sizeof(int64_t) * size); \
    diff = 0; \
    iter = 0; \
    while (1) { \
      fill_random(dst, size); \
      usec1 = utime(); \
      sorter_ ## name (dst, size); \
      usec2 = utime(); \
      diff += usec2 - usec1; \
      iter++; \
      if (diff >= 1000000.0) { \
        break; \
      } \
    } \
    free(dst); \
    char name_buf[128]; \
    sprintf(name_buf, "%s %lld %s", capital_word, size, platform); \
    printf("%-40s %4d %16.1f ns/op\n", name_buf, iter, diff * 1000.0 / (double) iter); \
  } \
} while (0)


int main(void) {
  int test, iter;
  double usec1, usec2, diff;
  char capital_word[128];
  char platform[128];
  platform_name(platform);
  TEST_STDLIB(qsort);
#if !defined(__linux__) && !defined(__CYGWIN__)
  TEST_STDLIB(heapsort);
  TEST_STDLIB(mergesort);
#endif
  TEST_SORT_H(binary_insertion_sort);
  TEST_SORT_H(bitonic_sort);
  TEST_SORT_H(quick_sort);
  TEST_SORT_H(merge_sort);
  TEST_SORT_H(heap_sort);
  TEST_SORT_H(shell_sort);
  TEST_SORT_H(tim_sort);
  TEST_SORT_H(merge_sort_in_place);
  TEST_SORT_H(grail_sort);
  TEST_SORT_H(sqrt_sort);
  TEST_SORT_H(rec_stable_sort);
  TEST_SORT_H(grail_sort_dyn_buffer);
  return 0;
}
