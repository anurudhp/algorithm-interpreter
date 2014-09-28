/*************
* This library is an alternative for the C++ STL String template
* which is not available on older compilers.
*
*	DISCLAIMER : This template does not guarentee the same performance 
*			or memory optimization or usability as the actual C++ STL string template.
*			This was designed only to give improved functionality to older compilers
*
* @license MIT License 
*	-- @c codelegend
**************/
/** 
* note that  every operator has to be overloaded with (char*) too,
*  so that it is compatible with Turbo C++ string constants ( c-style strings) ...
* this forces overloading a function multiple times, especially when it takes more than one 'String' parameter. 
*
* -- there is also provision for backward compatiblity : typecasting String to char*...
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
		
		
		/** functions : read only **/
		__SIZETYPE length();
		char& operator [] ( __SIZETYPE );
		char charAt( __SIZETYPE );
		
		String& operator + ( String& );
		String& operator + ( const char* );
		String& operator + ( const char );
		
		// searching
		__SIZETYPE indexOf( String& );
		__SIZETYPE indexOf( const char * );
		//long countOccurences( String& );
		//long countOccurences( const char * );
		
		// editing
		String& substr( __SIZETYPE, __SIZETYPE );
		//String& tolower();
		//String& toupper();
		//String& trim();
		//String& replace( String& , String& );
		//String& replace( String& , const char * );
		//String& replace( const char * , String& );
		//String& replace( const char * , const char * );
		
		
		/** functions : write **/
		
		/** Overloads for input/output **/
		friend istream& operator >> ( istream& , String& );
		friend ostream& operator << ( ostream& , String& );
		bool printToStream( ostream& );
		bool log();
		
		/** Backward compatibility **/
		operator char*();
		operator char();
};

#endif /* CODE_STRING_H */
