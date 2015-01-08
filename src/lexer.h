#ifndef COMPONENT_LEXER_H
#define COMPONENT_LEXER_H

#include "../lib/codelib.h"

// Token types:
#define INVALID -1
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
#define LITERAL 5
	#define STRING 51
	#define NUMBER 52
	#define BOOLEAN 53
	
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
    tokenType type, subtype;
	bufferIndex line, indent;
    String data;
	public:
	// constructors
	Token (const Token&);
	Token (String = "", tokenType = UNKNOWN, tokenType = UNKNOWN, bufferIndex = -1, bufferIndex = 0);
	Token operator= (const Token&);
	
	// properties
	tokenType getType() const;
	tokenType getSubtype() const;
	bufferIndex getLineNumber() const;
	bufferIndex getIndent() const;
	String value() const;
	//mutators
	bool setType(tokenType);
	bool setSubtype(tokenType);
	bool setLineNumber(bufferIndex);
	bool setIndent(bufferIndex);
	bool setValue(String);
};

// Token constants:
const Token eofToken("eof", INVALID),
            nullToken("", INVALID);
// install the reserved words, and other lexer data
bool importLexerData();

class LexerProcess
{
	private:
    bufferIndex line, indent;
	ifstream source;
	ParserProcess *parser;
	
	private:
	int trim();
	int endLine();
	String readString();
	String readNumber();
	String readIdentifier();
	String readOperator();
	
    public :
    LexerProcess(ParserProcess*);
	bool setup(String);
	bool destroy();
	~LexerProcess();
	
    Token getToken();
	Vector<Token> getStatement();
	bool eof();
	
	// static members:
	static Token toToken(String);
	static bool isValidIdentifier(String);
	static String mapKeywordToOperator(String);
};

#endif /* COMPONENT_LEXER_H */