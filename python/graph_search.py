#!/usr/bin/env python

import copy
import heap

class Node(object):

    def __init__(self, name ):
        
        self._name = name._neighbours = []

    def __get_name__(self):
        return self._name
    name = property(__get_name__)

    def add_neighbour(self, node):
        self._neighbours.append(node)

    def __get_neighbours__(self):
        return self._neighbours
    neighbours = property(__get_neighbours__)
    

def foo(n):
    print n.name

class path_finder:

    DFS = -1 # accesses the 'stack' as a stack
    BFS = 0 # accesses the 'stack' as a queue

    def __init__(self, goal, strategy):
        self._visited = []
        self._goal = goal
        self._path_found = False
        self._strategy = strategy

    def visit(self, node):

        stack = []
        
        stack.append(node)

        while stack:

            top = stack[self._strategy]
            stack.pop(self._strategy)

            if top in self._visited:
                pass
            else:
                self._visited.append(top)

                if top.name == self._goal.name:
                    self._path_found = True
                    return
                
                for n in top.neighbours:
                    stack.append(n)

    def __visited__(self, node):
        return node in self._visited

    def __get_visited__(self):
        return self._visited
    visited = property(__get_visited__)

    def __get_path__(self):
        return self._path
    path = property(__get_path__)

    def __path_found__(self):
        return self._path_found
    path_found = property(__path_found__)


def run_path_finder():

    n = Node("root")
    nn = Node("north")
    ns = Node("south")
    ne = Node("east")
    nw = Node("west")

    n.add_neighbour(nn)
    n.add_neighbour(ns)
    n.add_neighbour(ne)
    n.add_neighbour(nw)

    nnn = Node("north of north")
    nn.add_neighbour(nnn)

    nnnn = Node("north of north of north")
    nnn.add_neighbour(nnnn)

    nee = Node("east of east")
    ne.add_neighbour(nee)
    nen = Node("north of east")
    ne.add_neighbour(nen)

    nenw = Node("west of north of east")
    nen.add_neighbour(nenw)

    nnnn.add_neighbour(nen)

    target = nnnn
    start = n

    v = path_finder(target, path_finder.DFS)
    v.visit(start)
    
    if v.path_found:
        print "Path found from '%s' to '%s'" % (start.name, target.name)
        print [x.name for x in v.visited]
    else:
        print "No path found from '%s' to '%s'" % (start.name, target.name)


class Graph(object):

    def __init__(self, name):
        
        self._name = name
        self._connectivity = {}

    def has_node(self, node):
        return self._connectivity.has_key(node)

    def nodes_from(self, node):

        return self._connectivity[node]

    def add_node(self, node):

        self._connectivity[node] = None

    def add_directed_connection(self, n1, n2, cost = 999):

        try:
            self._connectivity[n1][n2] = cost
        except KeyError:
            self._connectivity[n1] = { n2 : cost }

    def add_undirected_connection(self, n1, n2, cost = 999):

        self.add_directed_connection(n1, n2, cost)
        self.add_directed_connection(n2, n1, cost)

    def cost(self, n1, n2):
        
        return self.nodes_from(n1)[n2]

    def path_cost(self, path):

        cost = 0
        if len(path) > 1:

            for i in range(len(path) - 1):
            
                cost += self.cost(path[i], path[i+1])
            
        return cost

    def __iter__(self):
        
        for node in self._connectivity.keys():
            yield node

    def __str__(self):

        s = "<Graph name:%s\n" % self._name

        for c1, c2 in self._connectivity.items():

            for dest in c2.items():
                s += "\t%s->%s (%s)\n" % (c1, dest[0], dest[1])

        s += ">"

        return s
    

def find_path(graph, start, end, path = []):

    path = path + [start]

    if start == end:
        return path

    if not graph.has_node(start):
        return None

    for node, cost in graph.nodes_from(start):

        if node not in path:
            newpath = find_path(graph, node, end, path)
            if newpath: return newpath

    return None

def find_shortest_path(graph, start, end, path = []):
    
    path = path + [start]

    if start == end:
        return path

    if not graph.has_node(start):
        return None

    shortest = None

    for node, cost in graph.nodes_from(start).items():

        if node not in path:
            newpath = find_shortest_path(graph, node, end, path)

            if newpath:
                if not shortest or len(newpath) < len(shortest):
                    shortest = newpath

    return shortest

def find_least_cost_path(graph, start, end, path = []):

    path = path + [start]

    if start == end:
        return path

    if not graph.has_node(start):
        return None

    shortest = None

    for node, cost in graph.nodes_from(start).items():
        
        if node not in path:
            newpath = find_least_cost_path(graph, node, end, path)

            if newpath:
                if not shortest or graph.path_cost(newpath) < graph.path_cost(shortest):
                    shortest = newpath

    return shortest

def find_all_paths(graph, start, end, path = []):

    path = path + [start]

    if start == end:
        return [path]

    if not graph.has_node(start):
        return []

    paths = []

    for node, cost in graph.nodes_from(start).items():
        
        if node not in path:
            newpaths = find_all_paths(graph, node, end, path)
            for newpath in newpaths:
                paths.append(newpath)
    
    return paths

def node_min_compare(l, r):
    if l[1] == r[1]: return 0
    elif l[1] > r[1]: return 1
    else: return -1

def dfs(graph, start, end):

    s = []
    s.append(start)

    visited = []

    while s:

        top = s[-1]
        s.pop(-1)

        visited.append(top)
        
        if top == end:
            return visited

        neighbours = graph.nodes_from(top)

        s = []
        for n in neighbours:
            if not n in visited:
                s.append(n)

    return []

def bfs(graph, start, end):

    q = []
    q.append(start)

    visited = []

    while q:

        top = q[0]
        q.pop(0)

        visited.append(top)
        
        if top == end:
            return visited

        neighbours = graph.nodes_from(top)

        #q = []
        for n in neighbours:
            if not n in visited:
                q.append(n)

    return []

def path(items, end):
    if items[end]:
        return path(items, items[end]) + [end]
    else:
        return [end]

def dijkstra(graph, start, end):

    if start not in graph: return []
    if end not in graph: return []

    dist = {}
    prev = {}
    f = []

    def get_key(elem):
        return elem[0]
    def set_cost(elem,k):
        elem[1]=k

    u = heap.Heap(node_min_compare, get_key, set_cost)

    for node in graph:
        dist[node] = float("inf")
        u.insert([node, float("inf")])
        prev[node] = None

    dist[start] = 0
    u.decrease_key(start,0)

    while u:

        top = u.top()[0]
        u.pop()

        f.append(top)

        if top == end:
            return path(prev,end), dist[end]

        for n, c in graph.nodes_from(top).items():
            if ( dist[top] + graph.cost(top,n) < dist[n]):
                dist[n] = dist[top] + graph.cost(top,n)
                u.decrease_key(n, dist[n])
                prev[n] = top
    
def make_graph(name):

    g = Graph(name)

    g.add_undirected_connection("Leeds", "Birmingham", 120)
    g.add_undirected_connection("York", "Leeds", 27)
    g.add_undirected_connection("York", "Newcastle", 85)
    g.add_undirected_connection("Newcastle", "Edinburgh", 118)
    g.add_undirected_connection("Edinburgh", "Glasgow", 47)
    g.add_undirected_connection("Birmingham", "Bristol", 88)
    g.add_undirected_connection("Birmingham", "London", 118)
    g.add_undirected_connection("Bristol", "London", 120)
    g.add_undirected_connection("London", "Paris", 283)
    g.add_undirected_connection("Paris", "Berlin", 652)
    g.add_undirected_connection("Glasgow", "Carlisle", 97)
    g.add_undirected_connection("Carlisle", "Manchester", 120)
    g.add_undirected_connection("Manchester", "Birmingham", 90)
    g.add_undirected_connection("Birmingham", "Coventry", 25)
    g.add_undirected_connection("Coventry", "London", 96)
    g.add_undirected_connection("Paris", "Brussels", 190)

    return g
    
def print_path(path):
    print "%s %s miles" % ("->".join(path[0]), path[1])

def test_bfs():

    g = Graph("test_bfs")
    g.add_directed_connection( "Birmingham", "Bristol" )
    g.add_directed_connection( "Birmingham", "London" )
    g.add_directed_connection( "London", "Paris" )
    g.add_directed_connection( "Paris", "Berlin" )
    g.add_directed_connection( "Paris", "Brussels" )
    g.add_directed_connection( "Brussels", "Amsterdam" )
    g.add_directed_connection( "Amsterdam", "Berlin" )

    print bfs(g, "Birmingham", "Berlin")

def test_dfs():
    
    g = Graph("test_dfs")
    print dfs(g, "Birmingham", "Berlin")

def test_dijkstra():

    g = make_graph("test_dijkstra")
    
    print_path(dijkstra(g, "Birmingham", "Berlin"))
    print_path(dijkstra(g, "Birmingham", "Bristol"))
    print_path(dijkstra(g, "Bristol", "Paris"))
    print_path(dijkstra(g, "Berlin", "Bristol"))
    print_path(dijkstra(g, "Glasgow", "Berlin"))
    print_path(dijkstra(g, "Edinburgh", "London"))
    print_path(dijkstra(g, "Berlin", "Brussels"))
    

if __name__ == "__main__":

    test_dijkstra()
