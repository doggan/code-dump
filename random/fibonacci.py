def fibonacci(n):
    if n == 0: return 0
    if n == 1: return 1
    return fibonacci(n - 1) + fibonacci(n - 2)

def fibonacci_it(n):
    if n == 0: return 0
    if n == 1: return 1
    a = 0
    b = 1
    result = 0
    for i in xrange(1, n):
        result = a + b
        a = b
        b = result
    return result

count = 10

print "Recursive Fibonacci:"
for i in xrange(0, count):
    print "f({}) = {}".format(i, fibonacci(i))

print "Iterative Fibonacci:"
for i in xrange(0, count):
    print "f({}) = {}".format(i, fibonacci_it(i))
