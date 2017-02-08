#include "bfs.h"
#include "dijkstra.h"
#include "dfs.h"
#include "graph.h"

#include <iostream>
#include <string>

typedef node<std::string>       node_type;

int main()
{
    using namespace std;

    graph<string, int> g;
    g.add_node( "Birmingham" );
    g.add_node( "London" );
    g.add_node( "Paris" );
    g.add_node( "Brussels" );
    g.add_node( "Amsterdam" );
    g.add_node( "Bristol" );
    g.add_node( "Berlin" );

    g.add_edge( "Birmingham", "Bristol", 88 );
    g.add_edge( "Birmingham", "London", 118 );
    g.add_edge( "London", "Paris", 283 );
    g.add_edge( "Paris", "Berlin", 652 );
    g.add_edge( "Paris", "Brussels", 190 );
    g.add_edge( "Brussels", "Amsterdam", 107 );
    g.add_edge( "Amsterdam", "Berlin", 405 );

    clog << "Trying a BFS search\n";
    auto result = bfs( g, string("Birmingham"), string("Berlin") );
    if ( result.empty() )
    {
        clog << "Path not found\n";
    }
    else
    {
        clog << "Path found:\n";
        for ( auto r : result )
        {
            if ( r == "Berlin" )
                clog << r << "\n";
            else
                clog << r << "->";
        }
    }
    clog << "-------------------------------------------------\n";

    clog << "Trying a DFS search\n";
    auto result2 = dfs( g, string( "Birmingham"), string( "Berlin" ) );
    if ( result2.empty() )
    {
        clog << "Path not found\n";
    }
    else
    {
        clog << "Path found:\n";
        for ( auto r : result2 )
        {
            if ( r == "Berlin" )
                clog << r << "\n";
            else
                clog << r << "->";
        }
    }
    clog << "-------------------------------------------------\n";

    clog << "Trying a Dijkstra search\n";
    auto result3 = dijkstra( g, string( "Birmingham" ), string( "Berlin" ) );
    for ( auto v : result3 )
    {
        clog << "Shortest distance Birmingham to " << v.first << " = " << v.second << "\n";
    }
    clog << "-------------------------------------------------\n";
}
