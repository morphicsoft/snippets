#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <iostream>
#include <typeinfo>

template< typename T >
class vector_2d
{
public:

    static const int dimension = 2;

    vector_2d()

        : comp_{}
    {}

    vector_2d( const T & x, const T & y )

        : comp_{ x, y }
    {}

    const T & x() const
    {
        return comp_[0];
    }

    const T & y() const
    {
        return comp_[1];
    }

private:

    T       comp_[ 2 ];
};

template< typename T >
std::ostream & operator<< ( std::ostream & os, const vector_2d<T> & v )
{
    os << "vector_2d<" << typeid(T).name() << ">(" << v.x() << ", " << v.y() << ")";
    return os;
}

template< typename T >
class vector_3d
{
public:

    static const int dimension = 3;

    vector_3d()

        : comp_{}
    {}

    vector_3d( const T & x, const T & y, const T & z )

        : comp_{ x, y, z }
    {}

    const T & x() const
    {
        return comp_[0];
    }

    const T & y() const
    {
        return comp_[1];
    }

    const T & z() const
    {
        return comp_[2];
    }

    vector_2d<T> to_2d() const
    {
        return vector_2d<T>( x(), y() );
    }

private:

    T       comp_[ 3 ];
};

template< typename T >
std::ostream & operator<< ( std::ostream & os, const vector_3d<T> & v )
{
    os << "vector_3d<" << typeid(T).name() << ">(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}

template< typename T, template <typename> class V >
T dot( const V<T> & u, const V<T> & v );

template< typename T>
T dot( const vector_2d<T> & u, const vector_2d<T> & v )
{
    return u.x() * v.x() + u.y() * v.y();
}

template< typename T >
T dot( const vector_3d<T> & u, const vector_3d<T> & v )
{
    return u.x() * v.x() +
            u.y() * v.y() +
            u.z() * v.z();
}

template< typename T >
T operator*( const vector_2d<T> & u, const vector_2d<T> & v )
{
    return u.x() * v.y() - u.y() * v.x();
}

template< typename T >
vector_3d<T> operator*( const vector_3d<T> & u, const vector_3d<T> & v )
{
    int i = u.y() * v.z() - u.z() * v.y();
    int j = u.z() * v.x() - u.x() * v.z();
    int k = u.x() * v.y() - u.y() * v.z();

    return vector_3d<T>( i, j, k );
}

template< typename T >
vector_2d<T> operator-( const vector_2d<T> & u, const vector_2d<T> & v )
{
    return vector_2d<T>( u.x() - v.x(), u.y() - v.y() );
}

#endif
