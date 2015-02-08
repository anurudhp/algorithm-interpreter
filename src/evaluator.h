#ifndef COMPONENT_EVALUATOR_H
#define COMPONENT_EVALUATOR_H

/*******************************
* The evaluator class
* Evaluates the RPN generated by the parser.
* Uses a function stack to handle recursive calls.
*   Exits on the first time it encounters a runtime error.
********************************/
class Evaluator
{
	private:
	Vector <Error> errors;
	VariableScope variables;
	Vector<Variable*> cache;
	Stack<Function> functionStack;
	Parser *parser;
	bool failed;
	
	public:
	Evaluator(Parser*, String = "");
	
	// interface:
	bool sendError(Error);
	bool sendError(String, String, bufferIndex = -1, int = ERROR_ERROR);
	bool showErrors(ostream&, bool = false);

	// variable caches
	Variable& getCachedVariable(String);
	String cacheVariable(Variable);
	String cacheVariable();
	String cacheVariableRef(Variable*);
	// variable access.
	Variable& getVariable(String, VariableScope&, bool = false);
	Token getVariableValue(Token, VariableScope&, bool = false);
	Vector<Variable>& getGlobals();
	
	// procedures
	bool runProgram();
	Token evaluateRPN(RPN, VariableScope&);
};

#endif /* COMPONENT_EVALUATOR_H */
