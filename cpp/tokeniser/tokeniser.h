#ifndef TOKENISER_H_INCLUDED
#define TOKENISER_H_INCLUDED

#include <iterator>
#include <sstream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>

/*
 * Note: would be better to use boost::tokenizer and boost::lexical_cast
 */

/**
 * Template function to carry out conversions between types, in particular
 * from std::string to numeric types.
 */

class bad_lexical_cast : public std::bad_cast
{
};

template< typename T, typename U >
T lexical_cast( const U & u )
{
    std::stringstream ss;
    ss << u;
    T t;
    ss >> t;

    if ( ss.fail() ) throw bad_lexical_cast();

    return t;
}

/**
 * @brief The char_separator class
 *
 * Represents a set of character separators to be used when tokenising a string.
 * 'dropped' separators are not retained in the parsed output, while 'kept'
 * separators are. Multiple consecutive kept separators are combined into a single
 * output token.
 *
 * A custom const_iterator type is supplied, which advances through the parsed
 * tokens. Parsing and string evaluation is carried out on a 'lazy' basis, so
 * only tokens which are actually accessed (e.g. via an iterator dereference)
 * will result in parsing activity. In other words, no cost is incurred for
 * tokenising unwanted parts of a string.
 *
 * The parse function is provided for conveniently parsing a subset of a string
 * into supplied arguments (any number, any type). Note there is no error
 * reporting.
 *
 */
class char_separator
{
public:

    typedef std::string                                     string_type;

    explicit char_separator( const string_type & dropped, const string_type & kept = "" )

        : m_dropped ( dropped )
        , m_kept    ( kept )
    {}

    string_type         all() const     { return m_dropped + m_kept; }
    const string_type & dropped() const { return m_dropped; }
    const string_type & kept() const    { return m_kept; }

private:

    string_type     m_dropped;
    string_type     m_kept;
};

class tokeniser
{
    typedef std::string                                     string_type;
    typedef string_type::const_iterator                     string_iterator;
    typedef std::pair<string_iterator, string_iterator>     iter_pair;
    typedef string_type::size_type                          string_pos;

    friend class const_iterator;
public:

    class const_iterator : public std::iterator<std::forward_iterator_tag, string_type >
    {
        friend std::string to_string( const_iterator const &, const_iterator const & );
    public:

        const_iterator( const tokeniser & tok )

            :   m_tok   ( tok )
            ,   m_begin ( m_tok.next( m_tok.string_begin() ).first )
            ,   m_end   ( m_tok.next( m_tok.string_begin() ).second )
        {}

        const_iterator( const tokeniser & tok, const string_iterator & begin, const string_iterator & end )

            :   m_tok   ( tok )
            ,   m_begin ( begin )
            ,   m_end   ( end )
        {}

        bool operator==( const const_iterator & rhs ) const
        {
            return m_begin == rhs.m_begin && m_end == rhs.m_end;
        }

        bool operator!=( const const_iterator & rhs ) const
        {
            return m_begin != rhs.m_begin || m_end != rhs.m_end;
        }

        string_type operator*() const
        {
            return string_type( m_begin, m_end );
        }

        const_iterator & operator++()
        {
            std::tie( m_begin, m_end ) = m_tok.next( m_end );

            return *this;
        }

        const_iterator operator++( int )
        {
            const_iterator it( *this );

            this->operator ++();

            return it;
        }

    private:

        const tokeniser &       m_tok;
        string_iterator         m_begin;
        string_iterator         m_end;
    };

    tokeniser( const string_type & str, const char_separator & sep )

        : m_str( str )
        , m_sep( sep )
    {}

    tokeniser( const string_type & str, const string_type & dropped )

        : m_str( str )
        , m_sep( dropped )
    {}

    const_iterator begin() const
    {
        return const_iterator( *this );
    }

    const_iterator end() const
    {
        return const_iterator( *this, string_end(), string_end() );
    }

    template< typename Head, typename... Tail >
    void parse( Head & head, Tail &... tail ) const
    {
        const_iterator it = begin();

        parse( it, head, tail... );
    }

private:

    template< typename Head, typename... Tail >
    void parse( const_iterator & it, Head & head, Tail &... tail ) const
    {
        head = lexical_cast<Head>( *it++ );
        if ( it != end() )
            parse( it, tail... );
    }

    template< typename T >
    void parse( const_iterator & it, T & t ) const
    {
        t = lexical_cast<T>( *it++ );
    }

    string_iterator first_begin() const
    {
        string_pos pos = m_str.find_first_not_of( m_sep.dropped() );
        string_iterator b = m_str.begin();
        std::advance( b, pos );
        return b;
    }

    string_iterator first_end() const
    {
        string_pos pos = m_str.find_first_not_of( m_sep.dropped() );
        string_iterator b = m_str.begin();

        std::advance( b, pos );

        string_iterator e = next( b ).second;

        return e;
    }

    string_iterator string_begin() const
    {
        return m_str.begin();
    }

    string_iterator string_end() const
    {
        return m_str.end();
    }

    iter_pair next( const string_iterator & begin ) const
    {
        if ( begin == m_str.end() )
            return std::make_pair( begin, begin );

        string_pos current_pos = std::distance( m_str.begin(), begin );

        string_pos start_pos = m_str.find_first_not_of( m_sep.dropped(), current_pos );
        if ( start_pos == std::string::npos )
            return std::make_pair( m_str.end(), m_str.end() );

        string_iterator b = m_str.begin();
        std::advance( b, start_pos );

        string_pos end_pos = start_pos;
        if ( m_sep.kept().find( m_str[ start_pos ] ) != std::string::npos )
        {
            end_pos = m_str.find_first_not_of( m_sep.kept(), start_pos  );
        }
        else
        {
            end_pos = m_str.find_first_of( m_sep.all(), start_pos + 1 );
        }

        if ( end_pos == std::string::npos )
            return std::make_pair( b, m_str.end() );

        string_iterator e = m_str.begin();
        std::advance( e, end_pos );

        return std::make_pair( b, e );
    }


private:

    string_type     m_str;
    char_separator  m_sep;
};

std::string to_string( tokeniser::const_iterator const & b, tokeniser::const_iterator const & e )
{
    return std::string( b.m_begin, e.m_end );
}


#endif
