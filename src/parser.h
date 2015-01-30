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
	Vector< RPN > statementSet;
	Vector< Vector<Variable> > variableSet;

	public:

	struct csIf {
		RPN ifCondition,ifStatements,elseStatements;
		Vector< Variable > ifVariables,elseVariables;
	};
	struct csFor {
		RPN forInitialization,forCondition,forUpdate,forStatements;
		Vector< Variable > forVariables;
	};

	bool clearStatements();
	bool clearVariables();
	bool addStatements(RPN)
	bool addVariables(Vector<Variable>);

	csIf getIf();
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

	friend class Evaluator;
};

#endif /* COMPONENT_PARSER_H */
