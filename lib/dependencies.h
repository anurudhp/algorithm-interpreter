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

// Custom types and constants
#define DEFAULT_STRING_SIZE 80
typedef unsigned long int __SIZETYPE;

// Custom Templates
class String;
template <typename> class Vector;
template <typename> class Stack;

#endif /* CUSTOM_DEPENDENCIES_H */
