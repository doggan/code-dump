#!/path/to/python

def merge_sort(items):
    if items is None:
        return

    helper = []
    merge_sort_impl(items, helper, 0, len(items))

def merge_sort_impl(items, helper, start, end):
    if (end - start) <= 1:
        return
    mid = (end - start) / 2 + start
    merge_sort_impl(items, helper, start, mid)
    merge_sort_impl(items, helper, mid, end)
    merge(items, helper, start, mid, end)

def merge(items, helper, start, mid, end):
    leftStart = start
    leftEnd = mid
    rightStart = mid
    rightEnd = end

    while leftStart < leftEnd and rightStart < rightEnd:
        if items[leftStart] < items[rightStart]:
            helper.append(items[leftStart])
            leftStart += 1
        else:
            helper.append(items[rightStart])
            rightStart += 1

    while leftStart < leftEnd:
        helper.append(items[leftStart])
        leftStart += 1
    while rightStart < rightEnd:
        helper.append(items[rightStart])
        rightStart += 1

    cnt = len(helper)
    for i in range(0, cnt):
        items[start + i] = helper[i]

    del helper[:]
