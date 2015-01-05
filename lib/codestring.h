/** 
* This library is an alternative for the C++ STL String template
* which is not available on older compilers.
**/
#ifndef CODE_STRING_H
#define CODE_STRING_H

#include "dependencies.h"

const __SIZETYPE DEFAULT_STRING_SIZE = 80;
const __SIZETYPE MAX_STRING_LENGTH = 10000;

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
	String(const String& );
	String(const char * );
	String(char );
	String& operator = (const String& );
	String& operator = (const char * );
	String& operator = (char );
	~String();
	bool clear();
	
	// Typecast : this operator helps us treat (String) objects as (char *) .
	//operator char*();
	char* c_str();
	
	// 2. functions : to get properties 
	__SIZETYPE length() const;
	char charAt(__SIZETYPE ) const;
	char& operator [] (__SIZETYPE ) const;
	
	// 3. manipulation
	String operator + (const String& ) const;
	String operator += (const String& );
	
	String substr(__SIZETYPE, __SIZETYPE = -1) const;
	String replace(const String& , const String& ) const;
	String tolower() const;
	String toupper() const;
	String trim() const;
	
	__SIZETYPE indexOf(const String& ) const;
	__SIZETYPE countOccurences(const String& ) const;
	
	// 4. Relational operators
	bool operator!() const;
	bool equals(const String& ) const;
	bool operator==(const String& ) const;
	bool operator!=(const String& ) const;
	
	// 5. Input/output
	bool print(ostream& ) const;
	bool get(istream&, char = '\n', bool = true);
	friend istream& operator >> (istream&, String& );
	friend ostream& operator << (ostream&, String& );
	
};

#include "codestring.cpp"

#endif /* CODE_STRING_H */
