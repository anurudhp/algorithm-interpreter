#ifndef CUSTOM_DEPENDENCIES_H
#define CUSTOM_DEPENDENCIES_H

// use all available libraries,
// coz I cant code everything from scratch.

// check compiler and include files accordingly

#if defined(__GNUC__)

#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cstdlib>
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
template <typename> class Vector;
template <typename> class Deque;
template <typename> class Queue;
template <typename> class Stack;

bool forcequit( char * message , int ecode = 1){
	cerr<<"FORCE QUIT : "<<message;
	exit( ecode );	
}

#endif /* CUSTOM_DEPENDENCIES_H */
