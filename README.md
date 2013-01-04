sort.h
======

Overview
--------

`sort.h` is an implementation a ton of sorting algorithms in C with a
user-defined type, that is defined at include time.

This means you don't have to pay the function call overhead of using
standard library routine.

You get the choice of many sorting routines, including:

* Shell sort
* Binary insertion sort
* Heap sort
* Quick sort
* Merge sort (stable)
* In-place merge sort (*not* stable)
* Bubble sort (ugh -- this is really only here for comparison)
* Tim sort (stable)

If you don't know which one to use, you should probably use Tim sort.

If you have a lot data that is semi-structured, then you should definitely use Tim sort.

If you have data that is really and truly random, quick sort is probably fastest.


Usage
-----

To use this library, you need to do three things:

* `#define SORT_TYPE` to be the type of the elements of the array you
  want to sort. (For pointers, you should declare this like: `#define SORT_TYPE int*`)
* `#define SORT_NAME` to be a unique name that will be prepended to all
  the routines, i.e., `#define SORT_NAME mine` would give you routines
  named `mine_heap_sort`, and so forth.
* `#include "sort.h"`.  Make sure that `sort.h` is in your include path,
  obviously.

Then, enjoy using the sorting routines.

Quick example:
<pre>
#define SORT_NAME int64
#define SORT_TYPE int64_t
#define SORT_CMP(x, y) ((x) - (y))
#include "sort.h"
</pre>

You would now have access to `int64_quick_sort`, `int64_tim_sort`, etc.

See `demo.c` for a more detailed example usage.

If you are going to use your own custom type, you must redefine
`SORT_CMP(x, y)` with your comparison function, so that it returns
a value less than zero if `x < y`, equal to zero if `x == y`, and
greater than 0 if `x > y`.

The default just uses the builtin `<`, `==`, and `>` operators:

`#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))`

It is often just fine to just subtract the arguments as well (though
this can cause some stability problems with floating-point types):

`#define SORT_CMP(x, y) ((x) - (y))`

Speed of routines
-----------------

The speed of each routine is highly dependent on your computer and the
structure of your data.

If your data has a lot of partially sorted sequences, then Tim sort
will beat the pants off of anything else.

Tim sort is not as good if memory movement is many orders of magnitude more
expensive than comparisons (like, many more than for normal int and double).
If so, then quick sort is probably your routine.  On the other hand, Tim
sort does extremely well if the comparison operator is very expensive,
since it strives hard to minimize comparisons.

Here is the output of `demo.c`, which will give you the timings for a run of
10,000 `int64_t`s on 2010-era MacBook Pro:

	Running tests
	stdlib qsort time: 1711.00 us per iteration
	stdlib heapsort time: 2751.00 us per iteration
	stdlib mergesort time: 1913.00 us per iteration
	quick sort time: 802.00 us per iteration
	bubble sort time: 221410.00 us per iteration
	merge sort time: 1231.00 us per iteration
	binary insertion sort time: 22810.00 us per iteration
	heap sort time: 820.00 us per iteration
	shell sort time: 1222.00 us per iteration
	tim sort time: 1076.00 us per iteration
	in-place merge sort time: 1027.00 us per iteration

Quicksort is the winner here.

Author
------
Christopher Swenson (chris@caswenson.com)


References
----------

* Wikipedia
* `timsort.txt` (under doc/)


License
-------

All code in this repository, unless otherwise specified, is hereby
licensed under the MIT Public License:

Copyright (c) 2010 Christopher Swenson.

Copyright (c) 2012 Google Inc. All Rights Reserved.

Copyright (c) 2012 Vojtech Fried.

In-place mergesort is:

Copyright (c) 2012, Andrey Astrelin, astrelin@tochka.ru

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.

