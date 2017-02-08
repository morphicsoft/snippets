#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

typedef std::vector<int> ar_type;

ar_type quicksort( const ar_type & ar )
{
    if ( ar.size() <= 1 )
	return ar;

    unsigned pivot_point = ar.size() / 2;
    
    auto pivot = ar[pivot_point];

    ar_type less, greater;

    for ( auto elem : ar ) {

	if ( elem < pivot )
	    less.push_back( elem );
	else if ( elem > pivot )
	    greater.push_back( elem );
    }

    ar_type l = quicksort( less );
    ar_type r = quicksort( greater );

    ar_type result;
    std::copy( l.begin(), l.end(), std::back_inserter( result ) );
    result.push_back( pivot );
    std::copy( r.begin(), r.end(), std::back_inserter( result ) );
    return result;
}

int main()
{
    ar_type unsorted;

    for ( int i = 0; i < 100; ++i ) {

	unsorted.push_back( i );
    }

    std::random_shuffle( unsorted.begin(), unsorted.end() );

    std::copy( unsorted.begin(), unsorted.end(), std::ostream_iterator<int>( std::cout, "," ));
    std::cout << "\n********************\n";

    ar_type sorted = quicksort( unsorted );

    std::copy( sorted.begin(), sorted.end(), std::ostream_iterator<int>( std::cout, "," ));
    std::cout << "\n********************\n";

    int err_count = 0;
    if ( sorted.size() != unsorted.size() ) {
	std::cout << "Error found in sorting!\n";

	++err_count;
    }

    for ( int i = 1; i < sorted.size(); ++i ) {
	
	if ( sorted[i] - sorted[i-1] != 1 ) {

	    std::cout << "Error found in sorting!\n";
	    ++err_count;
	}
    }

    if ( err_count == 0 ) {

	std::cout << "No errors found\n";
    }
}
