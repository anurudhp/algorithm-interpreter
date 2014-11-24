// Implementation file for String functions, include this file in string.h
// DO NOT INCLUDE/COMPILE THIS FILE SEPARATELY.
#ifdef CODE_STRING_H

/*******************************
* 1. Constructors and operator =
*******************************/

// empty constructor.
String::String(){
	_len = 0;
	_data = 0;
}

// copy contructors : assignment is copy
String::String( const String& s){
	_len = s.length();
	_data = 0;
	_data = new char[_len];
	if( !data ) forcequit("string|heap exhausted");
	for(int i=0;i<_len;i++)
		_data[i] = s.charAt(i);
}

String::String( char *s){
	_len = strlen(s);
	_data = 0;
	_data = new char[ _len ];
	if( !data ) forcequit("string|heap exhausted");
	for( __SIZETYPE i=0;i<_len;i++)
		_data[i] = s[i];
}

String::String( char ch){
	_len = 1;
	_data = 0;
	_data = new char[1];
	if( !data ) forcequit("string|heap exhausted");
	_data[0] = ch;
}

// operator = 
String& String::operator=( const String& s){
	_len = s.length();
	delete[] _data;
	_data = 0;
	_data = new char[_len];
	if( !data ) forcequit("string|heap exhausted");
	for(int i=0;i<_len;i++)
		_data[i] = s.charAt(i);
	return *this;
}

String& String::operator=( char* s ){
	_len = strlen(s);
	delete[] _data;
	_data = 0;
	_data = new char[_len];
	if( !data ) forcequit("string|heap exhausted");
	for(int i=0;i<_len;i++)
		_data[i] = s[i];
	return *this;
}

String& String::operator=( char ch ){
	_len = 1;
	delete[] _data;
	_data = 0;
	_data = new char[1];
	if( !data ) forcequit("string|heap exhausted");
	_data[0] = ch;
	return *this;
}
// Destructor : free some space
String::~String(){
	clear();
}

// clears all data.
bool String::clear(){
	_len = 0;
	delete[] _data;
	_data = 0;
	return 1;
}

// Typecast to string constant : char *
String::operator char*(){
	char ret[_len+1];
	for(__SIZETYPE i=0;i<_len;i++) ret[i] = _data[i];
	ret[_len] = '\0';
	return ret;
}


/**************************
* 2. Read properties/data of a given string
**************************/

//size() : returns the size of the string
__SIZETYPE String::length() const{
	return _len;
}

// return the character at an index
char String::charAt( __SIZETYPE index) const{
	if( index < 0 ) index += _len;
	if( index > _len ) return 0;
	return _data[index];
}

// operator [] : gives reference to char at index .
char String::zerochar = 0; // returned on erraneous requests(out of bounds)

char& String::operator[]( __SIZETYPE index ) const{
	if( index < 0 ) index += _len;
	if( index > _len ){
		zerochar = 0;
		return zerochar;
	}
	return _data[index];
}

/*******************************
* 3. Manipulations
********************************/

// Operator +
String String::operator+( const String& r ) const {
	int tlen = r.length();
	char tmp[ _len+ tlen +1];
	for(__SIZETYPE i=0;i<_len;i++)
		tmp[i] = _data[i];
	for(__SIZETYPE i=0;i<tlen;i++)
		tmp[i+_len] = r[i];
	tmp[_len+ tlen]= '\0';
	
	String c(tmp);
	return c;
}

// shorthand operator +=
String String::operator +=( const String& s) {
	return ( *this = *this + s );
}

// substring : returns the substring from index st , and length len.
String String::substr( __SIZETYPE st , __SIZETYPE  len = 1 ) const{
	if( st < 0 ) st += _len;
	
	String ret;
	if( st < 0 || st+len > _len  ) return ret; // return null string, as bounds are erred
	
	char s[ len+1 ];
	for( __SIZETYPE i=0 ; i<len ; ++i ){
		s[i] = _data[ st+i ];	
	}
	s[len] = '\0';
	ret = s;
	return ret;
}

// replace : replaces all occurences of <find> with <rep>
String String::replace( const String& find , const String& rep ) const {
	String tmp , ch(*this);
	
	__SIZETYPE repPos = ch.indexOf(find),
				flen = find.length(), 
				rlen = rep.length();
	while( repPos >= 0 ){
		tmp += ch.substr(0,repPos) + rep;
		ch = ch.substr(repPos+flen);
		repPos = ch.indexOf(find);
	}
	tmp += ch;
	return tmp;
}

// tolower : converts all alphabet values to lower case.
String String::tolower() const {
	String ret( *this );
	for( __SIZETYPE i=0 ; i<ret.length() ; i++ )
		if( ret[i] >= 'A' && ret[i] <= 'Z' )
			ret[i] = ret[i] - 'A' + 'a';
	return ret;
}

// toupper : converts all alphabet values to upper case.
String String::toupper() const {
	String ret( *this );
	for( __SIZETYPE i=0 ; i<ret.length() ; i++ )
		if( ret[i] >= 'a' && ret[i] <= 'z' )
			ret[i] = ret[i] - 'a' + 'A';
	return ret;
}

// trim : removes all preceding and trailing spaces
String String::trim() const {
	__SIZETYPE st,en;
	for( st = 0;  _data[st] == ' ' && st < _len ; st++ );
	if( st == _len ) return String("");
	
	for( en = _len-1 ; _data[en] == ' ' && en >= 0 ; en-- );
	return this->substr( st, en-st+1 );
}

// indexOf :  gives index of first occurrance of s
__SIZETYPE String::indexOf( const String& s ) const {
	__SIZETYPE tlen = s.length();
	bool isfound ;
	
	for( __SIZETYPE i = 0; i < _len-tlen ; ++i ){
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
__SIZETYPE String::countOccurences( const String& find ) const {
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

/***********************************
* 4. Relational Operators
************************************/

// operator ! : returns true if string is empty
bool String::operator!() const {
	return !_len;
}

// operator == : returns true if both strings are equal( each char matches )
bool String::operator==( const String& s ) const {
	if( _len != s.length() ) return false;
	for( __SIZETYPE it = 0; it < _len ; it++ )
		if( _data[it] != s[it] )
			return false;
	return true;	
}

// operator != : returns true if both strings are not equal
bool String::operator!=( const String& s ) const {
	return !( *this == s );	
}


/************************************
* 5. Stream IO operators : >> <<
************************************/
ostream& operator<<( ostream& output, String& str )
{ 
 	str.print( output );
 	return output;            
}
bool String::print( ostream &output ) const {
	for( __SIZETYPE i=0;i<_len;++i)
		output<<_data[i];
}

// this is the global input buffer for all strings.
char * String::stringInputBuffer = new char[ MAX_STRING_LENGTH ];
istream& operator>>( istream& input, String& str )
{ 
	input.getline( String::stringInputBuffer , MAX_STRING_LENGTH );
	str = String::stringInputBuffer;
 	return input;            
}

#endif /* CODE_STRING_H */
