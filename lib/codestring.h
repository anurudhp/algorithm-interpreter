/** 
* This library is an alternative for the C++ STL String template
* which is not available on older compilers.
*
* note that  every operator has to be overloaded with <char*> too,
*  so that it is compatible with Turbo C++ string constants ( c-style strings)
*
* there is also provision for backward compatiblity : typecasting String to char*
* this enables functions with <char *> as parameters, to be called with <String> arguments.
**/
#ifndef CODE_STRING_H
#define CODE_STRING_H

#include "dependencies.h"

class String
{
	private:
		__SIZETYPE len; // size
		char* data; // the actual string, but hidden to user. see operators [] and = for usage.
	public:
		// constructors
		String();
		String( String& );
		String( const char * );
		String& operator = ( String& );
		String& operator = ( const char * );
		
		// destructor 
		~String();
		
		// Typecasts :
		
		// this operator helps us treat (String) objects as (const char *) .
		// so,  any function using a parameter (const char *) , can be called passing a (String) to it.
		operator char*(); 
		operator char();
		
		
		/** functions : read only **/
		__SIZETYPE length();
		char& operator [] ( __SIZETYPE );
		char charAt( __SIZETYPE );
		
		String& operator + ( String& );
		String& operator + ( const char* );
		String& operator + ( const char );
		
		// shorthand operators
		String& operator += ( const char* );
		
		
		// searching
		__SIZETYPE indexOf( String& );
		__SIZETYPE indexOf( const char * );
		//long countOccurences( const char * );
		
		// editing
		String& substr( __SIZETYPE, __SIZETYPE );
		//String& tolower();
		//String& toupper();
		//String& trim();
		String& replace( const char * , const char * );
		
		/** functions : write **/
		
		/** Overloads for input/output **/
		friend istream& operator >> ( istream& , String& );
		friend ostream& operator << ( ostream& , String& );
		bool printToStream( ostream& );
		bool log();
		
};

#endif /* CODE_STRING_H */
