#ifndef SRC_VARIABLES_H_
#define SRC_VARIABLES_H_
#ifdef SRC_PARSER_H_

// Included in "parser.h"

/*****************************
* Class Variable: stores data about a variable
******************************/
class Variable {
  String _id;
  Token _value;
  Vector<String> _keys;
  Vector<Variable> _values;
  tokenType _type;
  Object *_object;

 public:
  Variable();
  explicit Variable(String id);
  explicit Variable(Token val);
  Variable(const Variable &v);
  Variable& operator= (const Variable &v);

  String id() const;  // the identifier/name of the variable
  tokenType type();
  Token value() const;  // token value.
  __SIZETYPE length() const;

  // value access:
  void setType(tokenType ty);
  bool setValue(const Variable &v);
  bool hasValueAt(Token key);
  Variable& valueAt(Token key);
  bool setValueAt(Token key, Variable value);

  // methods for arrays:
  bool pushValue(Variable v, bool isFront = false);
  bool popValue(Variable &v, bool isFront = false);
  // methods for objects:
  bool addPair(Token key, Variable val);
  bool deletePair(Token key, Variable &ref);
  String getKey(__SIZETYPE index);
  Function getMethod(String funcId);

  Token printValues(ostream& out = cout);

  friend class Object;
};

Variable nullVariableRef(";");

/****************************
* class VariableScope:
* Stores data about a scope.
* used to resolve variables, from top to bottom.
*****************************/
class VariableScope {
  Vector< Vector<Variable> > varstack;

 public:
  VariableScope();
  VariableScope& operator= (const VariableScope &sc);

  bool stackVariables();
  bool stackVariables(Vector<Variable> &vars);
  bool addVariable(Variable &var);
  bool popVariables();
  bool popVariables(Vector<Variable> &vars);

  bool exists(String id);
  bool existsAtTop(String id);
  Variable& resolve(String id);
  Vector<Variable>& getBaseVariables();
  __SIZETYPE depth() const;
};

/******************************
* class Function:
* Stores info about a function:
*    parameters, statements, and local variables
*******************************/
class Function {
  String _id;
  Vector<String> parameters;
  Vector<Variable> functionVariables;
  RPN statements;

  Token returnVal;
  bool hasRet;

 public:
  Function();
  explicit Function(String id);
  Function(String id, Vector<String> params, RPN st);
  Function(const Function& f);

  __SIZETYPE paramsSize() const;
  String id() const;
  RPN getStatements() const;

  // parsing:
  void setParams(Vector<String> p);
  void setStatements(RPN st);
  void setVariables(Vector<Variable> fv);
  // evaluating:
  Token evaluate(Vector<Variable> args, Evaluator &eval);
  bool hasReturned() const;
  bool setReturn(Token ret);
};

/**********************
* Stores data about a class declaration or object declaration
* Stores all methods.
**********************/
class Object {
  String _id;
  Function constructor;
  Vector<Function> prototypes;
  bool isFundamental;

 public:
  Object();
  Object(String name, bool f);

  String id();
  Function getConstructor();
  bool hasPrototype(String funcid);
  Function getPrototype(String funcid);
  Variable construct(Vector<Variable> initArgs);
};

#endif  // SRC_PARSER_H_
#endif  // SRC_VARIABLES_H_
