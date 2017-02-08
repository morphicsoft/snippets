#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#include "geom.h"

#include <cstdlib>
#include <deque>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>


/**
 * @namespace detail
 * Private QuadTree details.
 */
namespace detail
{
/*
 * namespace detail contains helper structures, not intended for use outside of quad_tree
 */

    /**
     * template to determine if a class has a member function named 'width'
     */
    template< typename T, typename R >
    class has_width
    {
        template< typename U,  R (U::*)() const> struct check;
        template< typename U > static char func( check< U, &U::width> * );
        template< typename U > static int func(...);

    public:
        enum { value = sizeof( func<T>(0)) == sizeof(char) };
    };

    /**
     * template to determine if a class has a member function named width
     * of the expected return type.
     */
    template< typename T >
    class has_width_member
    {
        typedef decltype(((T*)nullptr)->x())  return_type;

    public:

        enum { value = has_width< T, return_type >::value };
    };


    // forward decl
    template< int, typename >
    struct index;

    template< int, typename >
    struct intersects;

    enum { POINT_TYPE, RECTANGLE_TYPE };
}

/**
 * quad_tree implementation
 *
 * quad_tree is a templated class, where T is the type of object that will be held within
 * the tree.
 * Stored objects may either be point data, having just x() and y() member functions, or
 * may be represented as rectangles, with x(), y(), width() and height() member functions,
 * where those 4 functions define the axis-aligned bounding-box of a shape.
 *
 */
template< typename T >
class quad_tree
{
    static const int npos = -1;

    typedef T                                           object_type;
    typedef quad_tree< T >                              this_type;
    typedef const quad_tree< T >*                       const_ptr;
    typedef std::vector< T >                            container_type;
    typedef typename container_type::iterator           iterator;
    typedef std::unique_ptr< this_type >                unique_ptr;
    typedef decltype(((T*)nullptr)->x())                point_data_type;

public:

    typedef T                                           value_type;
    typedef point< point_data_type >                    point_type;
    typedef line_segment< point_data_type >             line_type;
    typedef rectangle< point_data_type >                rectangle_type;
    typedef std::vector< T >                            result_type;
    typedef typename result_type::const_iterator        result_iterator;
    typedef unsigned                                    size_type;

    quad_tree( const rectangle_type & bounds, int max_levels, int max_objects )

        : quad_tree( "00", bounds, 0, max_levels, max_objects )
    {
    }

    quad_tree( const quad_tree & ) = delete;
    quad_tree & operator=( const quad_tree & ) = delete;

    const std::string & id() const
    {
        return m_id;
    }

    const rectangle_type & bounds() const
    {
        return m_bounds;
    }

    /**
     * @brief is_leaf
     * @return true if this quad_tree has no children
     */
    bool is_leaf() const
    {
        return m_children.empty();
    }

    /**
     * @brief size
     * @return the held object count of this quad_tree plus all of
     * its children (recursively).
     */
    size_type size() const
    {
        size_type sz = m_objects.size();

        for ( const auto & child : m_children )

            sz += child->size();

        return sz;
    }

    /**
     * @brief retrieve
     *
     * Returns (by ref) a collection of contained objects which are (or may be) close
     * to the supplied object, based on the sub-node(s) of the tree intersected by the
     * object.
     */
    template< typename Object_type >
    void retrieve( const Object_type & r, result_type & result ) const
    {
        if ( !m_children.empty() )
        {
            int indices = intersects( r );

            for ( int i = 0; i < 4; ++i )
            {
                if ( indices & (1<<i) )
                {
                    m_children[ i ]->retrieve( r, result );
                }
            }
        }

        for ( const T & obj : m_objects)
        {
            result.push_back( obj );
        }
    }

    /**
     * @brief for_each_match
     *
     * Calls the supplied function for each object near to the BB of the supplied
     * object. Recursively checks which quad node(s) the supplied shape fits within
     * or overlaps, and invokes the supplied function if so.
     *
     * @param r The object to test against.
     * @param f Callback function accepting an argument of type Object_type.
     */
    template< typename Object_type, typename Functor_type >
    void for_each_match( const Object_type & r, const Functor_type & f ) const
    {
        if ( !is_leaf() )
        {
            int indices = intersects( r );

            for ( int i = 0; i < 4; ++i )
            {
                if ( indices & (1<<i) )
                {
                    m_children[ i ]->for_each_match( r, f );
                }
            }
        }

        for ( const T & obj : m_objects )
        {
            f( obj );
        }
    }

    /**
     * @brief for_each_match_iterative
     *
     * An iterative implementation of for_each_match.
     *
     * @param r The object to test against.
     * @param f Callback function accepting an argument of type Object_type.
     *
     */
    template< typename Object_type, typename Functor_type >
    void for_each_match_iterative( const Object_type & r, const Functor_type & f ) const
    {
        if ( is_leaf() ) return;

        std::deque< const_ptr > unvisited;

        unvisited.push_back( this );

        while( !unvisited.empty() )
        {
            const_ptr current = unvisited.back();
            unvisited.pop_back();

            if ( !current->is_leaf() )
            {
                int indices = current->intersects( r );

                for ( int i = 0; i < 4; ++i )
                {
                    if ( indices & (1<<i) )
                    {
                        unvisited.push_back( current->m_children[ i ].get() );
                    }
                }

            }

            for ( const T & obj : current->m_objects )
            {
                f( obj );
            }
        }
    }

    /**
     *
     * @brief line_intersect
     *
     * Provides same functionality as for_each_match, but with different
     * lookup algorithm which recursively checks if the supplied
     * line either intersects or has an endpoint within each quad node
     * and invokes supplied function if so. Hence disregarding any nodes
     * that the line does not pass through. This is recommended for long
     * line segments where the number of intersected quads is expected to be
     * significantly lower than found with the bounding-box lookup.
     *
     * @param l The line segment object to test against.
     * @param f Callback function accepting an argument of type Object_type.
     */
    template< typename Functor_type >
    void line_intersect( const line_type & l, const Functor_type & f ) const
    {
        // inefficient to generate an AABB around a long line, so test which
        // nodes are actually intersected by the given line

        bool intersects = false;

        for ( auto it = m_bounds.begin(); !intersects && it != m_bounds.end(); ++it )
        {
            intersects = has_intersect( l, *it );
        }

        if ( intersects || in_bounds( l.p1() ) || in_bounds( l.p2() ))
        {
            for ( const auto & child : m_children )
            {
                child->line_intersect( l, f );
            }

            for ( const T & obj : m_objects )
            {
                f( obj );
            }
        }
    }

    /**
     *
     * @brief first_line_intersect
     *
     * Provides same functionality as line_intersect, but accepts a function
     * object, which is assumed to expose a 'bool active()' member function.
     * This enables early return, for example when a first actual collision
     * is detected and no further tests are needed, thus saving continued
     * line-line intersection tests against further quad boundaries.
     *
     * @param l The line segment object to test against.
     * @param f Function object accepting an argument of type Object_type
     *          and exposing a member function 'bool active()'.
     */
    template< typename Functor_type >
    void first_line_intersect( const line_type & l, const Functor_type & f ) const
    {
        // inefficient to generate an AABB around a long line, so test which
        // nodes are actually intersected by the given line

        if ( !f.active() ) return;

        bool intersects = false;

        for ( auto it = m_bounds.begin(); !intersects && it != m_bounds.end(); ++it )
        {
            intersects = has_intersect( l, *it );
        }

        if ( intersects || in_bounds( l.p1() ) || in_bounds( l.p2() ))
        {
            for ( const auto & child : m_children )
            {
                if ( f.active() )
                    child->first_line_intersect( l, f );
            }

            for ( const T & obj : m_objects )
            {
                f( obj );
            }
        }
    }

    /**
     *
     * @brief line_intersect_iterative
     *
     * An iterative implementation of line_intersect.
     *
     * @param l The line segment object to test against.
     * @param f Callback function accepting an argument of type Object_type.
     */
    template< typename Functor_type >
    void line_intersect_iterative( const line_type & l, const Functor_type & f ) const
    {
        std::deque< const_ptr > unvisited;
        std::deque< const_ptr > intersected;

        const_ptr current_node = this;

        unvisited.push_back( this );

        while( !unvisited.empty() )
        {
            const_ptr current = unvisited.back();
            unvisited.pop_back();

            bool intersects = false;

            for ( auto it = m_bounds.begin(); !intersects && it != m_bounds.end(); ++it )
            {
                intersects = has_intersect( l, *it );
            }

            if ( intersects || in_bounds( l.p1() ) || in_bounds( l.p2() ))
            {
                intersected.push_back( current );

                for ( const auto & child : current->m_children )
                {
                    unvisited.push_back( child.get() );
                }
            }

        }

        for ( auto i : intersected )
        {
            for ( const T & obj : i->m_objects )
            {
                f( obj );
            }
        }
    }

    // mutators

    /**
     *
     * @brief insert
     *
     * Insert an object into the correct node of the quad tree
     *
     * @param v an instance of the quad_tree's value_type.
     *
     */
    void insert( const value_type & v )
    {
        if ( !m_children.empty() )
        {
            int idx = index( v );

            if ( idx != npos )
            {
                m_children[ idx ]->insert( v );
                return;
            }
        }

        m_objects.push_back( v );

        if ( m_objects.size() > m_max_objects && m_level < m_max_levels )
        {
            if ( m_children.empty() )
                split();

            for ( iterator it = begin(); it != end(); )
            {
                int idx = index( *it );
                if ( idx != npos )
                {
                    m_children[ idx ]->insert( *it );
                    it = m_objects.erase( it );
                }
                else
                {
                    ++it;
                }
            }

        }
    }

    /**
     *
     * @brief clear
     *
     * Clear the entire quad tree
     */
    void clear()
    {
        m_objects.clear();

        for ( quad_tree * q : m_children )
        {
            if( q ) q->clear();
        }
    }

private:

    static unique_ptr create( const std::string & id, const rectangle_type & bounds, size_type level, int max_levels, int max_objects )
    {
        return unique_ptr( new quad_tree( id, bounds, level, max_levels, max_objects ));
    }

    unique_ptr create( const std::string & id, const rectangle_type & r ) const
    {
        return create( id, r, m_level + 1, m_max_levels, m_max_objects );
    }

    quad_tree( const std::string & id, const rectangle_type & bounds, size_type level, int max_levels, int max_objects )

        : m_id( id )
        , m_bounds( bounds )
        , m_level( level )
        , m_max_levels( max_levels )
        , m_max_objects( max_objects )
    {
    }

    friend class detail::index<detail::POINT_TYPE, point_data_type>;
    friend class detail::index<detail::RECTANGLE_TYPE, point_data_type>;
    friend class detail::intersects<detail::POINT_TYPE, point_data_type>;
    friend class detail::intersects<detail::RECTANGLE_TYPE, point_data_type>;

    iterator begin()
    {
        return m_objects.begin();
    }

    iterator end()
    {
        return m_objects.end();
    }

    template< typename Obj_type >
    int index( const Obj_type & obj ) const
    {
        return detail::index< detail::has_width_member< Obj_type >::value, point_data_type >()( this, obj );
    }

    template< typename Obj_type >
    int intersects( const Obj_type & obj ) const
    {
        return detail::intersects< detail::has_width_member< Obj_type >::value, point_data_type >()( this, obj );
    }

    bool in_bounds( const point_type & p ) const
    {
        return ( p.x() >= m_bounds.x() && p.x() <= m_bounds.x() + m_bounds.width() ) &&
               ( p.y() >= m_bounds.y() && p.y() <= m_bounds.y() + m_bounds.height() );
    }

    void split()
    {
        point_data_type half_width = m_bounds.width() / 2;
        point_data_type half_height = m_bounds.height() / 2;
        m_children.push_back( create( std::string( m_id + ".00" ), rectangle_type( m_bounds.x(), m_bounds.y(), half_width, half_height )));
        m_children.push_back( create( std::string( m_id + ".01" ), rectangle_type( m_bounds.x() + half_width, m_bounds.y(), half_width, half_height )));
        m_children.push_back( create( std::string( m_id + ".10" ), rectangle_type( m_bounds.x() + half_width, m_bounds.y() + half_height, half_width, half_height )));
        m_children.push_back( create( std::string( m_id + ".11" ), rectangle_type( m_bounds.x(), m_bounds.y() + half_height, half_width, half_height )));
    }

private:
    std::string                 m_id;
    rectangle_type              m_bounds;
    size_type                   m_level;
    size_type                   m_max_levels;
    size_type                   m_max_objects;
    std::vector< unique_ptr >   m_children;
    container_type              m_objects;
};

/**
 * @brief Output stream operator specialised for quad_tree types.
 */
template< typename T >
std::ostream & operator<<( std::ostream & os, const quad_tree< T > & q )
{
    os << "<quadtree id:" << q.id() << ">";
    return os;
}

/**
 * @namespace detail
 */
namespace detail
{
    /*
     * quad_tree helper functions, which operate on either rectangular data
     * structures or point data structures
     */

    /**
     * @brief index
     *
     * Locate the quad node index of a rectangle, or -1 if the recangle
     * doesn't fully fit into one quad.
     */
    template<typename Data_type>
    struct index<RECTANGLE_TYPE, Data_type>
    {
        template< typename T, typename Rect_type >
        int operator()( const quad_tree<T> * q, const Rect_type & r ) const
        {
            int result = quad_tree<T>::npos;

            Data_type x_midpoint = q->m_bounds.x() + q->m_bounds.width() / 2;
            Data_type y_midpoint = q->m_bounds.y() + q->m_bounds.height() / 2;

            bool bottom_half = r.y() < y_midpoint && r.y() + r.height() < y_midpoint;
            bool top_half = r.y() >= y_midpoint;

            // fits in left half:
            if ( ( r.x() < x_midpoint ) && ( r.x() + r.width() < x_midpoint ) )
            {
                if ( top_half )         result = 3;
                else if ( bottom_half ) result = 0;
            }
            // fits in right half:
            else if ( r.x() >= x_midpoint )
            {
                if ( top_half )         result = 2;
                else if ( bottom_half ) result = 1;
            }

            return result;
        }

    };

    /**
     * @brief intersects
     *
     * Locate all the indices of quads which are intersected by the supplied shape
     */
    template< typename Data_type >
    struct intersects<RECTANGLE_TYPE, Data_type>
    {
        template< typename T, typename Rect_type >
        int operator()( const quad_tree<T> * q, const Rect_type & r ) const
        {
            typedef typename quad_tree< T >::point_type point_type;

            std::vector< point_type > corners = rectangle_corners( r );

            int result = 0;

            for ( const auto & p : corners )
            {
                int idx = q->index( p );

                if ( idx != quad_tree<T>::npos ) result |= (1<<idx);
            }

            return result;
        }
    };



    /**
     * @brief index
     *
     * Locate the quad node index of a point. A point should always fit into
     * a quad.
     */
    template<typename Data_type>
    struct index<POINT_TYPE, Data_type>
    {
        template< typename T, typename Point_type >
        int operator()( const quad_tree<T> * q, const Point_type & p ) const
        {
            int result = quad_tree<T>::npos;

            Data_type x_midpoint = q->m_bounds.x() + q->m_bounds.width() / 2;
            Data_type y_midpoint = q->m_bounds.y() + q->m_bounds.height() / 2;

            bool bottom_half = p.y() < y_midpoint;
            bool top_half = p.y() >= y_midpoint;

            // left half is between x and x_midpoint
            if ( p.x() < x_midpoint )
            {
                if ( top_half )
                    result = 3;
                else if ( bottom_half )
                    result = 0;
            }
            // right half is between x_midpoint and x + width
            else if ( p.x() >= x_midpoint )
            {
                if ( top_half )
                    result = 2;
                else if ( bottom_half )
                    result = 1;
            }

            return result;
        }
    };

    /**
     * @brief intersects
     *
     * Locate the index of the quad in which the supplied point lies.
     *
     * @return An int containing a bitmask of the intersected quads,
     * i.e. bits 0, 1, 2 & 3 correspond to the quads with those ids.
     */
    template< typename Data_type >
    struct intersects<POINT_TYPE, Data_type>
    {
        template< typename T, typename Point_type >
        int operator()( const quad_tree<T> * q, const Point_type & p ) const
        {
            int result = 0;

            int idx = q->index( p );

            if ( idx != quad_tree<T>::npos ) result |= (1<<idx);

            return result;
        }
    };


}

#endif // QUADTREE_H_INCLUDED
