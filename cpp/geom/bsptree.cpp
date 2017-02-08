#include "bsptree.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

vector2d vector2d::zero()
{
    return vector2d( 0.0, 0.0 );
}

vector2d::vector2d( double x, double y )

	: x_( x ), y_( y )
{}

double vector2d::x() const
{
    return x_;
}

double vector2d::y() const
{
    return y_;
}

double dot( const vector2d & l, const vector2d & r )
{
    return l.x() * r.x() + l.y() * r.y();
}

vector2d operator-( const vector2d & l, const vector2d & r )
{
    return vector2d( l.x() - r.x(), l.y() - r.y() );
}

std::ostream & operator<<( std::ostream & os, const vector2d & p )
{
    os << "<vector2d x:" << p.x() << " y:" << p.y() << ">";
    return os;
}

line_segment::line_segment( const vector2d & p1, const vector2d & p2 )

	: p1_( p1 ), p2_( p2 )
{}

const vector2d & line_segment::p1() const
{
    return p1_;
}

const vector2d & line_segment::p2() const
{
    return p2_;
}

line_segment line_segment::normal() const
{
    return line_segment( { -dy(), dx() }, { dy(), -dx() } );
}

line_segment line_segment::normalize() const
{
    auto d = distance();
    auto x = p1_.x() + dx() / d;
    auto y = p1_.y() + dy() / d;
    return line_segment( p1_, { x, y } );
}

line_segment line_segment::unit_normal() const
{
    return normal().normalize();
}

double line_segment::distance() const
{
    return std::sqrt( distance_squared() );
}

double line_segment::distance_squared() const
{
    return dx() * dx() + dy() * dy();
}

double line_segment::dx() const
{
    return p2_.x() - p1_.x();
}

double line_segment::dy() const
{
    return p2_.y() - p1_.y();
}

std::pair< bool, point >
intersect( const line_segment & l, const line_segment & r )
{
    double x1 = l.p1().x();
    double x2 = l.p2().x();
    double x3 = r.p1().x();
    double x4 = r.p2().x();
    double y1 = l.p1().y();
    double y2 = l.p2().y();
    double y3 = r.p1().y();
    double y4 = r.p2().y();

    double d = ( x1 - x2 ) * ( y3 - y4 ) - ( y1 - y2 ) * ( x3 - x4 );

    if ( is_small( d )) return std::make_pair( false, point::zero() );

    double pre = x1 * y2- y1 * x2;
    double post = x3 * y4 - y3 * x4;
    double x = ( pre * ( x3 - x4 ) - ( x1 - x2 ) * post ) / d;
    double y = ( pre * ( y3 - y4 ) - ( y1 - y2 ) * post ) / d;

    if ( x < std::min( x1, x2 ) || x > std::max( x1, x2 ) ||
         x < std::min( x3, x4 ) || x > std::max( x3, x4 ))
        return std::make_pair( false, point::zero() );

    if ( y < std::min( y1, y2 ) || y > std::max( y1, y2 ) ||
         y < std::min( y3, y4 ) || y > std::max( y3, y4 ))
        return std::make_pair( false, point::zero() );

    return std::make_pair( true, point( x, y ) );
}

std::ostream & operator<< ( std::ostream & os, const orientation & o )
{
    if      ( o == orientation::FRONT )     os << "FRONT";
    else if ( o == orientation::BACK )      os << "BACK";
    else if ( o == orientation::COLLINEAR ) os << "COLLINEAR";
    else if ( o == orientation::SPANNING )  os << "SPANNING";
    else                                    os << "UNKNOWN";

    return os;
}

orientation point_side( const line_segment & l, const vector2d & p )
{
    auto d = dot( p - l.p1(), l.normal().p2() );

    if      ( is_small( d ) )   return orientation::COLLINEAR;
    else if ( d > 0 )           return orientation::FRONT;
    else if ( d < 0 )           return orientation::BACK;
    else                        return orientation::UNKNOWN;

}

orientation line_side( const line_segment & l, const line_segment & r )
{
    return polygon_side( l, std::vector< line_segment >( { r } ) );
}

std::ostream & operator<<( std::ostream & os, const line_segment & ls )
{
    os << "<line_segment p1:" << ls.p1() << " p2:" << ls.p2() << ">";
    return os;
}

polygon::const_iterator polygon::begin() const
{
    return edges_.begin();
}

polygon::const_iterator polygon::end() const
{
    return edges_.end();
}

bool is_small( double d )
{
    return std::abs( d ) < std::numeric_limits<double>::epsilon();
}
