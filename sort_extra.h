/* Copyright (c) 2010-2024 Christopher Swenson. */
/* Copyright (c) 2012 Vojtech Fried. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#define GRAIL_SWAP1                    SORT_MAKE_STR(grail_swap1)
#define REC_STABLE_SORT                SORT_MAKE_STR(rec_stable_sort)
#define GRAIL_REC_MERGE                SORT_MAKE_STR(grail_rec_merge)
#define GRAIL_SORT_DYN_BUFFER          SORT_MAKE_STR(grail_sort_dyn_buffer)
#define GRAIL_SORT_FIXED_BUFFER        SORT_MAKE_STR(grail_sort_fixed_buffer)
#define GRAIL_COMMON_SORT              SORT_MAKE_STR(grail_common_sort)
#define GRAIL_SORT                     SORT_MAKE_STR(grail_sort)
#define GRAIL_COMBINE_BLOCKS           SORT_MAKE_STR(grail_combine_blocks)
#define GRAIL_LAZY_STABLE_SORT         SORT_MAKE_STR(grail_lazy_stable_sort)
#define GRAIL_MERGE_WITHOUT_BUFFER     SORT_MAKE_STR(grail_merge_without_buffer)
#define GRAIL_ROTATE                   SORT_MAKE_STR(grail_rotate)
#define GRAIL_BIN_SEARCH_LEFT          SORT_MAKE_STR(grail_bin_search_left)
#define GRAIL_BUILD_BLOCKS             SORT_MAKE_STR(grail_build_blocks)
#define GRAIL_FIND_KEYS                SORT_MAKE_STR(grail_find_keys)
#define GRAIL_MERGE_BUFFERS_LEFT_WITH_X_BUF SORT_MAKE_STR(grail_merge_buffers_left_with_x_buf)
#define GRAIL_BIN_SEARCH_RIGHT         SORT_MAKE_STR(grail_bin_search_right)
#define GRAIL_MERGE_BUFFERS_LEFT       SORT_MAKE_STR(grail_merge_buffers_left)
#define GRAIL_SMART_MERGE_WITH_X_BUF   SORT_MAKE_STR(grail_smart_merge_with_x_buf)
#define GRAIL_MERGE_LEFT_WITH_X_BUF    SORT_MAKE_STR(grail_merge_left_with_x_buf)
#define GRAIL_SMART_MERGE_WITHOUT_BUFFER SORT_MAKE_STR(grail_smart_merge_without_buffer)
#define GRAIL_SMART_MERGE_WITH_BUFFER  SORT_MAKE_STR(grail_smart_merge_with_buffer)
#define GRAIL_MERGE_RIGHT              SORT_MAKE_STR(grail_merge_right)
#define GRAIL_MERGE_LEFT               SORT_MAKE_STR(grail_merge_left)
#define GRAIL_SWAP_N                   SORT_MAKE_STR(grail_swap_n)
#define SQRT_SORT                      SORT_MAKE_STR(sqrt_sort)
#define SQRT_SORT_BUILD_BLOCKS         SORT_MAKE_STR(sqrt_sort_build_blocks)
#define SQRT_SORT_MERGE_BUFFERS_LEFT_WITH_X_BUF SORT_MAKE_STR(sqrt_sort_merge_buffers_left_with_x_buf)
#define SQRT_SORT_MERGE_DOWN           SORT_MAKE_STR(sqrt_sort_merge_down)
#define SQRT_SORT_MERGE_LEFT_WITH_X_BUF SORT_MAKE_STR(sqrt_sort_merge_left_with_x_buf)
#define SQRT_SORT_MERGE_RIGHT          SORT_MAKE_STR(sqrt_sort_merge_right)
#define SQRT_SORT_SWAP_N               SORT_MAKE_STR(sqrt_sort_swap_n)
#define SQRT_SORT_SWAP_1               SORT_MAKE_STR(sqrt_sort_swap_1)
#define SQRT_SORT_SMART_MERGE_WITH_X_BUF SORT_MAKE_STR(sqrt_sort_smart_merge_with_x_buf)
#define SQRT_SORT_SORT_INS             SORT_MAKE_STR(sqrt_sort_sort_ins)
#define SQRT_SORT_COMBINE_BLOCKS       SORT_MAKE_STR(sqrt_sort_combine_blocks)
#define SQRT_SORT_COMMON_SORT          SORT_MAKE_STR(sqrt_sort_common_sort)
#define BUBBLE_SORT                    SORT_MAKE_STR(bubble_sort)
#define SELECTION_SORT                 SORT_MAKE_STR(selection_sort)

SORT_DEF void REC_STABLE_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void GRAIL_SORT_DYN_BUFFER(SORT_TYPE *dst, const size_t size);
SORT_DEF void GRAIL_SORT_FIXED_BUFFER(SORT_TYPE *dst, const size_t size);
SORT_DEF void GRAIL_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void SQRT_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void BUBBLE_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void SELECTION_SORT(SORT_TYPE *dst, const size_t size);

/********* Sqrt sorting *********************************/
/*                                                       */
/* (c) 2014 by Andrey Astrelin                           */
/*                                                       */
/*                                                       */
/* Stable sorting that works in O(N*log(N)) worst time   */
/* and uses O(sqrt(N)) extra memory                      */
/*                                                       */
/* Define SORT_TYPE and SORT_CMP                         */
/* and then call SqrtSort() function                     */
/*                                                       */
/*********************************************************/

#define SORT_CMP_A(a,b) SORT_CMP(*(a),*(b))

static __inline void SQRT_SORT_SWAP_1(SORT_TYPE *a, SORT_TYPE *b) {
  SORT_TYPE c = *a;
  *a++ = *b;
  *b++ = c;
}

static __inline void SQRT_SORT_SWAP_N(SORT_TYPE *a, SORT_TYPE *b, int n) {
  while (n--) {
    SQRT_SORT_SWAP_1(a++, b++);
  }
}


static void SQRT_SORT_MERGE_RIGHT(SORT_TYPE *arr, int L1, int L2, int M) {
  int p0 = L1 + L2 + M - 1, p2 = L1 + L2 - 1, p1 = L1 - 1;

  while (p1 >= 0) {
    if (p2 < L1 || SORT_CMP_A(arr + p1, arr + p2) > 0) {
      arr[p0--] = arr[p1--];
    } else {
      arr[p0--] = arr[p2--];
    }
  }

  if (p2 != p0) while (p2 >= L1) {
      arr[p0--] = arr[p2--];
    }
}

/* arr[M..-1] - free, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1] */
static void SQRT_SORT_MERGE_LEFT_WITH_X_BUF(SORT_TYPE *arr, int L1, int L2, int M) {
  int p0 = 0, p1 = L1;
  L2 += L1;

  while (p1 < L2) {
    if (p0 == L1 || SORT_CMP_A(arr + p0, arr + p1) > 0) {
      arr[M++] = arr[p1++];
    } else {
      arr[M++] = arr[p0++];
    }
  }

  if (M != p0) while (p0 < L1) {
      arr[M++] = arr[p0++];
    }
}

/* arr[0,L1-1] ++ arr2[0,L2-1] -> arr[-L1,L2-1],  arr2 is "before" arr1 */
static void SQRT_SORT_MERGE_DOWN(SORT_TYPE *arr, SORT_TYPE *arr2, int L1, int L2) {
  int p0 = 0, p1 = 0, M = -L2;

  while (p1 < L2) {
    if (p0 == L1 || SORT_CMP_A(arr + p0, arr2 + p1) >= 0) {
      arr[M++] = arr2[p1++];
    } else {
      arr[M++] = arr[p0++];
    }
  }

  if (M != p0) while (p0 < L1) {
      arr[M++] = arr[p0++];
    }
}

static void SQRT_SORT_SMART_MERGE_WITH_X_BUF(SORT_TYPE *arr, int *alen1, int *atype, int len2,
    int lkeys) {
  int p0 = -lkeys, p1 = 0, p2 = *alen1, q1 = p2, q2 = p2 + len2;
  int ftype = 1 - *atype; /* 1 if inverted */

  while (p1 < q1 && p2 < q2) {
    if (SORT_CMP_A(arr + p1, arr + p2) - ftype < 0) {
      arr[p0++] = arr[p1++];
    } else {
      arr[p0++] = arr[p2++];
    }
  }

  if (p1 < q1) {
    *alen1 = q1 - p1;

    while (p1 < q1) {
      arr[--q2] = arr[--q1];
    }
  } else {
    *alen1 = q2 - p2;
    *atype = ftype;
  }
}


/*
  arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
  lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
  keys - arrays of keys, in same order as blocks. key<midkey means stream A
  nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
  llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
*/
static void SQRT_SORT_MERGE_BUFFERS_LEFT_WITH_X_BUF(int *keys, int midkey, SORT_TYPE *arr,
    int nblock, int lblock, int nblock2, int llast) {
  int l, prest, lrest, frest, pidx, cidx, fnext;

  if (nblock == 0) {
    l = nblock2 * lblock;
    SQRT_SORT_MERGE_LEFT_WITH_X_BUF(arr, l, llast, -lblock);
    return;
  }

  lrest = lblock;
  frest = keys[0] < midkey ? 0 : 1;
  pidx = lblock;

  for (cidx = 1; cidx < nblock; cidx++, pidx += lblock) {
    prest = pidx - lrest;
    fnext = keys[cidx] < midkey ? 0 : 1;

    if (fnext == frest) {
      SORT_TYPE_CPY(arr + prest - lblock, arr + prest, lrest);
      prest = pidx;
      lrest = lblock;
    } else {
      SQRT_SORT_SMART_MERGE_WITH_X_BUF(arr + prest, &lrest, &frest, lblock, lblock);
    }
  }

  prest = pidx - lrest;

  if (llast) {
    if (frest) {
      SORT_TYPE_CPY(arr + prest - lblock, arr + prest, lrest);
      prest = pidx;
      lrest = lblock * nblock2;
      frest = 0;
    } else {
      lrest += lblock * nblock2;
    }

    SQRT_SORT_MERGE_LEFT_WITH_X_BUF(arr + prest, lrest, llast, -lblock);
  } else {
    SORT_TYPE_CPY(arr + prest - lblock, arr + prest, lrest);
  }
}

/*
  build blocks of length K
  input: [-K,-1] elements are buffer
  output: first K elements are buffer, blocks 2*K and last subblock sorted
*/
static void SQRT_SORT_BUILD_BLOCKS(SORT_TYPE *arr, int L, int K) {
  int m, u, h, p0, p1, rest, restk, p;

  for (m = 1; m < L; m += 2) {
    u = 0;

    if (SORT_CMP_A(arr + (m - 1), arr + m) > 0) {
      u = 1;
    }

    arr[m - 3] = arr[m - 1 + u];
    arr[m - 2] = arr[m - u];
  }

  if (L % 2) {
    arr[L - 3] = arr[L - 1];
  }

  arr -= 2;

  for (h = 2; h < K; h *= 2) {
    p0 = 0;
    p1 = L - 2 * h;

    while (p0 <= p1) {
      SQRT_SORT_MERGE_LEFT_WITH_X_BUF(arr + p0, h, h, -h);
      p0 += 2 * h;
    }

    rest = L - p0;

    if (rest > h) {
      SQRT_SORT_MERGE_LEFT_WITH_X_BUF(arr + p0, h, rest - h, -h);
    } else {
      for (; p0 < L; p0++) {
        arr[p0 - h] = arr[p0];
      }
    }

    arr -= h;
  }

  restk = L % (2 * K);
  p = L - restk;

  if (restk <= K) {
    SORT_TYPE_CPY(arr + p + K, arr + p, restk);
  } else {
    SQRT_SORT_MERGE_RIGHT(arr + p, K, restk - K, K);
  }

  while (p > 0) {
    p -= 2 * K;
    SQRT_SORT_MERGE_RIGHT(arr + p, K, K, K);
  }
}


static void SQRT_SORT_SORT_INS(SORT_TYPE *arr, int len) {
  int i, j;

  for (i = 1; i < len; i++) {
    for (j = i - 1; j >= 0 && SORT_CMP_A(arr + (j + 1), arr + j) < 0; j--) {
      SQRT_SORT_SWAP_1(arr + j, arr + (j + 1));
    }
  }
}

/*
  keys are on the left of arr. Blocks of length LL combined. We'll combine them in pairs
  LL and nkeys are powers of 2. (2*LL/lblock) keys are guarantied
*/
static void SQRT_SORT_COMBINE_BLOCKS(SORT_TYPE *arr, int len, int LL, int lblock, int *tags) {
  int M, b, NBlk, midkey, lrest, u, i, p, v, kc, nbl2, llast;
  SORT_TYPE *arr1;
  M = len / (2 * LL);
  lrest = len % (2 * LL);

  if (lrest <= LL) {
    len -= lrest;
    lrest = 0;
  }

  for (b = 0; b <= M; b++) {
    if (b == M && lrest == 0) {
      break;
    }

    arr1 = arr + b * 2 * LL;
    NBlk = (b == M ? lrest : 2 * LL) / lblock;
    u = NBlk + (b == M ? 1 : 0);

    for (i = 0; i <= u; i++) {
      tags[i] = i;
    }

    midkey = LL / lblock;

    for (u = 1; u < NBlk; u++) {
      p = u - 1;

      for (v = u; v < NBlk; v++) {
        kc = SORT_CMP_A(arr1 + p * lblock, arr1 + v * lblock);

        if (kc > 0 || (kc == 0 && tags[p] > tags[v])) {
          p = v;
        }
      }

      if (p != u - 1) {
        SQRT_SORT_SWAP_N(arr1 + (u - 1)*lblock, arr1 + p * lblock, lblock);
        i = tags[u - 1];
        tags[u - 1] = tags[p];
        tags[p] = i;
      }
    }

    nbl2 = llast = 0;

    if (b == M) {
      llast = lrest % lblock;
    }

    if (llast != 0) {
      while (nbl2 < NBlk && SORT_CMP_A(arr1 + NBlk * lblock, arr1 + (NBlk - nbl2 - 1) * lblock) < 0) {
        nbl2++;
      }
    }

    SQRT_SORT_MERGE_BUFFERS_LEFT_WITH_X_BUF(tags, midkey, arr1, NBlk - nbl2, lblock, nbl2, llast);
  }

  for (p = len; --p >= 0;) {
    arr[p] = arr[p - lblock];
  }
}


static void SQRT_SORT_COMMON_SORT(SORT_TYPE *arr, int Len, SORT_TYPE *extbuf, int *Tags) {
  int lblock, cbuf;

  if (Len < 16) {
    SQRT_SORT_SORT_INS(arr, Len);
    return;
  }

  lblock = 1;

  while (lblock * lblock < Len) {
    lblock *= 2;
  }

  SORT_TYPE_CPY(extbuf, arr, lblock);
  SQRT_SORT_COMMON_SORT(extbuf, lblock, arr, Tags);
  SQRT_SORT_BUILD_BLOCKS(arr + lblock, Len - lblock, lblock);
  cbuf = lblock;

  while (Len > (cbuf *= 2)) {
    SQRT_SORT_COMBINE_BLOCKS(arr + lblock, Len - lblock, cbuf, lblock, Tags);
  }

  SQRT_SORT_MERGE_DOWN(arr + lblock, extbuf, Len - lblock, lblock);
}

void SQRT_SORT(SORT_TYPE *arr, size_t Len) {
  int L = 1;
  SORT_TYPE *ExtBuf;
  int *Tags;
  int NK;

  while (L * L < Len) {
    L *= 2;
  }

  NK = (int)((Len - 1) / L + 2);
  ExtBuf = SORT_NEW_BUFFER(L);

  if (ExtBuf == NULL) {
    return;  /* fail */
  }

  Tags = (int*)malloc(NK * sizeof(int));

  if (Tags == NULL) {
    return;
  }

  SQRT_SORT_COMMON_SORT(arr, (int)Len, ExtBuf, Tags);
  free(Tags);
  SORT_DELETE_BUFFER(ExtBuf);
}

/********* Grail sorting *********************************/
/*                                                       */
/* (c) 2013 by Andrey Astrelin                           */
/*                                                       */
/*                                                       */
/* Stable sorting that works in O(N*log(N)) worst time   */
/* and uses O(1) extra memory                            */
/*                                                       */
/* Define SORT_TYPE and SORT_CMP                         */
/* and then call GrailSort() function                    */
/*                                                       */
/* For sorting with fixed external buffer (512 items)    */
/* use GrailSortWithBuffer()                             */
/*                                                       */
/* For sorting with dynamic external buffer (O(sqrt(N)) items) */
/* use GrailSortWithDynBuffer()                          */
/*                                                       */
/* Also classic in-place merge sort is implemented       */
/* under the name of RecStableSort()                     */
/*                                                       */
/*********************************************************/

#define GRAIL_EXT_BUFFER_LENGTH 512

static __inline void GRAIL_SWAP1(SORT_TYPE *a, SORT_TYPE *b) {
  SORT_TYPE c = *a;
  *a = *b;
  *b = c;
}

static __inline void GRAIL_SWAP_N(SORT_TYPE *a, SORT_TYPE *b, int n) {
  while (n--) {
    GRAIL_SWAP1(a++, b++);
  }
}

static void GRAIL_ROTATE(SORT_TYPE *a, int l1, int l2) {
  while (l1 && l2) {
    if (l1 <= l2) {
      GRAIL_SWAP_N(a, a + l1, l1);
      a += l1;
      l2 -= l1;
    } else {
      GRAIL_SWAP_N(a + (l1 - l2), a + l1, l2);
      l1 -= l2;
    }
  }
}

static int GRAIL_BIN_SEARCH_LEFT(SORT_TYPE *arr, int len, SORT_TYPE *key) {
  int a = -1, b = len, c;

  while (a < b - 1) {
    c = a + ((b - a) >> 1);

    if (SORT_CMP_A(arr + c, key) >= 0) {
      b = c;
    } else {
      a = c;
    }
  }

  return b;
}
static int GRAIL_BIN_SEARCH_RIGHT(SORT_TYPE *arr, int len, SORT_TYPE *key) {
  int a = -1, b = len, c;

  while (a < b - 1) {
    c = a + ((b - a) >> 1);

    if (SORT_CMP_A(arr + c, key) > 0) {
      b = c;
    } else {
      a = c;
    }
  }

  return b;
}

/* cost: 2*len+nk^2/2 */
static int GRAIL_FIND_KEYS(SORT_TYPE *arr, int len, int nkeys) {
  int h = 1, h0 = 0; /* first key is always here */
  int u = 1, r;

  while (u < len && h < nkeys) {
    r = GRAIL_BIN_SEARCH_LEFT(arr + h0, h, arr + u);

    if (r == h || SORT_CMP_A(arr + u, arr + (h0 + r)) != 0) {
      GRAIL_ROTATE(arr + h0, h, u - (h0 + h));
      h0 = u - h;
      GRAIL_ROTATE(arr + (h0 + r), h - r, 1);
      h++;
    }

    u++;
  }

  GRAIL_ROTATE(arr, h0, h);
  return h;
}

/* cost: min(L1,L2)^2+max(L1,L2) */
static void GRAIL_MERGE_WITHOUT_BUFFER(SORT_TYPE *arr, int len1, int len2) {
  int h;

  if (len1 < len2) {
    while (len1) {
      h = GRAIL_BIN_SEARCH_LEFT(arr + len1, len2, arr);

      if (h != 0) {
        GRAIL_ROTATE(arr, len1, h);
        arr += h;
        len2 -= h;
      }

      if (len2 == 0) {
        break;
      }

      do {
        arr++;
        len1--;
      } while (len1 && SORT_CMP_A(arr, arr + len1) <= 0);
    }
  } else {
    while (len2) {
      h = GRAIL_BIN_SEARCH_RIGHT(arr, len1, arr + (len1 + len2 - 1));

      if (h != len1) {
        GRAIL_ROTATE(arr + h, len1 - h, len2);
        len1 = h;
      }

      if (len1 == 0) {
        break;
      }

      do {
        len2--;
      } while (len2 && SORT_CMP_A(arr + len1 - 1, arr + len1 + len2 - 1) <= 0);
    }
  }
}

/* arr[M..-1] - buffer, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1] */
static void GRAIL_MERGE_LEFT(SORT_TYPE *arr, int L1, int L2, int M) {
  int p0 = 0, p1 = L1;
  L2 += L1;

  while (p1 < L2) {
    if (p0 == L1 || SORT_CMP_A(arr + p0, arr + p1) > 0) {
      GRAIL_SWAP1(arr + (M++), arr + (p1++));
    } else {
      GRAIL_SWAP1(arr + (M++), arr + (p0++));
    }
  }

  if (M != p0) {
    GRAIL_SWAP_N(arr + M, arr + p0, L1 - p0);
  }
}
static void GRAIL_MERGE_RIGHT(SORT_TYPE *arr, int L1, int L2, int M) {
  int p0 = L1 + L2 + M - 1, p2 = L1 + L2 - 1, p1 = L1 - 1;

  while (p1 >= 0) {
    if (p2 < L1 || SORT_CMP_A(arr + p1, arr + p2) > 0) {
      GRAIL_SWAP1(arr + (p0--), arr + (p1--));
    } else {
      GRAIL_SWAP1(arr + (p0--), arr + (p2--));
    }
  }

  if (p2 != p0) while (p2 >= L1) {
      GRAIL_SWAP1(arr + (p0--), arr + (p2--));
    }
}

static void GRAIL_SMART_MERGE_WITH_BUFFER(SORT_TYPE *arr, int *alen1, int *atype, int len2,
    int lkeys) {
  int p0 = -lkeys, p1 = 0, p2 = *alen1, q1 = p2, q2 = p2 + len2;
  int ftype = 1 - *atype; /* 1 if inverted */

  while (p1 < q1 && p2 < q2) {
    if (SORT_CMP_A(arr + p1, arr + p2) - ftype < 0) {
      GRAIL_SWAP1(arr + (p0++), arr + (p1++));
    } else {
      GRAIL_SWAP1(arr + (p0++), arr + (p2++));
    }
  }

  if (p1 < q1) {
    *alen1 = q1 - p1;

    while (p1 < q1) {
      GRAIL_SWAP1(arr + (--q1), arr + (--q2));
    }
  } else {
    *alen1 = q2 - p2;
    *atype = ftype;
  }
}
static void GRAIL_SMART_MERGE_WITHOUT_BUFFER(SORT_TYPE *arr, int *alen1, int *atype, int _len2) {
  int len1, len2, ftype, h;

  if (!_len2) {
    return;
  }

  len1 = *alen1;
  len2 = _len2;
  ftype = 1 - *atype;

  if (len1 && SORT_CMP_A(arr + (len1 - 1), arr + len1) - ftype >= 0) {
    while (len1) {
      h = ftype ? GRAIL_BIN_SEARCH_LEFT(arr + len1, len2, arr) : GRAIL_BIN_SEARCH_RIGHT(arr + len1, len2,
          arr);

      if (h != 0) {
        GRAIL_ROTATE(arr, len1, h);
        arr += h;
        len2 -= h;
      }

      if (len2 == 0) {
        *alen1 = len1;
        return;
      }

      do {
        arr++;
        len1--;
      } while (len1 && SORT_CMP_A(arr, arr + len1) - ftype < 0);
    }
  }

  *alen1 = len2;
  *atype = ftype;
}

/***** Sort With Extra Buffer *****/

/* arr[M..-1] - free, arr[0,L1-1]++arr[L1,L1+L2-1] -> arr[M,M+L1+L2-1] */
static void GRAIL_MERGE_LEFT_WITH_X_BUF(SORT_TYPE *arr, int L1, int L2, int M) {
  int p0 = 0, p1 = L1;
  L2 += L1;

  while (p1 < L2) {
    if (p0 == L1 || SORT_CMP_A(arr + p0, arr + p1) > 0) {
      arr[M++] = arr[p1++];
    } else {
      arr[M++] = arr[p0++];
    }
  }

  if (M != p0) while (p0 < L1) {
      arr[M++] = arr[p0++];
    }
}

static void GRAIL_SMART_MERGE_WITH_X_BUF(SORT_TYPE *arr, int *alen1, int *atype, int len2,
    int lkeys) {
  int p0 = -lkeys, p1 = 0, p2 = *alen1, q1 = p2, q2 = p2 + len2;
  int ftype = 1 - *atype; /* 1 if inverted */

  while (p1 < q1 && p2 < q2) {
    if (SORT_CMP_A(arr + p1, arr + p2) - ftype < 0) {
      arr[p0++] = arr[p1++];
    } else {
      arr[p0++] = arr[p2++];
    }
  }

  if (p1 < q1) {
    *alen1 = q1 - p1;

    while (p1 < q1) {
      arr[--q2] = arr[--q1];
    }
  } else {
    *alen1 = q2 - p2;
    *atype = ftype;
  }
}

/*
  arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
  lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
  keys - arrays of keys, in same order as blocks. key<midkey means stream A
  nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
  llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
*/
static void GRAIL_MERGE_BUFFERS_LEFT_WITH_X_BUF(SORT_TYPE *keys, SORT_TYPE *midkey, SORT_TYPE *arr,
    int nblock, int lblock, int nblock2, int llast) {
  int l, prest, lrest, frest, pidx, cidx, fnext;

  if (nblock == 0) {
    l = nblock2 * lblock;
    GRAIL_MERGE_LEFT_WITH_X_BUF(arr, l, llast, -lblock);
    return;
  }

  lrest = lblock;
  frest = SORT_CMP_A(keys, midkey) < 0 ? 0 : 1;
  pidx = lblock;

  for (cidx = 1; cidx < nblock; cidx++, pidx += lblock) {
    prest = pidx - lrest;
    fnext = SORT_CMP_A(keys + cidx, midkey) < 0 ? 0 : 1;

    if (fnext == frest) {
      SORT_TYPE_CPY(arr + prest - lblock, arr + prest, lrest);
      prest = pidx;
      lrest = lblock;
    } else {
      GRAIL_SMART_MERGE_WITH_X_BUF(arr + prest, &lrest, &frest, lblock, lblock);
    }
  }

  prest = pidx - lrest;

  if (llast) {
    if (frest) {
      SORT_TYPE_CPY(arr + prest - lblock, arr + prest, lrest);
      prest = pidx;
      lrest = lblock * nblock2;
      frest = 0;
    } else {
      lrest += lblock * nblock2;
    }

    GRAIL_MERGE_LEFT_WITH_X_BUF(arr + prest, lrest, llast, -lblock);
  } else {
    SORT_TYPE_CPY(arr + prest - lblock, arr + prest, lrest);
  }
}

/***** End Sort With Extra Buffer *****/

/*
  build blocks of length K
  input: [-K,-1] elements are buffer
  output: first K elements are buffer, blocks 2*K and last subblock sorted
*/
static void GRAIL_BUILD_BLOCKS(SORT_TYPE *arr, int L, int K, SORT_TYPE *extbuf, int LExtBuf) {
  int m, u, h, p0, p1, rest, restk, p, kbuf;
  kbuf = K < LExtBuf ? K : LExtBuf;

  while (kbuf & (kbuf - 1)) {
    kbuf &= kbuf - 1;  /* max power or 2 - just in case */
  }

  if (kbuf) {
    SORT_TYPE_CPY(extbuf, arr - kbuf, kbuf);

    for (m = 1; m < L; m += 2) {
      u = 0;

      if (SORT_CMP_A(arr + (m - 1), arr + m) > 0) {
        u = 1;
      }

      arr[m - 3] = arr[m - 1 + u];
      arr[m - 2] = arr[m - u];
    }

    if (L % 2) {
      arr[L - 3] = arr[L - 1];
    }

    arr -= 2;

    for (h = 2; h < kbuf; h *= 2) {
      p0 = 0;
      p1 = L - 2 * h;

      while (p0 <= p1) {
        GRAIL_MERGE_LEFT_WITH_X_BUF(arr + p0, h, h, -h);
        p0 += 2 * h;
      }

      rest = L - p0;

      if (rest > h) {
        GRAIL_MERGE_LEFT_WITH_X_BUF(arr + p0, h, rest - h, -h);
      } else {
        for (; p0 < L; p0++) {
          arr[p0 - h] = arr[p0];
        }
      }

      arr -= h;
    }

    SORT_TYPE_CPY(arr + L, extbuf, kbuf);
  } else {
    for (m = 1; m < L; m += 2) {
      u = 0;

      if (SORT_CMP_A(arr + (m - 1), arr + m) > 0) {
        u = 1;
      }

      GRAIL_SWAP1(arr + (m - 3), arr + (m - 1 + u));
      GRAIL_SWAP1(arr + (m - 2), arr + (m - u));
    }

    if (L % 2) {
      GRAIL_SWAP1(arr + (L - 1), arr + (L - 3));
    }

    arr -= 2;
    h = 2;
  }

  for (; h < K; h *= 2) {
    p0 = 0;
    p1 = L - 2 * h;

    while (p0 <= p1) {
      GRAIL_MERGE_LEFT(arr + p0, h, h, -h);
      p0 += 2 * h;
    }

    rest = L - p0;

    if (rest > h) {
      GRAIL_MERGE_LEFT(arr + p0, h, rest - h, -h);
    } else {
      GRAIL_ROTATE(arr + p0 - h, h, rest);
    }

    arr -= h;
  }

  restk = L % (2 * K);
  p = L - restk;

  if (restk <= K) {
    GRAIL_ROTATE(arr + p, restk, K);
  } else {
    GRAIL_MERGE_RIGHT(arr + p, K, restk - K, K);
  }

  while (p > 0) {
    p -= 2 * K;
    GRAIL_MERGE_RIGHT(arr + p, K, K, K);
  }
}

/*
  arr - starting array. arr[-lblock..-1] - buffer (if havebuf).
  lblock - length of regular blocks. First nblocks are stable sorted by 1st elements and key-coded
  keys - arrays of keys, in same order as blocks. key<midkey means stream A
  nblock2 are regular blocks from stream A. llast is length of last (irregular) block from stream B, that should go before nblock2 blocks.
  llast=0 requires nblock2=0 (no irregular blocks). llast>0, nblock2=0 is possible.
*/
static void GRAIL_MERGE_BUFFERS_LEFT(SORT_TYPE *keys, SORT_TYPE *midkey, SORT_TYPE *arr, int nblock,
                                     int lblock, int havebuf, int nblock2, int llast) {
  int l, prest, lrest, frest, pidx, cidx, fnext;

  if (nblock == 0) {
    l = nblock2 * lblock;

    if (havebuf) {
      GRAIL_MERGE_LEFT(arr, l, llast, -lblock);
    } else {
      GRAIL_MERGE_WITHOUT_BUFFER(arr, l, llast);
    }

    return;
  }

  lrest = lblock;
  frest = SORT_CMP_A(keys, midkey) < 0 ? 0 : 1;
  pidx = lblock;

  for (cidx = 1; cidx < nblock; cidx++, pidx += lblock) {
    prest = pidx - lrest;
    fnext = SORT_CMP_A(keys + cidx, midkey) < 0 ? 0 : 1;

    if (fnext == frest) {
      if (havebuf) {
        GRAIL_SWAP_N(arr + prest - lblock, arr + prest, lrest);
      }

      prest = pidx;
      lrest = lblock;
    } else {
      if (havebuf) {
        GRAIL_SMART_MERGE_WITH_BUFFER(arr + prest, &lrest, &frest, lblock, lblock);
      } else {
        GRAIL_SMART_MERGE_WITHOUT_BUFFER(arr + prest, &lrest, &frest, lblock);
      }
    }
  }

  prest = pidx - lrest;

  if (llast) {
    if (frest) {
      if (havebuf) {
        GRAIL_SWAP_N(arr + prest - lblock, arr + prest, lrest);
      }

      prest = pidx;
      lrest = lblock * nblock2;
      frest = 0;
    } else {
      lrest += lblock * nblock2;
    }

    if (havebuf) {
      GRAIL_MERGE_LEFT(arr + prest, lrest, llast, -lblock);
    } else {
      GRAIL_MERGE_WITHOUT_BUFFER(arr + prest, lrest, llast);
    }
  } else {
    if (havebuf) {
      GRAIL_SWAP_N(arr + prest, arr + (prest - lblock), lrest);
    }
  }
}

static void GRAIL_LAZY_STABLE_SORT(SORT_TYPE *arr, int L) {
  int m, h, p0, p1, rest;

  for (m = 1; m < L; m += 2) {
    if (SORT_CMP_A(arr + m - 1, arr + m) > 0) {
      GRAIL_SWAP1(arr + (m - 1), arr + m);
    }
  }

  for (h = 2; h < L; h *= 2) {
    p0 = 0;
    p1 = L - 2 * h;

    while (p0 <= p1) {
      GRAIL_MERGE_WITHOUT_BUFFER(arr + p0, h, h);
      p0 += 2 * h;
    }

    rest = L - p0;

    if (rest > h) {
      GRAIL_MERGE_WITHOUT_BUFFER(arr + p0, h, rest - h);
    }
  }
}

/*
  keys are on the left of arr. Blocks of length LL combined. We'll combine them in pairs
  LL and nkeys are powers of 2. (2*LL/lblock) keys are guarantied
*/
static void GRAIL_COMBINE_BLOCKS(SORT_TYPE *keys, SORT_TYPE *arr, int len, int LL, int lblock,
                                 int havebuf, SORT_TYPE *xbuf) {
  int M, b, NBlk, midkey, lrest, u, p, v, kc, nbl2, llast;
  SORT_TYPE *arr1;
  M = len / (2 * LL);
  lrest = len % (2 * LL);

  if (lrest <= LL) {
    len -= lrest;
    lrest = 0;
  }

  if (xbuf) {
    SORT_TYPE_CPY(xbuf, arr - lblock, lblock);
  }

  for (b = 0; b <= M; b++) {
    if (b == M && lrest == 0) {
      break;
    }

    arr1 = arr + b * 2 * LL;
    NBlk = (b == M ? lrest : 2 * LL) / lblock;
    SMALL_STABLE_SORT(keys, NBlk + (b == M ? 1 : 0));
    midkey = LL / lblock;

    for (u = 1; u < NBlk; u++) {
      p = u - 1;

      for (v = u; v < NBlk; v++) {
        kc = SORT_CMP_A(arr1 + p * lblock, arr1 + v * lblock);

        if (kc > 0 || (kc == 0 && SORT_CMP_A(keys + p, keys + v) > 0)) {
          p = v;
        }
      }

      if (p != u - 1) {
        GRAIL_SWAP_N(arr1 + (u - 1)*lblock, arr1 + p * lblock, lblock);
        GRAIL_SWAP1(keys + (u - 1), keys + p);

        if (midkey == u - 1 || midkey == p) {
          midkey ^= (u - 1)^p;
        }
      }
    }

    nbl2 = llast = 0;

    if (b == M) {
      llast = lrest % lblock;
    }

    if (llast != 0) {
      while (nbl2 < NBlk && SORT_CMP_A(arr1 + NBlk * lblock, arr1 + (NBlk - nbl2 - 1) * lblock) < 0) {
        nbl2++;
      }
    }

    if (xbuf) {
      GRAIL_MERGE_BUFFERS_LEFT_WITH_X_BUF(keys, keys + midkey, arr1, NBlk - nbl2, lblock, nbl2, llast);
    } else {
      GRAIL_MERGE_BUFFERS_LEFT(keys, keys + midkey, arr1, NBlk - nbl2, lblock, havebuf, nbl2, llast);
    }
  }

  if (xbuf) {
    for (p = len; --p >= 0;) {
      arr[p] = arr[p - lblock];
    }

    SORT_TYPE_CPY(arr - lblock, xbuf, lblock);
  } else if (havebuf) {
    while (--len >= 0) {
      GRAIL_SWAP1(arr + len, arr + len - lblock);
    }
  }
}


static void GRAIL_COMMON_SORT(SORT_TYPE *arr, int Len, SORT_TYPE *extbuf, int LExtBuf) {
  int lblock, nkeys, findkeys, ptr, cbuf, lb, nk;
  int havebuf, chavebuf;
  long long s;

  if (Len <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(arr, Len);
    return;
  }

  lblock = 1;

  while (lblock * lblock < Len) {
    lblock *= 2;
  }

  nkeys = (Len - 1) / lblock + 1;
  findkeys = GRAIL_FIND_KEYS(arr, Len, nkeys + lblock);
  havebuf = 1;

  if (findkeys < nkeys + lblock) {
    if (findkeys < 4) {
      GRAIL_LAZY_STABLE_SORT(arr, Len);
      return;
    }

    nkeys = lblock;

    while (nkeys > findkeys) {
      nkeys /= 2;
    }

    havebuf = 0;
    lblock = 0;
  }

  ptr = lblock + nkeys;
  cbuf = havebuf ? lblock : nkeys;

  if (havebuf) {
    GRAIL_BUILD_BLOCKS(arr + ptr, Len - ptr, cbuf, extbuf, LExtBuf);
  } else {
    GRAIL_BUILD_BLOCKS(arr + ptr, Len - ptr, cbuf, NULL, 0);
  }

  /* 2*cbuf are built */
  while (Len - ptr > (cbuf *= 2)) {
    lb = lblock;
    chavebuf = havebuf;

    if (!havebuf) {
      if (nkeys > 4 && nkeys / 8 * nkeys >= cbuf) {
        lb = nkeys / 2;
        chavebuf = 1;
      } else {
        nk = 1;
        s = (long long)cbuf * findkeys / 2;

        while (nk < nkeys && s != 0) {
          nk *= 2;
          s /= 8;
        }

        lb = (2 * cbuf) / nk;
      }
    }

    GRAIL_COMBINE_BLOCKS(arr, arr + ptr, Len - ptr, cbuf, lb, chavebuf, chavebuf
                         && lb <= LExtBuf ? extbuf : NULL);
  }

  SMALL_STABLE_SORT(arr, ptr);
  GRAIL_MERGE_WITHOUT_BUFFER(arr, ptr, Len - ptr);
}

SORT_DEF void GRAIL_SORT(SORT_TYPE *arr, size_t Len) {
  GRAIL_COMMON_SORT(arr, (int)Len, NULL, 0);
}

SORT_DEF void GRAIL_SORT_FIXED_BUFFER(SORT_TYPE *arr, size_t Len) {
  SORT_TYPE ExtBuf[GRAIL_EXT_BUFFER_LENGTH];
  GRAIL_COMMON_SORT(arr, (int)Len, ExtBuf, GRAIL_EXT_BUFFER_LENGTH);
}

SORT_DEF void GRAIL_SORT_DYN_BUFFER(SORT_TYPE *arr, size_t Len) {
  int L = 1;
  SORT_TYPE *ExtBuf;

  while (L * L < Len) {
    L *= 2;
  }

  ExtBuf = SORT_NEW_BUFFER(L);

  if (ExtBuf == NULL) {
    GRAIL_SORT_FIXED_BUFFER(arr, Len);
  } else {
    GRAIL_COMMON_SORT(arr, (int)Len, ExtBuf, L);
    SORT_DELETE_BUFFER(ExtBuf);
  }
}

/****** classic MergeInPlace *************/

static void GRAIL_REC_MERGE(SORT_TYPE *A, int L1, int L2) {
  int K, k1, k2, m1, m2;

  if (L1 < 3 || L2 < 3) {
    GRAIL_MERGE_WITHOUT_BUFFER(A, L1, L2);
    return;
  }

  if (L1 < L2) {
    K = L1 + L2 / 2;
  } else {
    K = L1 / 2;
  }

  k1 = k2 = GRAIL_BIN_SEARCH_LEFT(A, L1, A + K);

  if (k2 < L1 && SORT_CMP_A(A + k2, A + K) == 0) {
    k2 = GRAIL_BIN_SEARCH_RIGHT(A + k1, L1 - k1, A + K) + k1;
  }

  m1 = GRAIL_BIN_SEARCH_LEFT(A + L1, L2, A + K);
  m2 = m1;

  if (m2 < L2 && SORT_CMP_A(A + L1 + m2, A + K) == 0) {
    m2 = GRAIL_BIN_SEARCH_RIGHT(A + L1 + m1, L2 - m1, A + K) + m1;
  }

  if (k1 == k2) {
    GRAIL_ROTATE(A + k2, L1 - k2, m2);
  } else {
    GRAIL_ROTATE(A + k1, L1 - k1, m1);

    if (m2 != m1) {
      GRAIL_ROTATE(A + (k2 + m1), L1 - k2, m2 - m1);
    }
  }

  GRAIL_REC_MERGE(A + (k2 + m2), L1 - k2, L2 - m2);
  GRAIL_REC_MERGE(A, k1, m1);
}

SORT_DEF void REC_STABLE_SORT(SORT_TYPE *arr, size_t L) {
  int m, h, p0, p1, rest;

  for (m = 1; m < L; m += 2) {
    if (SORT_CMP_A(arr + m - 1, arr + m) > 0) {
      GRAIL_SWAP1(arr + (m - 1), arr + m);
    }
  }

  for (h = 2; h < L; h *= 2) {
    p0 = 0;
    p1 = (int)(L - 2 * h);

    while (p0 <= p1) {
      GRAIL_REC_MERGE(arr + p0, h, h);
      p0 += 2 * h;
    }

    rest = (int)(L - p0);

    if (rest > h) {
      GRAIL_REC_MERGE(arr + p0, h, rest - h);
    }
  }
}

/* Bubble sort implementation based on Wikipedia article
   https://en.wikipedia.org/wiki/Bubble_sort
*/
SORT_DEF void BUBBLE_SORT(SORT_TYPE *dst, const size_t size) {
  size_t n = size;

  while (n) {
    size_t i, newn = 0U;

    for (i = 1U; i < n; ++i) {
      if (SORT_CMP(dst[i - 1U], dst[i]) > 0) {
        SORT_SWAP(dst[i - 1U], dst[i]);
        newn = i;
      }
    }

    n = newn;
  }
}

/* Selection sort */
SORT_DEF void SELECTION_SORT(SORT_TYPE *dst, const size_t size) {
  size_t i, j;

  /* don't bother sorting an array of size <= 1 */
  if (size <= 1) {
    return;
  }

  for (i = 0; i < size; i++) {
    for (j = i + 1; j < size; j++) {
      if (SORT_CMP(dst[j], dst[i]) < 0) {
        SORT_SWAP(dst[i], dst[j]);
      }
    }
  }
}

#undef GRAIL_SWAP1
#undef REC_STABLE_SORT
#undef GRAIL_REC_MERGE
#undef GRAIL_SORT_DYN_BUFFER
#undef GRAIL_SORT_FIXED_BUFFER
#undef GRAIL_COMMON_SORT
#undef GRAIL_SORT
#undef GRAIL_COMBINE_BLOCKS
#undef GRAIL_LAZY_STABLE_SORT
#undef GRAIL_MERGE_WITHOUT_BUFFER
#undef GRAIL_ROTATE
#undef GRAIL_BIN_SEARCH_LEFT
#undef GRAIL_BUILD_BLOCKS
#undef GRAIL_FIND_KEYS
#undef GRAIL_MERGE_BUFFERS_LEFT_WITH_X_BUF
#undef GRAIL_BIN_SEARCH_RIGHT
#undef GRAIL_MERGE_BUFFERS_LEFT
#undef GRAIL_SMART_MERGE_WITH_X_BUF
#undef GRAIL_MERGE_LEFT_WITH_X_BUF
#undef GRAIL_SMART_MERGE_WITHOUT_BUFFER
#undef GRAIL_SMART_MERGE_WITH_BUFFER
#undef GRAIL_MERGE_RIGHT
#undef GRAIL_MERGE_LEFT
#undef GRAIL_SWAP_N
#undef SQRT_SORT
#undef SQRT_SORT_BUILD_BLOCKS
#undef SQRT_SORT_MERGE_BUFFERS_LEFT_WITH_X_BUF
#undef SQRT_SORT_MERGE_DOWN
#undef SQRT_SORT_MERGE_LEFT_WITH_X_BUF
#undef SQRT_SORT_MERGE_RIGHT
#undef SQRT_SORT_SWAP_N
#undef SQRT_SORT_SWAP_1
#undef SQRT_SORT_SMART_MERGE_WITH_X_BUF
#undef SQRT_SORT_SORT_INS
#undef SQRT_SORT_COMBINE_BLOCKS
#undef SQRT_SORT_COMMON_SORT
#undef SORT_CMP_A
#undef BUBBLE_SORT
#undef SELECTION_SORT
#undef GRAIL_EXT_BUFFER_LENGTH
