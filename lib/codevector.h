#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

#include "dependencies.h"

template<typename T>
class Vector
{
	private:
    T *arr;
    __SIZETYPE len;

    public:
	
	//constructors and destructor
    Vector();
    Vector( __SIZETYPE );
    Vector( __SIZETYPE , T );
    Vector( Vector<T>& );
    Vector<T>& operator=( Vector<T>& );
    ~Vector();
    
    // basic data access
    __SIZETYPE size();
    T& operator[]( __SIZETYPE );
    
    Vector<T>& push_back( T );
    T pop_back();
    Vector<T>& resize( __SIZETYPE );
    Vector<T>& empty();
    
    // advanced functions
    __SIZETYPE find( T );
};

#include "codevector.tcc"

#endif /* CODE_VECTOR_H */
