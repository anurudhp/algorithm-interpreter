#ifndef SRC_EVALUATOR_H_
#define SRC_EVALUATOR_H_

/*******************************
* The evaluator class
* Evaluates the RPN generated by the parser.
* Uses a function stack to handle recursive calls.
*   Exits on the first time it encounters a runtime error.
********************************/
class Evaluator {
 private:
  Vector <Error> errors;
  VariableScope variables;
  Vector<Variable> globals;
  Vector<Variable*> cache;
  Stack<Function> functionStack;
  Parser *parser;
  bool failed;

 public:
  explicit Evaluator(Parser *pr, String args = "");

  // interface:
  bool addError(Error e);
  bool sendError(String cd, String msg = "", bufferIndex ln = -1);
  bool showErrors(ostream &out, bool clearAfterDisplay = false);

  // variable caches
  Variable& getCachedVariable(String id);
  String cacheVariable(Variable v);
  String cacheVariable();
  String cacheVariableRef(Variable *ref);
  // variable access.
  Variable& getVariable(String id, VariableScope &scope, bool searchCache = false);
  Token getVariableValue(Token var, VariableScope &scope, bool searchCache = false);
  Vector<Variable>& getGlobals();

  // procedures
  bool runProgram();
  Token evaluateRPN(RPN source, VariableScope &scope);
};

#endif  // SRC_EVALUATOR_H_
