#ifndef COMPONENT_EVALUATOR_H
#define COMPONENT_EVALUATOR_H

class Evaluator
{
	private:
	Vector <Error> errors;
	VariableScope variables;
	Vector<Variable*> cache;
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

	Variable& getVariable(String, VariableScope&, bool = false);
	Vector<Variable>& getGlobals();
	
	// procedures
	bool runProgram();
	Token evaluateRPN(RPN, VariableScope&, Vector<Token>* = NULL);
};

#endif /* COMPONENT_EVALUATOR_H */
