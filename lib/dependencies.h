#ifndef CUSTOM_DEPENDENCIES_H
#define CUSTOM_DEPENDENCIES_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
using namespace std;

typedef long int  __SIZETYPE;

// Custom Templates
class String;
template <class T> class Vector;
template <class T> class Deque;
template <class T> class Stack;
template <class T> class Queue;

void forcequit(int ecode = 1){
  cerr<<"FORCE QUIT : ";
  switch (ecode) {
    case 10: cerr << "string|heap exhausted"; break;
    case 11: cerr << "string|too long"; break;
    case 20: cerr << "vector|heap exhausted"; break;
    case 21: cerr << "vector|segfault"; break;
    case 30: cerr << "deque|underflow"; break;
  }
  exit(ecode);
}

#endif /* CUSTOM_DEPENDENCIES_H */
