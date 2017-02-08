#include "lockfree.h"

#include <iostream>

int main()
{
    using namespace std;

    lf_queue< int > q;

    q.push( 1 );
    q.push( 2 );
    q.push( 3 );

    int i1 = q.pop();

    cout << "popped: " << i1 << "\n";

    int i2 = q.pop();

    cout << "popped: " << i2 << "\n";
    cout << "popped: " << q.pop() << "\n";
}
