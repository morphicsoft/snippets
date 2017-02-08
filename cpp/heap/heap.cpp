#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

template< typename T,
          class Container = std::vector<T>,
          class Compare = std::less< T > >
class heap
{
    typedef Compare                                 comparator_type;

public:

    typedef T                                       value_type;
    typedef const T&                                const_reference;
    typedef Container                               container_type;
    typedef typename container_type::iterator       iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef typename container_type::size_type      size_type;

    heap() {}

    template< typename Iter_type >
    heap( const Iter_type & b, const Iter_type & e )
    {
        std::copy( b, e, std::back_inserter( heap_ ));

        for ( size_type i = size() / 2; i != npos; --i )
            downheap( i );
    }

    heap( std::initializer_list<T>&& list )

        : heap( list.begin(), list.end() )
    {
    }

    size_type           size() const;
    iterator            begin();
    const_iterator      begin() const;
    iterator            end();
    const_iterator      end() const;
    const_reference     top() const;

    void                insert( const T & elem );
    void                pop_heap();


private:

    size_type           left( size_type parent ) const;
    size_type           right( size_type parent ) const;
    size_type           parent( size_type child ) const;
    void                upheap( size_type index );
    void                downheap( size_type index );
    
private:
    container_type          heap_;

    static comparator_type  compare_;
    static const size_type  npos = static_cast<size_type>( -1 );

};

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::comparator_type heap<T,Container,Compare>::compare_;

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::size_type heap<T,Container,Compare>::size() const
{
    return heap_.size();
}

template< typename T, class Container, class Compare>
typename heap<T,Container,Compare>::iterator heap<T,Container,Compare>::begin()
{
    return heap_.begin();
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::const_iterator heap<T,Container,Compare>::begin() const
{
    return heap_.begin();
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::iterator heap<T,Container,Compare>::end()
{
    return heap_.end();
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::const_iterator heap<T,Container,Compare>::end() const
{
    return heap_.end();
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::const_reference heap<T,Container,Compare>::top() const
{
    return heap_.front();
}

template< typename T, class Container, class Compare >
void heap<T,Container,Compare>::insert( const T & elem )
{
    heap_.push_back( elem );
    upheap( heap_.size() - 1 );
}

template< typename T, class Container, class Compare >
void heap<T,Container,Compare>::pop_heap()
{
    heap_[0] = heap_.back();
    heap_.pop_back();
    downheap(0);
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::size_type heap<T,Container,Compare>::left( size_type parent ) const
{
    size_type i = ( parent * 2 ) + 1;
    return ( i < heap_.size() ) ? i : npos;
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::size_type heap<T,Container,Compare>::right( size_type parent ) const
{
    size_type i = ( parent * 2 ) + 2;
    return ( i < heap_.size() ) ? i : npos;
}

template< typename T, class Container, class Compare >
typename heap<T,Container,Compare>::size_type heap<T,Container,Compare>::parent( size_type child ) const
{
    return child ? (child - 1 ) / 2 : npos;
}

template< typename T, class Container, class Compare >
void heap<T,Container,Compare>::upheap( size_type index )
{
    while ( ( index != npos ) && ( parent(index) != npos ) &&
            ( compare_( heap_[index], heap_[parent(index)] ) ) )
    {
        std::swap( heap_[index], heap_[parent(index)]);
        index = parent(index);
    }
}

template< typename T, class Container, class Compare >
void heap<T,Container,Compare>::downheap( size_type index )
{
    size_type left_child = left( index );
    size_type right_child = right( index );

    if ( ( left_child != npos ) && ( right_child != npos ) &&
         compare_( heap_[right_child], heap_[left_child] ) )
    {
        left_child = right_child;
    }

    while ( ( left_child != npos ) && compare_( heap_[left_child], heap_[index] ) )
    {
        std::swap( heap_[index], heap_[left_child] );
        index = left_child;
        left_child = left(index);
        right_child = right(index);

        if ( ( left_child != npos ) && ( right_child != npos ) &&
             compare_( heap_[right_child], heap_[left_child] ))
        {
            left_child = right_child;
        }
    }
}

template< typename T, class Container, class Compare >
void print_heap( const heap<T, Container, Compare> & h )
{
    std::copy( h.begin(), h.end(), std::ostream_iterator<T>( std::cout, " " ));
    std::cout << "\n";
}

class test
{
public:

    test( int x, int y )
        : x_( x ), y_( y )
    {}

    int x() const { return x_; }
    int y() const { return y_; }

    bool operator< ( const test & rhs ) const
    {
        if ( x_ < rhs.x_ )  return true;
        if ( x_ > rhs.x_ )  return false;
        else                return y_ < rhs.y_;
    }

    bool operator> ( const test & rhs ) const
    {
        if ( x_ > rhs.x_ )  return true;
        if ( x_ < rhs.x_ )  return false;
        else                return y_ > rhs.y_;
    }

private:

    int x_;
    int y_;
};

std::ostream & operator<< ( std::ostream & os, const test & t )
{
    return os << "<test x:" << t.x() << " y:" << t.y() << ">";
}

int main()
{
    heap<test> min_heap;
    min_heap.insert(test(3, 2));
    print_heap(min_heap);
    min_heap.insert(test(2, 3));
    print_heap(min_heap);
    min_heap.insert(test(4, 4));
    print_heap(min_heap);
    min_heap.insert(test(8, 8));
    print_heap(min_heap);
    min_heap.insert(test(5, 5));
    print_heap(min_heap);
    min_heap.insert(test(1, 1));
    print_heap(min_heap);
    min_heap.insert(test(1, 1));
    print_heap(min_heap);
    min_heap.insert(test(7, 8));
    print_heap(min_heap);
    min_heap.insert(test(8, 7));
    print_heap(min_heap);

    heap<test>::size_type size = min_heap.size();
    heap<test>::size_type i = 0;
    for ( ; i < size; ++i )
    {
        std::cout << "Min element = " << min_heap.top() << "\n";
        min_heap.pop_heap();
    }

    heap<test, std::vector<test>, std::greater<test> > max_heap{ test(3,2), test(2,3), test(4, 4), test(8, 8), test( 5, 5 ), test(1, 1), test(1, 1), test(7, 8), test(8,7) };

    print_heap(max_heap);
    size = max_heap.size();
    i = 0;
    for ( ; i < size; ++i )
    {
        std::cout << "Max element = " << max_heap.top() << "\n";
        max_heap.pop_heap();
    }
}
