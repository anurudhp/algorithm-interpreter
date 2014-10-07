#include "codevector.h"

template<class T> class SearchTree
{
    T node;
    Vector<SearchTree*> ST;
    public:
    void Assign_Children();
};
