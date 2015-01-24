#ifndef COMPONENT_PARSER_H
#define COMPONENT_PARSER_H

// error codes.
#define WARNING 31
#define ERROR 32
#define FATAL 33

class Error
{
	String _code, _flag;
	bufferIndex _line;
	int _severity;

	public:
	Error(String = "", String = "", bufferIndex = -1, int = ERROR);
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
bool importErrorCodes();

typedef Queue<Token> Infix;
typedef Queue<Token> RPN;

class Variable
{
	String _id;
	Token _value;
	Vector<Token> _keys;
	Vector<Variable> _values;

	public:
	Variable();
	Variable(Token);
	Variable(const Variable&);
	Variable& operator= (const Variable&);

	String id() const;
	tokenType type();
	Token value() const;
	Variable& valueAt(Token);
	bool setValue(const Variable&);
	bool setValueAt(Token, Variable);
};

class VariableScope
{
	Vector< Vector<Variable> > varstack;

	public:
	VariableScope();
	VariableScope(const VariableScope&);
	
	bool stackVariables();
	bool stackVariables(Vector<Variable>&);
	bool addVariable(Variable&);
	bool popVariables();
	bool popVariables(Vector<Variable>&);
	Variable resolve(String);
	__SIZETYPE exists(String);
	__SIZETYPE depth() const;
};

class Function
{
	String _id;
	Vector<String> params;
	RPN statements;

	public:
	Function();
	Function(String);
	Function(String, Vector<String>, RPN);
	Function(const Function&);

	__SIZETYPE paramsSize() const;
	String id() const;
	RPN getStatements() const;

	bool setParams(Vector<String>);
	bool setStatements(RPN);
	bool validate();
	Token evaluate(Vector<Token>, Evaluator&);
};

// parser status.
#define SUCCESS 41
#define PENDING 42
#define STARTED 43
#define FAILED 44

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
	Function getFunction(String);

	// parsing procedures:
	bool parseSource();

	RPN parseBlock(bufferIndex = 0);
	RPN parseStatement(Infix);
	RPN parseDeclaration(Infix, tokenType);

	/* static parsing procedures: */
	// assumes that the tokens have already been read.
	static RPN expressionToRPN(Infix, Vector<Error>&, VariableScope&, Function (*)(String));
	// checks whether the RPN is valid.
	static bool validateRPN(RPN);

};

#endif /* COMPONENT_PARSER_H */
