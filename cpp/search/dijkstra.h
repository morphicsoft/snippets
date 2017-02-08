#ifndef DIJKSTRA_H_INCLUDED
#define DIJKSTRA_H_INCLUDED

#include "graph.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <vector>

template< typename Id_type, typename Weight_type >
struct node_distance
{
    node_distance( Id_type id_, Weight_type distance_ )
        : id( id_ ), distance( distance_ )
    {}
    Id_type     id;
    Weight_type distance;
};

template< typename Id_type, typename Weight_type >
bool operator< ( node_distance< Id_type, Weight_type > const & l, node_distance< Id_type, Weight_type > const & r )
{
    return l.distance < r.distance;
}

template< typename Id_type, typename Weight_type >
bool operator> ( node_distance< Id_type, Weight_type > const & l, node_distance< Id_type, Weight_type > const & r )
{
    return l.distance > r.distance;
}


template< typename Id_type, typename Weight_type >
std::map< Id_type, Weight_type > dijkstra( graph<Id_type, Weight_type > const & g, Id_type const & f, Id_type const & t )
{
    typedef std::map< Id_type, Weight_type >            distance_map;
    typedef node_distance< Id_type, Weight_type >       node_distance_type;
    typedef std::greater< node_distance_type >          greater;
    typedef std::priority_queue< node_distance_type, std::vector<node_distance_type>, greater >
                                                        node_queue;
    typedef std::map< Id_type, Id_type >                predecessor_map;

    distance_map    dist;
    node_queue      q;
    predecessor_map prev;

    for ( auto n : g )
    {
        dist[n.id()] = std::numeric_limits<Weight_type>::max();
    }

    dist[f] = 0;

    q.push( { f, 0 } );

    while( !q.empty() )
    {
        auto v = q.top().id;
        q.pop();

        for ( auto it = g.neighbours_begin( v ); it != g.neighbours_end( v ); ++it )
        {
            auto u = (*it).second;
            if ( dist[u] > dist[v] + g.edge_weight( v, u ) )
            {
                dist[u] = dist[v] + g.edge_weight( v, u );
                q.push( { u, dist[u] } );
                prev[u] = v;
            }
        }
    }

    return dist;
}

#endif // DIJKSTRA_H
