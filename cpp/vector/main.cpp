#include "vector.h"

#include <iostream>

int main()
{
    using namespace std;

    vector_2d<int> v1( 5, 10 );
    vector_2d<int> v2( 10, 5 );

    cout << dot( v1, v2 ) << "\n";
    cout << v1 * v2 << "\n";

    vector_3d<int> v3( 5, 10, 15 );
    vector_3d<int> v4( 15, 10, 5 );
    cout << dot( v3, v4 ) << "\n";

    cout << v3 * v4 << "\n";

    cout << v2 - v1 << "\n";
}
