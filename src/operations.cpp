#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_OPERATION_H
#define COMPONENT_PARSER_OPERATION_H

namespace Operations {
	const Token nullvalTok("null", KEYWORD, CONSTANT),
				trueTok("true", LITERAL, BOOLEAN),
				falseTok("false", LITERAL, BOOLEAN);

	Token typecastToken(Token, tokenType = STRING);

	Token add(Token, Token);
	Token subtract(Token, Token);
	Token multiply(Token, Token);
	Token divide(Token, Token);
	Token modulo(Token, Token);
	Token mathOperator(String, Token, Token);

	Token compare(String, Token, Token);
	Token logical(String, Token, Token);
	Token unaryOperator(String, Token);

	int priority(String);
	int comparePriority(Token, Token);
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
		String val = "\"";
		val += tok.value() + "\"";
		return Lexer::toToken(val);
	}
	else if (type == BOOLEAN) {
		if (tok.subtype() == NUMBER) {
			if (tok.value().toNumber() != 0.0) return trueTok;
			return falseTok;
		}
		if (tok.subtype() == STRING) {
			if (Lexer::tokenToString(tok).length() > 0) return trueTok;
			return falseTok;
		}
	}
	return nullvalTok;
}

Token Operations::mathOperator(String op, Token a, Token b) {
	if (op == "+") return add(a, b);
	if (op == "-") return subtract(a, b);
	if (op == "*") return multiply(a, b);
	if (op == "/") return divide(a, b);
	if (op == "%") return modulo(a, b);
	return nullvalTok;
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
		return Lexer::toToken(s1 + s2);
	}
}

Token Operations::subtract(Token t1, Token t2) {
	if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalTok;
	double a1 = t1.value().toNumber(),
		   a2 = t2.value().toNumber();
	return Lexer::toToken(numberToString(a1 - a2));
}

Token Operations::multiply(Token t1, Token t2) {
	if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalTok;
	double a1 = t1.value().toNumber(),
		   a2 = t2.value().toNumber();
	return Lexer::toToken(numberToString(a1 * a2));
}

Token Operations::divide(Token t1, Token t2) {
	if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalTok;
	double a1 = t1.value().toNumber(),
		   a2 = t2.value().toNumber();
	if (a2 == 0.0) return nullvalTok;
	return Lexer::toToken(numberToString(a1 / a2));
}

Token Operations::modulo(Token t1, Token t2) {
	if (!(t1.value().isInteger() && t2.value().isInteger())) return nullvalTok;
	long a1 = t1.value().toInteger(),
		 a2 = t2.value().toInteger();
	if (a2 == 0) return nullvalTok;
	return Lexer::toToken(integerToString(a1 % a2));
}

// relational and logical operators:
Token Operations::logical(String op, Token t1, Token t2) {
	t1 = typecastToken(t1, BOOLEAN);
	t2 = typecastToken(t2, BOOLEAN);

	if (op == "&&") {
		if(t1.value() == "true" && t2.value() == "true") return trueTok;
		return falseTok;
	}
	if (op == "||") {
		if(t1.value() == "true" || t2.value() == "true") return trueTok;
		return falseTok;
	}
	return nullvalTok;
}

// All unary operators
Token Operations::unaryOperator(String op, Token tok) {
	if (op == "!") {
		typecastToken(tok, BOOLEAN);
		if (tok.value() == "true") return falseTok;
		return falseTok;
	}

	return nullvalTok;
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

#endif /* COMPONENT_PARSER_OPERATION_H */
#endif /* COMPONENT_PARSER_H */
