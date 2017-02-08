#include "quadtree.h"
#include "linear_quadtree.h"

#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <ratio>

using namespace std;


//*
template< typename T >
class test_object
{
public:

    test_object( T x, T y, T w, T h, int data )

        : x_( x ), y_( y ), w_( w ), h_( h ), data_( data )
    {}

    T       x() const       { return x_; }
    T       y() const       { return y_; }
    T       width() const   { return w_; }
    T       height() const  { return h_; }
    int     data() const    { return data_; }

private:

    T       x_;
    T       y_;
    T       w_;
    T       h_;
    int     data_;
};
//*/

/*
class test_object
{
public:

    test_object( int x, int y )

        : x_( x ), y_( y )
    {}

    int x() const           { return x_; }
    int y() const           { return y_; }

private:
    int x_;
    int y_;
};
//*/

template< typename T >
std::ostream & operator<<( std::ostream & os, const test_object<T> & t )
{
    os << "<test_object position:(" << t.x() << "," << t.y()
       << ") size:(" << t.width() << ", " << t.height()
       << ") data:" << t.data() << ">";
}

template< typename T >
struct T_counter
{
    static int count;
};
template< typename T >
int T_counter< T >::count( 1 );

template< typename T >
class functor
{
public:
    functor( std::vector< T > & res )
        : res_( res )
    {
        res_.reserve( 10 );
    }

    void operator()( const T & t ) const
    {
        res_.push_back( t );
    }

    bool active() const
    {
        return res_.size() < 10;
    }

private:

    std::vector< T > & res_;
};

template< template <typename> class Quad_tree, class T, class Obj_type >
int find_matches( const Quad_tree<T> & qtree, const Obj_type & o )
{
    std::vector< T > res;
    qtree.for_each_match( o, [&res]( const T & obj ) { res.push_back( obj ); } );
    //cout << "(" << T_counter<Quad_tree<T>>::count++ << ")- number of potential collisions : " << res.size() << endl;

    /*
    for ( const T & item : res )
    {
        cout << "\t" << item << "\n";
    }
    //*/

    return res.size();
}

template< template <typename> class Quad_tree, class T, class Obj_type >
int find_line_matches( const Quad_tree<T> & qtree, const Obj_type & o)
{
    std::vector< T > res;
    qtree.line_intersect( o, [&res]( const T & obj ){ res.push_back( obj ); } );
    functor<T> f( res );
    qtree.first_line_intersect( o, f );
    //cout << "(" << T_counter<Quad_tree<T>>::count++ << ")- number of potential collisions : " << res.size() << endl;

    /*
    for ( const T & item : res )
    {
        cout << "\t" << item << "\n";
    }
    //*/

    return res.size();
}

void intersection_test()
{
    line_segment<int> l( { -1500, -1500 }, { 3637, -3645 } );
    line_segment<int> e( { 0, -3650 }, { 0, 0 } );

    cout << intersect( l, e ).first << "\n";
}


template< typename Quad_tree_type >
void test_quad_tree( Quad_tree_type & qtree, int n = 1 )
{
    //quad_tree< test_object<int> > qtree( { 0, 0, 1000, 1000 }, 10, 10 );

    int max_objects = 99;
    for ( int i = 0; i < max_objects; ++i )
    {
        //test_object<int> t( dist1( rng ), dist1( rng ), dist2( rng ), dist2( rng ), i );
        test_object<int> t ( i * 10, i * 10, 10, 10, i );
        qtree.insert( t );
    }
    //cout << "total number of objects : " << qtree.size() << endl;

    auto start = chrono::high_resolution_clock::now();

    for ( int i = 0; i < n; ++i )
    {
    //*
    { // #1 : should retrieve 100 objects

        auto endpoint1 = point<int>( 0, 0 );
        auto endpoint2 = point<int>( 999, 999 );
        auto bb = make_bounding_box( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #2 : should retrieve 100 objects

        auto endpoint1 = point<int>( 0, 999 );
        auto endpoint2 = point<int>( 999, 0 );
        rectangle<int> bb( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #3 : should retrieve 51 objects

        auto endpoint1 = point<int>( 500, 500 );
        auto endpoint2 = point<int>( 999, 999 );
        rectangle<int> bb( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #4 : should retrieve 59 objects

        auto endpoint1 = point<int>( 0, 0 );
        auto endpoint2 = point<int>( 499, 499 );
        rectangle<int> bb( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #5 : should retrieve 10 objects
        auto endpoint1 = point<int>( 510, 510 );
        auto endpoint2 = point<int>( 510, 999 );
        rectangle<int> bb( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #6 : should retrieve 45 objects
        auto endpoint1 = point<int>( 999, 999 );
        auto endpoint2 = point<int>( 600, 600 );
        rectangle<int> bb( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #7 : should retrieve 10 objects
        auto endpoint1 = point<int>( 500, 500 );
        auto endpoint2 = point<int>( 500, 500 );
        rectangle<int> bb( endpoint1, endpoint2 );

        auto num = find_matches( qtree, bb );
    }

    { // #8 : should retrieve 10 objects
        auto endpoint1 = point<int>( 500, 500 );
        auto num = find_matches( qtree, endpoint1 );
    }
    //*/
    { // #9
        auto endpoint1 = point<int>( 0, 0 );
        auto endpoint2 = point<int>( 999, 999 );
        line_segment<int> l( endpoint1, endpoint2 );

        auto num = find_line_matches( qtree, l );
    }

    { // #10
        auto endpoint1 = point<int>( 501, 501 );
        auto endpoint2 = point<int>( 999, 999 );
        line_segment<int> l( endpoint1, endpoint2 );

        auto num = find_line_matches( qtree, l );
    }
    }

    auto end = chrono::high_resolution_clock::now();

    cout << "Time taken: " << chrono::duration<double, milli >(end-start).count() << " ms\n";
}



int main()
{
    quad_tree< test_object<int> > qtree( { 0, 0, 1000, 1000 }, 10, 10 );
    test_quad_tree( qtree, 500 );

    linear_quadtree< test_object<int> > lqtree( { 0, 0, 1000, 1000 }, 5 );
    test_quad_tree( lqtree, 500 );

}

