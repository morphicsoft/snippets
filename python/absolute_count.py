#!/usr/bin/env python

def absolute_count(l):

    if not l: return 0
    if len(l) == 1: return 1

    b = 0
    e = len(l) - 1

    count = 0
    while True:

        left = l[b]
        next_left = l[b+1]

        right = l[e]
        next_right = l[e-1]

        if -left > right:
            if left == next_left:
                b += 1
            else:
                count += 1
                b += 1
        elif -left < right:
            if right == next_right:
                e -= 1
            else:
                count += 1
                e -= 1
        else:
            if left < 0: b +=1
            elif right >= 0: e -= 1

        if left >= 0 and right < 0: break
        if b >= e:
            count += 1
            break

    return count

if __name__ == "__main__":

#    print absolute_count( [] )
#    print absolute_count( None )
    print absolute_count( [-5, -3, 0, 1, 3] )
    print absolute_count( [-5, -3, 0, 1, 3, 4, 6, 7] )
    print absolute_count( [-5] )
    print absolute_count( [9] )
    print absolute_count( [9,9,9])
    print absolute_count( [-5, -5])
    print absolute_count( [1,5,5,5,5])
    print absolute_count( [-9,-6,-5,-5,-5,-5,-3,-3,0,0,1,5,6,7,7,8])
