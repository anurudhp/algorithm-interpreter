/**
* This Implementation is an alternative for the C++ STL String Class
**/
#ifndef LIB_CODESTRING_H_
#define LIB_CODESTRING_H_

#include "./dependencies.h"

const __SIZETYPE DEFAULT_STRING_SIZE = 80;
const __SIZETYPE MAX_STRING_LENGTH = 10000;

class String {
 private:
  __SIZETYPE _len;  // size
  char *_data, *_data2;  // the actual string

  // static data:
 protected:
  static char zerochar;
  static char* stringInputBuffer;

 public:
  // 1. constructors and destructor
  String();
  String(const String&);
  String(const char *);  // NOLINT(runtime/explicit)
  String(char ch);  // NOLINT(runtime/explicit)
  String& operator= (const String&);
  String& operator= (const char *);
  String& operator= (char ch);
  ~String();
  bool clear();

  // Typecast : this operator helps us treat (String) objects as (char *).
  char* c_str();

  // 2. functions : to get properties
  __SIZETYPE length() const;
  char charAt(__SIZETYPE) const;
  char& operator[] (__SIZETYPE) const;

  // 3. manipulation
  String operator+ (const String&) const;
  String operator+= (const String&);

  String substr(__SIZETYPE, __SIZETYPE = -1) const;
  String replace(const String&, const String&) const;
  String tolower() const;
  String toupper() const;
  String trim() const;

  __SIZETYPE indexOf(const String&) const;

  // 3.2. advanced type checks and conversion
  bool isNumber() const;
  bool isInteger() const;
  double toNumber() const;
  __SIZETYPE toInteger() const;

  // 4. Relational operators
  bool operator!() const;
  bool equals(const String&) const;
  bool operator==(const String&) const;
  bool operator!=(const String&) const;

  // 5. Input/output
  bool print(ostream&) const;
  bool get(istream&, char = ' ');
  friend istream& operator >> (istream&, String&);
  friend ostream& operator << (ostream&, const String&);
};

#include "codestring.cpp"

#endif  // LIB_CODESTRING_H_
