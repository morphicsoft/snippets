#ifndef BACKTRACK_H
#define BACKTRACK_H

#include "graph.h"

#include <algorithm>
#include <map>
#include <vector>

template<typename T>
std::vector<node<T> > build_path( std::map<node<T>, node<T> > const & parents, node<T> const & end, node<T> const & start )
{
    std::vector<node<T>> path;

    path.push_back( end );

    auto it = parents.find( end );

    while ( it != parents.end() )
    {
        path.push_back( (*it).second );
        it = parents.find( (*it).second );
    }

    std::reverse( path.begin(), path.end() );
    return path;
}


#endif // BACKTRACK_H
