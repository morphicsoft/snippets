#ifndef LOCKFREE_H_INCLUDED
#define LOCKFREE_H_INCLUDED

#include <atomic>
#include <stdexcept>
#include <memory>

template< typename T >
class lf_queue
{
    struct node
    {
        node() : next( nullptr ) {}

        //std::shared_ptr< T > data;
        T data;
        node * next;
    };

public:

    lf_queue()

        : head( new node )
        , tail( head.load() )
    {
    }

    lf_queue( const lf_queue & ) = delete;
    void operator=( const lf_queue & ) = delete;

    ~lf_queue()
    {
        while( node * const old_head = head.load() )
        {
            head.store( old_head->next );
            delete old_head;
        }
    }

    //std::shared_ptr< T > pop()
    T pop()
    {
        node * old_head = pop_head();
        if ( !old_head )
        {
            //return std::shared_ptr< T >();
            throw std::out_of_range( "Attempted to pop from an empty queue" );
        }

        //std::shared_ptr< T > const res( old_head->data );
        T res( old_head->data );
        delete old_head;
        return res;
    }

    void push( T new_value )
    {
        //std::shared_ptr< T > new_data( std::make_shared< T >( new_value ) );
        node * p = new node;
        node * const old_tail = tail.load();
        //old_tail->data.swap( new_data );
        old_tail->data = new_value;
        old_tail->next = p;
        tail.store( p );
    }

private:

    node * pop_head()
    {
        node * const old_head = head.load();

        if ( old_head == tail.load() )
        {
            return nullptr;
        }

        head.store( old_head->next );
        return old_head;
    }

private:

    std::atomic< node* > head;
    std::atomic< node* > tail;

};


#endif
