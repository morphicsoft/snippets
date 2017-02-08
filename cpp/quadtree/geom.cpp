#include "geom.h"

bool is_small( int i )
{
    return i == 0;
}

bool is_small( float d )
{
    return std::abs( d ) < std::numeric_limits<float>::epsilon();
}


bool is_small( double d )
{
    return std::abs( d ) < std::numeric_limits<double>::epsilon();
}

