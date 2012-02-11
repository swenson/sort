#include <sys/time.h>

#define SORT_NAME sorter
#define SORT_TYPE int64_t
#define SORT_CMP(x, y) (x - y)
#include "sort.h"

/* Used to control the stress test */
#define SEED 123
#define MAXSIZE 25600
#define TESTS 1000

/* helper functions */
void verify(int64_t *dst, const int size)
{
  int i;
  for (i = 1; i < size; i++)
  {
    if (dst[i - 1] > dst[i])
    {
      printf("Verify failed! at %d\n", i);
      //for (i = i - 2; i < size; i++)
      //  printf(" %lld", dst[i]);
      //printf("\n");
      break;
    }
  }
}

static void fill(int64_t *dst, const int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    dst[i] = lrand48();
  }
}

/* used for stdlib */
static inline int simple_cmp(const void *a, const void *b)
{
  const int64_t da = *((const int64_t *) a);
  const int64_t db = *((const int64_t *) b);
  return (da < db) ? -1 : (da == db) ? 0 : 1;
}


void run_tests(void)
{
  printf("Running tests\n");
  int test;
  srand48(SEED);

  printf("stdlib qsort\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    qsort(dst, size, sizeof(int64_t), simple_cmp);
    verify(dst, size);
  }
#ifndef __linux__
  printf("stdlib heapsort\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    heapsort(dst, size, sizeof(int64_t), simple_cmp);
    verify(dst, size);
  }
  printf("stdlib mergesort\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    mergesort(dst, size, sizeof(int64_t), simple_cmp);
    verify(dst, size);
  }
#endif
  printf("quick sort\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    sorter_quick_sort(dst, size);
    verify(dst, size);
  } 
    
  if (MAXSIZE < 10000)
  {
    printf("bubble sort\n");
    for (test = 0; test < TESTS; test++)
    {
      int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
      int64_t dst[size];
      fill(dst, size);
      sorter_bubble_sort(dst, size);
      verify(dst, size);
    }
    printf("binary insertion sort\n");
    for (test = 0; test < TESTS; test++)
    {
      int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
      int64_t dst[size];
      fill(dst, size);
      sorter_binary_insertion_sort(dst, size);
      verify(dst, size);
    }
  }
      
  printf("merge sort\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    sorter_merge_sort(dst, size);
    verify(dst, size);
  }
    
   
  printf("heap sort\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    sorter_heap_sort(dst, size);
    verify(dst, size);
  }
    
  printf("shell sort time\n");
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    sorter_shell_sort(dst, size);
    verify(dst, size);
  }
    
  printf("tim sort\n"); 
  for (test = 0; test < TESTS; test++)
  {
    int64_t size = (lrand48() % (MAXSIZE - 1)) + 1;
    int64_t dst[size];
    fill(dst, size);
    sorter_tim_sort(dst, size);
    verify(dst, size);
  }
}

int main(void)
{
  run_tests();
  return 0;
}
