#ifndef VECTOR_H
#define VECTOR_H

template<class T> class Vector
{

    T* arr;
    __SIZETYPE len;

    public:

    Vector();
    Vector(__SIZETYPE);                          //Constructor to initialize size of Vector
    void resize(__SIZETYPE);
    void push_back(T);
    __SIZETYPE find(T);
    T& operator[](__SIZETYPE);
    __SIZETYPE size();
    ~Vector();
};

template<class T> Vector<T>::Vector(){arr=NULL;len=0;}

template<class T> Vector<T>::Vector(__SIZETYPE init){
    arr=new T[init];
    len=init;
}

template<class T> void Vector<T>::resize(__SIZETYPE size){ //Size of Vector is changed.
    T temp[len];
    for(__SIZETYPE i=0;i<len;i++) temp[i]=arr[i];
    delete[] arr;
    arr=new T[size];
    for(__SIZETYPE i=0;i<size;++i){                        //Elements upto the desired size are retained.
        arr[i]=temp[i];
    }
    len=size;
    return;
}

template<class T> void Vector<T>::push_back(T val){
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

template<class T> __SIZETYPE Vector<T>::find(T el){

    for(__SIZETYPE i=0;i<len;i++){
        if(arr[i]==el) return i;
    }
    return len;                        //If the desired element doesn't exist in the array, it returns one after the last index
}

template<class T> T& Vector<T>::operator[](__SIZETYPE index){
    return arr[index];
}

template<class T> Vector<T>::~Vector(){
    len=0;
    delete[] arr;
}

template<class T> __SIZETYPE Vector<T>::size(){
    return len;
}
#endif //VECTOR_H
