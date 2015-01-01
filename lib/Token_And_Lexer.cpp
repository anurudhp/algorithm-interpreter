#include "dependencies.h"
#include "codestring.h"
#include "codevector.h"

#define END_OF_FILE -2
#define INVALID -1
#define OPERATOR 0
#define KEYWORD 1
#define IDENTIFIER 3
#define PUNCTUATOR 2
#define CONSTANT 4

Vector< Vector<String> > id;

class token
{

    public:
    __SIZETYPE type , subtype , line_number , char_number ;

    String data ;

    token () ;

};

token::token () {

    type = subtype = line_number = char_number = -1 ;

    data = "" ;
}

class Lexer_Process
{

    int line_number , char_number ;

    char residual ;

    public :

    Lexer_Process () ;

    token get_token ( fstream& ) ;

};

Lexer_Process :: Lexer_Process () {

    line_number = 1 ;
    char_number = 0 ;
    residual = ' ' ;

}

token Lexer_Process :: get_token ( fstream& f ) {

    char c ;
    token t ;
    int i ;

    if ( residual != ' ' ) {
        c = residual ;
        residual = ' ' ;
        goto LABEL2 ;
    }

    LABEL1 :
    if ( f.eof () == true ) {

        t.type = END_OF_FILE ;
        return t ;

    }
    f.get ( c ) ;
    ++ char_number ;
    LABEL2 :
    t.char_number = char_number ;
    t.line_number = line_number ;
    if ( c == ' ' ) goto LABEL1 ;
    if( c == '\n' ) {

        line_number ++ ;
        char_number = 0 ;
        goto LABEL1 ;

    }
    if ( c == '\'' ) {

        t.char_number ++ ;
        t.type = CONSTANT ;
        f.get( c ) ;
        ++ char_number ;
        while ( c != '\'' ) {

            t.data += c ;
            f.get ( c ) ;
            ++ char_number ;
            if ( c == '\n' ) {

                line_number ++ ;
                char_number = 0 ;
                t.type = INVALID ;
                t.data = "" ;
                return t ;

            }

        }

        return t ;
    }

    if ( c == '"' ) {

        t.char_number ++ ;
        t.type = CONSTANT ;
        f.get ( c ) ;
        ++ char_number ;
        while ( c != '"' ) {

            String temp ( c ) ;
            t.data += temp ;
            f.get ( c ) ;
            ++ char_number ;
            if ( c == '\n' ) {

                line_number ++ ;
                char_number ++ ;
                t.type = INVALID ;
                t.data = "";
                return t ;

            }

        }

        return t;
    }

    if ( isalpha ( c ) || c == '_' ) {

        while ( isalnum ( c ) || c == '_' ) {

            t.data += c ;
            f.get ( c ) ;
            char_number ++ ;
            if ( c == '\n' ) {

                line_number ++ ;
                char_number = 0 ;
                t.type = INVALID ;
                t.data = "" ;
                return t ;

            }

        }

        if ( c != ' ' && c != '\n' ) residual = c ;
        for ( i = 0 ; i < id[KEYWORD].size () ; i++ ) {

            if ( t.data == id[KEYWORD][i] ) {

                t.type = KEYWORD ;
                t.subtype = i ;
                t.data = "" ;
                return t ;

            }

        }

        t.type=IDENTIFIER;
        return t;

    }

    if ( isdigit ( c ) ) {

        while ( isdigit ( c ) ) {

            String temp ( c ) ;
            t.data += temp ;
            f.get ( c ) ;
            char_number ++ ;
            if ( c == '\n' ) {

                line_number ++ ;
                char_number = 0 ;
                t.type = INVALID ;
                t.data = "" ;
                return t ;

            }

        }

        if ( c != '\n' && c != ' ' ) residual = c ;
        t.type = CONSTANT ;
        return t;

    }

    t.data += c ;
    for ( __SIZETYPE i = 0 ; i < id[PUNCTUATOR].size () ; i ++ ) {

        if ( t.data == id [ PUNCTUATOR ] [ i ] ) {

            t.type = PUNCTUATOR ;
            t.subtype = i ;
            t.data = "" ;
            return t ;

        }

    }

    f.get ( c ) ;
    ++ char_number ;
    t.data += c ;
    LABEL3:
    for ( i=0 ; i < id[ OPERATOR ].size () ; i ++ ) {

        if ( t.data == id [ OPERATOR ] [ i ] ) {

            t.type = OPERATOR ;
            t.subtype = i ;
            t.data = "" ;
            return t ;

        }

    }

    if ( t.data.length () != 1 ) {

        residual = t.data [ 1 ] ;
        t.data = t.data [ 0 ] ;
        goto LABEL3 ;

    }

    t.type = INVALID;
    t.data = "" ;
    return t ;

}
