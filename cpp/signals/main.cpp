#include "signals.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

signals_::signal_<std::string, int> log_event;

class bump_event
{
public:

    bump_event( int x, int y )

        : x_( x ), y_( y )
    {}

    int x() const { return x_; }
    int y() const { return y_; }

private:
    int x_;
    int y_;
};

class bump_detector
{
public:

    signals_::signal_<std::string, bump_event> bump;

    bump_detector( const std::string & name )

        : name_( name )
    {}

    const std::string & name() const
    {
        return name_;
    }

    void bump_at( int x, int y )
    {
        log_event.raise( "Raising bump event", 5 );
        bump.raise( name_, bump_event( x, y ) );
    }

private:

    std::string     name_;
};

class robot
{
public:

    signals_::signal_<std::string, bump_event>    bump_detected;

    void add_detector( bump_detector & detector )
    {
        log_event.raise( "Adding detector", 4 );
        detector.bump.connect( &robot::bump_detected, *this );
        detectors_.push_back( detector );
    }

private:

    std::vector<bump_detector>      detectors_;
};

void print_bump( const std::string & name, const bump_event & event )
{
    std::stringstream ss;
    ss << "Bump event on sensor " << name << " at position (" << event.x() << ", " << event.y() << ")";
    log_event.raise( ss.str(), 9 );
}

void bump_event_intercept( const std::string & name, const bump_event & event )
{
    std::cout << "bump_intercept(" << name << ", " << event.x() << ", " << event.y() << ")" << std::endl;
}

class logger
{
public:
    logger( std::ostream & os )

        : os_   ( os )
        , level_( 0 )
    {}

    void set_filter_level( int level )
    {
        level_ = level;
    }

    void log( const std::string & text, int priority )
    {
        if ( priority >= level_ )
            os_ << "Log: " << text << "\n";
    }


private:

    std::ostream &  os_;
    int             level_;
};

int main()
{
    using namespace std;
    using namespace std::placeholders;

    logger l( std::clog );
    l.set_filter_level( 0 );

    log_event.connect( std::bind( &logger::log, &l, _1, _2 ));

    robot r;
    bump_detector d1( "d1" );
    bump_detector d2( "d2" );

    r.add_detector( d1 );
    // This blocks a signal altogether:
    log_event.block();
    r.add_detector( d2 );
    log_event.unblock();

    auto conn = r.bump_detected.connect( &print_bump );

    d1.bump.connect( &bump_event_intercept );
    d2.bump.connect( &bump_event_intercept );

    d1.bump.connect( std::bind<void>(
                 []( const std::string & name ) { std::clog << "lambda intercept of: " << name << "\n"; }, _1 ));

    // This blocks a specific signal connection...
    // Other connections to the signal will continue to work.
    conn->block();
    d1.bump_at( 1, 1 );
    d2.bump_at( 9, 9 );
    conn->unblock();

    d1.bump_at( 5, 5 );
    d2.bump_at( 3, 3 );

}
