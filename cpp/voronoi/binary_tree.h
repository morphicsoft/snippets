#ifndef BSP_TREE_H
#define BSP_TREE_H

template< typename T >
class binary_tree
{
    //struct node
    //{
    //    T       key;
    //    node *  left;
    //    node *  right;
    //};

public:

    binary_tree() : left_(nullptr), right_(nullptr), parent_(nullptr) {}

    void insert( const T & value );


private:

private:

    T               key_;
    binary_tree *   left_;
    binary_tree *   right_;
    binary_tree *   parent_;
};

#endif // BSP_TREE_H
