#ifndef COMPONENT_LEXER_H
#define COMPONENT_LEXER_H

// Token types:
#define DIRECTIVE -1
#define UNKNOWN 0
#define KEYWORD 1
	#define CONSTANT 11
	#define IOFUNCTION 12
#define OPERATOR 2
	#define UNARYOP  21
	#define BINARYOP 22
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

// global to store all reserved words, and symbols.
idList Keywords,
       Constants, // stores all predefined literal constant keywords
       IOfunctions, // stores all inbuilt input/output functions. 
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
	bool setType(tokenType);
	bool setSubtype(tokenType);
	bool setLineNumber(bufferIndex);
	bool setIndent(bufferIndex);
	bool setValue(String);
};

// Token constants:
const Token eofToken("$eof", DIRECTIVE),
            nullToken("$null", DIRECTIVE),
			newlineToken("\n", DIRECTIVE);
// module to install the reserved words, and other lexer data
bool importLexerData();

typedef Queue<Token> Infix;

class Lexer
{
	private:
	bufferIndex line, indent;
	ifstream source;
	Parser *parser;
	Deque<Token> innerBuffer;
	
	private:
	int trim();
	int endLine();
	String readString();
	String readNumber();
	String readIdentifier();
	String readOperator();
	
	public :
	Lexer(Parser*);
	bool setup(String);
	bool destroy();
	~Lexer();
	
	Token getToken();
	bool putbackToken(Token);
	Infix getTokensTill(String);
	Infix getStatement();
	bool eof();
	
	// static members:
	static Token toToken(String);
	static bool isValidIdentifier(String);
	static String entityMap(String);
	static String matchBracket(String);
};

#endif /* COMPONENT_LEXER_H */
