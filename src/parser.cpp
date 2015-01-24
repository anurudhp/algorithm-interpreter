#ifdef COMPONENT_PARSER_H
// do not include this file directly.
// use parser.h instead.

/************************************
* Implementation: class Error      **
*************************************/

Error::Error(String cd, String fg, bufferIndex ln, int s) {
	this->_code = cd;
	this->_flag = fg;
	this->_line = ln;
	this->_severity = s;
}
Error::Error(const Error& e) {
	this->_code = e._code;
	this->_flag = e._flag;
	this->_line = e._line;
	this->_severity = e._severity;
}
Error& Error::operator= (const Error& e) {
	this->_code = e._code;
	this->_flag = e._flag;
	this->_line = e._line;
	this->_severity = e._severity;
	return *this;
}

String Error::code() const { return this->_code; }
String Error::flag() const { return this->_flag; }
bufferIndex Error::lineNumber() const { return this->_line; }
int Error::severity() const { return this->_severity; }

String Error::message() const {
	__SIZETYPE i;
	String ret;
	ret = ret + "Line " + toString(this->_line) + ": ";
	if (this->_severity == FATAL) ret += "FATAL Error ";
	else if (this->_severity == ERROR) ret += "Error ";
	else if (this->_severity == WARNING) ret += "Warning ";
	i = errorCodes.indexOf(this->_code);
	if (i >= 0) ret += errorDesc[i];
	ret += this->_flag;
}

bool Error::setLineNumber(bufferIndex ln) { this->_line = ln; }
/*** END implementation: class Error ***/

bool loadErrorCodes() {
	ifstream ecreader("errorcodes.txt");
	String buff; Vector<String> vs;

	errorCodes.clear(); errorDesc.clear();

	while (!ecreader.eof()) {
		buff.get(ecreader, '\n');
		if (buff.substr(0, 1) != "#") {
			vs = strsplit(buff, ":");
			errorCodes.pushback(vs[0].trim());
			errorDesc.pushback(vs[1].trim());
		}
	}
}

// include the implementation of classes Variable and VariableScope
#include "variables.cpp"

/*************************************
* Implementation: class Parser
*************************************/

// Construction, and setup.
Parser::Parser(Lexer *ref, String args) {
	this->lexer = ref;
	this->status = PENDING;
}

Parser::~Parser() {
	functions.clear();
	errors.clear();
	output.clear();
}

/*** Parser Interface ***/
bool Parser::sendError(Error e) {
	this->errors.pushback(e);
	if (e.severity() == FATAL) {
		// a fatal error, so stop parsing further.
		this->status = FAILED;
	}
	return true;
}

Function Parser::getFunction(String id) {
	Function f;
	for (__SIZETYPE i = 0; i < this->functions.size(); i++) {
		f = this->functions[i];
		if (f.id() == id) return f;
	}
	return f;
}

/**** Parsing Procedures ****/
RPN Parser::parseDeclaration(Infix decl, tokenType type) {
	// procedure to parse array and hash table primitives.
	// errors can be directly caught. carefully call converting to RPN.
	return RPN();
}
RPN Parser::parseStatement(Infix in) {
	return RPN();
	
}
/**** Static Parsing Procedures ****/
RPN Parser::expressionToRPN(Infix args, Vector<Error>& errors, VariableScope& scope, Function (*getFunction)(String) ) {
	if (args.empty()) return RPN();

	RPN output;
	RPN preChange, postChange;
	Token current, prevtok;
	Stack<Token> operstack;

	static Token subscriptOp = Lexer::toToken("[]"),
	             ternaryOp = Token("?:", OPERATOR, MULTINARYOP),
	             statementEnd = Lexer::toToken(";"),
	             pluseqTok = Lexer::toToken("+="),
	             minuseqTok = Lexer::toToken("-="),
	             oneTok = Lexer::toToken("1");

	args.push(statementEnd);
	
	while (args.pop(current)) {

		if (current.value() == "\n" || current.value() == ";") {
			while (operstack.pop(current)) {
				// check for unbalanced ( and [
				if (current.value() == "(" || current.value() == "[") errors.pushback(Error("p", current.value(), current.lineNumber()));
				output.push(current);
			}
			current = statementEnd;
			output.push(statementEnd);
		}

		if (current.type() == IDENTIFIER) {
			if (scope.exists(current.value())) {
				current.setSubtype(VARIABLE);
				output.push(current);
			} 
			else if (!!(getFunction(current.value()).id())) {
				current.setSubtype(FUNCTION);
			}
		}
		if (current.subtype() == FUNCTION) {
			operstack.push(current);
			Token tmp;
			if (!args.pop(tmp) || tmp.value() != "(") errors.pushback(Error("p", "(", current.lineNumber())); // function call should be succeeded by a (
			operstack.push(tmp);
			output.push(tmp);
		}

		if (current.type() == PUNCTUATOR) {
			Token tmp;

			if (current.value() == "(" || current.value() == "[") {
				operstack.push(current);
			}
			else if (current.value() == ")") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "(") {
						if (!operstack.empty() && operstack.top().subtype() == FUNCTION) {
							operstack.pop(tmp);
							output.push(tmp);
						}
						break;
					}
					output.push(tmp);
				}
				if (tmp.value() != "(") errors.pushback(Error("p", ")", tmp.lineNumber())); // unbalanced )
			}
			else if (current.value() == "]") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "[") break;
					output.push(tmp);
				}
				if (tmp.value() != "[") errors.pushback(Error("p", "]", tmp.lineNumber())); // unbalanced ]
				output.push(subscriptOp);
			}
			else if (current.value() == ",") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "(") break;
					output.push(tmp);
				}
			}
			else if (current.value() == ":") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "?") break;
					output.push(tmp);
				}
				if (tmp.value() != "?") errors.pushback(Error("p", ":", tmp.lineNumber())); // unexpected : without a ?
				operstack.pop();
				operstack.push(ternaryOp);
			}
		}
		if (current.type() == OPERATOR) {
			if (current.value() == "++" || current.value() == "--") {
				if (prevtok.type() == VARIABLE) { // post-inc/dec
					postChange.push(prevtok);
					postChange.push(oneTok);
					postChange.push((current.value() == "++") ? pluseqTok : minuseqTok);
					postChange.push(statementEnd);
				} else if (args.pop(current) && current.type() == IDENTIFIER && scope.exists(current.value())) { // pre-inc/dec
					preChange.push(prevtok);
					preChange.push(oneTok);
					preChange.push((current.value() == "++") ? pluseqTok : minuseqTok);
					preChange.push(statementEnd);
				} else {
					errors.pushback(Error("p", current.value(), current.lineNumber())); // unexpected ++ or --
				}
			}
			else {
				Token oper;
				String tval;
				while (!operstack.empty()) {
					tval = operstack.top().value();
					if (tval == "(" || tval == "[") break;
					if (Operations::comparePriority(current.value(), tval) >= 0) {
						operstack.pop(oper);
						output.push(oper);
					}
					else break;
				}
				operstack.push(current);
			}
		}
		if (current.type() == LITERAL) {
			output.push(current);
		}
		prevtok = current;
	}

	// finally, after parsing the infix buffer, append the pre and post RPN.
	RPN finalOutput;
	while (preChange.pop(current)) finalOutput.push(current);
	while (output.pop(current)) finalOutput.push(current);
	while (postChange.pop(current)) finalOutput.push(current);

	return finalOutput;
}
/*** END implementation: class Parser ***/
#endif
