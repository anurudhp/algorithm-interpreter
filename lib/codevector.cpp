#include "codevector.h"

/** constructors **/
template<typename T>
Vector<T>::Vector(){
	arr = NULL;
	len = 0;
}

template<typename T> Vector<T>::Vector(__SIZETYPE init){
    arr = new T[init];
    len = init;
}

template<typename T> void Vector<T>::resize(__SIZETYPE size){ 
    T temp[len];
    for(__SIZETYPE i=0;i<len;i++) temp[i]=arr[i];
    delete[] arr;
    arr=new T[size];
    
    //Elements upto the desired size are retained.
    for(__SIZETYPE i=0;i<size;++i){
        arr[i]=temp[i];
    }
    len=size;
    return;
}

template<typename T> void Vector<T>::push_back(T val){
    T temp[len];
    for(__SIZETYPE i=0;i<len;i++) temp[i]=arr[i];
    delete[] arr;
    arr=new T[len+1];
    for(__SIZETYPE i=0;i<len;++i){
        arr[i]=temp[i];
    }
    arr[len]=val;
    len++;
    return;
}

template<typename T> __SIZETYPE Vector<T>::find(T el){

    for(__SIZETYPE i=0;i<len;i++){
        if(arr[i]==el) return i;
    }
    return -1;//If the desired element doesn't exist in the array, it returns -1
}

template<typename T> T& Vector<T>::operator[](__SIZETYPE index){
    return arr[index];
}

template<class T> Vector<T>::~Vector(){
    len=0;
    delete[] arr;
}

template<class T> __SIZETYPE Vector<T>::size(){
    return len;
}
