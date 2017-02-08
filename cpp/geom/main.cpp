#include "bsptree.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    line_segment l( { 5, 12 }, { 10, 15 } );

    //cout << point_side( l, point( 0, 10 ) ) << "\n";
    //cout << point_side( l, point( 0, 0 ) ) << "\n";
    //cout << point_side( l, point( 0, 5 ) ) << "\n";

    line_segment l1( { 5, 10 }, { 10, 10 } );
    line_segment l2( { 10, 10 }, { 10, 15 } );
    line_segment l3( { 10, 15 }, { 5, 15 } );
    line_segment l4( { 5, 15 }, { 5, 10 } );


    cout << point_side( l, { 5, 15 } ) << "\n";
    cout << point_side( l, { 5, 10 } ) << "\n";
    cout << line_side( l, l4 ) << "\n";

    vector< line_segment > sides( { l1, l2, l3, l4 } );
    polygon square( sides );
    cout << polygon_side( l, square ) << "\n";

    cout << intersect( l, l4 ).second << "\n";
    cout << intersect( l, l ).second << "\n";
}
