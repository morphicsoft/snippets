#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

class context_
{
public:
    virtual ~context_() {}
};

template < class C >
class context   : public context_
                , public C
{
public:

    C create( const context_ & ctxt )
    {
        return C();
    }

};

#endif
