#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SORT_NAME sorter
#define SORT_CONCAT(x, y) x ## _ ## y
#define SORT_MAKE_STR1(x, y) SORT_CONCAT(x,y)
#define SORT_MAKE_STR(x) SORT_MAKE_STR1(SORT_NAME,x)

static int swaps = 0;
static int cmps = 0;
#define SWAP(x,y) ({swaps++; typeof((x)) _t = (x); (x) = (y); (y) = _t;})
#define CMP(x,y) ((cmps++ - cmps + (x) - (y) ))

const uint64_t shell_gaps[48] = {1, 4, 10, 23, 57, 132, 301, 701, 1750, 4376, 10941, 27353, 68383, 170958, 427396, 1068491, 2671228, 6678071, 16695178, 41737946, 104344866, 260862166, 652155416, 1630388541, 4075971353, 10189928383, 25474820958, 63687052396, 159217630991, 398044077478, 995110193696, 2487775484241, 6219438710603, 15548596776508, 38871491941271, 97178729853178, 242946824632946, 607367061582366, 1518417653955916, 3796044134889791, 9490110337224478, 23725275843061196, 59313189607652991, 148282974019132478, 370707435047831196, 926768587619577991, 2316921469048944978, 5792303672622362446};


static void reset()
{
  swaps = 0;
  cmps = 0;
}

void shell_sort(int *dst, int64_t size)
{
  // TODO: binary search to find first gap?
  int inci = 47;
  int64_t inc = shell_gaps[inci];
  while (inc > (size >> 1))
  {
    inc = shell_gaps[--inci];
  }
  int i;
  while (1)
  {
    for (i = inc; i < size; i++)
    {
      typeof(dst[0]) temp = dst[i];
      int j = i;
      while ((j >= inc) && (CMP(dst[j - inc], temp) > 0))
      {
        dst[j] = dst[j - inc];
        swaps++;
        j -= inc;
      }
      dst[j] = temp;
    }
    if (inc == 1) break;
    inc = shell_gaps[--inci];
  }
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

/* quick sort: based on wikipedia */
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

/* heap sort: based on wikipedia */
void heap_sift_down(int *dst, int start, int end)
{
  int root = start;
  
  while ((root << 1) <= end)
  {
    int child = root << 1;
    if ((child < end) && (CMP(dst[child], dst[child + 1]) < 0))
      child++;
    if (CMP(dst[root], dst[child]) < 0)
    {
      SWAP(dst[root], dst[child]);
      root = child;
    }
    else
      return;
  }
}

void heapify(int *dst, int size)
{
  int start = size >> 1;
  while (start >= 0)
  {
    heap_sift_down(dst, start, size - 1);
    start--;
  }
}

void heap_sort(int *dst, int size)
{
  heapify(dst, size);
  int end = size - 1;
  
  while (end > 0)
  {
    SWAP(dst[end], dst[0]);
    heap_sift_down(dst, 0, end - 1);
    end--;
  }
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

  reset();
  printf("\nHeap sort\n");
  memcpy(dst, arr, sizeof(int) * size);
  heap_sort(dst, size);
  verify(dst, size);
  printf("heapsort; swaps %d, compares %d\n", swaps, cmps);
  
  reset();
  printf("\nShell sort\n");
  memcpy(dst, arr, sizeof(int) * size);
  shell_sort(dst, size);
  verify(dst, size);
  printf("shellsort; swaps %d, compares %d\n", swaps, cmps);
  
}

int main(void)
{
  run_tests();
}