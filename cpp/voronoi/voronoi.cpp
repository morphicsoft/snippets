#include "voronoi.h"

#include <queue>

bool operator>( const point & lhs, const point & rhs )
{
    return lhs.x() == rhs.x() ? lhs.y() > rhs.y() : lhs.x() > rhs.x();
}

void voronoi::run()
{
    while ( !points_.empty() )
        if ( !events_.empty() && events_.top().x() <= points_.top().x() )
            process_event();
        else
            process_point();

    while ( !events_.empty() )
        process_event();

    //finish_edges();
}

void voronoi::push( const point & p )
{
    points_.push( p );
}

void voronoi::process_event()
{

}

void voronoi::process_point()
{
    point p = points_.top();
    points.pop();

    update_beachfront( p );
}

void voronoi::update_beachfront( const point & p )
{
    if ( arcs_.empty() )
    {
        arcs_.insert( p );
    }
    else
    {
        for ( auto arc : arcs_ )
        {
            auto intsct = intersect( p, arc );
            if ( intsct.first )
            {

            }
            else
            {

            }
        }
    }
}
