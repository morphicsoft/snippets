#include <algorithm>
#include <iostream>
#include <iterator>
#include  <string>
#include <vector>

int main( int argc, char *argv[] )
{
    std::vector< std::string > v( argv, argv + argc );

    std::copy( v.begin(), v.end(), std::ostream_iterator<std::string>( std::cout, "\n" ) );
}
