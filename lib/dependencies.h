#ifndef LIB_DEPENDENCIES_H_
#define LIB_DEPENDENCIES_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdint>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;
using std::istream;
using std::fstream;
using std::ifstream;
using std::ofstream;

typedef long  __SIZETYPE;  // NOLINT(runtime/int)

// Custom Templates
class String;
template <class T> class Vector;
template <class T> class Deque;
template <class T> class Stack;
template <class T> class Queue;

void forcequit(int ecode = 1) {
  cerr << "FORCE QUIT : ";
  switch (ecode) {
    case 10: cerr << "string|heap exhausted"; break;
    case 11: cerr << "string|too long"; break;
    case 20: cerr << "vector|heap exhausted"; break;
    case 21: cerr << "vector|segfault"; break;
    case 30: cerr << "deque|underflow"; break;
  }
  exit(ecode);
}

#endif  // LIB_DEPENDENCIES_H_
