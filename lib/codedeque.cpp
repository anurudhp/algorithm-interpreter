// do not include this file separately
// include codedeque.h instead

/******************************************
** Implementation : template class Deque **
*******************************************/

// Constructors, destructor and operator =
template<class T>
Deque<T>:: Deque() {}

template<class T>
Deque<T>::Deque(const Deque<T>& d) {
	*this = d;
}
template<class T>
Deque<T>& Deque<T>::operator= (const Deque<T>& d) {
	this->data = d.data;
    return *this;
}

template<class T>
Deque<T>::~Deque() {
    clear();
}

// Properties
template<class T>
bool Deque<T>::empty() const {
    return data.empty();
}
template<class T>
T Deque<T>::front() const {
    return data[0];
}
template<class T>
T Deque<T>::rear() const {
    return data[data.size()-1];
}

// Basic manipulations

template<class T>
Deque<T>& Deque<T>::pushback(const T& val) {
    data.pushback(val);
    return *this;
}
template<class T>
Deque<T>& Deque<T>::pushfront(const T& val) {
	data.pushfront(val);
    return *this;
}

template<class T>
bool Deque<T>::popback(){
	return data.popback();
}
template<class T>
bool Deque<T>::popfront() {
	return data.popfront();
}
template<class T>
bool Deque<T>::clear() {
    while (popback());
    return true;
}

// End : implementation - deque

/******************************************
** Implementation : template class Stack **
*******************************************/

// Constructors, destructor and operator =
template<class T>
Stack<T>:: Stack() {}

template<class T>
Stack<T>::Stack(const Stack<T>& d) {
	*this = d;
}
template<class T>
Stack<T>& Stack<T>::operator= (const Stack<T>& d) {
	this->data = d.data;
    return *this;
}
template<class T>
Stack<T>& Stack<T>::operator= (const Deque<T>& d) {
	this->data = d;
    return *this;
}
template<class T>
Stack<T>::~Stack() {
    clear();
}

// Properties
template<class T>
bool Stack<T>::empty() const {
    return data.empty();
}
template<class T>
T Stack<T>::top() const {
    return data.rear();
}

// Basic manipulations
template<class T>
Stack<T>& Stack<T>::push(const T& val) {
    data.pushback(val);
    return *this;
}
template<class T>
bool Stack<T>::pop(){
	return data.popback();
}
template<class T>
bool Stack<T>::clear() {
    while (pop());
    return true;
}
// End : implementation - Stack

/******************************************
** Implementation : template class Queue **
*******************************************/

// Constructors, destructor and operator =
template<class T>
Queue<T>:: Queue() {}

template<class T>
Queue<T>::Queue(const Queue<T>& d) {
	*this = d;
}
template<class T>
Queue<T>& Queue<T>::operator= (const Queue<T>& d) {
	this->data = d.data;
    return *this;
}
template<class T>
Queue<T>& Queue<T>::operator= (const Deque<T>& d) {
	this->data = d;
    return *this;
}
template<class T>
Queue<T>::~Queue() {
    clear();
}

// Properties
template<class T>
bool Queue<T>::empty() const {
    return data.empty();
}
template<class T>
T Queue<T>::front() const {
    return data.front();
}

// Basic manipulations
template<class T>
Queue<T>& Queue<T>::push(const T& val) {
    data.pushback(val);
    return *this;
}
template<class T>
bool Queue<T>::pop(){
	return data.popfront();
}
template<class T>
bool Queue<T>::clear() {
    while (pop());
    return true;
}
// End : implementation - Queue
