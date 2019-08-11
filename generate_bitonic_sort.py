import requests

max_small_sort = 16
small_sort_only = True 

def generate_small_sort(n, rev=False):
    s = "http://jgamble.ripco.net/cgi-bin/nw.cgi?inputs=%d&algorithm=best&output=svg" % n
    ret = requests.get(s)
    content = ret.text
    lines = [l for l in content.split('\n') if l.startswith('[[')]
    swps = ""
    for _l in lines:
        l = _l[1:-1]
        ll = l.split(']')
        for _t in ll:
            if '[' not in _t:
                continue
            t = _t.split('[')[-1]
            i, j = t.split(',')
            if not rev:
                swps += "\tSORT_CSWAP(dst[%s], dst[%s]);\n" % (i, j)
            else:
                swps += "\tSORT_CSWAP(dst[%s], dst[%s]);\n" % (j, i)
        swps += '\n'
    if not rev:
        f = """
#define BITONIC_SORT_%d          SORT_MAKE_STR(bitonic_sort_%d)
static __inline void BITONIC_SORT_%d(SORT_TYPE *dst) {
%s}
""" % (n, n, n, swps[:-1])
    else:
        f = """
#define BITONIC_SORT_REVERSE_%d          SORT_MAKE_STR(bitonic_sort_reverse_%d)
static __inline void BITONIC_SORT_REVERSE_%d(SORT_TYPE *dst) {
%s}
""" % (n, n, n, swps[:-1])
    return f


bitonic_sort_str = """
#ifndef SORT_CSWAP
  #define SORT_CSWAP(x, y) { if(SORT_CMP((x),(y)) > 0) {SORT_SWAP((x),(y));}}
#endif
"""
bitonic_sort_str += "\n".join(generate_small_sort(i) for i in range(2,max_small_sort+1))

if small_sort_only:
    bitonic_sort_case = "\n".join("        case %d:\n            BITONIC_SORT_%d(dst);\n            break;" % (n, n) for n in range(2, max_small_sort+1))
    bitonic_sort_str += """
void BITONIC_SORT(SORT_TYPE *dst, const size_t size) {
    switch(size) {
        case 0:
        case 1:
            break;
%s
        default:
            BINARY_INSERTION_SORT(dst, size);
        }
}
    """ % (bitonic_sort_case)
    print(bitonic_sort_str)
    exit()
    


bitonic_sort_str += "\n".join(generate_small_sort(i, rev=True) for i in range(2,max_small_sort+1))

bitonic_sort_case = "\n".join("        case %d:\n            BITONIC_SORT_%d(dst);\n            break;" % (n, n) for n in range(2, max_small_sort+1))
bitonic_sort_case_rev = "\n".join("        case %d:\n            BITONIC_SORT_REVERSE_%d(dst);\n            break;" % (n, n) for n in range(2, max_small_sort+1))

bitonic_sort_str += """
#define BITONIC_SORT          SORT_MAKE_STR(bitonic_sort)
void BITONIC_SORT(SORT_TYPE *dst, const size_t size);
#define BITONIC_SORT_REVERSE          SORT_MAKE_STR(bitonic_sort_reverse)
void BITONIC_SORT_REVERSE(SORT_TYPE *dst, const size_t size);
"""

bitonic_sort_str += """
#define BITONIC_MERGE          SORT_MAKE_STR(bitonic_merge)
void BITONIC_MERGE(SORT_TYPE *dst, const size_t size) {
  size_t m, i, j;
  if (size <= 1) {
      return;
  }
  m = 1ULL<<(63 - CLZ(size-1));
  j = m;
  for (i = 0; i < size - m; ++i, ++j) {
      SORT_CSWAP(dst[i], dst[j]);
  }
  BITONIC_MERGE(dst, m);
  BITONIC_MERGE(dst + m, size - m);
}

#define BITONIC_MERGE_REVERSE          SORT_MAKE_STR(bitonic_merge_reverse)
void BITONIC_MERGE_REVERSE(SORT_TYPE *dst, const size_t size) {
  size_t m, i, j;
  if (size <= 1) {
      return;
  }
  m = 1ULL<<(63 - CLZ(size-1));
  j = m;
  for (i = 0; i < size - m; ++i, ++j) {
      SORT_CSWAP(dst[j], dst[i]);
  }
  BITONIC_MERGE_REVERSE(dst, m);
  BITONIC_MERGE_REVERSE(dst + m, size - m);
}

"""

bitonic_sort_str += """
void BITONIC_SORT(SORT_TYPE *dst, const size_t size) {
    switch(size) {
        case 0:
        case 1:
            break;
%s
        default:
            /*printf("Bitonic sort size = %%ld", size);*/
            BITONIC_SORT_REVERSE(dst, (size>>1));
            BITONIC_SORT(dst + (size>>1), size - (size>>1));
            BITONIC_MERGE(dst, size);
    }
}
""" % (bitonic_sort_case)

bitonic_sort_str += """
void BITONIC_SORT_REVERSE(SORT_TYPE *dst, const size_t size) {
    switch(size) {
        case 0:
        case 1:
            break;
%s
        default:
            /*printf("Bitonic sort reverse size = %%ld", size);*/
            BITONIC_SORT(dst, (size>>1));
            BITONIC_SORT_REVERSE(dst + (size>>1), size - (size>>1));
            BITONIC_MERGE_REVERSE(dst, size);
    }
}
""" % (bitonic_sort_case_rev)

import os

with open('bitonic_sort.h', 'w') as F:
  F.write(bitonic_sort_str)

os.system('astyle --options=astyle.options bitonic_sort.h')