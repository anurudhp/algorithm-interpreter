#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

#include "dependencies.h"
#include "codestring.h"

// vector template : a resize-able array.
// implemented as a doubly-linked list

template<typename T>
class Vector
{
	private:
    struct node{
        T value;
        node* link;
    };
    node* head;
    node* tail;
    __SIZETYPE length;

    public:
    // constructors,destructor and operator =
    Vector();
    Vector(const Vector<T>& v);
    Vector(__SIZETYPE len,T val);
    ~Vector();
    
    // properties
    T& operator[](__SIZETYPE index) const;
    __SIZETYPE size()const;

	// basic manipulation
    Vector<T>& pushback(T);
    Vector<T>& pushfront(T);
    T popback();
    T popfront();
    Vector<T>& insert(T,__SIZETYPE);
};
#include "codevector.cpp"

#endif /* CODE_VECTOR_H */
