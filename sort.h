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
static inline void binary_insertion_sort_start(int *dst, int start, int size)
{
  int i;
  for (i = start; i < size; i++)
  {
    int j;
    if (CMP(dst[i - 1], dst[i]) <= 0) continue;
    typeof(dst[0]) x = dst[i];
    int location = binary_insertion_find(dst, x, i);
    for (j = i - 1; j >= location; j--)
    {
      dst[j + 1] = dst[j];
      swaps++;
    }
    dst[location] = x;
    swaps += i - location;
  }
}

void binary_insertion_sort(int *dst, int size)
{
  binary_insertion_sort_start(dst, 1, size);
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
    binary_insertion_sort(&dst[left], right - left + 1);
    return;
  }
  int pivot = left + ((right - left) >> 1);
  int new_pivot = quick_sort_partition(dst, left, right, pivot);
  quick_sort_recursive(dst, left, new_pivot - 1);
  quick_sort_recursive(dst, new_pivot + 1, right);
}


void quick_sort(int *dst, int size)
{
  quick_sort_recursive(dst, 0, size - 1);
}

static inline void reverse_elements(int *dst, int start, int end)
{
  while (1)
  {
    if (start >= end) return;
    SWAP(dst[start], dst[end]);
    start++;
    end--;
  }
}

static inline int count_run(int *dst, int start, int size)
{
  if (start >= size - 2)
  {
    if (CMP(dst[size - 2], dst[size - 1]) > 0)
      SWAP(dst[size - 2], dst[size - 1]);
    return 2;
  }
  
  int curr = start + 2;
  
  if (CMP(dst[start], dst[start + 1]) <= 0)
  {
    // increasing run
    while (1)
    {
      if (curr == size - 1) break;
      if (CMP(dst[curr - 1], dst[curr]) > 0) break;
      curr++;
    }
    return curr - start;    
  }
  else
  {
    // decreasing run
    while (1)
    {
      if (curr == size - 1) break;
      if (CMP(dst[curr - 1], dst[curr]) <= 0) break;
      curr++;
    }
    // reverse in-place
    reverse_elements(dst, start, curr - 1);
    return curr - start;
  }
}

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif
#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif

static inline int compute_minrun(uint64_t size)
{
  int top_bit = 64 - __builtin_clzll(size);
  int shift = MAX(top_bit, 6) - 6;
  int minrun = size >> shift;
  uint64_t mask = (1ULL << shift) - 1;
  if (mask & size) minrun++;
  return minrun;
}


typedef struct {
  int start;
  int length;  
} tim_sort_run_t;
  
#define PUSH_NEXT() do {\
len = count_run(dst, curr, size);\
run = minrun;\
if (run < minrun) run = minrun;\
if (run > size - curr) run = size - curr;\
if (run > len)\
{\
  binary_insertion_sort_start(&dst[curr], len, run);\
  len = run;\
}\
run_stack[stack_curr++] = (tim_sort_run_t) {curr, len};\
curr += len;\
if (curr == size)\
{\
  /* finish up */ \
  while (stack_curr > 1) \
  { \
    tim_sort_merge(dst, run_stack, stack_curr, store); \
    stack_curr--; \
  } \
  return;\
}\
}\
while (0)
  
static inline int check_invariant(tim_sort_run_t *stack, int stack_curr)
{
  if (stack_curr < 2) return 1;
  if (stack_curr == 2)
  {
    int A = stack[stack_curr - 2].length;
    int B = stack[stack_curr - 1].length;
    if (A <= B) return 0;
    return 1;
  }
  int A = stack[stack_curr - 3].length;
  int B = stack[stack_curr - 2].length;
  int C = stack[stack_curr - 1].length;
  if ((A <= B + C) || (B <= C)) return 0;
  return 1;
}

typedef struct {
  size_t alloc;
  int *storage;
} temp_storage_t;

static inline tim_sort_resize(temp_storage_t *store, size_t new_size)
{
  if (store->alloc < new_size)
  {
    int *tempstore = realloc(store->storage, new_size * sizeof(store->storage[0]));
    if (tempstore == NULL)
    {
      fprintf(stderr, "Error allocating temporary storage for tim sort: need %lu bytes", sizeof(store->storage[0]) * new_size);
      exit(1);
    }
    store->storage = tempstore;
    store->alloc = new_size;
  }
}

static inline void tim_sort_merge(int *dst, tim_sort_run_t *stack, int stack_curr, temp_storage_t *store)
{
  int A = stack[stack_curr - 2].length;
  int B = stack[stack_curr - 1].length;
  int curr = stack[stack_curr - 2].start;

  tim_sort_resize(store, MIN(A, B));
  int *storage = store->storage;
  
  int i, j, k;
  
  if (A < B)
  {
    memcpy(storage, &dst[curr], A * sizeof(dst[0]));
    swaps += A;
    i = 0;
    j = curr + A;
    
    for (k = curr; k < curr + A + B; k++)
    {
      if ((i < A) && (j < curr + A + B))
      {
        if (CMP(storage[i], dst[j]) <= 0)
          dst[k] = storage[i++];
        else
          dst[k] = dst[j++];          
      }
      else if (i < A)
      {
        dst[k] = storage[i++];
      }
      else
        dst[k] = dst[j++];
      swaps++;
    }
  }
  else
  {    
    memcpy(storage, &dst[curr + A], B * sizeof(dst[0]));
    swaps += B;
    i = 0;
    j = curr;
    
    for (k = curr; k < curr + A + B; k++)
    {
      if ((i < B) && (j < curr + A))
      {
          if (CMP(dst[j], storage[i]) <= 0)
            dst[k] = dst[j++];
          else
            dst[k] = storage[i++];          
      }
      else if (i < B)
        dst[k] = storage[i++];
      else
        dst[k] = dst[j++];
      swaps++;
    }
  }
}

static inline int tim_sort_collapse(int *dst, tim_sort_run_t *stack, int stack_curr, temp_storage_t *store, size_t size)
{
  while (1)
  {
    // if the stack only has one thing on it, we are done with the collapse
    if (stack_curr <= 1) break;
    // if this is the last merge, just do it
    if ((stack_curr == 2) && (stack[0].length + stack[1].length == size))
    {
      tim_sort_merge(dst, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    }
    // check if the invariant is off for a stack of 2 elements
    else if ((stack_curr == 2) && (stack[0].length <= stack[1].length))
    {
      tim_sort_merge(dst, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    }
    else if (stack_curr == 2)
      break;
      
    int A = stack[stack_curr - 3].length;
    int B = stack[stack_curr - 2].length;
    int C = stack[stack_curr - 1].length;
    
    // check first invariant
    if (A <= B + C)
    {
      if (A < C)
      {
        tim_sort_merge(dst, stack, stack_curr - 1, store);
        stack[stack_curr - 3].length += stack[stack_curr - 2].length;
        stack[stack_curr - 2] = stack[stack_curr - 1];
        stack_curr--;
        
      }
      else
      {
        tim_sort_merge(dst, stack, stack_curr, store);
        stack[stack_curr - 2].length += stack[stack_curr - 1].length;
        stack_curr--;
      }
    }
    // check second invariant
    else if (B <= C)
    {
      tim_sort_merge(dst, stack, stack_curr, store);
      stack[stack_curr - 2].length += stack[stack_curr - 1].length;
      stack_curr--;      
    }
    else
      break;
  }
  return stack_curr;
}

void tim_sort(int *dst, int size)
{
  if (size < 64)
  {
    binary_insertion_sort(dst, size);
    return;
  }
  
  // compute the minimum run length
  size_t minrun = compute_minrun(size);
  
  // temporary storage for merges
  temp_storage_t _store, *store = &_store;
  store->alloc = 0;
  store->storage = NULL;
  
  tim_sort_run_t run_stack[128];
  int stack_curr = 0;
  int len, run;
  size_t curr = 0;
  
  PUSH_NEXT();
  PUSH_NEXT();
  PUSH_NEXT();
  
  while (1)
  {
    if (!check_invariant(run_stack, stack_curr))
    {
      stack_curr = tim_sort_collapse(dst, run_stack, stack_curr, store, size);
      continue;
    }
    PUSH_NEXT();
  }
}

#undef PUSH_NEXT

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
      //for (i = 0; i < size; i++)
      //  printf(" %d", dst[i]);
      //printf("\n");
      break;
    }
  }
}

static inline double utime()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return (1000000.0 * t.tv_sec + t.tv_usec);
  
}

static void fill(int *arr, int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    arr[i] = lrand48();
  }
}

void run_tests(void)
{
  
  printf("Running tests\n");
  int i;
  srand48(123);
  int size = 5000;
  int runs = 10;
  int arr[size];
  int dst[size];
  double start_time;
  double end_time;
  double total_time;
  
  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();  
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    quick_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }
  printf("quick sort time: %.2f us per iteration\n", total_time / runs);
  

  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();  
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    bubble_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }
  printf("bubble sort time: %.2f us per iteration\n", total_time / runs);
  
  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();  
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    merge_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }
  printf("merge sort time: %.2f us per iteration\n", total_time / runs);
  
  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    binary_insertion_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }
  printf("binary insertion sort time: %.2f us per iteration\n", total_time / runs);
  
  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    heap_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }
  printf("heap sort time: %.2f us per iteration\n", total_time / runs);
  
  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    shell_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }
  printf("shell sort time: %.2f us per iteration\n", total_time / runs);
  
  srand48(123);  
  total_time = 0.0;
  for (i = 0; i < runs; i++)
  {
    reset();
    fill(arr, size);
    memcpy(dst, arr, sizeof(int) * size);
    start_time = utime();
    tim_sort(dst, size);
    end_time = utime();
    total_time += end_time - start_time;
    verify(dst, size);
  }  
  printf("tim sort time: %.2f us per iteration\n", total_time / runs);
}

int main(void)
{
  run_tests();
}