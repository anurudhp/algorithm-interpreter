#ifndef CODE_DEQUE_H
#define CODE_DEQUE_H

#include "dependencies.h"
#include "codevector.h"

// Deque is a container adapter.
// It uses the template class Vector to store data.
template <class T>
class Deque {
	private:
	Vector<T> data;
	
	public:
	// constructors,destructor and operator =
	Deque();
	Deque(const Deque<T>& );
	Deque<T>& operator= (const Deque<T>& );
	~Deque();
	
	// properties
	bool empty() const;
	T front() const;
	T rear() const;

	// basic manipulation
	Deque<T>& pushback(const T& );
	Deque<T>& pushfront(const T& );
	bool popback();
	bool popfront();
	bool popback(T&);
	bool popfront(T&);
	bool clear();
};

// Template Class Stack : adapts deque.
template <class T>
class Stack {
	private:
	Deque<T> data;
	
	public:
	// constructors,destructor and operator =
	Stack();
	Stack( const Stack<T>& );
	Stack<T>& operator= (const Stack<T>& );
	Stack<T>& operator= (const Deque<T>& );
	~Stack();
	bool clear();
	
	// members
	bool empty() const;
	T top() const;
	Stack<T>& push(const T& );
	bool pop();
	bool pop(T&);
};

// Template Class Queue : adapts deque
template <class T>
class Queue {
	private:
	Deque<T> data;
	
	public:
	// constructors,destructor and operator =
	Queue();
	Queue(const Queue<T>& );
	Queue<T>& operator= (const Queue<T>& );
	Queue<T>& operator= (const Deque<T>& );
	~Queue();
	bool clear();
	
	// members
	bool empty() const;
	T front() const;
	Queue<T>& push(const T& );
	bool pop();
	bool pop(T&);
};

#include "codedeque.cpp"

#endif /* CODE_DEQUE_H */
