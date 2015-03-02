#ifdef SRC_PARSER_H_
#ifndef SRC_PARSER_OPERATIONS_CPP_
#define SRC_PARSER_OPERATIONS_CPP_

/********************************
* Static processes for executing all operations
* Supports: Math operators (+ - * / %),
*           Logical operators (&& ||)
*           Relational operators (== != > >= < <=)
* Has function to check and compare priority of different operators.
********************************/
struct Operations {
  static Token typecastToken(Token, tokenType = STRING);

  static Token add(Token, Token);
  static Token subtract(Token, Token);
  static Token multiply(Token, Token);
  static Token divide(Token, Token);
  static Token modulo(Token, Token);

  static Token compare(String, Token, Token);
  static Token logical(String, Token, Token);

  static Token unaryOperator(String, Token);
  static Token binaryOperator(String, Token, Token);

  static int priority(String);
  static int comparePriority(Token, Token);
};

Token Operations::typecastToken(Token tok, tokenType type) {
  if (tok.subtype() == type) return tok;

  if (type == NUMBER || type == INTEGER) {
    if (tok.subtype() == NUMBER) {
      // type is INTEGER
      long val = long(tok.value().toNumber());
      return Lexer::toToken(integerToString(val));
    }
    if (tok.subtype() == BOOLEAN) {
      if (tok.value() == "true") return Lexer::toToken("1");
      if (tok.value() == "false") return Lexer::toToken("0");
    }
    if (tok.subtype() == STRING) {
      String val = Lexer::tokenToString(tok);
      if (type == INTEGER) {
        if (val.isInteger()) return Lexer::toToken(val);
      } else {
        if (val.isNumber()) return Lexer::toToken(val);
      }
    }
    if (tok.subtype() == CONSTANT) {
      if (tok.value() == "infinity" || tok.value() == "minusinfinity") return tok;
    }
  } else if (type == STRING) {
    String val = tok.value();
    if (val.isInteger()) val = integerToString(val.toInteger());
    return Lexer::toToken(Lexer::stringToLiteral(val));
  } else if (type == BOOLEAN) {
    if (tok.subtype() == NUMBER) {
      if (tok.value().toNumber() != 0.0) return trueToken;
      return falseToken;
    }
    if (tok.subtype() == STRING) {
      if (Lexer::tokenToString(tok).length() > 0) return trueToken;
      return falseToken;
    }
    if (tok.subtype() == CONSTANT) {
      if (tok.value() == "null") return falseToken;
      if (tok.value() == "infinity" || tok.value() == "minusinfinity") return trueToken;
    }
  }
  return nullvalToken;
}

Token Operations::binaryOperator(String op, Token a, Token b) {
  if (op == "+") return add(a, b);
  if (op == "-") return subtract(a, b);
  if (op == "*") return multiply(a, b);
  if (op == "/") return divide(a, b);
  if (op == "%") return modulo(a, b);

  if (op == "&&" || op == "||") return logical(op, a, b);
  if (op[0] == '>' || op[0] == '<' || op.substr(0, 2) == "==" || op.substr(0, 2) == "!=") return compare(op, a, b);

  return nullvalToken;
}

Token Operations::add(Token t1, Token t2) {
  if (t1.subtype() == STRING || t2.subtype() == STRING) {
    t1 = typecastToken(t1, STRING);
    t2 = typecastToken(t2, STRING);
    String s1 = Lexer::tokenToString(t1),
         s2 = Lexer::tokenToString(t2);
    return  Lexer::toToken(Lexer::stringToLiteral(s1 + s2));
  }
  if (t1.subtype() == NUMBER || t2.subtype() == NUMBER || t1.subtype() == BOOLEAN || t2.subtype() == BOOLEAN) {
    if (t1.value() == "infinity" || t2.value() == "infinity") return Lexer::toToken("infinity");
    if (t1.value() == "minusinfinity" || t2.value() == "minusinfinity") return Lexer::toToken("minusinfinity");

    t1 = typecastToken(t1, NUMBER);
    t2 = typecastToken(t2, NUMBER);
    double a1 = t1.value().toNumber(),
         a2 = t2.value().toNumber();
    return Lexer::toToken(numberToString(a1 + a2));
  }
  if (t1.value() == "infinity" || t2.value() == "infinity") return Lexer::toToken("infinity");
  if (t1.value() == "minusinfinity" || t2.value() == "minusinfinity") return Lexer::toToken("minusinfinity");
  return nullvalToken;
}

Token Operations::subtract(Token t1, Token t2) {
  if (t1.value() == "infinity" || t2.value() == "minusinfinity") return Lexer::toToken("infinity");
  if (t2.value() == "infinity" || t1.value() == "minusinfinity") return Lexer::toToken("minusinfinity");

  if (t1.subtype() != NUMBER && t2.subtype() != NUMBER) return nullvalToken;
  double a1 = t1.value().toNumber(),
       a2 = t2.value().toNumber();
  Token subtr = Lexer::toToken(numberToString(a1 - a2));
  return subtr;
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
    if (t1.value() == "true" && t2.value() == "true") return trueToken;
    return falseToken;
  }
  if (op == "||") {
    if (t1.value() == "true" || t2.value() == "true") return trueToken;
    return falseToken;
  }
  return nullvalToken;
}

// All unary operators
Token Operations::unaryOperator(String op, Token tok) {
  if (op == "!") {
    typecastToken(tok, BOOLEAN);
    if (tok.value() == "true") {return falseToken;}
    return trueToken;
  }
  if (op == "-") {
    if (tok.value() == "infinity") return Lexer::toToken("minusinfinity");
    if (tok.value() == "minusinfinity") return Lexer::toToken("infinity");

    tok = typecastToken(tok, NUMBER);
    double num = -(tok.value().toNumber());
    return Lexer::toToken(numberToString(num));
  }

  return nullvalToken;
}

// Operator Priorities:
int Operations::priority(String op) {
  if (op == ".") return 20;
  if (op == "++" || op == "--") return 18;
  if (op == "typeof") return 15;
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
  // exception: unary `-` vs `typeof`
  if (a.value() == "-" && a.subtype() == UNARYOP && b.value() == "typeof") return 1;
  if (b.value() == "-" && b.subtype() == UNARYOP && a.value() == "typeof") return 0;
  // exception: `=` : right associativity
  if (pa == 2 && pb == 2) return -1;
  return (pa - pb);
}

Token Operations::compare(String op, Token t1, Token t2) {
  // == === != !== : for all types.
  if (t1.type() != LITERAL || t2.type() != LITERAL) return nullvalToken;

  if (op == "==") {
    if (t1.subtype() == BOOLEAN) t1 = typecastToken(t1, NUMBER);
    if (t2.subtype() == BOOLEAN) t2 = typecastToken(t2, NUMBER);

    if (t1.subtype() == CONSTANT || t2.subtype() == CONSTANT) {
      if (t1.subtype() == CONSTANT && t2.subtype() == CONSTANT) {
        if (t1.value() == t2.value()) return trueToken;
        return falseToken;
      }
      if (t2.subtype() == CONSTANT) return compare("==", t2, t1);
      if (t1.value() == "infinity") return falseToken;
      return nullvalToken;
    }
    if (t1.subtype() != t2.subtype()) t2 = typecastToken(t2, t1.subtype());

    if (t1.subtype() == NUMBER) {
      if (t1.value().toNumber() == t2.value().toNumber()) return trueToken;
    } else if (t1.subtype() == STRING) {
      if (t1.value() == t2.value()) {
        return trueToken;
      }
    }
    return falseToken;
  }
  if (op == "!=") {
    return unaryOperator("!", compare("==", t1, t2));
  }
  if (op == "===") {
    if (t1.subtype() != t2.subtype()) return falseToken;
    return compare("==", t1, t2);
  }
  if (op == "!==") {
    return unaryOperator("!", compare("===", t1, t2));
  }

  // > >= < <= : only for numbers (boolean converted to equivalent numbers)
  t1 = typecastToken(t1, NUMBER);
  t2 = typecastToken(t2, NUMBER);
  if (t1.value() == "null" || t2.value() == "null") return falseToken;
  if (op == "<") {
    if (t1.value() == "infinity" || t2.value() == "minusinfinity") return falseToken;
    if (t2.value() == "infinity" || t1.value() == "minusinfinity") return trueToken;

    double v1 = t1.value().toNumber(),
           v2 = t2.value().toNumber();
    if (v1 < v2) return trueToken;
    return falseToken;
  }
  if (op == ">") {
    return unaryOperator("!", logical("||", compare("==", t1, t2), compare("<", t1, t2)));
  }
  if (op == "<=") {
        if (t1.value().toNumber() == t2.value().toNumber()) return trueToken;
    return compare("<", t1, t2);
  }
  if (op == ">=") {
      if (t1.value().toNumber() == t2.value().toNumber()) return trueToken;
    return compare(">", t1, t2);
  }

  return nullvalToken;
}

#endif  // SRC_PARSER_OPERATIONS_CPP_
#endif  // SRC_PARSER_H_
