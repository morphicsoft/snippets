#!/usr/bin/env python

def make_graph():

    g = { "Birmingham": [ "Bristol", "London" ],
          "London": [ "Paris" ],
          "Paris": [ "Berlin", "Brussels" ],
          "Brussels": [ "Amsterdam" ],
          "Amsterdam": [ "Berlin" ] }

    return g
          

def bfs( graph, start, end ):

    q = []
    q.append([start])

    while q:
        path = q.pop(0)
        node = path[-1]

        if node == end:
            return path

        for n in graph.get(node, []):
            new_path = list(path)
            new_path.append(n)
            q.append(new_path)

    return []

def dfs( graph, start, end ):
    
    s = []
    s.append([start])

    while s:
        path = s.pop(-1)
        node = path[-1]

        if node == end:
            return path

        for n in graph.get(node, []):
            new_path = list(path)
            new_path.append(n)
            s.append(new_path)

    return []

if __name__ == "__main__":

    g = make_graph()
    print bfs( g, "Birmingham", "Berlin" )
    print dfs( g, "Birmingham", "Berlin" )
