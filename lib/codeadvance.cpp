#include "codestring.h"
#include "codevector.h"

// splits a string into parts, by spl.
Vector<String> strsplit(const String& str, const String& spl) {
	Vector<String> ret;
	String tmp = str;
	if (!spl) return ret;
	__SIZETYPE index;
	while (!!tmp) {
		index = tmp.indexOf(spl);
		if (index == -1) {
			ret.pushback(tmp);
			return ret;
		}
		ret.pushback(tmp.substr(0, index));
		tmp = tmp.substr(index + spl.length());
	}
	return ret;
}

// joins a vector of strings, concatenating join as a punctuator.
String strjoin(Vector<String> vs, const String& join) {
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