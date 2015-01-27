#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_IO_H
#define COMPONENT_PARSER_IO_H

namespace IOhandler {
	Token write(Token);

	Token read(tokenType);
	Token get();
	Token readLine();
};

Token IOhandler::write(Token t1) {
    if(t1.subtype() == NUMBER || t1.subtype() == BOOLEAN ) {
        for(int i = 0;i < t1.value().length();i++) cout<<t1.value()[i];
        return Lexer::toToken("true");
    }
    if(t1.subtype() == STRING) {
        for(int i = 1;i < t1.value().length() - 1;i++) cout<<t1.value()[i];
        return Lexer::toToken("true");
    }
    return Lexer::toToken("false");
}

Token IOhandler::read(tokenType t1) {
    if(t1 == NUMBER) {
        double number;
        cin>>number;
        String s1 = numberToString(number);
        return Lexer::toToken(s1);
    }
    if(t1 == STRING) {
        String s1,s2("\"");
        char A[MAX_STRING_LENGTH];
        cin.getline(A,MAX_STRING_LENGTH);
        s1 = A;
        s1 = s2 + s1 + s2;
        return Lexer::toToken(s1);
    }
    if(t1 == BOOLEAN) {
        String s1;
        char A[MAX_STRING_LENGTH];
        cin.getline(A,MAX_STRING_LENGTH);
        s1 = A;
        if(s1 != "true" && s1 != "false") return nullToken;
        return Lexer::toToken(s1);
    }
}

Token IOhandler::get() {
    char c;
    cin>>c;
    String s1(c);
    Token t1 = Lexer::toToken(s1);
    return t1;
}

Token IOhandler::readLine() {
    String s1;
    char c;
    c = cin.get();
    while(c != '\n'){
        s1 += c;
        c = cin.get();
    }
    return Lexer::toToken(s1);
}


#endif /* COMPONENT_PARSER_IO_H */
#endif /* COMPONENT_PARSER_H */
