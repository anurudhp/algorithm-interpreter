#ifndef COMPONENT_LEXER_H
#define COMPONENT_LEXER_H

// Token types:
#define DIRECTIVE -1
#define UNKNOWN 0
#define KEYWORD 1
	#define CONSTANT 11
#define OPERATOR 2
	#define UNARYOP  21
	#define BINARYOP 22
	#define MULTINARYOP 23
#define PUNCTUATOR 3
#define IDENTIFIER 4
	#define VARIABLE 41
	#define FUNCTION 42
	#define CLASS    43
	#define ARRAY    44
	#define OBJECT   45
#define LITERAL 5
	#define STRING 51
	#define NUMBER 52
	#define BOOLEAN 53
#define HASHED 6
	
#define TABLEN 4
#define MAX_ID_LENGTH 18

typedef int tokenType;
typedef __SIZETYPE bufferIndex;
typedef Vector<String> idList;

// error codes.
#define ERROR_WARNING 31
#define ERROR_ERROR 32
#define ERROR_FATAL 33

class Error
{
	String _code, _flag;
	bufferIndex _line;
	int _severity;

	public:
	Error(String = "", String = "", bufferIndex = -1, int = ERROR_ERROR);
	Error(const Error&);
	Error& operator= (const Error&);

	String code() const;
	String flag() const;
	String message() const;
	bufferIndex lineNumber() const;
	int severity() const;

	void setLineNumber(bufferIndex);
};

idList errorCodes, errorDesc;
bool importErrorCodes(ifstream&);

// global to store all reserved words, and symbols.
idList Keywords,
       Constants, // stores all predefined literal constant keywords
       InbuiltFunctions, // stores all inbuilt functions. 
       Punctuators, // all symbols which separate lexer input.
       Opstarts,
       unaryOperators, 
       binaryOperators;

class Token
{
	private:
	tokenType _type, _subtype;
	bufferIndex _line, _indent;
	String _value;
	public:
	// constructors
	Token (const Token&);
	Token (String = "", tokenType = UNKNOWN, tokenType = UNKNOWN, bufferIndex = -1, bufferIndex = 0);
	Token operator= (const Token&);

	// properties
	tokenType type() const;
	tokenType subtype() const;
	bufferIndex lineNumber() const;
	bufferIndex indent() const;
	String value() const;
	//mutators
	void setType(tokenType);
	void setSubtype(tokenType);
	void setLineNumber(bufferIndex);
	void setIndent(bufferIndex);
	void setValue(String);
};

// Token constants:
const Token eofToken("$eof", DIRECTIVE),
            nullToken("$null", DIRECTIVE),
            newlineToken("$endline", DIRECTIVE);
// module to install the reserved words, and other lexer data
bool importLexerData();

typedef Queue<Token> Infix;

class Lexer
{
	private:
	bufferIndex line, indent;
	ifstream source;
	Vector<Error> errors;
	Deque<Token> innerBuffer;

	private:
	int trim();
	int endLine();
	String readString();
	String readNumber();
	String readIdentifier();
	String readOperator();

	public :
	Lexer(String);
	~Lexer();

	Vector<Error> getErrors() const;
	Token getToken();
	bool putbackToken(Token);
	Infix getTokensTill(String);
	bool ended();

	// static members:
	static Token toToken(String);
	static bool isValidIdentifier(String);
	static String entityMap(String);
	static String matchBracket(String);
	static String tokenToString(Token);
};

#endif /* COMPONENT_LEXER_H */
