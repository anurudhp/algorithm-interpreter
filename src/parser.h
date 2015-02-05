#ifndef COMPONENT_PARSER_H
#define COMPONENT_PARSER_H

typedef Queue<Token> RPN;

#include "variables.h"

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
		Vector< Variable > forVariables, counterVariables;
	};

	bool clearStatements();
	bool clearVariables();
	bool addStatements(RPN);
	bool addVariables(Vector<Variable>);
	
	void setValues(csIf);
	void setValues(csFor);

	csIf getIf();
	csFor getFor();
	
	Vector<RPN> getStatements();
};


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
	Vector <HashedData> hashes;
	RPN output;
	int status;

	// lexer interaction interface:
	Lexer *lexer;

	public:
	Parser(Lexer*, String = "");
	~Parser();

	// interface
	bool sendError(Error);
	bool sendError(String, String, bufferIndex = -1, int = ERROR_ERROR);
	Vector<Error> getErrors();
	bool showErrors(ostream&, bool = false);
	Function getFunction(String);
	
	Token hashify(HashedData&);
	HashedData getHashedData(String);

	// parsing procedures:
	bool parseSource();

	RPN parseBlock(bufferIndex = 0);
	RPN parseDeclaration(Token);
	RPN parseFunction(Token);

	// assumes that the tokens have already been read.
	RPN expressionToRPN(Infix);
	// checks whether the RPN is valid.
	bool validateRPN(RPN);
	
	static Token toArgsToken(__SIZETYPE);
	friend class Evaluator;RPN getOutput(){return output;}
};

#endif /* COMPONENT_PARSER_H */
