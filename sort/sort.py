#!/path/to/python

def merge_sort(items):
    """Merge sort using a single buffer as intermediate storage.
    """
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
    merge_impl(items, helper, start, mid, end)

def merge_impl(items, helper, start, mid, end):
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

def merge_sort_slice(items):
    """Merge sort using list slicing.
    """
    if items is None:
        return

    merge_sort_slice_impl(items)

def merge_sort_slice_impl(items):
    if len(items) <= 1:
        return
    mid = len(items) / 2
    leftHalf = items[:mid]
    rightHalf = items[mid:]
    merge_sort_slice_impl(leftHalf)
    merge_sort_slice_impl(rightHalf)
    merge_slice_impl(items, leftHalf, rightHalf)

def merge_slice_impl(items, leftHalf, rightHalf):
    leftCnt = 0
    rightCnt = 0
    cnt = 0
    while leftCnt < len(leftHalf) and rightCnt < len(rightHalf):
        if leftHalf[leftCnt] < rightHalf[rightCnt]:
            items[cnt] = leftHalf[leftCnt]
            leftCnt += 1
        else:
            items[cnt] = rightHalf[rightCnt]
            rightCnt += 1
        cnt +=1

    while leftCnt < len(leftHalf):
        items[cnt] = leftHalf[leftCnt]
        leftCnt += 1
        cnt += 1
    while rightCnt < len(rightHalf):
        items[cnt] = rightHalf[rightCnt]
        rightCnt += 1
        cnt += 1
