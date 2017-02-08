#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iterator>
#include <map>
#include <set>
#include <ostream>
#include <vector>


template< typename Id_type >
class node
{
public:

    typedef Id_type                                 id_type;

    node( const id_type & id ) : id_( id ) {}

    id_type         id() const      { return id_; }

private:

    id_type             id_;
};

template< typename Id_type, typename Weight_type >
class edge
{
public:
    typedef Id_type                                 id_type;
    typedef Weight_type                             weight_type;

    edge( const id_type & from, const id_type & to, const Weight_type & weight = -1 )

        :   from_   ( from )
        ,   to_     ( to )
        ,   weight_ ( weight )
    {}

    id_type         from() const    { return from_; }
    id_type         to() const      { return to_; }
    weight_type     weight() const  { return weight_; }

private:

    id_type         from_;
    id_type         to_;
    weight_type     weight_;
};

template< typename Id_type, typename Weight_type >
bool operator < ( edge<Id_type, Weight_type> const & l, edge<Id_type, Weight_type> const & r )
{
    if ( l.from() < r.from() ) return true;
    if ( l.from() > r.from() ) return false;
    return l.to() < r.to();
}

template< typename Id_type, typename Weight_type >
class graph
{
public:
    typedef Id_type                                 id_type;
    typedef Weight_type                             weight_type;
    typedef node<id_type>                           node_type;
    typedef edge<id_type, weight_type>              edge_type;
    typedef std::set<node_type>                     node_set;
    typedef std::set<edge_type>                     edge_set;
    typedef std::multimap<id_type, id_type>         neighbour_map;
    typedef typename node_set::const_iterator       const_iterator;
    typedef typename neighbour_map::const_iterator  neighbour_iterator;

    bool            has_edge( const id_type & f, const id_type & t ) const
    {
        return edges_.find( edge_type{ f, t } ) != edges_.end();
    }

    weight_type     edge_weight( const id_type & f, const id_type & t ) const
    {
        return (*edges_.find( { f, t } ) ).weight();
    }

    const_iterator begin() const
    {
        return nodes_.begin();
    }

    const_iterator end() const
    {
        return nodes_.end();
    }

    neighbour_iterator neighbours_begin( const Id_type & id ) const
    {
        return neighbours_.lower_bound( id );
    }

    neighbour_iterator neighbours_end( const Id_type & id ) const
    {
        return neighbours_.upper_bound( id );
    }

    void            add_node( const id_type & id )
    {
        nodes_.insert( { id } );
    }

    void            add_edge( const id_type & f, const id_type & t, const weight_type & w )
    {
        edges_.insert( { f, t, w } );
        neighbours_.insert( { f, t } );
    }

private:

    node_set        nodes_;
    edge_set        edges_;
    neighbour_map   neighbours_;
};


template< typename T >
bool operator< ( node<T> const & l, node<T> const & r )
{
    return l.id() < r.id();
}

template< typename T >
bool operator== ( node<T> const & l, node<T> const & r )
{
    return l.id() == r.id();
}

template< typename T >
std::ostream & operator<< ( std::ostream & os, const node<T> & n )
{
    return os << n.id();
}

#endif // GRAPH_H_INCLUDED
