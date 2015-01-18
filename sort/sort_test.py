#!/path/to/python

import time
import random
from sort import merge_sort
from sort import merge_sort_slice

def list_to_str(items):
    return "[" + ", ".join([str(i) for i in items]) + "]"

def check_order(items, verbose = False):
    items_orig = list(items)
    items_copy = list(items)
    merge_sort(items)
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
    iteration_count = 500
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

    print "Merge Sort        : %s (ms)" % round(merge_sort_time * 1000)
    print "Merge Sort (slice): %s (ms)" % round(merge_sort_slice_time * 1000)

test_perf()

# check_order([])
# check_order([1])
# check_order([2, 1, 0])
# check_order([3, 2, 1, 0])
