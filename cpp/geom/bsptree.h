#ifndef BSPTREE_H
#define BSPTREE_H

#include <cmath>
#include <initializer_list>
#include <iosfwd>
#include <utility>
#include <vector>

class vector2d
{
public:

    static vector2d zero();

    vector2d( double x, double y );

    double      x() const;
    double      y() const;

private:

    double      x_;
    double      y_;
};
typedef vector2d point;

vector2d operator-( const vector2d & l, const vector2d & r );

double dot( const vector2d & l, const vector2d & r );

std::ostream & operator<<( std::ostream & os, const vector2d & p );

class line_segment
{
public:
    line_segment( const vector2d & p1, const vector2d & p2 );

    const vector2d &    p1() const;
    const vector2d &    p2() const;
    line_segment        normal() const;
    line_segment        normalize() const;
    line_segment        unit_normal() const;
    double              distance() const;
    double              distance_squared() const;


private:
    double              dx() const;
    double              dy() const;

private:
    vector2d        p1_;
    vector2d        p2_;
};

std::pair< bool, vector2d >
intersect( const line_segment & l, const line_segment & r );

class parametric_line
{
public:
    parametric_line( double m, double c );

private:

    double          m_;
    double          c_;
};

enum class orientation
{
    FRONT,
    BACK,
    COLLINEAR,
    SPANNING,
    UNKNOWN
};
std::ostream & operator<<( std::ostream & os, const orientation & o );

orientation point_side( const line_segment & l, const vector2d & p );
orientation line_side( const line_segment & l, const line_segment & r );

template< typename container_T >
orientation polygon_side( const line_segment & l, const container_T & p )
{
    bool front = false;
    bool back = false;

    for ( auto edge : p )
    {
        auto side1 = point_side( l, edge.p1() );
        if (side1 == orientation::FRONT )
            front = true;
        else if ( side1 == orientation::BACK )
            back = true;

        auto side2 = point_side( l, edge.p2() );
        if (side2 == orientation::FRONT )
            front = true;
        else if ( side2 == orientation::BACK )
            back = true;
    }

    if ( front && back )
        return orientation::SPANNING;
    else if ( front )
        return orientation::FRONT;
    else if ( back )
        return orientation::BACK;
    else
        return orientation::COLLINEAR;
}

std::ostream & operator<<( std::ostream & os, const line_segment & ls );


class polygon
{
public:

    typedef std::vector< line_segment >             container_type;
    typedef typename container_type::const_iterator const_iterator;

    template< typename container_T >
    polygon( const container_T & c )

        : edges_( c.begin(), c.end() )
    {}

    const_iterator      begin() const;
    const_iterator      end() const;

private:

    container_type      edges_;
};


class node
{
public:
    typedef std::vector< polygon > container_type;

private:
    line_segment        partition_;
    node *              left_;
    node *              right_;

    container_type      polygons_;
};

class bsp_tree
{
public:
    explicit bsp_tree( const node & root );

    const node &        root() const;
private:
};

bool is_small( double d );



#endif // BSPTREE_H
