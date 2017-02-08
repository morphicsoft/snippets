#ifndef VORONOI_H_INCLUDED
#define VORONOI_H_INCLUDED

#include "binary_tree.h"

#include <list>
//#include <memory>
//#include <vector>
#include <queue>

class point
{
public:
    point() : x_(0), y_(0) {}
    point( int x, int y ) : x_( x ), y_( y ) {}

    int x() const { return x_; }
    int y() const { return y_; }

private:

    int x_;
    int y_;
};

bool operator>( const point & lhs, const point & rhs );

class parabola
{
public:
private:

    point       p_;
    parabola *  prev_;
    parabola *  next_;
};

class event
{
public:

    event( int x, const point & p, const parabola & a )

        : x_(x), p_(p), a_(a), valid_(true)
    {}

    int x() const { return x_; }

private:

    int     x_;
    point   p_;
    parabola     a_;
};

class segment
{
public:

    explicit segment( const point & p )
        : start_( p ), end_( {0, 0} ), done_(false)
    {}

private:

    point   start_;
    point   end_;
    bool    done_;

};

class voronoi
{
public:
    typedef std::priority_queue< point, std::vector<point>, std::greater<point> >   point_queue;
    typedef std::priority_queue< event, std::vector<event>, std::greater<event> >   event_queue;

    void run();

    void push( const point & p );

private:

    void process_event();
    void process_point();
    void update_beachfront( const point & p );

private:

    point_queue                 points_;
    event_queue                 events_;
    binary_tree<parabola>       parabolas_;
};

#endif
