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
	int comparePriority(Token, Token);
};

Token Operations::mathOperator(String op, Token a, Token b) {
	if (op == "+") return add(a, b);
	if (op == "-") return subtract(a, b);
	if (op == "*") return multiply(a, b);
	if (op == "/") return divide(a, b);
	if (op == "%") return modulo(a, b);
	return nullval;
}

Token Operations::add(Token t1, Token t2) {
    if (t1.subtype() != t2.subtype()) return nullToken;
    if (t1.subtype() == NUMBER) {
        double a1 = t1.value().toNumber(),
		       a2 = t2.value().toNumber();
        return Lexer::toToken(numberToString(a1 + a2));
    }
    if (t1.subtype() == STRING) {
        String s1 = Lexer::tokenToString(t1),
		       s2 = Lexer::tokenToString(t2);
        return Lexer::toToken(s1 + s2);
    }
}

Token Operations::subtract(Token t1, Token t2) {
    if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullToken;
    double a1 = t1.value().toNumber(),
	       a2 = t2.value().toNumber();
    return Lexer::toToken(numberToString(a1 - a2));
}

Token Operations::multiply(Token t1, Token t2) {
    if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullToken;
    double a1 = t1.value().toNumber(),
	       a2 = t2.value().toNumber();
    return Lexer::toToken(numberToString(a1 * a2));
}

Token Operations::divide(Token t1, Token t2) {
    if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullToken;
    double a1 = t1.value().toNumber(),
	       a2 = t2.value().toNumber();
    if (a2 == 0.0) return nullToken;
    return Lexer::toToken(numberToString(a1 / a2));
}

Token Operations::modulo(Token t1, Token t2) {
    if (!(t1.value().isInteger() && t2.value().isInteger())) return nullToken;
    long a1 = t1.value().toInteger(),
	     a2 = t2.value().toInteger();
    if (a2 == 0) return nullToken;
    return Lexer::toToken(integerToString(a1 % a2));
}

// Operator Priorities:
int Operations::priority(String op) {
	if (op == ".") return 6;
	if (op == "++" || op == "--") return 5;
	if (op == "*" || op == "/" || op == "%")  return 4;
	if (op == "+" || op == "-") return 3;
	if (op.substr(0,2) == "==" || op.substr(0, 2) == "!=" || op[0] == '<' || op[0] == '>') return 2;
	if (op == "!" || op == "&&" || op == "||" || op == "=" || op[1] == '=') return 1;
	return 0;
}

int Operations::comparePriority(Token a, Token b) {
	return priority(a.value()) - priority(b.value());
}

#endif /* COMPONENT_PARSER_OPERATION_H */
#endif /* COMPONENT_PARSER_H */
