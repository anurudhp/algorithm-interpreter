#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

#include "dependencies.h"

template<class T> class Vector
{
    T* arr;
    __SIZETYPE len;

    public:

    Vector();
    Vector(__SIZETYPE);                          //Constructor to initialize size of Vector
    ~Vector();
    
    T& operator[](__SIZETYPE);
    __SIZETYPE size();
    void push_back(T);
    
    void resize(__SIZETYPE);
    __SIZETYPE find(T);
};

#endif /* CODE_VECTOR_H */
