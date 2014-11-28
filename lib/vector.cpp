#include "dependencies.h"
#include "codestring.h"

template<typename T>
class Vector
{

    struct node
    {
        T value;
        node* link;
    };

    node* head;
    node* tail;
    __SIZETYPE length;

    public:
    T operator[](__SIZETYPE index)const;
    __SIZETYPE size()const;

    Vector();
    Vector(const Vector<T>& v);
    Vector(__SIZETYPE len,T val);

    Vector<T>& pushback(T);
    Vector<T>& pushfront(T);
    Vector<T>& insert(T,__SIZETYPE);

    T popback();
    T popfront();

    ~Vector();

};

template<typename T>
Vector<T>:: Vector(){

    head=NULL;
    tail=NULL;
    length=0;
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v){

    if(v.size()==0){
        head=NULL;
        tail=NULL;
        length=0;
    }

    else{
        length=v.size();
        node* n=new node;
        head=n;
        tail=n;
        n->value=v[0];
        n->link=NULL;

        for(__SIZETYPE i=1;i<v.size();i++){
            n=new node;
            tail->link=n;
            tail=n;
            n->value=v[i];
            n->link=NULL;
        }
    }
}

template<typename T>
Vector<T>::Vector(__SIZETYPE len,T val){

    if(len==0){
        head=NULL;
        tail=NULL;
        length=0;
    }

    else{
        length=len;
        node* n=new node;
        head=n;
        tail=n;
        n->value=val;
        n->link=NULL;

        for(int i=1;i<length;i++){
            n=new node;
            n->value=val;
            n->link=NULL;
            tail->link=n;
            tail=n;
        }
    }
}

template<typename T>
__SIZETYPE Vector<T>::size()const{

    return length;

}

template<typename T>
T Vector<T>::operator[](__SIZETYPE index)const{

    node* pointer=head;
    for(int i=0;i<index;i++){
        pointer=pointer->link;
    }
    return pointer->value;

}

template<typename T>
Vector<T>& Vector<T>::pushback(T val){

    length++;
    node* n=new node;
    n->value=val;
    n->link=NULL;
    if(tail!=NULL) tail->link=n;
    tail=n;
    if(length==1) head=n;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::pushfront(T val){

    length++;
    node* n=new node;
    n->value=val;
    n->link=head;
    head=n;
    if(length==1) tail=head;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::insert(T val, __SIZETYPE index){

    if(index<0||index>length) return *this;
    length++;
    node* n=new node;
    n->value=val;
    bool check=false;
    if(index==0){
        n->link=head;
        head=n;
        check=true;
    }
    if(index==length-1){
        tail->link=n;
        tail=n;
        tail->link=NULL;
        check=true;
    }
    if(check==true) return *this;
    node *prev=head,*next;
    for(int i=0;i<index-1;i++) prev=prev->link;
    next=prev->link;
    prev->link=n;
    n->link=next;
    return *this;

}

template<typename T>
T Vector<T>::popback(){

    T val;
    if(length==0) return 0;
    if(length==1){
        val=head->value;
        delete head;
        head=NULL;
        tail=NULL;
        length=0;
        return val;
    }
    length--;
    node* pointer=head;
    for(int i=1;i<length;i++){
        pointer=pointer->link;
    }
    val=tail->value;
    delete tail;
    tail=pointer;
    tail->link=NULL;
    return val;
}

template<typename T>
T Vector<T>::popfront(){

    T val;
    if(length==0) return 0;
    if(length==1){
        val=head->value;
        delete head;
        head=NULL;
        tail=NULL;
        length=0;
        return val;
    }
    length--;
    node* newhead;
    val=head->value;
    newhead=head->link;
    delete head;
    head=newhead;
    return val;
}

template<typename T>
Vector<T>::~Vector(){

    node* pointer=head;
    node* next;
    for(int i=0;i<length;i++){
        next=pointer->link;
        delete pointer;
        pointer=next;
    }
}
//END.
