#ifndef CODE_VECTOR_H
#define CODE_VECTOR_H

#include "dependencies.h"
#include "codestring.h"

// vector template : a resize-able array.
// implemented as a doubly-linked list
template <typename T>
class Vector {
	private:
	struct node {
		T value;
		node *next, *prev;
	};
	node *head, *tail;
	__SIZETYPE length;

	public:
	// iterator class
	class iterator {
		private:
		node *ptr;
		__SIZETYPE index;
		const Vector<T> *vec;

		public:
		iterator();
		iterator(const iterator& );
		iterator(const Vector<T>&, __SIZETYPE );
		iterator operator= (const iterator& );

		// mutators
		T& operator* () const;
		bool operator++ ();
		bool operator-- ();
		bool operator++ (int );
		bool operator-- (int );
		iterator operator+ (__SIZETYPE );
		iterator operator- (__SIZETYPE );
		iterator operator+= (__SIZETYPE );
		iterator operator-= (__SIZETYPE );
		
		// relational operators
		bool operator! () const;
		bool operator== (const iterator& ) const;
		bool operator!= (const iterator& ) const;
		bool operator< (const iterator& ) const;
		bool operator> (const iterator& ) const;
		bool operator<= (const iterator& ) const;
		bool operator>= (const iterator& ) const;

		friend class Vector<T>;
	};
	
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
	bool isEmpty() const;

	// basic manipulation
	Vector<T>& pushback(const T& );
	Vector<T>& pushfront(const T& );
	bool popback();
	bool popfront();
	Vector<T>& insert(__SIZETYPE, const T& );
	Vector<T>& insert(iterator&, const T& );

	// iterators
	iterator begin() const;
	iterator end() const;
};

#include "codevector.cpp"

template class Vector<int>;
template class Vector<char>;
template class Vector<String>;

#endif /* CODE_VECTOR_H */
