#ifdef COMPONENT_LEXER_H
// do not include this file directly.
// use lexer.h instead.

/************************************
* Implementation: class Error      **
*************************************/

Error::Error(String cd, String fg, bufferIndex ln, int s) {
	this->_code = cd;
	this->_flag = fg;
	this->_line = ln;
	this->_severity = s;
}
Error::Error(const Error& e) {
	this->_code = e._code;
	this->_flag = e._flag;
	this->_line = e._line;
	this->_severity = e._severity;
}
Error& Error::operator= (const Error& e) {
	this->_code = e._code;
	this->_flag = e._flag;
	this->_line = e._line;
	this->_severity = e._severity;
	return *this;
}

String Error::code() const { return this->_code; }
String Error::flag() const { return this->_flag; }
bufferIndex Error::lineNumber() const { return this->_line; }
int Error::severity() const { return this->_severity; }

String Error::message() const {
	__SIZETYPE i;
	String ret;
	ret = ret + "Line " + integerToString(this->_line) + ": ";

	if (this->_severity == ERROR_FATAL) ret += "FATAL Error: ";
	else if (this->_severity == ERROR_ERROR) ret += "Error: ";
	else if (this->_severity == ERROR_WARNING) ret += "Warning: ";

	i = errorCodes.indexOf(this->_code, stringEquals);
	if (i >= 0) ret += errorDesc[i];
	ret += this->_flag;

	return ret;
}

void Error::setLineNumber(bufferIndex ln) { this->_line = ln; }
/*** END implementation: class Error ***/

bool importErrorCodes(ifstream& ecreader) {
	if (!ecreader) return false;
	String buff; Vector<String> vs;

	errorCodes.clear(); errorDesc.clear();
	while (!ecreader.eof()) {
		ecreader >> buff;
		if (buff.substr(0, 1) != "#" && buff.indexOf(":") > 0) {
			vs = strsplit(buff, ":");
			errorCodes.pushback(vs[0].trim());
			errorDesc.pushback(vs[1]);
		}
	}
	ecreader.close();
	return true;
}


/*********************************
* Implementation: Class Token   **
*********************************/

// constructors
Token::Token(String val, tokenType t, tokenType st, bufferIndex ln, bufferIndex in) {
	this->_value = val;
	this->_type = t;
	this->_subtype = st;
	this->_line = ln;
	this->_indent = in;
}
Token::Token(const Token& t) {
	this->_value = t._value;
	this->_type = t._type;
	this->_subtype = t._subtype;
	this->_line = t._line;
	this->_indent = t._indent;
}
Token Token::operator= (const Token& t) {
	this->_value = t._value;
	this->_type = t._type;
	this->_subtype = t._subtype;
	this->_line = t._line;
	this->_indent = t._indent;
	return *this;
}
// properties
tokenType Token::type() const { return this->_type; }
tokenType Token::subtype() const { return this->_subtype; }
bufferIndex Token::lineNumber() const { return this->_line; }
bufferIndex Token::indent() const { return this->_indent; }
String Token::value() const { return this->_value; }

// mutators
void Token::setType(tokenType t) { this->_type = t; }
void Token::setSubtype(tokenType st) { this->_subtype = st; }
void Token::setLineNumber(bufferIndex ln) { this->_line = ln; }
void Token::setIndent(bufferIndex in) { this->_indent = in; }
void Token::setValue(String s) { this->_value = s; }
// end implementation: class Token

/****************************************
* Implementation: Class Lexer   **
****************************************/

// constructors, and dynamic data managers.
Lexer::Lexer (String data) {
	char str[MAX_STRING_LENGTH];
	data.c_str(str);
	this->source.open(str);
	if (!this->source) this->errors.pushback(Error("l0", data, -1, ERROR_FATAL));
	this->line = 1;
	this->indent = 0;
}
Lexer::~Lexer() {
	if (this->source) this->source.close();
	errors.clear();
	innerBuffer.clear();
}

/*** static members ***/

bool Lexer::isValidIdentifier(String val) {
	if (val.length() > MAX_ID_LENGTH) return false;
	if (!isalpha(val[0]) && val[0] != '_') return false;
	for (__SIZETYPE i = 0; i < val.length(); i++) if (!isalnum(val[i]) && val[i] != '_') return false;
	return true;
}
// maps a keyword to a corresponding operator.
// possible only if the keyword can be replaced by the operator in the code,
// without altering the flow of logic.
String Lexer::entityMap(String val) {
	if (val == "and") return "&&";
	if (val == "or") return "||";
	if (val == "not") return "!";
	if (val == "equals") return "==";
	return val;
}

// extracts the string value from a literal token.
String Lexer::tokenToString(Token t) {
	if (t.type() != LITERAL && t.subtype() != STRING) return "";
	String val = t.value().substr(1);
	val = val.substr(0, val.length() - 1);
	return val;
}

// Converts a string into a token,
// assumes string to be somewhat valid.
Token Lexer::toToken(String val) {
	if (!val) return nullToken;
	if (val[0] == -1) return nullToken;

	val = entityMap(val);
	// string literal:
	if (val[0] == '\"' || val[0] == '\'') {
		return Token(val, LITERAL, STRING);
	}
	// numeric literal
	if (val[0] >= '0' && val[0] <= '9') {
		return Token(val, LITERAL, NUMBER);
	}
	// punctuator
	if (Punctuators.indexOf(val, stringEquals) >= 0) {
		return Token(val, PUNCTUATOR);
	}

	// keywords
	if (Keywords.indexOf(val, stringEquals) >= 0) {
		if (Constants.indexOf(val, stringEquals) >= 0) {
			if (val == "true" || val == "false") return Token(val, LITERAL, BOOLEAN);
			return Token(val, KEYWORD, CONSTANT);
		}

		return Token(val, KEYWORD);
	}

	// inbuilt functions
	if (InbuiltFunctions.indexOf(val, stringEquals) >= 0) return Token(val, IDENTIFIER, FUNCTION);

	// operators. assumes that the operator has been extracted properly.
	if (binaryOperators.indexOf(val, stringEquals) >= 0) {
		return Token(val, OPERATOR, BINARYOP);
	}
	if (unaryOperators.indexOf(val, stringEquals) >= 0) {
		return Token(val, OPERATOR, UNARYOP);
	}

	// identifier
	if (isValidIdentifier(val)) {
		return Token(val, IDENTIFIER);
	}

	return Token(val);
}

/*** Member functions: actual lexing procedures. ***/
// removes all leading spaces, and discard comments.
int Lexer::trim() {
	if (this->source.eof()) return -1;

	int sp = 0;
	while (this->source.peek() == ' ') {
		this->source.get();
		sp++;
	}
	if (this->source.peek() == '#') {
		while (this->source.get() != '\n' && !this->source.eof()); // ignore the rest of the line.
		this->endLine();
		return this->trim();
	}
	if (this->source.eof()) return -1;
	return sp;
}

// Increases the line number, and sets up the next line
// extracts the tabs from next line, sets the indent, and returns the number.
// assumes that the \n is already read from the buffer.
int Lexer::endLine() {
	this->innerBuffer.pushback(newlineToken);
	if (this->source.eof()) return -1;
	this->line++;
	// extract the indentation.
	int num = 0;
	while (this->source.peek() == '\t') {
		this->source.get();
		num++;
	}
	this->indent = num;
	return (num);
}

// extracts a string: as '...' or "..."
String Lexer::readString() {
	char st = this->source.get(),
		 tmp = 0;
	String ret = st;
	while (tmp != st) {
		tmp = this->source.get();
		if (tmp == '\n') {
			// error. string not terminated properly.
			this->errors.pushback(Error("l1", st, this->line));
			this->endLine();
			// return a null string.
			return "";
		}

		if (tmp == '\\') {
			// escape: get the next character.
			tmp = this->source.get();
			if (tmp != '\n') {
				switch (tmp) {
				case 'n': tmp = '\n'; break;
				case 't': tmp = '\t'; break;
				case 'a': tmp = '\a'; break;
				case 'r': tmp = '\r'; break;
				}
			}
			else this->endLine();
		}
		ret += tmp;
	}
	return ret;
}

// reads a numeric value: can contain utmost one decimal point.
String Lexer::readNumber() {
	String num;
	bool isDeci = false;
	char ch = this->source.get();
	while ((ch >= '0' && ch <= '9') || ch == '.') {
		if (ch == '.') {
			if (!isDeci) {
				isDeci = true;
			} else {
				// error- too many decimal points
				this->errors.pushback(Error("l2", "", this->line));
				return "0.0";
			}
		}
		num += ch;
		ch = this->source.get();
	}
	if (ch == '\n') {
		this->endLine();
	} else {
		if (!this->source.eof()) this->source.putback(ch);
	}
	return num;
}

// reads an identifier/keyword,
// assuming the starting character in the buffer is a alpha or underscore.
// does `not` check whether it is valid.
String Lexer::readIdentifier() {
	String ret;
	int len = 0;
	char ch = this->source.get();
	while (isalnum(ch) || ch == '_') {
		ret += ch;
		len++;
		ch = this->source.get();
	}
	if (!this->source.eof()) this->source.putback(ch);
	return ret;
}

// reads an operator, without touching any adjacent characters.
// this does not do a full check for all operators.
// note: some operators are 'decided' by the parser, because they depend on situation.
String Lexer::readOperator() {
	char ch = this->source.peek();
	if (Opstarts.indexOf(ch, stringEquals) == -1) return "";

	this->source.get();
	String ret = ch;

	// check whether can be followed by =
	static const idList eq(strsplit("+-*/%=!<>"));
	if (eq.indexOf(ch, stringEquals) >= 0 && this->source.peek() == '=') {
		ret += (this->source.get());
		// a second =
		if ((ch == '=' || ch == '!') && (this->source.peek() == '=')) {
			ret += this->source.get();
		}
	} else if (ch == '+' || ch == '-' || ch == '&' || ch == '|') { // operators ++ -- && ||
		if (this->source.peek() == ch) ret += (this->source.get());
	}

	return ret;
}

Token Lexer::getToken() {
	// check for a previously buffered token.
	if (!this->innerBuffer.empty()) {
		Token tmp;
		this->innerBuffer.popfront(tmp);
		return tmp;
	}

	this->trim();
	char ch = this->source.peek();
	if (this->source.eof()) return eofToken;

	String val;
	bufferIndex tline = this->line, tindent = this->indent;

	if (ch == '\n') {
		this->source.get();
		this->endLine();
		return this->getToken();
	}

	if (ch == '\'' || ch == '\"') {
		val = this->readString(); // string literal
	} else if (isalpha(ch) || ch == '_') {
		val = this->readIdentifier(); // identifier/keyword
	} else if (isdigit(ch)) {
		val = this->readNumber(); // numeric constant
	} else if (Punctuators.indexOf(ch, stringEquals) >= 0) {
		val = ch; // punctuator. keep it as it is.";
		this->source.get();
	} else if (Opstarts.indexOf(ch, stringEquals) >= 0) {
		val = this->readOperator();
	} else { // just ignore the character, as of now. This should flag an unknown character error.
		val = ch;
		this->source.get();
	}

	Token tok = toToken(val);
	tok.setLineNumber(tline);
	tok.setIndent(tindent);
	return tok;
}

bool Lexer::putbackToken(Token a) { this->innerBuffer.pushfront(a); return true; }

// extracts tokens till the delimiter.
// NOTE: does not return the delimiter token.
Infix Lexer::getTokensTill(String delim) {
	Infix ret;
	Token tmp;
	while (!this->ended()) {
		tmp = this->getToken();
		if (tmp.value() == "$eof" || tmp.value() == delim) return ret;
		ret.push(tmp);
	}
}

Vector<Error> Lexer::getErrors() const { return this->errors; }

bool Lexer::ended() {
	return (this->source && this->source.eof() && this->innerBuffer.empty());
}

bool importLexerData() {
	Keywords = strsplit("var let typeof String Number Boolean Array and or not equals delete", ' ');
	InbuiltFunctions = strsplit("print input readNumber readString readLine get", ' ');
	Constants = strsplit("null infinity true false", ' ');
	Keywords.append(Constants);
	Punctuators = strsplit("()[]{},:;");
	// operators.
	binaryOperators = strsplit("+ += - -= * *= / /= % %= = == === != !== > >= < <= && || ? . []", ' ');
	unaryOperators = strsplit("! ++ --", ' ');
	Opstarts = strsplit("+-*/%=?&|<>!.");
	return true;
}

#endif
