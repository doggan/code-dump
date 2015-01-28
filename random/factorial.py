def factorial(n):
    if n == 0: return 1
    return n * factorial(n - 1)

def factorial_it(n):
    result = 1
    for i in xrange(1, n):
        result *= (i + 1)
    return result

count = 10

print "Recursive factorial:"
for i in xrange(0, count):
    print "{}! = {}".format(i, factorial(i))

print "Iterative factorial:"
for i in xrange(0, count):
    print "{}! = {}".format(i, factorial_it(i))
