#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SORT_NAME sorter
#define SORT_CONCAT(x, y) x ## _ ## y
#define SORT_MAKE_STR1(x, y) SORT_CONCAT(x,y)
#define SORT_MAKE_STR(x) SORT_MAKE_STR1(SORT_NAME,x)

static int swaps = 0;
static int cmps = 0;
#define SWAP(x,y) ({swaps++; typeof((x)) _t = (x); (x) = (y); (y) = _t;})
#define CMP(x,y) ((cmps++ - cmps + (x) - (y) ))

static void reset()
{
  swaps = 0;
  cmps = 0;
}

static inline int binary_insertion_find(int *dst, int x, int size)
{
  int l, c, r;
  l = 0;
  r = size - 1;
  c = r >> 1;
  typeof(dst[0]) lx, cx, rx;
  lx = dst[l];
  
  /* check for beginning conditions */
  if (CMP(x, lx) < 0)
    return 0;
  else if (CMP(x, lx) == 0)
  {
    int i = 1;
    while (CMP(x, dst[i]) == 0) i++;
    return i;
  }
  
  rx = dst[r];
  // guaranteed not to be >= rx
  cx = dst[c];
  while (1)
  {
    int val = CMP(x, cx);
    if (val < 0)
    {
      if (c - l <= 1) return c;
      r = c;
      rx = cx;
    }
    else if (val > 0)
    {
      if (r - c <= 1) return c + 1;
      l = c;
      lx = cx;
    }
    else
    {
      while (cx == x)
      {
        cx = dst[++c];
      }
      return c;
    }
    c = l + ((r - l) >> 1);
    cx = dst[c];
  }
}

void binary_insertion_sort(int *dst, int size)
{
  int i;
  for (i = 1; i < size; i++)
  {
    int j;
    //for (j = 0; j < i; j++)
    //  printf("%d ", dst[j]);
    
    //printf("\nFinding position for %d\n", dst[i]);
    if (CMP(dst[i - 1], dst[i]) <= 0) continue;
    typeof(dst[0]) x = dst[i];
    int location = binary_insertion_find(dst, x, i);
    //printf("Put at position %d\n", location);
    //memmove(&dst[location], &dst[location + 1], i - location);
    for (j = i - 1; j >= location; j--)
    {
      dst[j + 1] = dst[j];
      swaps++;
    }
    dst[location] = x;
    swaps += i - location;
  }
}


void bubble_sort(int *dst, int size)
{
  int i;
  int j;
  for (i = 0; i < size; i++)
  {
    for (j = i + 1; j < size; j++)
    {
      if (CMP(dst[j], dst[i]) < 0)
        SWAP(dst[i], dst[j]);
    }
  }
}

void merge_sort(int *dst, int size)
{
  if (size < 16)
  {
    bubble_sort(dst, size);
    return;
  }

  int middle = size / 2;
  
  merge_sort(dst, middle);
  merge_sort(&dst[middle], size - middle);
  
  int newdst[size];
  int out = 0;
  int i = 0;
  int j = middle;
  while (out != size)
  {
    swaps++;
    if ((i < middle) && ((j == size) || (CMP(dst[i], dst[j]) <= 0)))
    {
      newdst[out] = dst[i++];
    }
    else
    {
      newdst[out] = dst[j++];
    }
    out++;
  }
  memcpy(dst, newdst, size * sizeof(int));
}

int quick_sort_partition(int *dst, int left, int right, int pivot)
{
  typeof(dst[0]) value = dst[pivot];
  SWAP(dst[pivot], dst[right]);
  int index = left;
  int i;
  for (i = left; i < right; i++)
  {
    if (dst[i] <= value)
    {
      SWAP(dst[i], dst[index]);
      index++;
    }
  }
  SWAP(dst[right], dst[index]);
  return index;
}
void quick_sort_recursive(int *dst, int left, int right)
{
  if (right <= left) return;
  if ((right - left + 1) < 16)
  {
    bubble_sort(&dst[left], right - left + 1);
    return;
  }
  int pivot = left + (right - left) >> 1;
  int new_pivot = quick_sort_partition(dst, left, right, pivot);
  quick_sort_recursive(dst, left, new_pivot - 1);
  quick_sort_recursive(dst, new_pivot + 1, right);
}


void quick_sort(int *dst, int size)
{
  quick_sort_recursive(dst, 0, size - 1);
}

void tim_sort(int *dst, int size)
{
  
}

void heap_sort(int *dst, int size)
{
  
}

void shell_sort(int *dst, int size)
{
  
}
void verify(int *dst, int size)
{
  int i;
  for (i = 1; i < size; i++)
  {
    if (dst[i - 1] > dst[i])
    {
      printf("Verify failed! at %d\n", i);
      for (i = 0; i < size; i++)
        printf(" %d", dst[i]);
      printf("\n");
      break;
    }
  }
}
void run_tests(void)
{
  printf("Running tests\n");
  int i;
  srand48(123);
  int size = 32;
  int arr[size];
  for (i = 0; i < size; i++)
  {
    arr[i] = lrand48();
  }
  int dst[size];
  
  reset();  
  printf("\nQuick sort\n");
  memcpy(dst, arr, sizeof(int) * size);
  quick_sort(dst, size);
  verify(dst, size);
  printf("quicksort; swaps %d, compares %d\n", swaps, cmps);

  reset();  
  printf("\nBubble sort\n");
  memcpy(dst, arr, sizeof(int) * size);
  bubble_sort(dst, size);
  verify(dst, size);
  printf("bubblesort; swaps %d, compares %d\n", swaps, cmps);

  reset();  
  printf("\nMerge sort\n");
  memcpy(dst, arr, sizeof(int) * size);
  merge_sort(dst, size);
  verify(dst, size);
  printf("mergesort; swaps %d, compares %d\n", swaps, cmps);
  
  reset();
  printf("\nBinary insertion sort\n");
  memcpy(dst, arr, sizeof(int) * size);
  binary_insertion_sort(dst, size);
  verify(dst, size);
  printf("binaryinsertionsort; swaps %d, compares %d\n", swaps, cmps);
  
}

int main(void)
{
  run_tests();
}