/** 
* This library is an alternative for the C++ STL String template
* which is not available on older compilers.
**/
#ifndef CODE_STRING_H
#define CODE_STRING_H

#include "dependencies.h"

const __SIZETYPE DEFAULT_STRING_SIZE = 80;
const __SIZETYPE MAX_STRING_LENGTH = 100000;

class String
{
private:
	__SIZETYPE _len; // size
	char* _data; // the actual string
	
	// static data:
protected:
	static char zerochar;
	static char* stringInputBuffer; 
	
public:
	// 1. constructors and destructor
	String();
	String( const String& );
	String( char * );
	String( char );
	String& operator = ( const String& );
	String& operator = ( char * );
	String& operator = ( char );
	~String();
	bool clear();
	
	// Typecast : this operator helps us treat (String) objects as (char *) .
	operator char*();
	
	// 2. functions : to get properties 
	__SIZETYPE length() const;
	char charAt( __SIZETYPE ) const;
	char& operator [] ( __SIZETYPE ) const;
	
	// 3. manipulation
	String operator + ( const String& ) const;
	String operator += ( const String& );
	
	String substr( __SIZETYPE, __SIZETYPE ) const;
	String replace( const String& , const String& ) const;
	String tolower() const;
	String toupper() const;
	String trim() const;
	
	__SIZETYPE indexOf( const String& ) const;
	__SIZETYPE countOccurences( const String& ) const;
	
	// 4. Relational operators
	bool operator!() const;
	bool operator==( const String& ) const;
	bool operator!=( const String& ) const;
	
	// 5. Overloads for input/output
	friend istream& operator >> ( istream& , String& );
	friend ostream& operator << ( ostream& , String& );
	bool print( ostream& ) const;
};

#include "codestring.cpp"

#endif /* CODE_STRING_H */
