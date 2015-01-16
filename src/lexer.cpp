#ifdef COMPONENT_LEXER_H
// do not include this file directly.
// use lexer.h instead.

/*********************************
* Implementation: Class Token   **
*********************************/

// constructors
Token::Token(String val, tokenType t, tokenType st, bufferIndex ln, bufferIndex in) {
	this->data = val;
	this->type = t;
	this->subtype = st;
	this->line = ln;
	this->indent = in;
}
Token::Token(const Token& t) {
	this->data = t.data;
	this->type = t.type;
	this->subtype = t.subtype;
	this->line = t.line;
	this->indent = t.indent;
}
Token Token::operator= (const Token& t) {
	this->data = t.data;
	this->type = t.type;
	this->subtype = t.subtype;
	this->line = t.line;
	this->indent = t.indent;
	return *this;
}
// properties
tokenType Token::getType() const {
	return this->type;
}
tokenType Token::getSubtype() const {
	return this->subtype;
}
bufferIndex Token::getLineNumber() const {
	return this->line;
}
bufferIndex Token::getIndent() const {
	return this->indent;
}
String Token::value() const {
	return this->data;
}

// mutators
bool Token::setType(tokenType t) {
	this->type = t;
}
bool Token::setSubtype(tokenType st) {
	this->subtype = st;
}
bool Token::setLineNumber(bufferIndex ln) {
	this->line = ln;
}
bool Token::setIndent(bufferIndex in) {
	this->indent = in;
}
bool Token::setValue(String s) {
	this->data = s;
}
// end implementation: class Token

/****************************************
* Implementation: Class Lexer   **
****************************************/

// constructors, and dynamic data managers.
Lexer::Lexer (Parser* pr) {
	this->parser = pr;
	this->line = -1;
	this->indent = 0;
}
bool Lexer::setup(String filename) {
	this->destroy();
	this->source.open(filename.c_str());
	if (!this->source) {
		return false;
	}
	this->line = 1;
	this->indent = 0;
	return true;
}
bool Lexer::destroy() {
	if (this->source) this->source.close();
	this->line = -1;
	this->indent = 0;
}
Lexer::~Lexer() {
	this->destroy();
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
String Lexer::mapKeywordToOperator(String val) {
	if (val == "and") return "&&";
	if (val == "or") return "||";
	if (val == "not") return "!";
	if (val == "equals") return "==";
	return val;
}

// Converts a string into a token,
// assumes string to be somewhat valid.
Token Lexer::toToken(String val) {
	if (!val) return nullToken;
	if (val[0] == -1) return nullToken;
	
	// string literal:
	if (val[0] == '\"' || val[0] == '\'') {
		return Token(val, LITERAL, STRING);
	}
	// numeric literal
	if (val[0] >= '0' && val[0] <= '9') {
		return Token(val, LITERAL, NUMBER);
	}
	// punctuator
	if (Punctuators.indexOf(val) >= 0) {
		return Token(val, PUNCTUATOR);
	}
	
	val = mapKeywordToOperator(val);
	// keywords and inbuilt functions
	if (Keywords.indexOf(val) >= 0) {
		if (Constants.indexOf(val) >= 0) {
			if (val == "true" || val == "false") return Token(val, LITERAL, BOOLEAN);
			return Token(val, KEYWORD, CONSTANT);
		}
		if (IOfunctions.indexOf(val) >= 0) return Token(val, KEYWORD, IOFUNCTION);
		return Token(val, KEYWORD);
	}
	
	// operators. assumes that the operator has been extracted properly.
	if (binaryOperators.indexOf(val) >= 0) {
		return Token(val, OPERATOR, BINARYOP);
	}
	if (unaryOperators.indexOf(val) >= 0) {
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
		while (this->source.get() != '\n' && !this->source.eof()) // ignore the rest of the line.
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
	this->innerBuffer.pushback(Token(";", PUNCTUATOR, UNKNOWN, this->line, this->indent));
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
			this->parser.send("e", Error("l1", "", this->line));
			this->endLine();
			// return a null string.
			return "";
		}
		
		ret += tmp;
		if (tmp == '\\') {
			// escape: get the next character.
			tmp = this->source.get();
			if (tmp != '\n') ret += tmp;
			else this->endLine();
		}
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
				this->parser.send("e", Error("l2", "", this->line)); 
				return "";
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
	if (Opstarts.indexOf(ch) == -1) return "";
	
	this->source.get();
	String ret = ch;
	
	// check whether can be followed by =
	static const idList eq(strsplit("+-*/%=!<>"));
	if (eq.indexOf(ch) >= 0 && this->source.peek() == '=') {
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
	} else if (Punctuators.indexOf(ch) >= 0) {
		val = ch; // punctuator. keep it as it is.";
		this->source.get();
	} else if (Opstarts.indexOf(ch) >= 0) {
		val = this->readOperator();
	} else { // just ignore the character, as of now. This should flag an unknown character error.
		val = ch;
		this->source.get();
	}
	
	Token tok = toToken(val);
	tok.setLineNumber(tline);
	tok.setIndent(tindent);

	this->innerBuffer.pushback(tok);
	this->innerBuffer.popfront(tok);
	return tok;
}

bool Lexer::putbackToken(Token a) { return this->innerBuffer.pushfront(a); }

bool Lexer::eof() { return (this->source.eof() && this->innerBuffer.empty()); }

bool importLexerData() {
	Keywords = strsplit("var let typeof String Number Boolean Array and or not equals delete", ' ');
	IOfunctions = strsplit("print input readNumber readString", ' ');
	Constants = strsplit("undefined null infinity true false", ' ');
	Keywords.append(IOfunctions);
	Keywords.append(Constants);
	Punctuators = strsplit("()[]{},:;");
	// operators.
	binaryOperators = strsplit("+ += - -= * *= / /= % %= = == === != !== > >= < <= && || ? . []", ' ');
	unaryOperators = strsplit("! ++ --", ' ');
	Opstarts = strsplit("+-*/%=?&|<>!.");
	return true;
}

#endif
