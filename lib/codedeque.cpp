// do not include this file separately
// include codedeque.h instead

/******************************************
** Implementation : template class Deque **
*******************************************/

// Constructors, destructor and operator =
template<typename T>
Deque<T>:: Deque() {}

template<typename T>
Deque<T>::Deque(const Deque<T>& d) {
	*this = d;
}
template<typename T>
Deque<T>& Deque<T>::operator= (const Deque<T>& d) {
	this->data = d.data;
    return *this;
}

template<typename T>
Deque<T>::~Deque() {
    clear();
}

// Properties
template<typename T>
bool Deque<T>::empty() const {
    return data.empty();
}
template<typename T>
T Deque<T>::front() const {
    return *(data.begin());
}
template<typename T>
T Deque<T>::rear() const {
    return *(data.end());
}

// Basic manipulations

template<typename T>
Deque<T>& Deque<T>::pushback(const T& val) {
    data.pushback(val);
    return *this;
}
template<typename T>
Deque<T>& Deque<T>::pushfront(const T& val) {
	data.pushfront(val);
    return *this;
}

template<typename T>
bool Deque<T>::popback(){
	return data.popback();
}
template<typename T>
bool Deque<T>::popfront() {
	return data.popfront();
}
template<typename T>
bool Deque<T>::clear() {
    while (popback());
    return true;
}

// End : implementation - deque

/******************************************
** Implementation : template class Stack **
*******************************************/

// Constructors, destructor and operator =
template<typename T>
Stack<T>:: Stack() {}

template<typename T>
Stack<T>::Stack(const Stack<T>& d) {
	*this = d;
}
template<typename T>
Stack<T>& Stack<T>::operator= (const Stack<T>& d) {
	this->data = d.data;
    return *this;
}
template<typename T>
Stack<T>& Stack<T>::operator= (const Deque<T>& d) {
	this->data = d;
    return *this;
}
template<typename T>
Stack<T>::~Stack() {
    clear();
}

// Properties
template<typename T>
bool Stack<T>::empty() const {
    return data.empty();
}
template<typename T>
T Stack<T>::top() const {
    return data.rear();
}

// Basic manipulations
template<typename T>
Stack<T>& Stack<T>::push(const T& val) {
    data.pushback(val);
    return *this;
}
template<typename T>
bool Stack<T>::pop(){
	return data.popback();
}
template<typename T>
bool Stack<T>::clear() {
    while (pop());
    return true;
}
// End : implementation - Stack

/******************************************
** Implementation : template class Queue **
*******************************************/

// Constructors, destructor and operator =
template<typename T>
Queue<T>:: Queue() {}

template<typename T>
Queue<T>::Queue(const Queue<T>& d) {
	*this = d;
}
template<typename T>
Queue<T>& Queue<T>::operator= (const Queue<T>& d) {
	this->data = d.data;
    return *this;
}
template<typename T>
Queue<T>& Queue<T>::operator= (const Deque<T>& d) {
	this->data = d;
    return *this;
}
template<typename T>
Queue<T>::~Queue() {
    clear();
}

// Properties
template<typename T>
bool Queue<T>::empty() const {
    return data.empty();
}
template<typename T>
T Queue<T>::front() const {
    return data.front();
}

// Basic manipulations
template<typename T>
Queue<T>& Queue<T>::push(const T& val) {
    data.pushback(val);
    return *this;
}
template<typename T>
bool Queue<T>::pop(){
	return data.popfront();
}
template<typename T>
bool Queue<T>::clear() {
    while (pop());
    return true;
}
// End : implementation - Queue
