#ifndef COMPONENT_PARSER_H
#define COMPONENT_PARSER_H

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

	bool setLineNumber(bufferIndex);
};

idList errorCodes, errorDesc;
bool importErrorCodes(ifstream&);

typedef Queue<Token> RPN;

#include "variables.h"

// parser status.
#define PARSE_SUCCESS 41
#define PARSE_PENDING 42
#define PARSE_STARTED 43
#define PARSE_FAILED 44

class Parser
{
	private:
	VariableScope variables; // stores globals.
	Vector <Function> functions;
	Vector <Error> errors;
	RPN output;
	int status;
	
	// lexer interaction interface:
	Lexer *lexer;

	public:
	Parser(Lexer*, String);
	~Parser();

	// interface
	bool sendError(Error);
	Vector<Error> getErrors();
	bool showErrors(ostream&, bool = false);
	Function getFunction(String);

	// parsing procedures:
	bool parseSource();

	RPN parseBlock(bufferIndex = 0);
	RPN parseDeclaration(tokenType);
	RPN parseFunction();

	// assumes that the tokens have already been read.
	RPN expressionToRPN(Infix);

	/* static parsing procedures: */
	// checks whether the RPN is valid.
	static bool validateRPN(RPN);

	static Token toArgsToken(__SIZETYPE);

};

#endif /* COMPONENT_PARSER_H */
