#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
/*
double square_root( double d )
{
    if ( d < 0 )
    {
	throw std::out_of_range( "d < 0" );
    }
    return sqrt( d );
}

void print_usage()
{
    std::cout << "Usage is conc nnn\n";
}

int main( int argc, const char * argv[] )
{
    using namespace std;

    if ( argc < 2 )
    {
	print_usage();
	exit(-1);
    }

    cout << "Calling square_root asynchronously...\n";

    // request async execution in separate thread...
    double num = ::atof( argv[1] );
    future<double> f = async( launch::async, square_root, num );

    cout << "returned. Getting result...\n";

    try
    {
	double y = f.get();
	cout << "ok, result is " << y << "\n";
    }
    catch( std::exception & e )
    {
	cout << "Exception : " << e.what() << "\n";
    }
}

*/
//*

std::condition_variable cv;
std::mutex m;
bool ready = false;

void say_hello( int thid )
{
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{ return ready; });

    for ( int i = 0; i < 10; ++i )
    {
	std::cout << "Hello from thread " << thid << "\n";
    }
}

int main()
{
    using namespace std;
    cout << "Max threads: " << thread::hardware_concurrency() << "\n";
    vector< std::thread > threads;

    cout << "Creating threads...\n";
    for ( int i = 0; i < 10; ++i )
    {
	threads.push_back( std::thread( say_hello, i ) );
    }

    chrono::milliseconds dura( 2000 );
    this_thread::sleep_for( dura );
    cout << "threads joined, signalling ready...\n";

    {
	std::lock_guard<std::mutex> lk(m);
	ready = true;
	cout << "ready to run, notifying threads...\n";
    }
    cv.notify_all();

    cout << "notified threads, joining...\n";
    for_each( threads.begin(), threads.end(), mem_fn( &thread::join ));
}

//*/
