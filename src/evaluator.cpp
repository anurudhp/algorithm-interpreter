#ifdef COMPONENT_EVALUATOR_H

Evaluator::Evaluator(Parser *pr) {
	this->parser = pr;
}

Token Evaluator::evaluateRPN(RPN source, VariableScope sc) {
	Token tmp;
	Stack<Token> valuestack;
	Queue<Token> statementValues;
	while (source.pop(tmp)) {
		if (tmp.type() == LITERAL) valuestack.push(tmp);
		if (tmp.value() == ";") {
			Token t;
			valuestack.pop(t);
			statementValues.push(t);
			continue;
		}
		
		if (tmp.type() == OPERATOR) {
			if (tmp.subtype() == UNARYOP) {
				Token a;
				valuestack.pop(a);
				valuestack.push(Operations::unaryOperator(tmp.value(), a));
			}
			else if (tmp.subtype() == BINARYOP) {
				// check if it is assignment operator:
				String oper = tmp.value();
				
				bool isAssign = false;
				Vector<String> assignmentOp(strsplit("= += -= *= /= %=", ' '));
				if (assignmentOp.indexOf(tmp.value())) {
					oper = tmp.value().substr(0, 1);
					isAssign = true;
				}
				
				Token a, b;
				valuestack.pop(b);
				valuestack.pop(a);
				valuestack.push(Operations::binaryOperator(oper, a, b));
			}
		}
	}
	while (valuestack.pop(tmp)) statementValues.push(tmp);
	
	
	return tmp;
}
#endif // COMPONENT_EVALUATOR_H