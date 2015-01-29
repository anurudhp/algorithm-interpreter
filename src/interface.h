#ifndef COMPONENT_INTERFACE_H
#define COMPONENT_INTERFACE_H

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

#include "lexer.h"
#include "parser.h"
#include "evaluator.h"

#include "lexer.cpp"
#include "io.cpp"
#include "operations.cpp"
#include "parser.cpp"
#include "evaluator.cpp"

#endif /* COMPONENT_INTERFACE_H */
