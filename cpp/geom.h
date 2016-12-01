#ifndef QUADTREE_GEOM_H
#define QUADTREE_GEOM_H

#include <algorithm>
#include <complex>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

/**
 * A generalised 2d point class, used by quad_tree.
 **/
template< typename T >
class point
{
public:

    static point zero()
    {
        return point( T(), T() );
    }

    point( const T & x, const T & y )

        : m_x( x )
        , m_y( y )
    {}

    const T & x() const         { return m_x; }
    const T & y() const         { return m_y; }

private:

    T m_x;
    T m_y;
};

/**
 * Output stream operator specialised for point<T> types.
 */
template< typename T >
std::ostream & operator<<( std::ostream & os, const point<T> & r )
{
    os << "<point x:" << r.x() << " y:" << r.y() << ">";
    return os;
}

/**
 * A generalised 2d line_segment class.
 *
 * Where line segment is defined by two endpoints.
 *
 */
template< typename T >
class line_segment
{
public:

    typedef point< T >              point_type;

    template< typename U >
    line_segment( const U & p1, const U & p2 )

        : m_p1( { p1.x(), p1.y() } )
        , m_p2( { p2.x(), p2.y() } )
    {}

    line_segment( const point_type & p1, const point_type & p2 )

        : m_p1( p1 )
        , m_p2( p2 )
    {}

    const point_type &  p1() const  { return m_p1; }
    const point_type &  p2() const  { return m_p2; }

private:

    point_type  m_p1;
    point_type  m_p2;
};

/**
 * A generalised 2d rectangle class, used by quad_tree to facilitate
 * quick look-up of line segments (for intersection testing).
 */
template< typename T >
class rectangle
{
public:

    typedef point< T >              point_type;
    typedef line_segment< T >       line_type;
    typedef const line_type *       const_iterator;

    // construct from 2 end-points (form a bounding box)
    template< typename U >
    rectangle( const U & p1, const U & p2 )

        : rectangle( std::min( p1.x(), p2.x() ), std::min( p1.y(), p2.y() ),
                     std::abs( p2.x() - p1.x() ), std::abs( p2.y() - p1.y() ) )
    {
    }

    rectangle( const T & x, const T & y, const T & w, const T & h )

        : m_x( x )
        , m_y( y )
        , m_w( w )
        , m_h( h )
        , m_edges( {line_type( point_type{ x, y }, point_type{ x + w, y } ),
                    line_type( point_type{ x + w, y }, point_type{ x + w, y + h } ),
                    line_type( point_type{ x + w, y + h }, point_type{ x, y + h } ),
                    line_type( point_type{ x, y + h }, point_type{ x, y } ) } )
    {

    }

    const_iterator begin() const
    {
        return m_edges;
    }

    const_iterator end() const
    {
        return m_edges + 4;
    }

    // accessors
    const T & x() const         { return m_x; }
    const T & y() const         { return m_y; }
    const T & width() const     { return m_w; }
    const T & height() const    { return m_h; }

private:

    T               m_x;
    T               m_y;
    T               m_w;
    T               m_h;

    line_type       m_edges[4];
};

/**
 * Output stream operator specialised for rectangle<T> types.
 */
template< typename T >
std::ostream & operator<<( std::ostream & os, const rectangle<T> & r )
{
    os << "<rectangle x:" << r.x() << " y:" << r.y()
       << " width:" << r.width() << " height:" << r.height() << ">";
    return os;
}

/**
 * Convenience function.
 */
template< template <typename> class Point_type, typename T >
rectangle< T > make_bounding_box( const Point_type< T > & p1, const Point_type< T > & p2 )
{
    return typename rectangle< T >::rectangle( p1, p2 );
}

/**
 * Convenience function to obtain a collection of points which represent
 * the vertices of a rectangle, from a rectangle instance.
 */
template< template <typename> class Rectangle_type, typename T >
std::vector< point< T > >
rectangle_corners( const Rectangle_type<T> & r )
{
    typedef point< T > point_type;
    point_type p[ 4 ] = {   point_type( r.x(), r.y() ),
                            point_type( r.x(), r.y() + r.height() ),
                            point_type( r.x() + r.width(), r.y() ),
                            point_type( r.x() + r.width(), r.y() + r.height() ) };

    return std::vector< point_type >( { p[0], p[1], p[2], p[3] } );
}

/**
 * Determine whether the supplied number is close enough to zero.
 */
bool is_small( int );
bool is_small( float );
bool is_small( double );

/**
 * Find the intersection point of two line segments (if it exists).
 *
 * @param l First line segment.
 * @param r Second line segment.
 *
 * The line segments must have the same underlying type.
 *
 * @return std::pair<bool, pointT>, where bool indicates if an intersection
 * was found, and pointT is the point of intersection (with underlying type T).
 */
template< typename T >
std::pair< bool, point< T > >
intersect( const line_segment< T > & l, const line_segment< T > & r )
{
    T x1 = l.p1().x();
    T x2 = l.p2().x();
    T x3 = r.p1().x();
    T x4 = r.p2().x();
    T y1 = l.p1().y();
    T y2 = l.p2().y();
    T y3 = r.p1().y();
    T y4 = r.p2().y();

    T d = ( x1 - x2 ) * ( y3 - y4 ) - ( y1 - y2 ) * ( x3 - x4 );

    if ( is_small( d )) return std::make_pair( false, point<T>::zero() );

    T pre = x1 * y2 - y1 * x2;
    T post = x3 * y4 - y3 * x4;
    T x = ( pre * ( x3 - x4 ) - ( x1 - x2 ) * post ) / d;
    T y = ( pre * ( y3 - y4 ) - ( y1 - y2 ) * post ) / d;

    if ( x < std::min( x1, x2 ) || x > std::max( x1, x2 ) ||
         x < std::min( x3, x4 ) || x > std::max( x3, x4 ))
        return std::make_pair( false, point<T>::zero() );

    if ( y < std::min( y1, y2 ) || y > std::max( y1, y2 ) ||
         y < std::min( y3, y4 ) || y > std::max( y3, y4 ))
        return std::make_pair( false, point<T>::zero() );

    return std::make_pair( true, point<T>( x, y ) );
}

template< typename T >
bool has_intersect( const line_segment< T > & l, const line_segment< T > & r )
{
    T x1 = l.p1().x();
    T x2 = l.p2().x();
    T x3 = r.p1().x();
    T x4 = r.p2().x();
    T y1 = l.p1().y();
    T y2 = l.p2().y();
    T y3 = r.p1().y();
    T y4 = r.p2().y();

    T d = ( x1 - x2 ) * ( y3 - y4 ) - ( y1 - y2 ) * ( x3 - x4 );

    if ( is_small( d )) return false;

    T pre = x1 * y2 - y1 * x2;
    T post = x3 * y4 - y3 * x4;
    T x = ( pre * ( x3 - x4 ) - ( x1 - x2 ) * post ) / d;
    T y = ( pre * ( y3 - y4 ) - ( y1 - y2 ) * post ) / d;

    if ( x < std::min( x1, x2 ) || x > std::max( x1, x2 ) ||
         x < std::min( x3, x4 ) || x > std::max( x3, x4 ))
        return false;

    if ( y < std::min( y1, y2 ) || y > std::max( y1, y2 ) ||
         y < std::min( y3, y4 ) || y > std::max( y3, y4 ))
        return false;

    return true;
}


#endif // GEOM_H
