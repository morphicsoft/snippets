#include <iostream>

void print_bits( int c )
{
    for ( int i = 0; i < 4; ++i )
    {
	bool b = c & (1 << i);
	std::cout << "bit #" << i << " " << b << "\n";
    } 
}


int main()
{
    using namespace std;

    int c = 0;

    cout << c << "\n";
    // index 0 is set:
    c |= 1;

    // index 1 is set:
    c |= 1 << 1;

    // index 2 is set:
    //c |= 1 << 2;

    // index 3 is set:
    c |= 1 << 3;

    print_bits( c );
}
