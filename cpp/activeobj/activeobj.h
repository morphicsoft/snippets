#ifndef ACTIVEOBJ_H_INCLUDED
#define ACTIVEOBJ_H_INCLUDED

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <thread>


template< typename T >
class message_queue
{
public:
    typedef T                           value_type;
    typedef std::queue< T >             queue_type;

    void push( const T & value )
    {
        std::lock_guard< std::mutex > lk( mtx_ );
        q_.push( value );
        cond_.notify_one();
    }

    value_type pop()
    {
        std::unique_lock< std::mutex > lk( mtx_ );

        cond_.wait( lk, [this]{ return !q_.empty(); } );
        value_type value = q_.front();
        q_.pop();

        return value;
    }

private:

    queue_type                  q_;
    std::mutex                  mtx_;
    std::condition_variable     cond_;

};

class active
{
public:

    typedef std::function<void()>           message_type;
    typedef message_queue< message_type >   queue_type;
    typedef std::unique_ptr< std::thread >  thread_ptr;

    active()
        :   done_   ( false )
    {
        th_ = thread_ptr( new std::thread( [=]{ this->run(); } ) );
    }

    active( const active & ) = delete;
    void operator=( const active & ) = delete;

    ~active()
    {
        send( [&]
        {
              done_ = true;
        } );
        th_->join();
    }

    void send( const message_type & f )
    {
        mq_.push( f );
    }
    
private:

    void run()
    {
        while ( !done_ )
        {
            message_type message = mq_.pop();
            message();
        }
    }

private:

    bool            done_;
    queue_type      mq_;
    thread_ptr      th_;
};

class log_file
{
public:

    log_file( std::ostream & os ) : os_( os ) {}

    log_file( const log_file & ) = delete;
    void operator=( const log_file & ) = delete;

    void write( const std::string & s )
    {
        a_.send( [=]
        {
            // add in some latency (to demonstrate async nature of write call)
            std::chrono::milliseconds pause( 50 );
            std::this_thread::sleep_for( pause );

            os_ << s;
        } );
    }

private:

    std::ostream &  os_;
    active          a_;
};

#endif
