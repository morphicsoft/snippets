#ifndef BIN_FRACTION_H
#define BIN_FRACTION_H

#include <bitset>
//#include <cmath>

std::bitset<32> bin_fraction( double d, int start )
{
    // creates a binary fraction from the supplied real
    // starting at the specified position (thus the start
    // position dictates the length).

    std::bitset<32> result;

    int i = d;

    double r = d - i;

    int max = start;

    while ( r > 0.0 && max > 0 )
    {
        r = r * 2;
        --max;

        if ( r >= 1.0 )
        {
            result[max-1] = 1;
            r -= 1;
        }
        else
        {
            result[max-1] = 0;
        }
    }

    return result;
}

#endif // BIN_FRACTION_H
