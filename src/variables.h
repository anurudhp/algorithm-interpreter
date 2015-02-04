#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_VARIABLES_H
#define COMPONENT_PARSER_VARIABLES_H

// Included in "parser.h"

class Variable
{
	String _id;
	Token _value;
	Vector<String> _keys;
	Vector<Variable> _values;
	tokenType _type;
	Object *_object;

	public:
	Variable();
	Variable(String);
	Variable(Token);
	Variable(const Variable&);
	Variable& operator= (const Variable&);

	String id() const;
	tokenType type();
	Token value() const;

	// value access:
	void setType(tokenType);
	bool setValue(const Variable&);
	bool hasValueAt(Token);
	Variable& valueAt(Token);
	bool setValueAt(Token, Variable);
	Function getMethod(String);

	Token printValues(ostream& = cout);

	friend class Object;
};

Variable nullVariableRef(";");

class VariableScope
{
	Vector< Vector<Variable> > varstack;

	public:
	VariableScope();
	VariableScope& operator= (const VariableScope&);

	bool stackVariables();
	bool stackVariables(Vector<Variable>&);
	bool addVariable(Variable&);
	bool popVariables();
	bool popVariables(Vector<Variable>&);

	bool exists(String);
	bool existsAtTop(String);
	Variable& resolve(String);
	Vector<Variable>& getBaseVariables();
	__SIZETYPE depth() const;
};

class Function
{
	String _id;
	Vector<String> parameters;
	Vector<Variable> functionVariables;
	RPN statements;

	Token returnVal;
	bool hasRet;

	public:
	Function();
	Function(String);
	Function(String, Vector<String>, RPN);
	Function(const Function&);

	__SIZETYPE paramsSize() const;
	String id() const;
	RPN getStatements() const;

	// parsing:
	void setParams(Vector<String>);
	void setStatements(RPN);
	void setVariables(Vector<Variable>);
	// evaluating:
	Token evaluate(Vector<Variable>, Evaluator&);
	bool hasReturned() const;
	bool setReturn(Token);
};

class Object
{
	String _id;
	Function constructor;
	Vector<Function> prototypes;
	bool isFundamental;

	public:
	Object();
	Object(String, bool);

	String id();
	Function getConstructor();
	bool hasPrototype(String);
	Function getPrototype(String);
	Variable construct(Vector<Variable>);
};

#endif /* COMPONENT_PARSER_VARIABLES_H */
#endif /* COMPONENT_PARSER_H */
