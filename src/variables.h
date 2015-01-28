#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_VARIABLES_H
#define COMPONENT_PARSER_VARIABLES_H

// Included in "parser.h"

class Variable
{
	String _id;
	Token _value;
	Vector<String> _keys;
	Vector<Variable*> _values;
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
	bool hasValueAt(String);
	Variable& valueAt(Token);
	bool setValueAt(Token, Variable);
	
	// member function access:
	tokenType hasMember(Token);
	Function getMemberFunction(String);
	Variable getDataMember(String);
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

class Object
{
	Function constructor;
	Vector<Function> prototypes;
	
	public:
	Function getConstructor();
	bool hasPrototype(String);
	Function getPrototype(String);
	Variable construct(Vector<Variable>);
};

#endif /* COMPONENT_PARSER_VARIABLES_H */
#endif /* COMPONENT_PARSER_H */
