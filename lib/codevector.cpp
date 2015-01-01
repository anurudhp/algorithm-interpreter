// do not include this file separately
// include codevector.h instead

/*********************************************
** Implementation :                         **
** template class Vector                    **
**********************************************/

/***********************
* Constructors, destructor and operator =
************************/
template<class T>
Vector<T>::Vector() {
	head = tail = NULL;
	length = 0;
}

template<class T>
Vector<T>::Vector(const Vector<T>& v) {
	*this = v;
}
template<class T>
Vector<T>& Vector<T>::operator= (const Vector<T>& v) {
	length = 0;
	Vector<T>::iterator en = v.end();
	head = tail = NULL;
	for( Vector<T>::iterator it = v.begin(); it <= en; it++)
		pushback( *it );
	return *this;
}

template<class T>
Vector<T>::Vector(__SIZETYPE len, T val) {
	length = 0;
	head = tail = NULL;
	for( __SIZETYPE i = 0; i<len; i++)
		pushback(val);
}
template<class T>
bool Vector<T>::clear() {
	while( popback() );
	return true;
}
template<class T>
Vector<T>::~Vector() {
	clear();
}

/*********************
* Properties
**********************/

template<class T>
__SIZETYPE Vector<T>::size() const {
	return length;
}
template<class T>
bool Vector<T>::empty() const {
	return (length == 0);
}

// operator[] : returns reference to value at index
template<class T>
T& Vector<T>::operator[] (__SIZETYPE index) const {
	node *pointer = head;
	while( index-- && pointer )
		pointer = pointer->next;
	if( pointer == NULL ) forcequit("vector|segfault");
	return pointer->value;
}

/**************************
* Basic manipulations
***************************/

template<class T>
Vector<T>& Vector<T>::pushback(const T& val) {
	node *n = NULL;
	n = new node;
	if(!n) forcequit("vector|heap exhausted");

	length++;
	n->value = val;
	n->next = NULL;
	n->prev = tail;
	if(tail != NULL) tail->next = n;
	tail = n;
	if(length == 1) head = n;
	return *this;
}

template<class T>
Vector<T>& Vector<T>::pushfront( const T& val){
	node *n = NULL;
	n = new node;
	if(!n) forcequit("vector|heap exhausted");

	length++;
	n->value = val;
	n->next = head;
	n->prev = NULL;
	if(head != NULL) head->prev = n;
	head = n;
	if(length == 1) tail = head;
	return *this;
}

template<class T>
bool Vector<T>::popback(){
	if(length == 0) return false;

	if(length == 1) {
		delete head;
		head = tail = NULL;
	} else {
		node *n = tail->prev;
		delete tail;
		tail = n;
		tail->next = NULL;
	}
	length--;
	return true;
}

template<class T>
bool Vector<T>::popfront() {
	if(length == 0) return false;

	if(length==1) {
		delete head;
		head = tail = NULL;
	} else {
		node *newhead;
		newhead = head->next;
		delete head;
		head = newhead;
		head->prev = NULL;
	}
	length--;
	return true;
}

template<class T>
Vector<T>& Vector<T>::insert(__SIZETYPE index, const T& val) {
	if (index<0 || index>length) return *this;
	iterator it = begin();
	while( index-- && it++ );
	return insert( it, val);
}

// insert using an iterator.
template<class T>
Vector<T>& Vector<T>::insert(iterator& it, const T& val) {
	if( it.ptr->next == NULL ) return pushback(val);

	length++;
	node *n = new node;
	if(!n) forcequit("vector|heap exhausted");

	node *oldnext = it.ptr->next;
	n->value = val;
	n->prev = it.ptr;
	n->next = oldnext;

	it.ptr->next = n;
	oldnext->prev = n;

	return *this;
}

// iterator constants for a vector
template <class T>
typename Vector<T>::iterator Vector<T>::begin() const {
	Vector<T>::iterator it;
	it.ptr = this->head;
	it.vec = this;
	it.index = 0;
	return it;
}
template <class T>
typename Vector<T>::iterator Vector<T>::end() const {
	Vector<T>::iterator it;
	it.ptr = this->tail;
	it.vec = this;
	it.index = length-1;
	return it;
}
// end implementation : Vector

/******************************************
** Implementation :                      **
** template class Vector::iterator       **
******************************************/

// constructors and operator =
// there is no need for a custom destructor, as all data in iterator is 'static'
template <class T>
Vector<T>::iterator::iterator() {
	this->ptr = NULL;
	this->index = -1;
	this->vec = NULL;
}

template <class T>
Vector<T>::iterator::iterator(const iterator& it) {
	*this = it;
}

template <class T>
typename Vector<T>::iterator Vector<T>::iterator::operator= (const iterator& it) {
	this->ptr = it.ptr;
	this->index = it.index;
	this->vec = it.vec;
}

template <class T>
Vector<T>::iterator::iterator(const Vector<T>& vec, __SIZETYPE len) {
	*this = vec.begin();
	this->index = len;
	while( len-- && this->operator++() );
}

// mutators
template <class T>
T& Vector<T>::iterator::operator* () const {
	if( ptr == NULL ){
		T v;
		return v;
	}
	return ptr->value;
}
template <class T>
bool Vector<T>::iterator::operator++ () {
	if( ptr == NULL ) return false;
	ptr = ptr->next;
	index++;
	return !!ptr;
}
template <class T>
bool Vector<T>::iterator::operator-- () {
	if( ptr == NULL ) return false;
	ptr = ptr->prev;
	index--;
	return !!ptr;
}
template <class T>
bool Vector<T>::iterator::operator++ (int) {
	operator++();
	return !!ptr;
}
template <class T>
bool Vector<T>::iterator::operator-- (int) {
	operator--();
	return !!ptr;
}

template <class T>
typename Vector<T>::iterator Vector<T>::iterator::operator+ (__SIZETYPE len) {
	while(len-- && this->operator++());
	return *this;
}
template <class T>
typename Vector<T>::iterator Vector<T>::iterator::operator- (__SIZETYPE len) {
	while(len-- && this->operator--());
	return *this;
}
template <class T>
typename Vector<T>::iterator Vector<T>::iterator::operator+= (__SIZETYPE len) {
	return (*this = *this + len);
}
template <class T>
typename Vector<T>::iterator Vector<T>::iterator::operator-= (__SIZETYPE len) {
	return (*this = *this - len);
}

// relational operators
template <class T>
bool Vector<T>::iterator::operator! () const {
	return ( this->ptr == NULL );
}
template <class T>
bool Vector<T>::iterator::operator== ( const Vector<T>::iterator& it ) const {
	return ( this->ptr == it.ptr );
}
template <class T>
bool Vector<T>::iterator::operator!= ( const Vector<T>::iterator& it ) const {
	return ( this->ptr != it.ptr );
}

template <class T>
bool Vector<T>::iterator::operator< ( const Vector<T>::iterator& it ) const {
	return ( index < it.index );
}
template <class T>
bool Vector<T>::iterator::operator> ( const Vector<T>::iterator& it ) const {
	return ( index > it.index );
}
template <class T>
bool Vector<T>::iterator::operator<= ( const Vector<T>::iterator& it ) const {
	return ( index <= it.index );
}
template <class T>
bool Vector<T>::iterator::operator>= ( const Vector<T>::iterator& it ) const {
	return ( index >= it.index );
}

