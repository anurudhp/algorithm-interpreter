/*************
* This library is an alternative for the C++ STL String template
* which is not available on older compilers.
* Just include this header, and happily feel that strings 
* in older c++ compilers are just another fundamental type!

*	DISCLAIMER : This template does not guarentee the same performance 
*			or memory optimization or usability as the actual C++ STL string template.
*			This was designed only to give improved functionality to older compilers
*
*	note that  every operator has to be overloaded with (char*) too,
*  so that it is compatible with Turbo C++ string constants ( c-style strings) ...
**************/

/************** NOTE : DEVELOPMENT IS NOT DONE YET. MODULE MAY NOT COMPILE PROPERLY *******************/

#ifndef CODE_STRING_H
#define CODE_STRING_H

#include "dependencies.h"

class String
{
	private:
		__SIZETYPE len; // size
		char* data; // the actual string, but hidden to user. see operators [] and = for usage.
	public:
		bool log();
		// data
		
		// constructors
		String();
		String( String& );
		String( const char * );
		String operator = ( String );
		String operator = ( const char * );
		
		// destructor 
		~String();
		
		
		// functions : do not affect String
		__SIZETYPE length();
		String operator[](__SIZETYPE);
		char charAt( __SIZETYPE );
		String* operator + ( String );
		
	
};
#endif /* CODE_STRING_H */ 
