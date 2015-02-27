#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_IO_H
#define COMPONENT_PARSER_IO_H

/*******************
* Executes all global inbuilt functions
******************/
struct InbuiltFunctions {
  // I/O functions:
  static char INFCHAR;
  static Token write(Token, ostream& = cout);
  static Token read(tokenType, istream& = cin);
  static Token get(istream& = cin);
  static Token readLine(istream& = cin);
};

char InbuiltFunctions::INFCHAR = 236;

// write a token's value to the out stream.
Token InbuiltFunctions::write(Token t, ostream& out) {
  if (t.type() != LITERAL) return falseToken;
    if (t.subtype() == BOOLEAN || t.subtype() == CONSTANT) {
    if (t.value() == "infinity") cout << INFCHAR;
    else if (t.value() == "minusinfinity") cout << '-' << INFCHAR;
        else out << t.value();
        return trueToken;
    }
  if (t.subtype() == NUMBER) {
    String val(t.value().trim());
    if (val.isInteger()) out << val.toInteger();
    else out << val.toNumber();
        return trueToken;
  }
    if (t.subtype() == STRING) {
        String val = Lexer::tokenToString(t);
    for (__SIZETYPE i = 0; i < val.length(); i++) {
      if (val.charAt(i) == '\\') {
        // escape special chars
        char c = val.charAt(i + 1);
        switch (val.charAt(i + 1)) {
        case 'n': c = '\n'; break;
        case 't': c = '\t'; break;
        case 'a': c = '\a'; break;
        case 'r': c = '\r'; break;
        }
        out << c;
        i++;
      }
      else out << val.charAt(i);
    }
        return trueToken;
    }
    return falseToken;
}

// read a value of type `t1`
Token InbuiltFunctions::read(tokenType t1, istream& in) {
  String s;
    if(t1 == NUMBER) {
        double number;
        in >> number; in.get();
        return Lexer::toToken(numberToString(number));
    }
    if(t1 == STRING) {
        in >> s; in.get();
        return Lexer::toToken(Lexer::stringToLiteral(s));
    }
  return nullvalToken;
}

// gets a single char.
Token InbuiltFunctions::get(istream& in) {
    char c = in.get();
    String val = Lexer::stringToLiteral(String(c));
    return  Lexer::toToken(val);
}
// gets a single line.
Token InbuiltFunctions::readLine(istream& in) {
    String s;
  s.get(in, '\n');
    return Lexer::toToken(Lexer::stringToLiteral(s));
}

#endif /* COMPONENT_PARSER_IO_H */
#endif /* COMPONENT_PARSER_H */
