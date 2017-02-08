#ifndef COORD_H_INCLUDED
#define COORD_H_INCLUDED

#include <ostream>
#include <string>

struct point_2d
{
    point_2d() : x( 0 ), y( 0 ) {}
    point_2d( double x_, double y_ ) : x( x_ ), y( y_ ) {}

    point_2d & operator*=( const point_2d & rhs )
    {
        x *= rhs.x;
        y *= rhs.y;

        return *this;
    }

    point_2d & operator+=( const point_2d & rhs )
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    point_2d & operator-=( const point_2d & rhs )
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    double x;
    double y;
};

point_2d operator-( const point_2d & lhs, const point_2d & rhs )
{
    return point_2d( lhs.x - rhs.x, lhs.y - rhs.y );
}

point_2d operator/( const point_2d & lhs, const point_2d & rhs )
{
    return point_2d( lhs.x / rhs.x, lhs.y / rhs.y );
}

std::ostream & operator<<( std::ostream & os, const point_2d & p )
{
    os << "<point2d (" << p.x << ", " << p.y << ")";
    return os;
}

class coord_system_2d
{
public:

    explicit coord_system_2d( const std::string & name, coord_system_2d * parent = nullptr )

        : name_     ( name )
        , parent_   ( parent )
    {}

    const point_2d & origin() const
    {
        return origin_;
    }

    const point_2d & extent() const
    {
        return extent_;
    }

    const coord_system_2d * parent() const
    {
        return parent_;
    }

    void set_origin( double x, double y )
    {
        origin_ = point_2d( x, y );
    }

    void set_extent( double x, double y )
    {
        extent_ = point_2d( x, y );
    }

private:

    std::string         name_;
    coord_system_2d *   parent_;

    point_2d            origin_;
    point_2d            extent_;
    double              rotation_;
};

point_2d transform_point( const point_2d & p, const coord_system_2d & to, const coord_system_2d & from )
{
    point_2d offset = from.origin() - to.origin();
    point_2d scale_factor = to.extent() / from.extent();

    point_2d result( p );
    result *= scale_factor;
    result += offset;

    return result;
}

point_2d to_parent_cs( const point_2d & p, const coord_system_2d & from )
{
    coord_system_2d const * parent = from.parent();

    if ( !parent )
        parent = &from;

    return transform_point( p, *parent, from );
}


#endif
