#include "getopts.h"

#include <iostream>

int main( int argc, char * argv[] )
{
    using namespace std;

    options allowed;
    allowed.add_options()
            ( "log", value<std::string>(), "Log file to load" )
            ( "timings", value<std::string>(), "Timing file to load" )
            ( "start", value<int>(), "Start time" )
            ( "noposefeatures", "Turn off pose features" )
    ;

    variables_map m = parse_command_line( argc, argv, allowed );

    /*
    if ( m.contains( "log" ) )
    {
        clog << "Log file was set to " << m[ "log" ] << "\n";
    }

    if ( m.contains( "start" ) )
    {
        clog << "Start time was set to " << m[ "start" ].as<int>() << "\n";
    }

    */
}
