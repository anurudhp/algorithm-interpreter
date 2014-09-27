#include "codestring.h"

/**
* Basic functions.
*	EDITING THESE CHANGES THE FRAMEWORK OF THE TEMPLATE
*	All functions must be overloaded with (char *) ,
*	as strings in Turbo C++ are c- style, 
*	whereas GNU GCC/G++ manipulates them as string constants ( refer STL string ).
*	The overload makes the user beleive that string constants in Turbo are not c-style
*	 but are actually objects of the String object .
**/

// empty constructor.
String::String(){
	len = 0;
}

// copy contructors.
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

// Destructor : free some space, for fun
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
// operator [] : gives char at index . returns null if out of bounds.
String String::operator[]( __SIZETYPE index ){
	String temp;
	if( index < len ) return temp;
	char ch[] = {data[index]};
	temp = ch;
	return temp;
}
char String::charAt( __SIZETYPE pos){
	if( pos > length() ) return char(0);
	return data[pos];
}

String& String::operator+(String r){
	int tlen = r.length();
	char* tmp = new char[ len+ tlen +1];
	for(__SIZETYPE i=0;i<len;i++)
		tmp[i] = data[i];
	for(__SIZETYPE i=0;i<tlen;i++)
		tmp[i+len] = r.charAt(i);
	tmp[len+ tlen]= '\0';
	String* c = new String(tmp);
	return c;
}

/**********************************
* Functions : read - write
***********************************/

/** operator = **/
String String::operator=( String s){
	len = s.length();
	//delete[] data;
	data = new char[len];
	for(int i=0;i<len;i++)
		data[i] = s.charAt(i);
	return *this;
}
String String::operator=( const char  * s ){
	len = strlen(s);
	data = new char[len];
	for(int i=0;i<len;i++)
		data[i] = s[i];
	return *this;
}


