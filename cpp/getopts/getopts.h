#ifndef GETOPTS_H_INCLUDED
#define GETOPTS_H_INCLUDED

#include <map>
#include <string>

template< typename T, typename U >
T lexical_cast( const U & u )
{
    std::stringstream ss;
    ss << u;
    T t;
    ss >> t;
    return t;
}

class value_base
{
public:
    virtual ~value_base() {}
private:
};

template< typename T >
class value : public value_base
{};

class string_value
{
public:

    explicit string_value( const std::string & value )

        : m_value( value )
    {}

    operator std::string() const
    {
        return m_value;
    }

    template< typename T >
    T as() const
    {
        return lexical_cast<T>( m_value );
    }

private:

    std::string     m_value;
};

class options
{
public:

    typedef std::map< std::string, value_base >         options_map;

    options & add_options()
    {
        return *this;
    }

    options & operator()( const std::string & opt, const std::string & )
    {
        m_options.insert( { opt, value<void>() } );
        return *this;
    }

    template< typename T >
    options & operator()( const std::string & opt, const value<T> & type, const std::string & )
    {
        m_options.insert( { opt, type } );
        return *this;
    }


private:

    //string_map      m_options;
    options_map         m_options;

};

class variables_map
{};

variables_map parse_command_line( int argc, char ** argv, const options & allowed )
{
    variables_map opt;

    while( *argv )
    {
        ++argv;
    }

    return opt;
}

#endif
