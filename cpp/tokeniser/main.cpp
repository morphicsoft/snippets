#include "tokeniser.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

void tokenise( const std::string & s, const std::string & drop, const std::string & keep )
{
    char_separator sep( drop, keep );
    tokeniser tok( s, sep );

    std::clog << "String '" << s << "' broken into following tokens:\n";
    for ( auto t : tok )
        std::clog << "|" << t << "|\n";
    std::clog << "---------------------------------------------------------------\n";
}

int main()
{
    using namespace std;

    /*
    try
    {
        lexical_cast<int>( std::string( "ABC" ) );
        clog << "Warning: lexical_cast should have failed\n";
    }
    catch( bad_lexical_cast & )
    {
        clog << "OK, expected exception thrown\n";
    }
    catch( ... )
    {
        clog << "Warning: unexpected exception thrown\n";
    }

    string s = "@1@2";
    string s0 = ",,@10,05,10,15,20,25,30";
    string s1 = "@10,,5,10,15,20,25,30";
    string s2 = "@,5,10,15,20,25,30";
    string s3 = "@10,5,10,15.5,20,25,30@@";
    string s4 = "@10,";

    string s5 = "aa,b";

    string drop = ",";
    string keep = "@-";

    tokenise( s, drop, keep );
    tokenise( s0, drop, keep );
    tokenise( s1, drop, keep );
    tokenise( s2, drop, keep );
    tokenise( s3, drop, keep );
    tokenise( s4, drop, keep );

    tokenise( "", drop, keep );
    tokenise( ",,,,,,,,,,", drop, keep );
    tokenise( "@-@-@-,@-@", drop, keep );

    tokenise( "banana banana", "", "" );
    tokenise( "bananabanana", "b", "" );
    tokenise( "bananabanana", "", "b" );
    tokenise( "bananabanana", "n", "b" );
    */
    string s9 = "30919	[Gyrodometry]	[]	20	No gyro data between start/end times: 30867 -> 30914	@ Gyrodometry.cpp:74";
    tokeniser t( s9, char_separator( " \t" ) );

    int time = 0;
    string cat;
    string sub_cat;
    int p;
    string rest;

    auto it = t.begin();

    time = lexical_cast<int>( *it++ );
    cat = *it++;
    sub_cat = *it++;
    p = lexical_cast<int>( *it++ );
    rest = to_string( it, t.end() );

    clog << rest << "\n";
}
