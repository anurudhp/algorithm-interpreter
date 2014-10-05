#include "codevector.h"

template<class T> class SearchTree
{
    T node;
    Vector<SearchTree*> ST;
    public:
    void Assign_Children();
    //This function needs to be defined such that based on the nature of this node,
    //the number of children and their type is decided. Pointers to these children are stored in ST.
};
