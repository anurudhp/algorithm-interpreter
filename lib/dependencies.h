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
using namespace std;

#else

#include <string.h>
#include <ctype.h>
#include <iostream.h>
#include <fstream.h>

#endif

#define DEFAULT_STRING_SIZE 80
#define MAX_STRING_LENGTH 100000

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



#endif /* CUSTOM_DEPENDENCIES_H */
