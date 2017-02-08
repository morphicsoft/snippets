#include "context.h"

#include <sstream>
#include <iostream>

class c1_settings
{
public:
    int a;
    int b;
};

class c2_settings
{
public:
    int c;
    int d;
};

class app_settings  : public c1_settings
                    , public c2_settings
{
public:

private:
};

class C1
{
public:
    C1( int a, int b )

        : a_( a )
        , b_( b )
    {}

    int a() const { return a_; }
    int b() const { return b_; }

private:

    int a_;
    int b_;
};

std::ostream & operator<<( std::ostream & os, const C1 & c1 )
{
    os << "<C1 a:" << c1.a() << " b: " << c1.b() << ">";
    return os;
}

class C2
{
public:
    C2( int c, int d )

        : c_( c )
        , d_( d )
    {}

private:

    int c_;
    int d_;
};

class C1_factory
{
public:

    static C1 create( int a, int b )
    {
        return C1( a, b );
    }

    static C1 create( const c1_settings & sett )
    {
        return C1( sett.a, sett.b );
    }

    static C1 create( std::istream & is )
    {
        int a = 0;
        int b = 0;

        is >> a;
        is >> b;

        return C1( a, b );
    }

};

class C2_factory
{
public:

    static C2 create( const c2_settings & sett )
    {
        return C2( sett.c, sett.d );
    }
};

class D
{
public:

    D( const C1 & first, const C1 & second, const C2 & c2 )

        : c1_first_( first )
        , c1_second_( second )
        , c2_( c2 )
    {}

private:

    C1 c1_first_;
    C1 c1_second_;
    C2 c2_;
};

class D_factory : public C1_factory
                , public C2_factory
{
public:

    static D create( const c1_settings & c1_sett, const c2_settings & c2_sett )
    {
        return D( C1_factory::create( c1_sett ), C1_factory::create( c1_sett ), C2_factory::create( c2_sett ));
    }

    static D create( const app_settings & sett )
    {
        return create( sett, sett );
    }
};

int main()
{
    using namespace std;

    app_settings sett;
    sett.a = 5;
    sett.b = 10;
    sett.c = 15;
    sett.d = 20;

    C1 c1 = C1_factory::create( sett );
    cout << c1 << "\n";

    C2 c2 = C2_factory::create( sett );

    std::stringstream ss;
    ss << 9 << " " << 99;

    C1 another_c1 = C1_factory::create( ss );
    cout << another_c1 << "\n";

    C1 yac1 = C1_factory::create( 42, 84 );
    cout << yac1 << "\n";

    D d = D_factory::create( sett );
}
