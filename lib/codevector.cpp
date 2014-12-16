// do not include this file separately
// include codevector.h instead

/***********************
* Constructors, destructor and operator =
************************/
template<typename T>
Vector<T>:: Vector(){

    head = NULL;
    tail = NULL;
    length = 0;
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
        n->next=NULL;
        n->prev=NULL;

        for(__SIZETYPE i=1;i<v.size();i++){
            n=new node;
            tail->next=n;
            n->prev=tail;
            tail=n;
            n->value=v[i];
            n->next=NULL;
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
        n->next=NULL;
        n->prev=NULL;

        for(int i=1;i<length;i++){
            n=new node;
            n->value=val;
            n->next=NULL;
            tail->next=n;
            n->prev=tail;
            tail=n;
        }
    }
}

template<typename T>
Vector<T>::~Vector(){
    node* pointer=head;
    node* next;
    for(int i=0;i<length;i++){
        next=pointer->next;
        delete pointer;
        pointer=next;
    }
}

/*******************************
* Properties
******************************/

// size() : returns the size/length of the vector
template<typename T>
__SIZETYPE Vector<T>::size()const{
    return length;
}

// operator[] : returns reference to value at index
template<typename T>
T& Vector<T>::operator[](__SIZETYPE index) const {
    node* pointer = head;
    for(int i = 0; i < index; i++ )
        pointer = pointer->next;
    return pointer->value;
}

/************************************
* Basic manipulations
*************************************/
template<typename T>
Vector<T>& Vector<T>::pushback(T val){

    length++;
    node* n=new node;
    n->value=val;
    n->next=NULL;
    n->prev=tail;
    if(tail!=NULL) tail->next=n;
    tail=n;
    if(length==1) head=n;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::pushfront(T val){

    length++;
    node* n=new node;
    n->value=val;
    n->next=head;
    n->prev=NULL;
    if(head!=NULL) head->prev=n;
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
        n->next=head;
        n->prev=NULL;
        head=n;
        check=true;
    }
    if(index==length-1){
        tail->next=n;
        n->prev=tail;
        tail=n;
        tail->next=NULL;
        check=true;
    }
    if(check==true) return *this;
    node* pointer=head;
    for(int i=1;i<index;i++) pointer=pointer->next;
    pointer->next->prev=n;
    n->next=pointer->next;
    pointer->next=n;
    n->prev=pointer;
    return *this;
}

template<typename T>
void Vector<T>::popback(){

    if(length==1){
        delete head;
        head=NULL;
        tail=NULL;
        length=0;
        return;
    }
    length--;
    node*pointer=tail->prev;
    delete tail;
    tail=pointer;
    tail->next=NULL;
    return;
}

template<typename T>
void Vector<T>::popfront(){

    if(length==1){
        delete head;
        head=NULL;
        tail=NULL;
        length=0;
        return;
    }
    length--;
    node* newhead;
    newhead=head->next;
    delete head;
    head=newhead;
    head->prev=NULL;
    return;
}

