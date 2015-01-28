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
	__SIZETYPE en = v.size();
	head = tail = NULL;
	for (__SIZETYPE it = 0; it < en; it++)
		pushback(v[it]);
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
	if (index < 0) index += length;
	if (index < 0 || index >= length) forcequit("vector|segfault");
	if (index == (length - 1)) return tail->value;

	node *pointer = head;
	while (index-- && pointer) pointer = pointer->next;
	if (pointer == NULL) forcequit("vector|segfault");
	return pointer->value;
}

/**************************
* Basic manipulations
***************************/

template <class T>
bool Vector<T>::allocate(__SIZETYPE len, const T& val) {
	clear();
	length = 0;
	head = tail = NULL;
	for( __SIZETYPE i = 0; i<len; i++)
		pushback(val);
	return true;
}

template<class T>
bool Vector<T>::pushback(const T& val) {
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
	return true;
}

template<class T>
bool Vector<T>::pushfront( const T& val){
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
	return true;
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
bool Vector<T>::popback(T& ref) {
	if(length == 0) return false;
	ref = tail->value;
	return popback();
}
template<class T>
bool Vector<T>::popfront(T& ref) {
	if(length == 0) return false;
	ref = head->value;
	return popfront();
}

template<class T>
bool Vector<T>::insert(__SIZETYPE index, const T& val) {
	if (index < 0) return false;
	if (index >= length) return pushback(val);
	if (index == 0) return pushfront(val);

	node *n = NULL;
	n = new node;
	if (!n) forcequit("vector|heap exhausted");

	n->value = val;

	node *right = head, *left;
	for (int i = 0; i < index; i++) {
		right = right->next;
	}
	left = right->prev;
	left->next = right->prev = n;
	n->prev = left;
	n->next = right;
	length++;

	return true;
}

template<class T>
bool Vector<T>::remove(__SIZETYPE index) {
	if (index < 0) index += length;
	if (index >= length || index < 0) return false;

	if (index == 0) return popfront();
	if (index == length - 1) return popback();

	node *n = head;
	while (index-- && n) n = n->next;
	node *left = n->prev, *right = n->next;
	left->next = right;
	right->prev = left;
	delete n;
	length--;
	return true;
}

// advanced manipulations

template<class T>
__SIZETYPE Vector<T>::indexOf(const T& val, bool (*equals)(T, T)) const {
	if (length == 0) return -1;
	node *n = head;
	__SIZETYPE ret = 0;
	while (n && equals(n->value, val)) {
		n = n->next;
		ret++;
	}
	if (n == NULL) return -1;
	return ret;
}

template<class T>
bool Vector<T>::append(const Vector<T>& vec, bool isFront) {
	node *st;
	if (!isFront) {
		st = vec.head;
		for (__SIZETYPE i = 0; i < vec.length; i++, st = st->next) {
			this->pushback(st->value);
		}
	} else {
		st = vec.tail;
		for (__SIZETYPE i = vec.length - 1; i >= 0; i--, st = st->prev) {
			this->pushfront(st->value);
		}
	}
	return true;
}
// end implementation : Vector
