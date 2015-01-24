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
	
	int priority(String);
	int comparePriority(String, String);
};

Token Operations::mathOperator(String op, Token a, Token b) {/*
	if (op == "+") return add(a, b);
	if (op == "-") return subtract(a, b);
	if (op == "*") return multiply(a, b);
	if (op == "/") return divide(a, b);
	if (op == "%") return modulo(a, b);
	*/
	return nullval;
}

int Operations::priority(String op) {
	if (op == ".") return 6;
	if (op == "++" || op == "--") return 5;
	if (op == "*" || op == "/" || op == "%")  return 4;
    if (op == "+" || op == "-") return 3;
    if (op.substr(0,2) == "==" || op.substr(0, 2) == "!=" || op[0] == '<' || op[0] == '>') return 2;
    if (op == "!" || op == "&&" || op == "||" || op == "=" || op[1] == '=') return 1;
    return 0;
}

int Operations::comparePriority(String a, String b) {
	return priority(a) - priority(b);
}
#endif /* COMPONENT_PARSER_OPERATION_H */
#endif /* COMPONENT_PARSER_H */
