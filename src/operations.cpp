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

    bool isInteger(Token);
	double toNumber(Token);
	String toString(Token);
	long toInteger(Token);

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

bool Operations::isInteger(Token t) {
    if(t.value().indexOf(".") == -1) return true;
    return false;
}

double Operations::toNumber(Token t) {
    if(t.subtype() != NUMBER) return 0.0;
    double val = 0.0,beginning = 0;
    if(t.value()[0] == '-') beginning = 1;
    if(isInteger(t)){
        for(int i = t.value().length()-1, j = 1;i >= beginning;i--, j *= 10) {
            val += double((t.value()[i] - '0') * j);
        }
        if(beginning == 1) val *= -1.0;
        return val;
    }
    else{
        long long pos=t.value().indexOf(".");
        for(int i = pos-1, j = 1;i >= beginning;i--, j *= 10) {
            val += double((t.value()[i] - '0') * j);
        }
        double j = 0.1;
        for(int i = pos+1;i < t.value().length();i++, j /= 10.0){
            val += double(t.value()[i]-'0') * j;
        }
        if(beginning == 1) val *= -1.0;
        return val;
    }
}

long Operations::toInteger(Token t) {
    if(!isInteger(t)) return 0;
    return int(toNumber(t));
}

String Operations::toString(Token t) {
    if(t.subtype() != STRING) return "";
    return t.value().substr(1, t.value().length() - 2);
}

#endif /* COMPONENT_PARSER_OPERATION_H */
#endif /* COMPONENT_PARSER_H */
