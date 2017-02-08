#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

std::string trim( const std::string & s, const std::string & chars = " \t" )
{
    auto b = s.find_first_not_of( chars );
    if ( b == std::string::npos )
        return "";

    auto e = s.find_last_not_of( chars );
    auto r = e - b + 1;

    return s.substr( b, r );
}

std::vector< std::string > split( const std::string & s, const std::string & delim )
{
    std::vector< std::string > result;

    if ( s.empty() ) return result;
    if ( delim.empty() ) return result;

    std::string::size_type b = 0;
    std::string::size_type e = s.find( delim );

    while( e != std::string::npos )
    {
        result.push_back( s.substr( b, e - b ));
        b = e + 1;
        e = s.find( delim, b );
    }

    result.push_back( s.substr( b ) );

    return result;
}

std::ostream & operator<< ( std::ostream & os, const std::vector< std::string > & v )
{
    std::copy( v.begin(), v.end(), std::ostream_iterator< std::string >( std::cout, "/" ));
    return os;
}

int main()
{
    using namespace std;
    
    cout << "|" << trim( "", "" ) << "|\n";
    cout << "|" << trim( "", " " ) << "|\n";
    cout << "|" << trim( " ", " " ) << "|\n";
    cout << "|" << trim( " * ", "" ) << "|\n";
    cout << "|" << trim( " * ", " " ) << "|\n";
    cout << "|" << trim( " * ", "*" ) << "|\n";

    cout << "|" << split( "", "" ) << "|\n";
    cout << "|" << split( "", "," ) << "|\n";
    cout << "|" << split( ",", "," ) << "|\n";
    cout << "|" << split( ",,", "," ) << "|\n";
    cout << "|" << split( "a", "," ) << "|\n";
    cout << "|" << split( "a,", "," ) << "|\n";
    cout << "|" << split( "a,b", "," ) << "|\n";
}
