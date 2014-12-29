// do not include this file separately
// include codevector.h instead

/*********************************************
** Implementation :                         **
** template class Vector                    **
**********************************************/

/***********************
* Constructors, destructor and operator =
************************/
template<typename T>
Vector<T>::Vector() {
	head = tail = NULL;
	length = 0;
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v) {
	*this = v;
}
template<typename T>
Vector<T>& Vector<T>::operator= (const Vector<T>& v) {
	length = v.size();
	head = tail = NULL;
	for( __SIZETYPE i = 0; i<length; i++)
		pushback( v[i] );
	return *this;
}

template<typename T>
Vector<T>::Vector(__SIZETYPE len, T val) {
	length = len;
	head = tail = NULL;
	for( __SIZETYPE i = 0; i<length; i++)
		pushback(val);
}
template<typename T>
bool Vector<T>::clear() {
	while( popback() );
	return true;
}
template<typename T>
Vector<T>::~Vector() {
	clear();
}

/*********************
* Properties
**********************/

template<typename T>
__SIZETYPE Vector<T>::size() const {
	return length;
}
template<typename T>
bool Vector<T>::isEmpty() const {
	return (length == 0);
}

// operator[] : returns reference to value at index
template<typename T>
T& Vector<T>::operator[] (__SIZETYPE index) const {
	node *pointer = head;
	while( index-- )
		pointer = pointer->next;
	return pointer->value;
}

/**************************
* Basic manipulations
***************************/

template<typename T>
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

template<typename T>
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

template<typename T>
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

template<typename T>
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

template<typename T>
Vector<T>& Vector<T>::insert(__SIZETYPE index, const T& val) {
	if (index<0 || index>length) return *this;
	iterator it = begin();
	while( index-- && it++ );
	return insert( it, val);
}

// insert using an iterator.
template<typename T>
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
template <typename T>
typename Vector<T>::iterator Vector<T>::begin() const {
	Vector<T>::iterator it;
	it.ptr = this->head;
	it.vec = this;
	return it;
}
template <typename T>
typename Vector<T>::iterator Vector<T>::end() {
	Vector<T>::iterator it;
	it.ptr = this->tail;
	it.vec = this;
	return it;
}
// end implementation : Vector

/******************************************
** Implementation :                      **
** template class Vector::iterator       **
******************************************/

// constructors and operator =
// there is no need for a custom destructor, as all data in iterator is 'static'
template <typename T>
Vector<T>::iterator::iterator() {
	this->ptr = NULL;
	this->index = -1;
	this->vec = NULL;
}

template <typename T>
Vector<T>::iterator::iterator(const iterator& it) {
	*this = it;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator= (const iterator& it) {
	this->ptr = it.ptr;
	this->index = it.index;
	this->vec = it.vec;
}

template <typename T>
Vector<T>::iterator::iterator(const Vector<T>& vec, __SIZETYPE len) {
	*this = vec.begin();
	this->index = len;
	while( len-- && this->operator++() );
}

// mutators
template <typename T>
T& Vector<T>::iterator::operator* () const {
	if( ptr == NULL ){
		T v;
		return v;
	}
	return ptr->value;
}
template <typename T>
bool Vector<T>::iterator::operator++ () {
	if( ptr == NULL ) return false;
	ptr = ptr->next;
	index++;
	return !!ptr;
}
template <typename T>
bool Vector<T>::iterator::operator-- () {
	if( ptr == NULL ) return false;
	ptr = ptr->prev;
	index--;
	return !!ptr;
}
template <typename T>
bool Vector<T>::iterator::operator++ (int) {
	operator++();
	return !!ptr;
}
template <typename T>
bool Vector<T>::iterator::operator-- (int) {
	operator--();
	return !!ptr;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator+ (__SIZETYPE len) {
	while(len-- && this->operator++());
	return *this;
}
template <typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator- (__SIZETYPE len) {
	while(len-- && this->operator--());
	return *this;
}
template <typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator+= (__SIZETYPE len) {
	return (*this = *this + len);
}
template <typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator-= (__SIZETYPE len) {
	return (*this = *this - len);
}

// relational operators
template <typename T>
bool Vector<T>::iterator::operator! () const {
	return ( this->ptr == NULL );
}
template <typename T>
bool Vector<T>::iterator::operator== ( const Vector<T>::iterator& it ) const {
	return ( this->ptr == it.ptr );
}
template <typename T>
bool Vector<T>::iterator::operator!= ( const Vector<T>::iterator& it ) const {
	return ( this->ptr != it.ptr );
}

template <typename T>
bool Vector<T>::iterator::operator< ( const Vector<T>::iterator& it ) const {
	return ( index < it.index );
}
template <typename T>
bool Vector<T>::iterator::operator> ( const Vector<T>::iterator& it ) const {
	return ( index > it.index );
}
template <typename T>
bool Vector<T>::iterator::operator<= ( const Vector<T>::iterator& it ) const {
	return ( index <= it.index );
}
template <typename T>
bool Vector<T>::iterator::operator>= ( const Vector<T>::iterator& it ) const {
	return ( index >= it.index );
}

