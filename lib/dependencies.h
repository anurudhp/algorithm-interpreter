#ifndef CUSTOM_DEPENDENCIES_H
#define CUSTOM_DEPENDENCIES_H

// use all available libraries,
// check compiler and include files accordingly

#if defined(__GNUC__)

#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <typeinfo>
using namespace std;

#else

#include <string.h>
#include <ctype.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>

#define bool int
#define true 1
#define false 0

#endif

// Custom types
#if defined(__GNUC__)
typedef long long int  __SIZETYPE;
#else
typedef long int  __SIZETYPE; // older compilers dont support long long.
#endif

// Custom Templates
class String;
template <class T> class Vector;
template <class T> class Deque;
template <class T> class Stack;
template <class T> class Queue;

bool forcequit( char * message , int ecode = 1){
	cerr<<"FORCE QUIT : "<<message;
	exit( ecode );
	return ecode; // warning in turbo.
}

#endif /* CUSTOM_DEPENDENCIES_H */
