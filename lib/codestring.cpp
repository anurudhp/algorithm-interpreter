// Implementation file for String functions, include this file in string.h
// DO NOT INCLUDE/COMPILE THIS FILE SEPARATELY.
#ifdef CODE_STRING_H

/*******************************
* 1. Constructors and operator =
*******************************/

// empty constructor.
String::String() {
	_len = 0;
	_data = _data2 = NULL;
}

// copy contructors : assignment is copy
String::String(const String& s) {
	_data2 = NULL;
	_len = s.length();
	_data = NULL;
	_data = new char[_len];
	if( !_data ) forcequit(10);
	for(int i=0; i<_len; i++)
		_data[i] = s.charAt(i);
}

String::String(const char *s) {
	_data2 = NULL;
	_len = strlen(s);
	_data = NULL;
	_data = new char[ _len ];
	if( !_data ) forcequit(10);
	for( __SIZETYPE i=0; i<_len; i++)
		_data[i] = s[i];
}

String::String(char ch) {
	_data2 = NULL;
	_len = 1;
	_data = NULL;
	_data = new char[1];
	if( !_data ) forcequit(10);
	_data[0] = ch;
}

// operator =
String& String::operator= (const String& s) {
	clear();
	_len = s.length();
	_data = new char[_len];
	if( !_data ) forcequit(10);
	for(int i=0; i<_len; i++)
		_data[i] = s.charAt(i);
	return *this;
}

String& String::operator= (const char* s ) {
	clear();
	_len = strlen(s);
	_data = new char[_len];
	if( !_data ) forcequit(10);
	for(int i=0; i<_len; i++)
		_data[i] = s[i];
	return *this;
}

String& String::operator=(char ch ) {
	clear();
	_len = 1;
	_data = new char[1];
	if( !_data ) forcequit(10);
	_data[0] = ch;
	return *this;
}
// Destructor : free some space
String::~String() {
	clear();
}
// clears all data.
bool String::clear() {
	_len = 0;
	delete[] _data;
	_data = NULL;
	delete[] _data2;
	_data2 = NULL;
	return 1;
}

// Typecast to string constant : char *
char* String::c_str() {
	delete []_data2;
	_data2 = NULL;
	_data2 = new char[_len + 1];
	for(__SIZETYPE i=0; i<_len; i++) _data2[i] = _data[i];
	_data2[_len] = '\0';
	return _data2;
}

/**************************
* 2. Read properties/data of a given string
**************************/

//size() : returns the size of the string
__SIZETYPE String::length() const {
	return _len;
}

// return the character at an index
char String::charAt(__SIZETYPE index) const {
	if( index < 0 ) index += _len;
	if( index >= _len || index < 0 ) return 0;
	return _data[index];
}

// operator [] : gives reference to char at index .
char String::zerochar = 0; // returned on erraneous requests(out of bounds)
char& String::operator[] (__SIZETYPE index ) const {
	if( index < 0 ) index += _len;
	if( index > _len || index < 0 ) {
		zerochar = 0;
		return zerochar;
	}
	return _data[index];
}

/*******************************
* 3. Manipulations
********************************/

// Operator +
String String::operator+ (const String& r ) const {
	int tlen = r.length();
	char tmp[_len+ tlen +1];
	for(__SIZETYPE i=0; i<_len; i++)
		tmp[i] = _data[i];
	for(__SIZETYPE j=0; j<tlen; j++)
		tmp[j+_len] = r[j];
	tmp[_len+ tlen]= '\0';

	String c(tmp);
	return c;
}

// shorthand operator +=
String String::operator+= (const String& s) {
	return ( *this = *this + s );
}

// substring : returns the substring from index st , and length len.
// if len is -1 (default), the substring from st till the end is returned.
// if st is negative, it is counted from the end of the string.
String String::substr(__SIZETYPE st, __SIZETYPE  len) const {
	String ret;
	if( st < 0 ) st += _len;
	if( st < 0 ) st = 0;
	if( st + len > _len || len == -1 ) len = _len - st;

	char s[ len+1 ]; 

	for( __SIZETYPE i=0 ; i<len ; ++i ){
		s[i] = _data[ st+i ];
	}
	s[len] = '\0';
	ret = s;
	return ret;
}

// replace : replaces all occurences of <find> with <rep>
String String::replace(const String& find, const String& rep) const {
	String tmp , ch(*this);

	__SIZETYPE repPos = ch.indexOf(find),
	           flen = find.length();
	while( repPos >= 0 ) {
		tmp += ch.substr(0,repPos) + rep;
		ch = ch.substr(repPos+flen);
		repPos = ch.indexOf(find);
	}
	tmp += ch;
	return tmp;
}

// tolower : converts all alphabet values to lower case.
String String::tolower() const {
	String ret(*this );
	for( __SIZETYPE i=0; i<ret.length(); i++)
		if( ret[i] >= 'A' && ret[i] <= 'Z' )
			ret[i] = ret[i] - 'A' + 'a';
	return ret;
}

// toupper : converts all alphabet values to upper case.
String String::toupper() const {
	String ret( *this );
	for( __SIZETYPE i=0; i<ret.length(); i++)
		if( ret[i] >= 'a' && ret[i] <= 'z' )
			ret[i] = ret[i] - 'a' + 'A';
	return ret;
}

// trim : removes all preceding and trailing spaces
String String::trim() const {
	__SIZETYPE st,en;
	for( st = 0; _data[st] == ' ' && st < _len; st++);
	if( st == _len ) return String();

	for( en = _len-1; _data[en] == ' ' && en >= 0; en--);
	return this->substr( st, en-st+1 );
}

// indexOf :  gives index of first occurrance of s
__SIZETYPE String::indexOf(const String& s ) const {
	__SIZETYPE tlen = s.length();
	bool isfound ;

	for( __SIZETYPE i = 0; i < _len-tlen+1 ; ++i ){
		isfound = true;
		for( __SIZETYPE j = 0; j < tlen ; ++j ){
			if( s[j] != _data[i+j] ){
				isfound = false;
				break;
			}
		}
		if( isfound ) return i;
	}
	return -1;
}

// numOccurences : gives the number of times s appears in the given string ( without overlaps )
__SIZETYPE String::countOccurences(const String& find ) const {
	__SIZETYPE tot = 0;
	int flen = find.length();
	for( int i = 0; i<_len-flen ; i++ ){
		bool match = false;
		for( int j = 0; j<flen ; j++ )
			if( _data[i+j] != find[j] ){
				match = false;
				break;
			}
		if( match ){
			tot++;
			i += flen-1;
		}
	}
	return tot;
}

/**********************************
* 3.2. Advanced type checks and
*      conversions
**********************************/

bool String::isInteger() const {
	if (!this->isNumber()) return false;
	double s = this->toNumber();
	return (s == (double(int(s))));
}
bool String::isNumber() const {
	__SIZETYPE i;
	bool isDeci = false;
	for (i = 0; i < _len; i++) {
		if (_data[i] == '-') {
			if (i != 0) return false;
			else continue;
		}
		if (_data[i] == '.') {
			if (isDeci) return false;
			isDeci = true;
		}
		else if (!(_data[i] >= '0' && _data[i] <= '9')) return false;
	}
	return true;
}

long String::toInteger() const {
	String tmp(*this);
	return atol(tmp.c_str());
}
double String::toNumber() const {
	String tmp(*this);
	return atof(tmp.c_str());
}

/***********************************
* 4. Relational Operators
************************************/

// operator ! : returns true if string is empty
bool String::operator!() const {
	return !_len;
}
bool String::equals(const String& s) const {
	if( _len != s.length() ) return false;
	for( __SIZETYPE it = 0; it < _len ; it++ )
		if( _data[it] != s[it] )
			return false;
	return true;
}
// operator == : returns true if both strings are equal( each char matches )
bool String::operator==(const String& s) const {
	return equals(s);
}

// operator != : returns true if both strings are not equal
bool String::operator!=(const String& s ) const {
	return !(equals(s));
}


/************************************
* 5. Stream IO operators : >> <<
************************************/
ostream& operator<<(ostream& output, const String& str) {
	str.print(output);
	return output;
}
bool String::print(ostream& output) const {
	for( __SIZETYPE i=0; i<_len; ++i) output<<_data[i];
	return true;
}

// this is the global input buffer for all strings.
char *String::stringInputBuffer = new char[ MAX_STRING_LENGTH ];

bool String::get(istream& input, char delim) {
	input.getline(stringInputBuffer, MAX_STRING_LENGTH, delim);
	*this = stringInputBuffer;
	return true;
} 
istream& operator>>(istream& input, String& str) {
	str.get(input);
	return input;
}

#endif /* CODE_STRING_H */
