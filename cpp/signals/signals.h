#ifndef SIGNALS_H_INCLUDED
#define SIGNALS_H_INCLUDED

#include <functional>
#include <memory>
#include <vector>

namespace signals_ {

class can_be_blocked
{
public:

    can_be_blocked() : blocked_( false ) {}

    void block()            { blocked_ = true; }
    void unblock()          { blocked_ = false; }

protected:
    bool is_blocked() const { return blocked_; }

private:

    bool blocked_;
};

template< typename... Args >
class connection : public can_be_blocked
{
public:

    virtual ~connection() {}
    void fire( Args... args ) const
    {
        if ( !is_blocked() )
            fire_impl( args... );
    }

protected:
    virtual void fire_impl( Args... args ) const = 0;
};

template< typename... Args >
class nonmember_connection : public connection< Args... >
{
public:

    typedef std::function< void(Args...) >      slot_type;

    nonmember_connection( const slot_type & slot )

        :   slot_( slot )
    {
    }

    void fire_impl( Args... args ) const
    {
        slot_( args... );
    }

private:
    slot_type       slot_;
};

template< class C, typename... Args >
class const_member_connection : public connection< Args... >
{
public:
    typedef void (C::*slot_type)(Args...) const;

    const_member_connection( const C & c, const slot_type & slot )

        :   c_      ( c )
        ,   slot_   ( slot )
    {
    }

    void fire_impl( Args... args ) const
    {
        (c_.*slot_)( args... );
    }

private:

    const C &       c_;
    slot_type       slot_;
};

template< class C, typename... Args >
class member_connection : public connection< Args... >
{
public:
    typedef void (C::*slot_type)(Args...);

    member_connection( C & c, const slot_type & slot )

        :   c_      ( c )
        ,   slot_   ( slot )
    {
    }

    void fire_impl( Args... args ) const
    {
        (c_.*slot_)( args... );
    }

private:

    C &             c_;
    slot_type       slot_;
};

template< typename... >
class signal_;

// @todo: need to handle members of const classes...
template< class C, typename... Args>
class member_signal_connection : public connection< Args... >
{
public:

    typedef signal_< Args... >                  signal_type;
    typedef signal_type (C::*slot_type);

    member_signal_connection( C & c, slot_type slot )

        :   c_      ( c )
        ,   slot_   ( slot )
    {
    }

    void fire_impl( Args... args ) const
    {
        (c_.*slot_).raise( args... );
    }

private:

    C &             c_;
    slot_type       slot_;
};

template< typename... Args>
class nonmember_signal_connection : public connection< Args... >
{
public:

    typedef signal_< Args... >                  slot_type;

    nonmember_signal_connection( slot_type * slot )

        :   slot_   ( slot )
    {
    }

    void fire_impl( Args... args ) const
    {
        (*slot_).raise( args... );
    }

private:

    slot_type *     slot_;
};

template< typename... Args>
class signal_ : public can_be_blocked
{
public:

    typedef connection<Args...>                 connection_type;
    typedef std::shared_ptr< connection_type >  connection_ptr;

    signal_() {}

    typedef std::function< void(Args...) >  slot_type;

    template< class C >
    connection_type * connect( void (C::*func)(Args...), C & c )
    {
        connection_type * conn = new member_connection<C, Args...>( c, func );
        connections_.emplace_back( conn );
        return conn;
    }

    template< class C >
    connection_type * connect( void (C::*func)(Args...) const, const C & c )
    {
        connection_type * conn = new const_member_connection<C, Args...>( c, func );
        connections_.emplace_back( conn );
        return conn;
    }

    connection_type * connect( const slot_type & slot )
    {
        connection_type * conn = new nonmember_connection<Args...>( slot );
        connections_.emplace_back( conn );
        return conn;
    }

    template< class C >
    connection_type * connect( signal_ (C::*slot), C & c )
    {
        connection_type * conn = new member_signal_connection<C, Args...>( c, slot );
        connections_.emplace_back( conn );
        return conn;
    }

    connection_type * connect( signal_ * sig )
    {
        connection_type * conn = new nonmember_signal_connection<Args...>( sig );
        connections_.emplace_back( conn );
        return conn;
    }

    void raise( Args... args )
    {
        if ( is_blocked() ) return;
        for ( auto & conn : connections_ )
        {
            conn->fire( args... );
        }
    }

private:

    std::vector< connection_ptr >   connections_;
};
/*
template< typename Signal_type, typename... Args >
void raise( Signal_type & signal, Args... args )
{
    signal.raise( args... );
}

template<typename Signal_type, typename Slot_type, typename... Args>
typename Signal_type::connection_type * connect( Signal_type & signal, const Slot_type & slot, Args... args )
{
    return signal.connect( slot, args... );
}*/

}

#endif
