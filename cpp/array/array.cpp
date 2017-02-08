#include <array>
#include <iostream>

struct point
{
    point( int x_, int y_ )
        : x(x_), y(y_)
    {}

    int x;
    int y;
};
std::ostream & operator<<( std::ostream & os, const point & p )
{
    return os << "<point x:" << p.x << " y:" << p.y << "\n";
}

template< int N >
struct S
{
    S( int w );

    point ar[N];
};

template< int N >
S<N>::S( int w )
{
    ar[0] = point( w, w );
    for ( int i = 1; i < N; ++i )
        ar[i] = w / ( 2 << ( i-1 ) );
}

int main()
{
    S<5> s( 3200 );

    std::cout << s.ar[0] << "\n";
    std::cout << s.ar[1] << "\n";
}
