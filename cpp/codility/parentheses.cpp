#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int solution( const std::string & S ) 
{
    int nest_level = 0;

    if ( S.size() % 2 != 0 ) return 0;

    for ( auto s : S )
    {
	if ( s == '(' ) ++nest_level;
	else if ( s == ')' ) --nest_level;
	else
	    ;
	if ( nest_level < 0 ) return 0;
    }

    return nest_level == 0;
}

int main()
{
    clog << solution( "()" ) << "\n";
    clog << solution( "(())" ) << "\n";
    clog << solution( "(())()" ) << "\n";
    clog << solution( "())" ) << "\n";
}
