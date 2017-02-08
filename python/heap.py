#!/usr/bin/env python

class Heap(object):

    # to do : create separate IndexedHeap class
    # OR... consider indexing the elements externally?

    def __init__(self, compare = cmp, key_getter = None, key_setter = None):
        self._heap = []
        self._compare = compare
        self._key = key_getter
        self._set = key_setter

        # n.b. indexing won't work if there are duplicate keys
        self.__build_index__()

    def top(self):
        return self._heap[0]

    def clear(self):
        self._heap = []
        self._index = {}

    def increase_key(self, k, new_k):
        if self._set:
            pos = self._index[k]
            elem = self._heap[pos]
            self._set(elem, new_k)
            self.__upheap__(pos)

    decrease_key = increase_key

    def insert(self, item):
        if self._key:
            key = self._key(item)
            self._index[self._key(item)] = len(self._heap)
        self._heap.append(item)
        if not len(self._index) == len(self):
            raise Exception("Index out of sync")
        self.__upheap__(len(self._heap)-1)

    def pop(self):
        if len(self) > 1:
            if self._key:
                itm1, itm2 = self._heap[0], self._heap[-1]
                idx1 = self._key(itm1)
                idx2 = self._key(itm2)
                self._index[idx1], self._index[idx2] = self._index[idx2], self._index[idx1]
            self._heap[0], self._heap[-1] = self._heap[-1], self._heap[0]
        
        rv = self._heap.pop()
        if self._key:
            del self._index[self._key(rv)]

        self.__downheap__(0)
        return rv

    def __build_index__(self):
        self._index = {}
        if self._key:
            for (count, elem) in enumerate(self._heap):
                self._index[self._key(elem)] = count

    def __contains__(self, item):
        return item in self._heap

    def __iter__(self):
        return iter(self._heap)

    def __len__(self):
        return len(self._heap)

    def len_index(self):
        return len(self._index)

    def __upheap__(self, item):

        p = self.__parent__(item)

        while item > -1 and p > -1 and self._compare( self._heap[item], self._heap[p] ) < 0:
            self.__swap_index__(item, p)
            self._heap[item], self._heap[p] = self._heap[p], self._heap[item]
            item = p
            p = self.__parent__(item)

    def __downheap__(self, item):
        
        left = item * 2 + 1
        right = item * 2 + 2
        smallest = item

        if left < len(self) and self._compare( self._heap[left], self._heap[smallest] ) < 0:
            smallest = left

        if right < len(self) and self._compare( self._heap[right], self._heap[smallest] ) < 0:
            smallest = right

        if not smallest == item:
            self.__swap_index__(item, smallest)
            self._heap[item], self._heap[smallest] = self._heap[smallest], self._heap[item]
            self.__downheap__(smallest)

    def __swap_index__(self, pos1, pos2):
        if self._key:
            itm1, itm2 = self._heap[pos1], self._heap[pos2]
            idx1, idx2 = self._key(itm1), self._key(itm2)
            self._index[idx1], self._index[idx2] = self._index[idx2], self._index[idx1]

    def __parent__(self, item):

        if item >= 0:
            return (item - 1) / 2
        else:
            return -1

    def __str__(self):
        return str(self._heap)

    def print_index(self):
        print "-" * 80
        for k, v in self._index.items():
            print k, v
        print "-" * 80

def print_heap(h):

    s = ""
    for item in h:
        s += "%s " % item

    print s

def min_compare(l,r):
    return l - r

def max_compare(l,r):
    return r - l


class C(object):
    def __init__(self, k, v):
        self._k = k
        self._cost = v

    def get_key(self):
        return self._k
    key = property(get_key)

    def get_cost(self):
        return self._cost
    def set_cost(self, cost):
        self._cost = cost
    cost = property(get_cost)

    def compare(self, other):
        return self.cost - other.cost

    def __str__(self):
        return "<C key:%s cost:%s>" % (self.key, self.cost)

if __name__ == "__main__":

    '''
    h = Heap(max_compare)
    h.insert(3)
    h.insert(2)
    h.insert(4)
    h.insert(8)
    h.insert(5)
    h.insert(1)
    h.insert(1)
    h.insert(7)
    h.insert(8)

    sz = len(h)
    for i in range(sz):
        print(h.pop())

    h.print_index()
    '''

    h2 = Heap(C.compare, C.get_key, C.set_cost)
    h2.insert(C("a", 3))
    h2.insert(C("b", 2))
    h2.insert(C("c", 4))
    h2.insert(C("d", 8))
    h2.insert(C("e", 5))
    h2.insert(C("f", 9))
    h2.insert(C("g", 1))
    h2.insert(C("h", 7))
    h2.insert(C("u", 6))

    h2.decrease_key("a", 0)
    h2.increase_key("u", 12)

    #h2.print_index()
    
    for i in range(len(h2)):
        print h2.pop()
