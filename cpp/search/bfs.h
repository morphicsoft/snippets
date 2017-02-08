#ifndef BFS_H_INCLUDED
#define BFS_H_INCLUDED

#include "graph.h"

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

template< typename Id_type, typename Weight_type >
std::vector<Id_type> bfs( graph<Id_type, Weight_type > const & g, Id_type const & f, Id_type const & t )
{
    typedef std::vector<Id_type>                    path_type;
    typedef std::queue<path_type>                   queue_type;
    typedef std::set<Id_type>                       set_type;
    typedef graph<Id_type, Weight_type >            graph_type;
    typedef typename graph_type::neighbour_iterator iterator;

    using namespace std;

    queue_type  q;
    set_type    v;   // visited

    q.push( { f } );
    v.insert( f );

    while ( !q.empty() )
    {
        auto current_path = q.front();
        auto c = current_path[current_path.size()-1];
        v.insert( c );
        q.pop();

        if ( c == t ) return current_path;

        for ( auto it = g.neighbours_begin( c ); it != g.neighbours_end( c ); ++it )
        {
            if ( v.find( (*it).second ) == v.end() )
            {
                path_type new_path = current_path;
                new_path.push_back( (*it).second );
                q.push( new_path );
            }
        }
    }

    return {};
}

#endif
