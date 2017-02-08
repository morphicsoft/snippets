#!/usr/bin/env python

def bini(n):
    q = n
    s = ""
    while q > 0:
        q, r = divmod(q, 2)
        s = str(r) + s

    return "0" + s

def binfd(n):
    if n > 1: return

    s = ""

    max = 32;
    while n > 0 and max > 0:
        n = n * 2
        max -= 1
        if n >= 1:
            s += "1"
            n = n - 1
        else:
            s += "0"

    if n > 0: return s + "0..."
    else: return s + "0"

def binf(n):
    import math
    i = math.floor(n)
    f = n - i
    s = bini(int(i)) + "." + binfd(f)
    return s

def print_binf(n):
    print "%s -> %s" % (n, binf(n))

print_binf(3)
print_binf(100)
print_binf(0.7)
print_binf(0.49)
print_binf(0.5)
print_binf(0.51)

