#ifdef COMPONENT_PARSER_H
// do not include this file directly.
// use parser.h instead.

// include the implementation of classes:
//     Variable, VariableScope, Function
#include "variables.cpp"

/*******************************
* Implementation: class HashedData
********************************/
bool HashedData::clearStatements() { return this->statementSet.clear(); }
bool HashedData::clearVariables() { return this->variableSet.clear(); }
bool HashedData::addStatements(RPN st) { return this->statementSet.pushback(st); }
bool HashedData::addVariables(Vector<Variable> v) { return this->variableSet.pushback(v); }

void HashedData::setValues(csIf i) {
	this->clearStatements();
	this->addStatements(i.ifCondition);
	this->addStatements(i.ifStatements);
	this->addStatements(i.elseStatements);

	this->clearVariables();
	this->addVariables(i.ifVariables);
	this->addVariables(i.elseVariables);
}
void HashedData::setValues(csFor f) {
	this->clearStatements();
	this->addStatements(f.forInitialization);
	this->addStatements(f.forCondition);
	this->addStatements(f.forUpdate);
	this->addStatements(f.forStatements);

	this->clearVariables();
	this->addVariables(f.forVariables);
}


HashedData::csIf HashedData::getIf() {
	csIf c;
	c.ifCondition = this->statementSet[0];
	c.ifStatements = this->statementSet[1];
	c.elseStatements = this->statementSet[2];
	c.ifVariables = this->variableSet[0];
	c.elseVariables = this->variableSet[1];
	return c;
}

HashedData::csFor HashedData::getFor() {
	csFor c;
	c.forInitialization = this->statementSet[0];
	c.forCondition = this->statementSet[1];
	c.forUpdate = this->statementSet[2];
	c.forStatements = this->statementSet[3];
	c.forVariables = this->variableSet[0];
	return c;
}

/*******************************
* end implementation: HashedData
********************************/

/*************************************
* Implementation: class Parser
*************************************/

// Construction, and setup.
Parser::Parser(Lexer *ref, String args) {
	this->lexer = ref;
	this->status = PARSE_PENDING;
}

Parser::~Parser() {
	this->functions.clear();
	this->errors.clear();
	this->output.clear();
}

/*** Parser Interface ***/
bool Parser::sendError(Error e) {
	this->errors.pushback(e);
	if (e.severity() == ERROR_FATAL) {
		// a fatal error, so stop parsing further.
		this->status = PARSE_FAILED;
	}
	return true;
}
bool Parser::sendError(String cd, String msg, bufferIndex ln, int s) {
	return this->sendError(Error(cd, msg, ln, s));
}

bool Parser::showErrors(ostream& out, bool clearAfterDisplay) {
	if (this->errors.size() > 0) {
		for (__SIZETYPE i = 0; i < this->errors.size(); i++) {
			out << this->errors[i].message() << endl;
		}
		if (clearAfterDisplay) this->errors.clear();
		return true;
	}
	return false;
}

Function Parser::getFunction(String id) {
	Function f;
	for (__SIZETYPE i = 0; i < this->functions.size(); i++) {
		f = this->functions[i];
		if (f.id() == id) return f;
	}
	return f;
}

Token Parser::hashify(HashedData& hd) {
	long ind = this->hashes.size();
	this->hashes.pushback(hd);
	String hash("#d");
	hash += integerToString(ind);
	return Token(hash, DIRECTIVE, HASHED);
}
HashedData Parser::getHashedData(String hash) {
	long ind = hash.substr(0, 2).toNumber();
	return this->hashes[ind];
}

/**** Parsing Procedures ****/
bool Parser::parseSource() {
	// some error checks before proceeding:
	Vector<Error> lexerErrors = this->lexer->getErrors();
	for (__SIZETYPE i = 0; i < lexerErrors.size(); i++) this->sendError(lexerErrors[i]);
	if (this->showErrors(cerr)) {
		this->status = PARSE_FAILED;
		return false;
	}

	// Actual parsing:
	Token tok = this->lexer->getToken();
	if (tok.value() == "$eof") {
		// no code??
		this->status = PARSE_SUCCESS;
		return true;
	}
	this->lexer->putbackToken(tok);

	this->output = this->parseBlock(0); // parse the global block

	if (this->showErrors(cerr)) {
		this->status = PARSE_FAILED;
		return false;
	}

	this->status = PARSE_SUCCESS;
	return true;
}

/* 
* Parses a block of a given depth.
* assumes that block has ended if indent changes.
* Variables can be declared inside blocks, and will be treated as locals to that block.
*/
RPN Parser::parseBlock(bufferIndex depth) {
	RPN blockOutput;
	Token current;
	Infix lineBuffer;
	 
	Token eqTok = Lexer::toToken("=");
	
	variables.stackVariables(); // variables in this block.

	while (!lexer->ended()) {
		current = lexer->getToken();
		if (current.value() == "$eof") break;
		if (current.value() == "$endline") continue;
		if (current.indent() != depth) {
			// block ended, break
			lexer->putbackToken(current);
			break;
		}
		lineBuffer = lexer->getTokensTill("$endline");
		
		// variable declaration(s)
		if (current.value() == "var" || current.value() == "const" || current.value() == "global") {
			Token vartok;

			if (!lineBuffer.pop(vartok)) this->sendError("p3", "after var", current.lineNumber()); // expected identifier
			else if (vartok.type() != IDENTIFIER) this->sendError("p4.2", vartok.value(), vartok.lineNumber()); // invalid identifier
			else {
				bool reDec = false;
				if (current.value() == "global") {
					Vector<Variable>& globs = variables.getBaseVariables();
					for (__SIZETYPE i = 0; i < globs.size(); i++) 
						if (globs[i].id() == vartok.value()) {
							this->sendError("p", vartok.value(), vartok.lineNumber()); // re-declaration of global variable
							reDec = true;
						}
				}
				else if (variables.existsAtTop(vartok.value())) {
					this->sendError("p", vartok.value(), vartok.lineNumber()); // re-declaration of variable
					reDec = true;
				}
				
				// no errors, add the variable.
				if (!reDec) {
					Variable v(vartok);
					if (current.value() == "global") variables.getBaseVariables().pushback(v);
					else variables.addVariable(v);
					
					// now parse the declaration:
					Token tmp; RPN decl;

					if (lineBuffer.pop(tmp) && tmp.value() == "=") { // initialised.
						// peek the next token for the type.
						if (lineBuffer.empty()) this->sendError("p3", "expression after = for initialisation", current.lineNumber()); // expected
						
						this->lexer->putbackToken(newlineToken);
						Stack<Token> v2;
						while (lineBuffer.pop(tmp)) v2.push(tmp);
						while (v2.pop(tmp)) this->lexer->putbackToken(tmp);

						vartok.setSubtype(VARIABLE);
						decl = this->parseDeclaration(vartok);
						while (decl.pop(tmp)) blockOutput.push(tmp);
					}
				}
			}
		}
		else if (current.value() == "if") { // parse an `if` block.
			Token tmp;
			HashedData hdIf;
			HashedData::csIf i;
			
			i.ifCondition = this->expressionToRPN(lineBuffer);
			i.ifStatements = this->parseBlock(depth + 1);
			this->variables.popVariables(i.ifVariables);
			
			// check for an `else`
			tmp = lexer->getToken();
			if (tmp.value() == "else" && tmp.indent() == depth) {
				// a laddered `if`?
				__SIZETYPE nextDepth = depth + 1;
				Token next = lexer->getToken();
				if (next.value() == "if") {
					lexer->putbackToken(next);
					nextDepth = depth;
				}

				i.elseStatements = this->parseBlock(nextDepth);
				this->variables.popVariables(i.elseVariables);
			}
			
			// hash and store:
			hdIf.setValues(i);
			blockOutput.push(current); // the `if` token
			blockOutput.push(this->hashify(hdIf)); // the hashed token.
		}
		
		else if (current.value() == "function") {
			// `function` declaration format:
			// function <name> ( <param1>, <param2>, <param3>)
			Token funcname, tmp;
			if (!lineBuffer.pop(funcname)) this->sendError("p", "after keyword function", current.lineNumber()); // expected
			if (funcname.type() != IDENTIFIER) this->sendError("p", " valid function name", current.lineNumber()); // expected
			if (!!this->getFunction(funcname.value()).id()) this->sendError("p", String(" of function ") + funcname.value(), current.lineNumber()); // re-declaration

			Function func(funcname.value());
			
			Vector<String> params;
			// extract the parameter list:
			if (!lineBuffer.pop(tmp) || tmp.value() != "(") this->sendError("p1", "( after function name", current.lineNumber()); // expected
			if (!lineBuffer.pop(tmp)) this->sendError("p1", "closing ) ", current.lineNumber()); // expected
			if (tmp.value() != ")") { // has non-void parameter list
				// first parameter is `tmp`
				if (tmp.type() != IDENTIFIER) this->sendError("p4.2", tmp.value(), tmp.lineNumber());
				while (lineBuffer.pop(tmp)) {
					if (tmp.value() == ")") break;
					if (tmp.value() != ",") this->sendError("p4.3", "", tmp.lineNumber()); // expected function arg separator ,
					else if (!lineBuffer.pop(tmp) || tmp.type() != IDENTIFIER) this->sendError("p1", " identifier after ,", tmp.lineNumber()); // expected identifier after separator ,
					else params.pushback(tmp.value());
				}
				if (tmp.value() != ")") this->sendError("p1", " closing ) in function declaration", tmp.lineNumber());
			}
			
			func.setParams(params);
			// set the function's statements (hashes stored in parser itself)
			RPN funcst = this->parseBlock(depth + 1);
			func.setStatements(funcst);

			// set the function's locals
			Vector<Variable> v;
			variables.popVariables(v);
			func.setVariables(v);
		}
		else {
			Infix i;
			i.push(current);
			while (lineBuffer.pop(current)) i.push(current);
			
			RPN tempout = this->expressionToRPN(i);
			while (tempout.pop(current)) blockOutput.push(current);
			blockOutput.push(Token(";", PUNCTUATOR));
		}
		
	}
	return blockOutput;
}

RPN Parser::parseDeclaration(Token var) {
	// procedure to parse variable initialisations, on declaration
	// accounts for parsing arrays and hash tables.
	// errors can be directly caught. carefully call converting to RPN.
	
	Token current;
	RPN decl;
	if (this->lexer->ended()) return decl;
	current = this->lexer->getToken();
	
	if (current.value() == "{") { // parse a hash-table (object primitive)
		// ns
	}
	else if (current.value() == "[") { // parse array (primitive)
		long depthP = 0, depthB = 0, index = 0;
		Infix args;
		Token tmp, tmp2;
		while (!this->lexer->ended()) {
			tmp = this->lexer->getToken();
			if (tmp.value() == "(") depthP++;
			else if (tmp.value() == ")") depthP--;
			else if (tmp.value() == "[") depthB++;
			else if (tmp.value() == "]") depthB--;

			if ((tmp.value() == "," && depthB ==  0 && depthP == 0) ||
			    (tmp.value() == "]" && depthB == -1 && depthP == 0)) {
					RPN arrayElem = this->expressionToRPN(args);
					args.clear();
					decl.push(var);
					decl.push(Lexer::toToken(integerToString(index)));
					decl.push(Lexer::toToken("[]"));
					while (arrayElem.pop(tmp2)) decl.push(tmp2);
					decl.push(Lexer::toToken("="));
					decl.push(Lexer::toToken(";"));
					index++;
			}
			if (depthB == -1) break;
			if (tmp.type() != DIRECTIVE) args.push(tmp);
		}
		
		while (depthB-- > 0) this->sendError("p2", "[", current.lineNumber());
		while (depthP-- > 0) this->sendError("p2", "(", current.lineNumber());
	}
	else { // parse an expression.
		long depth = 0;
		Infix args;
		Token tmp;
		args.push(var);
		args.push(Lexer::toToken("="));
		this->lexer->putbackToken(current);

		while (!this->lexer->ended()) {
			tmp = this->lexer->getToken();
			// DEBUG(tmp.value())
			if (tmp.value() == "(" || tmp.value() == "[") depth++;
			else if (tmp.value() == ")" || tmp.value() == "]") depth--;
			else if ((tmp.value() == ","  && depth == 0) || tmp.value() == "$endline") break;
			args.push(tmp);
		}
		Infix a2 = args;
		// while (a2.pop(tmp)) DEBUG(tmp.value())
		decl = this->expressionToRPN(args);
		decl.push(Lexer::toToken(";"));
	}
	
	return decl;
}

/**** Static Parsing Procedures ****/
RPN Parser::expressionToRPN(Infix args) {
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

	static Token funcInvoke = Token("@invoke", DIRECTIVE, FUNCTION);

	Stack<__SIZETYPE> funcarglist;

	while (args.pop(current)) {
		if (current.value() == "$endline" || current.value() == ";") {
			while (operstack.pop(current)) {
				// check for unbalanced ( and [
				if (current.value() == "(" || current.value() == "[") this->sendError(Error("p1", current.value(), current.lineNumber()));
				output.push(current);
			}
			current = statementEnd;
			output.push(statementEnd);
		}

		if (current.type() == IDENTIFIER) {
			// check whether it is a function:
			if (!args.empty() && args.front().value() == "(") {
				current.setSubtype(FUNCTION);
			} else { // is a variable/property
				current.setSubtype(VARIABLE);
				output.push(current);
			}
		}
		if (current.subtype() == FUNCTION) {
			operstack.push(current);
			Token tmp;
			if (!args.pop(tmp) || tmp.value() != "(") this->sendError(Error("p3.1", "", current.lineNumber())); // function call should be succeeded by a (
			operstack.push(tmp);

			if (!args.empty() && args.front().value() == ")") {
				// zero argument function call:
				funcarglist.push(0);
			} else {
				funcarglist.push(1);
			}
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
							Token func;
							operstack.pop(func);
							output.push(func);

							__SIZETYPE argnum;
							if (funcarglist.pop(argnum)) output.push(toArgsToken(argnum));
							else {
								// unable to find number of arguments: arglist is empty.
								// should not happen.
								this->sendError(Error("p3.2", "@args", tmp.lineNumber()));
							}

							if (!operstack.empty() && operstack.top().value() == ".") {
								// dereference the method.
								Token deref;
								operstack.pop(deref);
								output.push(deref);
							}
							output.push(funcInvoke);
						}
						break;
					}
					output.push(tmp);
				}
				if (tmp.value() != "(") {
					this->sendError(Error("p1", ")", tmp.lineNumber())); // unbalanced )
				}
			}
			else if (current.value() == "]") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "[") break;
					output.push(tmp);
				}
				if (tmp.value() != "[") this->sendError(Error("p1", "]", tmp.lineNumber())); // unbalanced ]
				output.push(subscriptOp);
			}
			else if (current.value() == ",") {
				while (!operstack.empty()) {
					if (operstack.top().value() == "(") break;
					operstack.pop(tmp);
					output.push(tmp);
				}
				if (!funcarglist.empty()) funcarglist.top()++;
			}
			else if (current.value() == ":") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "?") break;
					output.push(tmp);
				}
				if (tmp.value() != "?") this->sendError(Error("p2", ": without a ?", tmp.lineNumber())); // unexpected : without a ?
				operstack.pop();
				operstack.push(ternaryOp);
			}
		}
		if (current.type() == OPERATOR) {
			if (current.value() == "-") { // check for unary minus
				if (!prevtok.value() || prevtok.value() == "(" || prevtok.value() == ";") {
					current.setSubtype(UNARYOP);
				}
			}

			if (current.value() == "++" || current.value() == "--") {
				// this is kindof problematic, coz `a[i]++` is a valid expression.
				// support for these operators will be added later. as of now ignore them.
				this->sendError(Error("ns", current.value(), current.lineNumber()));
			}
			else {
				Token oper, tval;
				while (!operstack.empty()) {
					tval = operstack.top();
					if (tval.value() == "(" || tval.value() == "[") break;
					if (Operations::comparePriority(tval, current) >= 0) {
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
	while (operstack.pop(current)) {
		// check for unbalanced ( and [
		if (current.value() == "(" || current.value() == "[") this->sendError(Error("p1", current.value(), current.lineNumber()));
		output.push(current);
	}

	// finally, after parsing the infix buffer, append the pre and post RPN.
	RPN finalOutput;
	while (preChange.pop(current)) finalOutput.push(current);
	while (output.pop(current)) finalOutput.push(current);
	while (postChange.pop(current)) finalOutput.push(current);

	return finalOutput;
}

Token Parser::toArgsToken(__SIZETYPE num) {
	String s = "@args|";
	s += integerToString(num);
	return Token(s, DIRECTIVE, FUNCTION);
}
/*** END implementation: class Parser ***/
#endif
