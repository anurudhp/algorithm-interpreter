/****
* Implementation file for String functions, include this file in string.h
* DO NOT INCLUDE/COMPILE THIS FILE SEPARATELY.
****/

#ifdef CODE_STRING_H
/**
*	All functions must be overloaded with (char *) ,
*	as strings in Turbo C++ are c- style, 
*	whereas GNU GCC/G++ manipulates them as string constants ( refer STL string ).
*	The overload makes the user believe that string constants in Turbo are not c-style
*	but are actually objects of the String object .
**/

/*******************************
* Constructors and operator =
*******************************/

// empty constructor.
String::String(){
	len = 0;
}

// copy contructors : assignment is copy
String::String( String& s){
	len = s.length();
	data = new char[len];
	for(int i=0;i<len;i++)
		data[i] = s.charAt(i);
}

String::String( const char *s){
	len = strlen(s);
	data = new char[ len ];
	for( __SIZETYPE i=0;i<len;i++)
		data[i] = s[i]; // this string template does not manipulate data as a c style string.
							   // so dont use strcpy()
}

/** operator = **/
String& String::operator=( String& s){
	len = s.length();
	//delete[] data;
	data = new char[len];
	for(int i=0;i<len;i++)
		data[i] = s.charAt(i);
	return *this;
}

String& String::operator=( const char  * s ){
	len = strlen(s);
	data = new char[len];
	for(int i=0;i<len;i++)
		data[i] = s[i];
	return *this;
}

// Destructor : free some space
String::~String(){
	len = 0;
	delete[] data;
}

/**
* Actual String functions
* There are two types of functions, 
* -- those which affect the data in the string
* -- those which do not.
*/

/**********************************
* Functions : read only
***********************************/

//size() : returns the size of the string
__SIZETYPE String::length(){
	return len;
}

// return the character at an index
char String::charAt( __SIZETYPE index){
	if( index < 0 ) index += len;
	if( index > len ) return 0;
	return data[index];
}

// Operator +
String& String::operator+(String& r){
	int tlen = r.length();
	char* tmp = new char[ len+ tlen +1];
	for(__SIZETYPE i=0;i<len;i++)
		tmp[i] = data[i];
	for(__SIZETYPE i=0;i<tlen;i++)
		tmp[i+len] = r.charAt(i);
	tmp[len+ tlen]= '\0';
	String* c = new String(tmp);
	return *c;
}
String& String::operator+( const char *r){
	int tlen = strlen(r);
	char* tmp = new char[ len+ tlen +1];
	for(__SIZETYPE i=0;i<len;i++)
		tmp[i] = data[i];
	for(__SIZETYPE i=0;i<tlen;i++)
		tmp[i+len] = r[i];
	tmp[len+ tlen]= '\0';
	String* c = new String(tmp);
	return *c;
}
String& String::operator+( const char c ){
	char *s = new char[1];
	s[0] = c;
	return this->operator+(s);
}



/*** substring functions ***/
String& String::substr( __SIZETYPE l, __SIZETYPE u = 0 ){
	if( l < 0 ) l += len;
	if( u <= 0 ) u += len;
	
	String *ret = new String();
	if( l >= u || u > len  ) return *ret; // return null string, as bounds are erred
	
	char *s = new char[ u-l+1 ];
	for( __SIZETYPE i=l; i<u ; ++i ){
		s[i-l] = data[i];	
	}
	s[u-l] = '\0';
	*ret = s;
	return *ret;
}

// searching :

// returns the index of  beginning of string s, if found, else returns -1
__SIZETYPE String::indexOf( String& s ){
	__SIZETYPE tlen = s.length();
	bool isfound ;
	for( __SIZETYPE i = 0; i < len-tlen ; ++i ){
		isfound = true;
		for( __SIZETYPE j = 0; j < tlen ; ++j ){
			if( s.charAt(j) != data[i+j] ){
				isfound = false;
				break;
			}
		}
		if( isfound ) return i;
	}
	return -1;
}
__SIZETYPE String::indexOf( const char *s ){
	__SIZETYPE tlen = strlen(s);
	bool isfound ;
	
	for( __SIZETYPE i = 0; i < len-tlen ; ++i ){
		isfound = true;
		for( __SIZETYPE j = 0; j < tlen ; ++j ){
			if( s[j] != data[i+j] ){
				isfound = false;
				break;
			}
		}
		if( isfound ) return i;
	}
	return -1;
}


/**********************************
* Functions : write
***********************************/

// replace : replaces all occurences of <find> with <rep>
String& String::replace( const char *__find , const char *__rep ){
	String *tmp = new String();
	String ch(*this), find(__find), rep(__rep);
	
	__SIZETYPE repPos = ch.indexOf(find),
				flen = find.length(), 
				rlen = rep.length();
	while( repPos >= 0 ){
		(*tmp) += ch.substr(0,repPos) + rep;
		ch = ch.substr(repPos+flen);
		repPos = ch.indexOf(find);
	}
	*tmp += ch;
	return *tmp;
}

// shorthand operator +=
String& String::operator +=( const char *s ){
	(*this) = (*this)+s;
	return *this;
}
/**********************************
* Functions : read/write
**********************************/

// operator [] : gives reference to char at index . returns null if out of bounds.
char& String::operator[]( __SIZETYPE index ){
	if( index < 0 ) index += len;
	if( index > len ){
		char *t = new char;
		return *t;
	}
	return data[index];
}

/************************************
* Stream IO operators : >> <<
************************************/
ostream& operator<<( ostream &output, String &D )
{ 
 	D.printToStream( output );
 	return output;            
}
bool String::printToStream( ostream &output ){
	for( __SIZETYPE i=0;i<len;++i)
		output<<data[i];	
}

// this is the global input buffer for all strings.
// as string length is not known, 
// creating a huge buffer during each input operation is not viable.
// so , just create a global buffer and use it for every string. ( C++ is synchronous, so cheers! :) )
char *stringInputBuffer = new char[ MAX_STRING_LENGTH ];

istream& operator>>( istream &input, String &D )
{ 
	cin.getline( stringInputBuffer , MAX_STRING_LENGTH );
	D = stringInputBuffer;
 	return input;            
}

/*******************************
* Compatiblity modules 
* for string constants (char *)
********************************/
String::operator char*(){
	char *ret = new char[len+1];
	for(__SIZETYPE i=0;i<len;i++) ret[i] = data[i];
	ret[len] = '\0';
	return ret;
}
String::operator char(){
	return data[0];
}


bool String::log(){
	printToStream( cout );
}

#endif /* CODE_STRING_H */
