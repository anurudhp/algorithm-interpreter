#ifndef COMPONENT_PARSER_H
#define COMPONENT_PARSER_H

typedef Queue<Token> RPN;

#include "variables.h"

/*****************************
* class HashedData
* Stores statements and local variables, to be saved as blocks in the parser
* Instead of the whole rpn being stored in a single vector,
* storing them in blocks helps in easy access.
*****************************/
class HashedData
{
	Vector < RPN > statementSet;
	Vector < Vector<Variable> > variableSet;

	public:
	// stores data about a single `if` block.
	// 
	struct csIf {
		RPN ifCondition, ifStatements, elseStatements;
		Vector <Variable> ifVariables, elseVariables;
	};
	// stores data about a single `for` block.
	// counterVariables are the variables declared in the for block, but initialised only once.
	struct csFor {
		RPN forInitialization, forCondition, forUpdate, forStatements;
		Vector <Variable> forVariables, counterVariables;
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

/********************************
* The Parser class
*  Takes tokens from a lexer object,
*  and converts it into the Reverse Polish Notation
*  Stores blocks in member `hashes`
*********************************/
class Parser
{
	private:
	VariableScope variables; // stores globals.
	Vector <Function> functions; // all global functions.
	Vector <Error> errors; // parser errors
	Vector <HashedData> hashes; // if, while, for, foreach blocks.
	RPN output; // the main code
	int status; // passed/failed

	// lexer interaction interface:
	Lexer *lexer;

	public:
	Parser(Lexer*, String = "");
	~Parser();

	// interface: Communication with the parser
	bool addError(Error);
	bool sendError(String, String = "", bufferIndex = -1, int = ERROR_ERROR);
	Vector<Error> getErrors();
	bool showErrors(ostream&, bool = false);
	Function getFunction(String);
	
	Token hashify(HashedData&);
	HashedData getHashedData(String);

	// parsing procedures:
	bool parseSource();

	RPN parseBlock(bufferIndex = 0);
	RPN parseDeclaration();

	// assumes that the tokens have already been read.
	RPN expressionToRPN(Infix);
	// checks whether the RPN is valid.
	bool validateRPN(RPN);
	
	static Token toArgsToken(__SIZETYPE);
	static Vector<Error> sortErrors(Vector<Error>);
	friend class Evaluator;RPN getOutput(){return output;}
};

#endif /* COMPONENT_PARSER_H */
