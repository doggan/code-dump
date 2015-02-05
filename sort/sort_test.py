#!/path/to/python

import time
import random
from sort import merge_sort
from sort import merge_sort_slice
from sort import quick_sort

def list_to_str(items):
    return "[" + ", ".join([str(i) for i in items]) + "]"

def check_order(sort_func, items, verbose = False):
    items_orig = list(items)
    items_copy = list(items)
    sort_func(items)
    items_copy.sort()
    if verbose:
        if cmp(items, items_copy) == 0:
            print "PASS: " + list_to_str(items_orig) + " => " + list_to_str(items)
        else:
            print "FAIL: " + list_to_str(items_orig) + " => " + list_to_str(items)
    else:
        if cmp(items, items_copy) != 0:
            print "FAIL"

def gen_list(size):
    return [random.randint(0, 1000) for i in xrange(size)]

def test_perf():
    iteration_count = 100
    item_count = 500

    t_start = time.time()
    for i in xrange(0, iteration_count):
        items = gen_list(item_count)
        merge_sort(items)
    t_end = time.time()
    merge_sort_time = t_end - t_start

    t_start = time.time()
    for i in xrange(0, iteration_count):
        items = gen_list(item_count)
        merge_sort_slice(items)
    t_end = time.time()
    merge_sort_slice_time = t_end - t_start

    t_start = time.time()
    for i in xrange(0, iteration_count):
        items = gen_list(item_count)
        quick_sort(items)
    t_end = time.time()
    quick_sort_time = t_end - t_start

    # Quicksort on a sorted list
    t_start = time.time()
    orig_items = range(0, item_count)
    for i in xrange(0, iteration_count):
        items = list(orig_items)
        quick_sort(items)
    t_end = time.time()
    quick_sort_time_sorted = t_end - t_start

    # Quicksort on a reverse-sorted list
    t_start = time.time()
    orig_items = reversed(range(0, item_count))
    for i in xrange(0, iteration_count):
        items = list(orig_items)
        quick_sort(items)
    t_end = time.time()
    quick_sort_time_sorted_rev = t_end - t_start

    print "Merge Sort        : %s (ms)" % round(merge_sort_time * 1000)
    print "Merge Sort (slice): %s (ms)" % round(merge_sort_slice_time * 1000)
    print "Quick Sort        : %s (ms)" % round(quick_sort_time * 1000)
    print "- - - - - - - - - - - - - - - - - -"
    print "Quick Sort (sorted)     : %s (ms)" % round(quick_sort_time_sorted * 1000)
    print "Quick Sort (rev-sorted) : %s (ms)" % round(quick_sort_time_sorted_rev * 1000)

test_perf()

sort_func = merge_sort
check_order(sort_func, [])
check_order(sort_func, [1])
check_order(sort_func, [2, 1, 0])
check_order(sort_func, [3, 2, 1, 0])

sort_func = merge_sort_slice
check_order(sort_func, [])
check_order(sort_func, [1])
check_order(sort_func, [2, 1, 0])
check_order(sort_func, [3, 2, 1, 0])

sort_func = quick_sort
check_order(sort_func, [])
check_order(sort_func, [1])
check_order(sort_func, [2, 1, 0])
check_order(sort_func, [3, 2, 1, 0])
