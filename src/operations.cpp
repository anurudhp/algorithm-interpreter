#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_OPERATION_H
#define COMPONENT_PARSER_OPERATION_H

struct Operations {
	static Token typecastToken(Token, tokenType = STRING);

	static Token add(Token, Token);
	static Token subtract(Token, Token);
	static Token multiply(Token, Token);
	static Token divide(Token, Token);
	static Token modulo(Token, Token);
	static Token mathOperator(String, Token, Token);

	static Token compare(String, Token, Token);
	static Token logical(String, Token, Token);
	static Token unaryOperator(String, Token);

	static int priority(String);
	static int comparePriority(Token, Token);
};

Token Operations::typecastToken(Token tok, tokenType type) {
	if (tok.subtype() == type) return tok;

	if (type == NUMBER) {
		if (tok.subtype() == BOOLEAN) {
			if (tok.value() == "true") return Lexer::toToken("1");
			if (tok.value() == "false") return Lexer::toToken("0");
		}
		if (tok.subtype() == STRING) {
			String val = Lexer::tokenToString(tok);
			if (val.isNumber()) return Lexer::toToken(val);
		}
	}
	else if (type == STRING) {
		String val = Lexer::stringToLiteral(tok.value());
		return Lexer::toToken(val);
	}
	else if (type == BOOLEAN) {
		if (tok.subtype() == NUMBER) {
			if (tok.value().toNumber() != 0.0) return trueToken;
			return falseToken;
		}
		if (tok.subtype() == STRING) {
			if (Lexer::tokenToString(tok).length() > 0) return trueToken;
			return falseToken;
		}
	}
	return nullvalToken;
}

Token Operations::mathOperator(String op, Token a, Token b) {
	if (op == "+") return add(a, b);
	if (op == "-") return subtract(a, b);
	if (op == "*") return multiply(a, b);
	if (op == "/") return divide(a, b);
	if (op == "%") return modulo(a, b);
	return nullvalToken;
}

Token Operations::add(Token t1, Token t2) {
	if (t1.subtype() == NUMBER) {
		t2 = typecastToken(t2, NUMBER);
		double a1 = t1.value().toNumber(),
			   a2 = t2.value().toNumber();
		return Lexer::toToken(numberToString(a1 + a2));
	}
	if (t1.subtype() == STRING) {
		t2 = typecastToken(t2, STRING);
		String s1 = Lexer::tokenToString(t1),
			   s2 = Lexer::tokenToString(t2);
		return Lexer::toToken(Lexer::stringToLiteral(s1 + s2));
	}
}

Token Operations::subtract(Token t1, Token t2) {
	if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalToken;
	double a1 = t1.value().toNumber(),
		   a2 = t2.value().toNumber();
	return Lexer::toToken(numberToString(a1 - a2));
}

Token Operations::multiply(Token t1, Token t2) {
	if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalToken;
	double a1 = t1.value().toNumber(),
		   a2 = t2.value().toNumber();
	return Lexer::toToken(numberToString(a1 * a2));
}

Token Operations::divide(Token t1, Token t2) {
	if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalToken;
	double a1 = t1.value().toNumber(),
		   a2 = t2.value().toNumber();
	if (a2 == 0.0) return nullvalToken;
	return Lexer::toToken(numberToString(a1 / a2));
}

Token Operations::modulo(Token t1, Token t2) {
	if (!(t1.value().isInteger() && t2.value().isInteger())) return nullvalToken;
	long a1 = t1.value().toInteger(),
		 a2 = t2.value().toInteger();
	if (a2 == 0) return nullvalToken;
	return Lexer::toToken(integerToString(a1 % a2));
}

// relational and logical operators:
Token Operations::logical(String op, Token t1, Token t2) {
	t1 = typecastToken(t1, BOOLEAN);
	t2 = typecastToken(t2, BOOLEAN);

	if (op == "&&") {
		if(t1.value() == "true" && t2.value() == "true") return trueToken;
		return falseToken;
	}
	if (op == "||") {
		if(t1.value() == "true" || t2.value() == "true") return trueToken;
		return falseToken;
	}
	return nullvalToken;
}

// All unary operators
Token Operations::unaryOperator(String op, Token tok) {
	if (op == "!") {
		typecastToken(tok, BOOLEAN);
		if (tok.value() == "true") return falseToken;
		return falseToken;
	}
	if (op == "-") {
		tok = typecastToken(tok, NUMBER);
		double num = -(tok.value().toNumber());
		return Lexer::toToken(numberToString(num));
	}

	return nullvalToken;
}

// Operator Priorities:
int Operations::priority(String op) {
	if (op == ".") return 20;
	if (op == "++" || op == "--") return 15;
	if (op == "*" || op == "/" || op == "%")  return 10;
	if (op == "+" || op == "-") return 8;
	if (op == "<" || op == "<=" || op == ">" || op == ">=") return 6;
	if (op == "==" || op == "===" || op == "!=" || op == "!==") return 5;
	if (op == "!" || op == "&&" || op == "||") return 4;
	if (op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "%=" || op == "=") return 2;
	return 0;
}

int Operations::comparePriority(Token a, Token b) {
	int pa = priority(a.value()), pb = priority(b.value());
	return (pa - pb);
}

Token Operations::compare(String S, Token t1, Token t2) {
	if(t1.subtype() != NUMBER && t1.subtype() != BOOLEAN) return nullvalToken;
	if(t2.subtype() != NUMBER && t2.subtype() != BOOLEAN) return nullvalToken;
	if(S == "<") {
		if(t1.subtype() != t2.subtype()) return nullvalToken;
		if(Operations::typecastToken(t1,NUMBER).value().toNumber() < Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
		return falseToken;
	}
	if(S == ">"){
		if(t1.subtype() != t2.subtype()) return nullvalToken;
		if(Operations::typecastToken(t1,NUMBER).value().toNumber() > Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
		return falseToken;
	}
	if(S == "<="){
		if(t1.subtype() != t2.subtype()) return nullvalToken;
		if(Operations::typecastToken(t1,NUMBER).value().toNumber() <= Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
		return falseToken;
	}
	if(S == ">="){
		if(t1.subtype() != t2.subtype()) return nullvalToken;
		if(Operations::typecastToken(t1,NUMBER).value().toNumber() >= Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
		return falseToken;
	}
	if(S == "=="){
		if(t1.subtype() == t2.subtype()) {
			if(Operations::typecastToken(t1,NUMBER).value().toNumber() == Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
			return falseToken;
		}
		if(Operations::typecastToken(t1,BOOLEAN).value() == Operations::typecastToken(t2,BOOLEAN).value()) return trueToken;
		return falseToken;
	}
	if(S == "!="){
		if(t1.subtype() == t2.subtype()) {
			if(Operations::typecastToken(t1,NUMBER).value().toNumber() != Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
			return falseToken;
		}
		if(Operations::typecastToken(t1,BOOLEAN).value() != Operations::typecastToken(t2,BOOLEAN).value()) return trueToken;
		return falseToken;
	}
	if(S == "==="){
		if(t1.subtype() != t2.subtype()) falseToken;
		if(Operations::typecastToken(t1,NUMBER).value().toNumber() == Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
		return falseToken;
	}
	if(S == "!=="){
		if(t1.subtype() != t2.subtype()) return falseToken;
		if(Operations::typecastToken(t1,NUMBER).value().toNumber() != Operations::typecastToken(t2,NUMBER).value().toNumber()) return trueToken;
		return falseToken;
	}
	return nullvalToken;
}

#endif /* COMPONENT_PARSER_OPERATION_H */
#endif /* COMPONENT_PARSER_H */
