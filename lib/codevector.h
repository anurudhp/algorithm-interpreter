#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

#include "dependencies.h"
#include "codestring.h"

// vector template : a resize-able array.
// implemented as a doubly-linked list
template <class T>
class Vector {
	private:
	struct node {
		T value;
		node *next, *prev;
	};
	node *head, *tail;
	__SIZETYPE length;

	public:
	// constructors,destructor and operator =
	Vector();
	Vector(const Vector<T>& );
	Vector<T>& operator= (const Vector<T>& );
	Vector(__SIZETYPE len,T val);
	~Vector();
	bool clear();

	// properties
	T& operator[](__SIZETYPE index) const;
	__SIZETYPE size() const;
	bool empty() const;

	// basic manipulation
	Vector<T>& pushback(const T& );
	Vector<T>& pushfront(const T& );
	bool popback();
	bool popfront();
	Vector<T>& insert(__SIZETYPE, const T& );
};

#include "codevector.cpp"

#endif /* CODE_VECTOR_H */
