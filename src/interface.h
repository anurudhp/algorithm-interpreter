#ifndef SRC_INTERFACE_H_
#define SRC_INTERFACE_H_

#include "../lib/codelib.h"

class Token;
class Error;
class Lexer;
class Parser;
class Evaluator;

class Variable;
class VariableScope;
class Function;
class Object;

#include "./lexer.h"
#include "./parser.h"
#include "./evaluator.h"

#include "./lexer.cpp"
#include "./operations.cpp"
#include "./inbuiltfunc.cpp"
#include "./parser.cpp"
#include "./evaluator.cpp"

void showConsole(bool firsttime = false);
bool loadData();
void logRPN(RPN r);
void parseCommand(String message);
bool runProgram(Vector<String> args);

#endif  // SRC_INTERFACE_H_
