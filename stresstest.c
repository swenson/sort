#include <sys/time.h>

#define SORT_NAME sorter
#define SORT_TYPE int64_t
#define SORT_CMP(x, y) (x - y)
#include "sort.h"

/* Used to control the stress test */
#define SEED 123
#define MAXSIZE 25600
#define TESTS 1000

#define RAND_RANGE(__n, __min, __max) \
    (__n) = (__min) + (long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / (RAND_MAX + 1.0))) 

enum {
	FILL_RANDOM,
	FILL_SORTED,
	FILL_SORTED_10,
	FILL_SORTED_100,
	FILL_SORTED_10000,
	FILL_SWAPPED_N2,
	FILL_SWAPPED_N8,
	FILL_LAST_ELEMENT
};

char *test_names[FILL_LAST_ELEMENT] = {
	"random numbers",
	"sorted numbers",
	"sorted blocks of length 10",
	"sorted blocks of length 100",
	"sorted blocks of length 10000",
	"swapped size/2 pairs",
	"swapped size/8 pairs"
};

/* used for stdlib */
static inline int simple_cmp(const void *a, const void *b)
{
  const int64_t da = *((const int64_t *) a);
  const int64_t db = *((const int64_t *) b);
  return (da < db) ? -1 : (da == db) ? 0 : 1;
}

static inline double utime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (1000000.0 * t.tv_sec + t.tv_usec);
}

/* helper functions */
int verify(int64_t *dst, const int size)
{
  int i;
  for (i = 1; i < size; i++)
  {
    if (dst[i - 1] > dst[i])
    {
      printf("Verify failed! at %d", i);
      //for (i = i - 2; i < size; i++)
      //  printf(" %lld", dst[i]);
      //printf("\n");
      return 0;
    }
  }
  return 1;
}

static void fill_random(int64_t *dst, const int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    dst[i] = lrand48();
  }
}

static void fill_sorted(int64_t *dst, const int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    dst[i] = i;
  }
}

static void fill_sorted_blocks(int64_t *dst, const int size, const int block_size)
{
  int i, filled, this_block_size;
  filled = 0;
  for (i = 0; i < size; i += block_size)
  {
	this_block_size = (filled + block_size) < size ? block_size : (size - filled);
	fill_random(dst + filled, this_block_size);
    qsort(dst + filled, this_block_size, sizeof(int64_t), simple_cmp);
	filled += this_block_size;
  }
}

static void fill_swapped(int64_t *dst, const int size, const swapped_cnt)
{
  int i, tmp;
  size_t ind1, ind2;

  fill_sorted(dst, size);

  for (i = 0; i < swapped_cnt; i++)
  {
    ind1 = lrand48();
	RAND_RANGE(ind1, 0, size);
    ind2 = lrand48();
	RAND_RANGE(ind2, 0, size);

	tmp = dst[ind1];
	dst[ind1] = dst[ind2];
	dst[ind2] = tmp;
  }
}

static void fill(int64_t *dst, const int size, int type)
{
	switch (type) {
		case FILL_SORTED:
			fill_sorted(dst, size);
			break;
		case FILL_SORTED_10:
			fill_sorted_blocks(dst, size, 10);
			break;
		case FILL_SORTED_100:
			fill_sorted_blocks(dst, size, 100);
			break;
		case FILL_SORTED_10000:
			fill_sorted_blocks(dst, size, 10000);
			break;
		case FILL_SWAPPED_N2:
			fill_swapped(dst, size, size/2);
			break;
		case FILL_SWAPPED_N8:
			fill_swapped(dst, size, size/8);
		case FILL_RANDOM:
		default:
			fill_random(dst, size);
			break;
	}
}

void run_tests(int64_t *sizes, int sizes_cnt, int type)
{
  int test, res;
  double usec1, usec2, diff;

  printf("-------\nRunning tests with %s:\n-------\n", test_names[type]);

  res = 0;
  diff = 0;
  printf("%-20s", "stdlib qsort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    qsort(dst, size, sizeof(int64_t), simple_cmp);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);

#ifndef __linux__
  res = 0;
  diff = 0;
  printf("%-20s", "stdlib heapsort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    heapsort(dst, size, sizeof(int64_t), simple_cmp);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);

  res = 0;
  diff = 0;
  printf("%-20s", "stdlib mergesort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    mergesort(dst, size, sizeof(int64_t), simple_cmp);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);

#endif
  res = 0;
  diff = 0;
  printf("%-20s", "quick sort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    sorter_quick_sort(dst, size);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  } 
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);
    
  if (MAXSIZE < 10000)
  {
	  res = 0;
	  diff = 0;
    printf("%-20s", "bubble sort");
    for (test = 0; test < sizes_cnt; test++)
    {
      int64_t size = sizes[test];
      int64_t dst[size];
      fill(dst, size, type);
	  usec1 = utime();
      sorter_bubble_sort(dst, size);
	  usec2 = utime();
      verify(dst, size);
	if (!res) break;
	  diff += usec2 - usec1;
    }
	printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);

	res = 0;
	diff = 0;
    printf("%-20s", "binary insertion sort\n");
    for (test = 0; test < sizes_cnt; test++)
    {
      int64_t size = sizes[test];
      int64_t dst[size];
      fill(dst, size, type);
	  usec1 = utime();
      sorter_binary_insertion_sort(dst, size);
	  usec2 = utime();
      verify(dst, size);
	if (!res) break;
	  diff += usec2 - usec1;
    }
	printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);
  }
      
  res = 0;
  diff = 0;
  printf("%-20s", "merge sort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    sorter_merge_sort(dst, size);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);
    
   
  res = 0;
  diff = 0;
  printf("%-20s", "heap sort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    sorter_heap_sort(dst, size);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);
    
  res = 0;
  diff = 0;
  printf("%-20s", "shell sort time");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    sorter_shell_sort(dst, size);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);
    
  res = 0;
  diff = 0;
  printf("%-20s", "tim sort"); 
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    sorter_tim_sort(dst, size);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);

  res = 0;
  diff = 0;
  printf("%-20s", "in-place merge sort");
  for (test = 0; test < sizes_cnt; test++)
  {
    int64_t size = sizes[test];
    int64_t dst[size];
    fill(dst, size, type);
	usec1 = utime();
    sorter_merge_sort_in_place(dst, size);
	usec2 = utime();
    res = verify(dst, size);
	if (!res) break;
	diff += usec2 - usec1;
  }
  printf(" - %s, %10.1f usec\n", res ? "ok" : "FAILED", diff);
}

int main(void)
{
	int i = 0;
	int64_t sizes[TESTS];

	srand48(SEED);

	fill_random(sizes, TESTS);
	for (i = 0; i < TESTS; i++) {
		RAND_RANGE(sizes[i], 0, MAXSIZE);
	}

	for (i = 0; i < FILL_LAST_ELEMENT; i++) {
		run_tests(sizes, TESTS, i);
	}
	return 0;
}
