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
	Object *_object;

	public:
	Variable();
	Variable(Token);
	Variable(const Variable&);
	Variable& operator= (const Variable&);

	String id() const;
	tokenType type();
	Token value() const;

	// value access:
	bool setValue(const Variable&);
	bool hasValueAt(Token);
	Variable& valueAt(Token);
	bool setValueAt(Token, Variable);
	Function getMethod(String);
	
	friend class Object;
};

Variable nullVariableRef;

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

class Object
{
	String _id;
	Function constructor;
	Vector<Function> prototypes;
	
	public:
	String id();
	Function getConstructor();
	bool hasPrototype(String);
	Function getPrototype(String);
	Variable construct(Vector<Variable>);
};

#endif /* COMPONENT_PARSER_VARIABLES_H */
#endif /* COMPONENT_PARSER_H */
