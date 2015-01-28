#include "codestring.h"
#include "codevector.h"

// forward declarations:
Vector<String> strsplit(String, String);
String strjoin(Vector<String>, String);
String integerToString(long);
String numberToString(double);
bool stringEquals(String, String);

/** Implementation **/
// splits a string into parts, by spl.
Vector<String> strsplit(String str,  String spl = "") {
	Vector<String> ret;
	if (!spl) {
		// splits it into single characters.
		for (__SIZETYPE i = 0; i < str.length(); i++)
			ret.pushback(str[i]);
		return ret;
	}
	__SIZETYPE index;
	while (!!str) {
		index = str.indexOf(spl);
		if (index == -1) {
			ret.pushback(str);
			return ret;
		}
		ret.pushback(str.substr(0, index));
		str = str.substr(index + spl.length());
	}
	return ret;
}

// joins a vector of strings, concatenating join as a punctuator.
String strjoin(Vector<String> vs, String join = "") {
	String ret;
	bool first = true;
	for (__SIZETYPE i = 0; i < vs.size(); i++) {
		if (first) {
			first = false;
		} else {
			ret += join;
		}
		ret += vs[i];
	}
	return ret;
}

// conversions to string: stringify
String integerToString(long val) {
	char str[100];
	sprintf(str, "%ld", val);
	return String(str);
}
String numberToString(double val) {
	char str[100];
	sprintf(str, "%f", val);
	return String(str);
}

bool stringEquals(String a, String b) {
	return (a.equals(b));
}
