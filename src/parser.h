#ifndef COMPONENT_PARSER_H
#define COMPONENT_PARSER_H

typedef Queue<Token> RPN;

#include "variables.h"

// parser status.
#define PARSE_SUCCESS 41
#define PARSE_PENDING 42
#define PARSE_STARTED 43
#define PARSE_FAILED 44

class HashedData
{
    Vector< RPN > statements;
    Vector< Vector<Variable> > variables;

    public:

    struct csIf {
        RPN if_condition,if_statements,else_statements;
        Vector< Variable > if_variables,else_variables;
    };
    struct csWhile {
        RPN while_condition,while_statements;
        Vector< Variable > while_variables;
    };
    struct csFor {
        RPN for_initialization,for_condition,for_update,for_statements;
        Vector< Variable > for_variables;
    };

    HashedData();

    csIf getIf();
    csWhile getWhile();
    csFor getFor();

};

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
	RPN getOutput();

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
