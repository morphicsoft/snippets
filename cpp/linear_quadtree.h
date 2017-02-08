#ifndef LINEAR_QUADTREE_H
#define LINEAR_QUADTREE_H

#include "bin_fraction.h"
#include "geom.h"

#include <bitset>
#include <cmath>
#include <set>
#include <unordered_map>
#include <vector>

template< class Rectangle_type >
struct quad_node_comparator
{
    typedef std::pair< Rectangle_type, int > quad_node_type;
    bool operator() ( const quad_node_type & lhs, const quad_node_type & rhs ) const
    {
        if ( lhs.first.x() < rhs.first.x() )            return true;
        if ( lhs.first.x() > rhs.first.x() )            return false;
        if ( lhs.first.y() < rhs.first.y() )            return true;
        if ( lhs.first.y() > rhs.first.y() )            return false;
        if ( lhs.first.width() > rhs.first.width() )    return true;
        if ( lhs.first.width() < rhs.first.width() )    return false;
        return lhs.first.height() > rhs.first.height();
    }
};

namespace std
{
    bool operator< ( const std::bitset<32> & lhs, const std::bitset<32> & rhs )
    {
        return lhs.to_ulong() < rhs.to_ulong();
    }
}

/**
 *
 * linear_quadtree stores nodes within a flat structure (as opposed to recursive)
 * and performs look-ups through a hashed locator code.
 *
 * The look-up index is built during construction based on the dimensions and depth
 * of the octree, and no further resizing is carried out. The aim is to provide
 * fast look-up with static scenery.
 *
 */
template< typename T >
class linear_quadtree
{
public:

    //static const int npos = -1;
    enum { npos = -1 };

    typedef T                                           object_type;
    typedef linear_quadtree< T >                        this_type;
    typedef const linear_quadtree< T >*                 const_ptr;
    typedef decltype(((T*)nullptr)->x())                point_data_type;

public:

    typedef T                                           value_type;
    typedef point< point_data_type >                    point_type;
    typedef line_segment< point_data_type >             line_type;
    typedef rectangle< point_data_type >                rectangle_type;
    typedef std::pair< rectangle_type, int >            quad_node_type;
    typedef std::vector< quad_node_type >               quad_node_array;
    typedef typename quad_node_array::size_type         offset_type;
    typedef typename quad_node_array::const_iterator    quad_node_iterator;
    typedef std::bitset<32>                             index_element;
    //typedef std::unordered_multimap<index_element, T >  container_type;
    typedef std::unordered_multimap<index_element, T >  container_type;
    typedef typename container_type::size_type          size_type;

    // @todo : use a hash table? (since look-ups are now exact)...
    typedef std::vector<index_element>                  index_type;

    linear_quadtree( const rectangle_type & bounds, int max_levels )

        : max_levels_( max_levels )
    {
        // populate a flat structure of quad bounds
        bounds_.reserve( upper( max_levels ));
        index_.reserve( upper( max_levels ));
        bounds_.push_back( { bounds, npos } );
        index_.push_back( index_element( 1 ));

        for ( int i = 1; i < max_levels; ++i )
        {
            int b = upper( i-1 );
            int e = upper( i );

            for ( int j = b; j < e; ++j )

                populate_children( bounds_[j].first, j, i );
        }
    }

    void print_children( const quad_node_iterator & pit ) const
    {
        if ( pit == end() ) return;
        std::cout << "Children of " << (*pit).first << "\n";
        for ( quad_node_iterator chit = begin_children( pit ); chit != end_children( pit ); ++chit )
        {
            std::cout << "\t" << (*chit).first << "\n";
        }

    }

    void print_bounds( std::ostream & os ) const
    {
        os << "Generated " << bounds_.size() << " rectangles:" << "\n";
        for ( const auto & i : bounds_ )
            os << i.first << ", parent: " << i.second << "\n";
    }

    void print_index( std::ostream & os ) const
    {
        for ( size_t i = 0; i < index_.size(); ++i )
            os << index_[i] << "\t" << bounds_[i].first << "\n";
    }

    size_type size() const
    {
        return objects_.size();
    }

    quad_node_iterator begin() const
    {
        return bounds_.begin();
    }

    quad_node_iterator end() const
    {
        return bounds_.end();
    }

    quad_node_iterator begin_children( const quad_node_iterator & parent ) const
    {
        quad_node_iterator b = begin();
        typename quad_node_iterator::difference_type parent_offset = std::distance( b, parent );
        typename quad_node_iterator::difference_type child_offset = parent_offset * 4 + 1;

        if ( child_offset > bounds_.size() )
            return end();

        std::advance( b, child_offset );

        return b;
    }

    quad_node_iterator end_children( const quad_node_iterator & parent ) const
    {
        quad_node_iterator e = begin();
        typename quad_node_iterator::difference_type parent_offset = std::distance( e, parent );
        typename quad_node_iterator::difference_type child_offset = parent_offset * 4 + 5;

        if ( child_offset > bounds_.size() )
            return end();

        std::advance( e, parent_offset * 4 + 5 );

        return e;
    }

    void insert( const value_type & obj )
    {
        // find the indices of the four corners
        std::set<offset_type> ind = indices( obj );

        // if the corners are in different nodes, then move up the levels
        // until the smallest single containing node is found
        while ( ind.size() > 1 )
        {
            ind = parent_indices( ind );
        }

        if ( ind.empty() ) return;
        offset_type off = *ind.begin();
        index_element idx( index_[off] );
        //std::cout << "Object " << obj << " is in quad " << bounds_[off].first << std::endl;
        objects_.insert( { idx, obj } );
    }

    template< typename Functor_type >
    void for_each_match( const point_type & p, const Functor_type & f ) const
    {
        offset_type off = index( p );

        while ( off != npos )
        {
            index_element idx( index_[off] );
            auto range = objects_.equal_range( idx );

            for ( auto it = range.first; it != range.second; ++it )
            {
                f( (*it).second );
            }

            // @todo: should be possible to get parent via arithmetic?
            off = bounds_[off].second;
        }
    }

    template< typename Functor_type >
    void for_each_match( const rectangle_type & r, const Functor_type & f ) const
    {
        for_each_match( begin(), r, f );
    }

    int quadrant( const rectangle_type & bounds, const point_type & p ) const
    {
        int x_midpoint = bounds.x() + bounds.width() / 2;
        int y_midpoint = bounds.y() + bounds.height() / 2;

        bool bottom_half = p.y() < y_midpoint;
        bool top_half = p.y() >= y_midpoint;

        int result = npos;

        // left half is between x and x_midpoint
        if ( p.x() < x_midpoint )
        {
            if ( top_half )
                result = 3;
            else if ( bottom_half )
                result = 1;
        }
        // right half is between x_midpoint and x + width
        else if ( p.x() >= x_midpoint )
        {
            if ( top_half )
                result = 4;
            else if ( bottom_half )
                result = 2;
        }
        return result;
    }

    std::set<int> quadrants( const rectangle_type & bounds, const rectangle_type & r ) const
    {
        std::set<int> result;

        for ( const auto & corner : rectangle_corners( r ) )
        {
            result.insert( quadrant( bounds, corner ));
        }

        return result;
    }

    template< typename Functor_type >
    void for_each_match( const quad_node_iterator & it, const rectangle_type & r, const Functor_type & f ) const
    {
        //std::cout << "for_each_child_match " << (*it).first << "\n";

        for ( const auto & q : quadrants( (*it).first, r ) )
        {
            if ( q != npos )
            {
                quad_node_iterator b = begin();
                typename quad_node_iterator::difference_type this_offset = std::distance( b, it );
                typename quad_node_iterator::difference_type child_offset = this_offset * 4 + q;

                if ( child_offset < bounds_.size() )
                {
                    size_t num_objs = objects_.count( index_[child_offset] );

                    if ( num_objs > 0 )
                    {
                        std::advance( b, child_offset );
                        for_each_match( b, r, f );
                    }
                }
            }
        }

        offset_type off = std::distance( begin(), it );
        index_element idx( index_[off] );
        auto range = objects_.equal_range( idx );

        for ( auto it = range.first; it != range.second; ++it )
        {
            f( (*it).second );
        }
    }

    template< typename Functor_type >
    void line_intersect( const line_type & l, const Functor_type & f ) const
    {
        quad_node_iterator it = begin();
        line_intersect( it, l, f );
    }

    template< typename Functor_type >
    void first_line_intersect( const line_type & l, const Functor_type & f ) const
    {
        quad_node_iterator it = begin();
        first_line_intersect( it, l, f );
    }

private:

    bool in_bounds( const rectangle_type & r, const point_type & p ) const
    {
        bool in_x_bounds = p.x() >= r.x() && p.x() <= r.x() + r.width();
        bool in_y_bounds = p.y() >= r.y() && p.y() <= r.y() + r.height();

        return in_x_bounds && in_y_bounds;
    }

    offset_type index( const point_type & p ) const
    {
        // Locate the offset of a point within the index.

        std::bitset<32> loc = locator( p );
        //std::cout << loc << std::endl;

        index_type::const_iterator pos = std::lower_bound( index_.begin(), index_.end(), loc );

        if ( pos == index_.end() )
        {
            return npos;
        }
        else
        {
            return std::distance( index_.begin(), pos );
        }

    }

    template< template <typename> class Rectangle_type, typename U >
    std::set<offset_type> indices( const Rectangle_type<U> & r ) const
    {
        std::vector< point_type > corners = rectangle_corners( r );

        std::set< offset_type > result;

        for ( const auto & p : corners )
        {
            int idx = index( p );

            if ( idx != npos ) result.insert( idx );
        }

        return result;

    }

    std::set<offset_type> parent_indices( const std::set<offset_type> & ind ) const
    {
        std::set<offset_type> result;

        for ( auto i : ind )
        {
            offset_type parent = bounds_[i].second;

            if ( parent != npos )
                result.insert( parent );
        }

        if ( result.empty() )
        {
            result.insert( 0 ); // default
        }

        return result;
    }

    template< typename Functor_type >
    void line_intersect( const quad_node_iterator & it, const line_type & l, const Functor_type & f ) const
    {
        int intersect_count = 0;
        for ( const auto & edge : (*it).first )
        {
            intersect_count += intersect( l, edge ).first;
        }

        bool p1_in_bounds = in_bounds( (*it).first, l.p1() );
        bool p2_in_bounds = in_bounds( (*it).first, l.p2() );

        if ( intersect_count > 0 || p1_in_bounds || p2_in_bounds )
        {
            for ( quad_node_iterator ch_it = begin_children( it ); ch_it != end_children( it ); ++ch_it )
            {
                line_intersect( ch_it, l, f );
            }

            auto off = std::distance( begin(), it );
            index_element idx( index_[off] );
            auto range = objects_.equal_range( idx );

            for ( auto obj_it = range.first; obj_it != range.second; ++obj_it )
            {
                f( (*obj_it).second );
            }
        }
    }

    template< typename Functor_type >
    void first_line_intersect( const quad_node_iterator & it, const line_type & l, const Functor_type & f ) const
    {
        if ( !f.active() ) return;

        int intersect_count = 0;
        for ( const auto & edge : (*it).first )
        {
            intersect_count += intersect( l, edge ).first;
        }

        bool p1_in_bounds = in_bounds( (*it).first, l.p1() );
        bool p2_in_bounds = in_bounds( (*it).first, l.p2() );

        if ( intersect_count > 0 || p1_in_bounds || p2_in_bounds )
        {
            for ( quad_node_iterator ch_it = begin_children( it ); ch_it != end_children( it ); ++ch_it )
            {
                first_line_intersect( ch_it, l, f );
            }

            auto off = std::distance( begin(), it );
            index_element idx( index_[off] );
            auto range = objects_.equal_range( idx );

            for ( auto obj_it = range.first; obj_it != range.second; ++obj_it )
            {
                f( (*obj_it).second );
            }
        }
    }

    std::bitset<32> locator( const point_type & p ) const
    {
        // Builds a binary locator based on Morton ordering of the point coordinates.
        // Locator is used to identify the position of the point within the index.

        //std::cout << "locate point " << p << "\n";
        // calculate the location code for a given point (a binary fraction assuming Morton order)
        double dx = double( p.x() - bounds_[0].first.x() ) / double( bounds_[0].first.width() );
        double dy = double( p.y() - bounds_[0].first.y() ) / double( bounds_[0].first.height() );

        std::bitset<32> bitsx = bin_fraction(dx, max_levels_);
        //std::cout << "bitsx " << bitsx << "\n";
        std::bitset<32> bitsy = bin_fraction(dy, max_levels_);
        //std::cout << "bitsy " << bitsy << "\n";
        std::bitset<32> result;
        for ( int i = 31; i >= 0; --i )
        {
            result[(i*2)+1] = bitsy[i];
            result[i*2] = bitsx[i];
        }
        result[max_levels_+3] = 1;  // sentinel bit set to assume lowest-level
                                    // (since we're locating a single point)

        return result;
    }

    int upper( int n ) const
    {
        return (std::pow(4, n) - 1) / 3;
    }

    void populate_children( const rectangle_type & bounds, int parent, int level )
    {
        int x = bounds.x();
        int y = bounds.y();
        int w = bounds.width();
        int h = bounds.height();

        unsigned parent_idx = index_[parent].to_ulong() << 2;
        unsigned level_idx = parent_idx;

        // Quadtree nodes are added in Morton order, which is recorded in the
        // index. A sentinel bit is set which corresponds to the level (depth)
        // of the node, in order to ensure uniqueness of node ids.

        bounds_.push_back( { rectangle_type{ x, y, w/2, h/2 }, parent } );
        index_.push_back( level_idx );
        bounds_.push_back( { rectangle_type{ x + w/2, y, w/2, h/2 }, parent } );
        index_.push_back( level_idx | 1 );
        bounds_.push_back( { rectangle_type{ x, y + h/2, w/2, h/2 }, parent } );
        index_.push_back( level_idx | 2 );
        bounds_.push_back( { rectangle_type{ x + w/2, y + h/2, w/2, h/2 }, parent } );
        index_.push_back( level_idx | 3 );
    }

private:

    quad_node_array         bounds_;
    index_type              index_;
    int                     max_levels_;
    container_type          objects_;
};

#endif
