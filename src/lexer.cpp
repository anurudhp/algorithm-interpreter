#ifdef SRC_LEXER_H_
// do not include this file directly.
// use lexer.h instead.

/************************************
* Implementation: class Error      **
*************************************/

Error::Error(String cd, String fg, bufferIndex ln, int s)
  :_code(cd), _flag(fg), _line(ln), _severity(s) {}
Error::Error(const Error& e)
  :_code(e._code), _flag(e._flag), _line(e._line), _severity(e._severity) {}

Error& Error::operator= (const Error& e) {
  this->_code = e._code;
  this->_flag = e._flag;
  this->_line = e._line;
  this->_severity = e._severity;
  return *this;
}
// accessors and properties
String Error::code() const { return this->_code; }
String Error::flag() const { return this->_flag; }
bufferIndex Error::lineNumber() const { return this->_line; }
int Error::severity() const { return this->_severity; }

String Error::message() const {
  __SIZETYPE i;
  String ret;
  if (this->_severity == ERROR_RUNTIME) ret += "(Runtime Error) ";
  if (this->_line >= 0) ret = ret + "Line " + integerToString(this->_line) + ": ";

  if (this->_severity == ERROR_FATAL) ret += "FATAL Error: ";
  else if (this->_severity == ERROR_ERROR) ret += "Error: ";
  else if (this->_severity == ERROR_WARNING) ret += "Warning: ";

  i = errorCodes.indexOf(this->_code);
  if (i >= 0) ret += errorDesc[i];
  ret += this->_flag;

  return ret;
}
// mutator
void Error::setLineNumber(bufferIndex ln) { this->_line = ln; }
/*** END implementation: class Error ***/

/*********************************
* Implementation: Class Token   **
*********************************/

// constructors
Token::Token(String val, tokenType t, tokenType st, bufferIndex ln, bufferIndex in)
  :_value(val) {
  this->_type = t;
  this->_subtype = st;
  this->_line = ln;
  this->_indent = in;
}
Token::Token(const Token& t)
  :_value(t._value) {
  this->_type = t._type;
  this->_subtype = t._subtype;
  this->_line = t._line;
  this->_indent = t._indent;
}
Token& Token::operator= (const Token& t) {
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
Lexer::Lexer(String data) {
  this->source.open(data.c_str());
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

// rules for valid identifiers:
//   1) Length cannot be more than MAX_ID_LENGTH.
//   2) should start only with _ or alphabet
//   3) all other characters can be either _ or alphanumeric.
bool Lexer::isValidIdentifier(String val) {
  if (val.length() > MAX_ID_LENGTH) return false;
  if (!isalpha(val[0]) && val[0] != '_') return false;
  for (__SIZETYPE i = 0; i < val.length(); i++) {
    if (!isalnum(val[i]) && val[i] != '_') return false;
  }
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
  if (t.type() != LITERAL || t.subtype() != STRING) return t.value();
  String val = t.value().substr(1);
  val = val.substr(0, val.length() - 1);
  return val;
}

// converts a c++ string to a string literal:
// encloses it in ""
String Lexer::stringToLiteral(String val) {
  String ret = "\"";
  ret = ret + val + ret;
  return ret;
}

// returns the equivalent type name as a string.
String Lexer::typeToString(tokenType ty) {
  switch (ty) {
    case STRING: return "string";
    case NUMBER: return "number";
    case BOOLEAN: return "boolean";
    case ARRAY: return "array";
    case OBJECT: return "object";
  }
  return "";
}

// Converts a string into a token,
// assumes string to be somewhat valid.
Token Lexer::toToken(String val) {
  if (!val) return nullToken;
  if (val[0] == -1) return nullToken;

  val = entityMap(val);
  // string literal:
  if ((val[0] == '\"' || val[0] == '\'') && (val.length() >= 2)) {
    val[0] = val[-1] = '\"';
    return Token(val, LITERAL, STRING);
  }
  // numeric literal
  if ((val[0] >= '0' && val[0] <= '9') ||
      (val[0] == '-' && val.length() > 1 && val.substr(1).isNumber())) {
    return Token(val, LITERAL, NUMBER);
  }
  // punctuator
  if (Punctuators.indexOf(val) >= 0) {
    return Token(val, PUNCTUATOR);
  }

  // keywords
  if (Keywords.indexOf(val) >= 0) {
    if (val == "typeof") return Token(val, OPERATOR, UNARYOP);
    if (Constants.indexOf(val) >= 0) {
      if (val == trueToken.value() || val == falseToken.value()) return Token(val, LITERAL, BOOLEAN);
      return Token(val, LITERAL, CONSTANT);
    }

    return Token(val, KEYWORD);
  }

  // inbuilt functions
  if (InbuiltFunctionList.indexOf(val) >= 0) return Token(val, IDENTIFIER, FUNCTION);

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
  // unable to identify.
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
    while (this->source.get() != '\n' && !this->source.eof()) {}  // ignore the rest of the line.
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

// string literal formats: '...' or "..."
String Lexer::readString() {
  char st = this->source.get(),
     tmp = 0;
  String ret = st;
  bool esc = false;
  while ((tmp != st || esc) && !this->ended()) {
    esc = false;
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
      ret += '\\';
      tmp = this->source.get();
      if (tmp == '\n') {
        this->endLine();
      } else {
        esc = true;
      }
    }
    ret += tmp;
  }
  return ret;
}

// reads a numeric value: can contain upto one decimal point.
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
// does NOT check whether it is valid.
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
  idList eq(strsplit("+-*/%=!<>"));
  if (eq.indexOf(ch) >= 0 && this->source.peek() == '=') {
    ret += (this->source.get());
    // a second =
    if ((ch == '=' || ch == '!') && (this->source.peek() == '=')) {
      ret += this->source.get();
    }
  } else if (ch == '+' || ch == '-' || ch == '&' || ch == '|') {  // operators ++ -- && ||
    if (this->source.peek() == ch) ret += (this->source.get());
  }

  return ret;
}

Token Lexer::getToken() {
  // check for a previously buffered token.
  this->trim();
  if (!this->innerBuffer.empty()) {
    Token tmp;
    this->innerBuffer.popfront(tmp);
    return tmp;
  }

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
    val = this->readString();  // string literal
  } else if (isalpha(ch) || ch == '_') {
    val = this->readIdentifier();  // identifier/keyword
  } else if (isdigit(ch)) {
    val = this->readNumber();  // numeric constant
  } else if (Punctuators.indexOf(ch) >= 0) {
    val = ch;  // punctuator. keep it as it is.";
    this->source.get();
  } else if (Opstarts.indexOf(ch) >= 0) {
    val = this->readOperator();
  } else {  // just ignore the character, as of now. This should flag an unknown character error.
    val = ch;
    this->source.get();
  }

  Token tok = toToken(val);
  tok.setLineNumber(tline);
  tok.setIndent(tindent);
  return tok;
}

// returns a token back to the lexer.
// any future getToken() call will first return this token.
bool Lexer::putbackToken(Token tok) { this->innerBuffer.pushfront(tok); return true; }

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
  return ret;
}

Vector<Error> Lexer::getErrors() const { return this->errors; }

// checks whether all tokens have been sent.
bool Lexer::ended() {
  return (this->source && this->source.eof() && this->innerBuffer.empty());
}

// Loads lexer data:
// Keywords and constants, inbuilt functions, punctuators, operators
// and sets the value of most static tokens.
// @parameter datareader: a file object which has opened the lexerdata file in ios::in mode.
bool loadLexerData() {
  Keywords = strsplit("var global typeof delete and or not equals", ' ');
  Keywords.append(strsplit("if else while until do for foreach function return", ' '));

  InbuiltFunctionList = strsplit("print printLine readNumber readString readLine", ' ');
  InbuiltFunctionList.append(strsplit("String Number Boolean Array Object Integer", ' '));

  Constants = strsplit("null infinity minusinfinity true false", ' ');
  Keywords.append(Constants);

  Punctuators = strsplit("()[]{},:;");
  // operators.
  binaryOperators = strsplit("+ += - -= * *= / /= % %= = == === != !== > >= < <= && || ? . []", ' ');
  unaryOperators = strsplit("! ++ --", ' ');
  Opstarts = strsplit("+-*/%=?&|<>!.");

  // setup the directive tokens:
  eofToken = Token("$eof", DIRECTIVE);
    nullToken = Token("$null", DIRECTIVE);
  newlineToken = Token("$endline", DIRECTIVE);

  // literal tokens
  nullvalToken = Token("null", LITERAL, CONSTANT);
  trueToken = Token("true", LITERAL, BOOLEAN);
  falseToken = Token("false", LITERAL, BOOLEAN);

  return true;
}

// Loads error data:
// codes and descriptions
// @parameter ecreader: a file object which has opened the errordata file in ios::in mode.
bool importErrorCodes(ifstream& ecreader) {  // NOLINT
  if (!ecreader) return false;
  String buff; Vector<String> vs;

  errorCodes.clear(); errorDesc.clear();
  while (!ecreader.eof()) {
    buff.get(ecreader, '\n');
    if (buff.substr(0, 1) != "#" && buff.indexOf(":") > 0) {
      vs = strsplit(buff, ":");
      errorCodes.pushback(vs[0].trim());
      vs.popfront();
      errorDesc.pushback(strjoin(vs, ":"));
    }
  }
  ecreader.close();
  return true;
}

#endif  // SRC_LEXER_H_
