#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_OPERATION_H
#define COMPONENT_PARSER_OPERATION_H

namespace Operations {
	const Token nullval("null", KEYWORD, CONSTANT);
	
	Token add(Token, Token);
	Token subtract(Token, Token);
	Token multiply(Token, Token);
	Token divide(Token, Token);
	Token modulo(Token, Token);
	Token mathOperator(String, Token, Token);
	
	Token compare(String, Token, Token);
	Token logical(String, Token, Token);
};

Token Operations::mathOperator(String op, Token a, Token b) {
	if (op == "+") return add(a, b);
	if (op == "-") return subtract(a, b);
	if (op == "*") return multiply(a, b);
	if (op == "/") return divide(a, b);
	if (op == "%") return modulo(a, b);
	
	return nullval;
}
#endif /* COMPONENT_PARSER_OPERATION_H */
#endif /* COMPONENT_PARSER_H */
