#!/usr/bin/env python

vertices = { 'A' : [ 'B', 'C', 'D' ],
             'B' : [ 'C', 'E', 'F', 'H' ],
             'C' : [],
             'D' : [ 'E' ],
             'E' : [ 'D' ],
             'F' : [ 'B' ],
             'G' : [ 'F' ],
             'H' : [ 'C', 'G' ] }

edges = { ( 'A', 'B' ) : 10,
          ( 'A', 'C' ) : 30,
          ( 'A', 'D' ) : 20,
          ( 'B', 'C' ) : 10,
          ( 'B', 'E' ) : 30,
          ( 'B', 'F' ) : 20,
          ( 'B', 'H' ) : 10,
          ( 'D', 'E' ) : 10,
          ( 'E', 'D' ) : 10,
          ( 'F', 'B' ) : 20,
          ( 'G', 'F' ) : 15,
          ( 'H', 'C' ) : 10,
          ( 'H', 'G' ) : 10 }

trace = True

def cost(v1, v2):
    
    if v1 == v2: return 0
    return edges.get( (v1, v2), float("inf" ))

def genericShortestPath( V, E, start ):

    '''
    A single-source shortest path implementation which examines
    vertices in an arbitrary order.
    '''

    dist = {}
    for v in V:
        dist[v] = float("inf")

    dist[start] = 0
    predecessor = {}

    toBeChecked = [start]

    while toBeChecked:

        if trace: print dist
        v = toBeChecked.pop(0)

        for u in V[v]:
            
            if dist[u] > dist[v] + cost(v,u):
                dist[u] = dist[v] + cost(v,u)
                predecessor[u] = v
                toBeChecked.append(u)

    return predecessor

def dijkstra( V, E, start ):

    from heapq import heappush, heappop

    dist = {}
    for v in V:
        dist[v] = float("inf")
    toBeChecked = []

    dist[start] = 0
    predecessor = {}

    heappush( toBeChecked, ( cost(start, start), start ) )

    while toBeChecked:

        if trace: print dist
        c, v = heappop( toBeChecked )

        for u in V[v]:
            
            if dist[u] > dist[v] + cost(v, u):
                dist[u] = dist[v] + cost(v, u)
                predecessor[u] = v
                heappush( toBeChecked, ( dist[u], u ) )

    return predecessor
    

trace = 0
print genericShortestPath( vertices, edges, 'A' )
print
print dijkstra( vertices, edges, 'A' )
