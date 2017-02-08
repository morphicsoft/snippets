#include "activeobj.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <functional>
#include <random>
#include <thread>
#include <vector>

log_file logger( std::clog );

void do_some_logging( int id )
{
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution< int > dist( 1, 9 );

    for ( int i = 1; i <= 10; ++i )
    {
        std::stringstream ms;
        ms << "Message from thread " << id << "\n";
        logger.write( ms.str() );

        // sleep for a random amount of time, so that the threads are
        // randomly interleaved
        std::chrono::milliseconds pause( dist( gen ) * 100 );
        std::this_thread::sleep_for( pause );
    }
}

int main()
{
    using namespace std;

    vector< thread > threads;

    for ( int i = 0; i < 10; ++i )
    {
        threads.push_back( thread( do_some_logging, i ) );
    }

    logger.write( "Hello from main thread!\n" );

    for_each( threads.begin(), threads.end(), mem_fn( &thread::join ) );
}
