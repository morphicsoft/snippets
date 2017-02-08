#ifndef DFS_H_INCLUDED
#define DFS_H_INCLUDED

#include "graph.h"

#include <iostream>
#include <map>
#include <stack>
#include <set>
#include <vector>

template< typename Id_type, typename Weight_type >
std::vector<Id_type> dfs( graph<Id_type, Weight_type > const & g, Id_type const & f, Id_type const & t )
{
    typedef std::vector<Id_type>                    path_type;
    typedef std::stack<path_type>                   stack_type;
    typedef std::set<Id_type>                       set_type;
    typedef graph<Id_type, Weight_type >            graph_type;
    typedef typename graph_type::neighbour_iterator iterator;

    using namespace std;

    stack_type  s;
    set_type    v;   // visited

    s.push( { f } );
    v.insert( f );

    while ( !s.empty() )
    {
        auto current_path = s.top();
        auto c = current_path[current_path.size()-1];
        v.insert( c );
        s.pop();

        if ( c == t ) return current_path;

        for ( auto it = g.neighbours_begin( c ); it != g.neighbours_end( c ); ++it )
        {
            if ( v.find( (*it).second ) == v.end() )
            {
                path_type new_path = current_path;
                new_path.push_back( (*it).second );
                s.push( new_path );
            }
        }
    }

    return {};
}

#endif // DFS_H_INCLUDED

