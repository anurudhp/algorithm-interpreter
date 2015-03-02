#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

typedef Queue<Token> RPN;

#include "./variables.h"

/*****************************
* class HashedData
* Stores statements and local variables, to be saved as blocks in the parser
* Instead of the whole rpn being stored in a single vector,
* storing them in blocks helps in easy access.
*****************************/
class HashedData {
  Vector < RPN > statementSet;
  Vector < Vector<Variable> > variableSet;

 public:
  // stores data about a single `if` block.
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
  bool addStatements(RPN st);
  bool addVariables(Vector<Variable> v);

  void setValues(csIf i);
  void setValues(csFor f);

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
class Parser {
 private:
  VariableScope variables;  // stores globals.
  Vector <Function> functions;  // all global functions.
  Vector <Error> errors;  // parser errors
  Vector <HashedData> hashes;  // if, while, for, foreach blocks.
  RPN output;  // the main code
  int status;  // passed/failed

  // lexer interaction interface:
  Lexer *lexer;

 public:
  explicit Parser(Lexer *ref, String args = "");
  ~Parser();

  // interface: Communication with the parser
  bool addError(Error e);
  bool sendError(String cd, String msg = "", bufferIndex ln = -1, int s = ERROR_ERROR);
  Vector<Error> getErrors();
  bool showErrors(ostream &out, bool clearAfterDisplay = false);
  Function getFunction(String id);

  Token hashify(HashedData &hd);
  HashedData getHashedData(String hash);

  // parsing procedures:
  bool parseSource();

  RPN parseBlock(bufferIndex depth = 0);
  RPN parseDeclaration();

  // assumes that the tokens have already been read.
  RPN expressionToRPN(Infix args);
  // checks whether the RPN is valid.
  bool validateRPN(RPN rpn);

  static Token toArgsToken(__SIZETYPE num);
  static Vector<Error> sortErrors(Vector<Error> errors);
  friend class Evaluator;
  RPN getOutput() { return output; }  // TODO(codelegend): should be removed.
};

#endif  // SRC_PARSER_H_
