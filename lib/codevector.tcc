#ifdef CODE_VECTOR_H

// Definition of Vector template functions
// include this file in "codevector.h"

/***** constructors and destructor *****/
template<typename T>
Vector<T>::Vector(){
	arr = 0;
	len = 0;
}

template<typename T>
Vector<T>::Vector( __SIZETYPE s ){
    arr = new T[s];
    len = s;
}

// allocates @s elements, of value @init
template<typename T>
Vector<T>::Vector( __SIZETYPE s, T init ){
    arr = new T[s];
    len = s;
    for( __SIZETYPE i=0 ; i<len ; i++ )
    	arr[i] = init;
}

template<typename T>
Vector<T>::Vector( Vector<T>& vec ){
    len = vec.size();
    arr = new T[len];
    
    for( __SIZETYPE it = 0; it < len ; it++ )
    	arr[it] = vec[it]; // refer to operator []
}

template<typename T>
Vector<T>& Vector<T>::operator=( Vector<T>& vec ){
    len = vec.size();
    arr = new T[len];
    
    for( __SIZETYPE it = 0; it < len ; it++ )
    	arr[it] = vec[it];
    
    return *this;
}

template<typename T>
Vector<T>::~Vector(){
    len=0;
    delete[] arr;
}

/*** basic data access functions ***/
template <typename T> 
__SIZETYPE Vector<T>::size(){
    return len;
}

// operator [] : returns value (referenced) at index
template<typename T>
T& Vector<T>::operator[]( __SIZETYPE index ){
	if( index < 0 ) index += len; // if index is -ve, count from end
	if( index < 0 || index >= len ) index = 0 ;
    return arr[index];
}

// push_back : pushes an element to the end.
template<typename T>
Vector<T>& Vector<T>::push_back( T val ){
    T temp[len];
    for( __SIZETYPE i=0 ; i<len ; i++ )
		temp[i] = arr[i];
    
    delete[] arr;
    arr = new T[len+1];
    for( __SIZETYPE i=0 ; i<len ; i++ )
        arr[i] = temp[i];
        
    arr[len++] = val;
    return *this;
}

// pop_back : removes the last element and returns it.
template<typename T>
T Vector<T>::pop_back(){
    T temp[len];
    for( __SIZETYPE i=0 ; i<len ; i++ )
		temp[i] = arr[i];
    
    delete[] arr;
    arr = new T[--len];
    for( __SIZETYPE i=0 ; i<len ; i++ )
        arr[i] = temp[i];
        
    return temp[len];
}

// resize : changes the size of the vector to @size
template<typename T>
Vector<T>& Vector<T>::resize( __SIZETYPE size ){ 
    T temp[len];
    __SIZETYPE copylen = ( size < len )? size : len ;
    
    for( __SIZETYPE i=0 ; i<copylen ; i++ ) 
		temp[i] = arr[i];
    delete[] arr;
    if( size > 0 ) arr = new T[size];
    for( __SIZETYPE i=0 ; i<copylen ; ++i )
        arr[i]=temp[i];
    
    len = size;
    return *this;
}

// flush : deletes all elements of the vector
template<typename T>
Vector<T>& Vector<T>::flush(){ 
	this->resize(0);
	return *this;
}
/*** Advanced functions ***/

template<typename T> 
__SIZETYPE Vector<T>::find( T el ){

    for( __SIZETYPE i=0 ; i<len ; i++ ){
        if(arr[i] == el) return i;
    }
    return -1;
}

#endif /* CODE_VECTOR_H */
