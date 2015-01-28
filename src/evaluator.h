#ifndef COMPONENT_EVALUATOR_H
#define COMPONENT_EVALUATOR_H

class Evaluator
{
	private:
	Vector<Variable> cache;
	Parser *parser;
	
	public:
	Evaluator(Parser*);
	
	bool runProgram();
	Token evaluateRPN(RPN, VariableScope);
	
	Variable getCached(String);
	String cacheVariable(Variable);
};

#endif /* COMPONENT_EVALUATOR_H */
