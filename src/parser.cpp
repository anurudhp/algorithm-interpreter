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
	this->addVariables(f.counterVariables);
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
	c.counterVariables = this->variableSet[1];
	return c;
}

Vector<RPN> HashedData::getStatements() { return this->statementSet; }

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
// adds an error to the vector of errors
bool Parser::addError(Error e) {
	this->errors.pushback(e);
	if (e.severity() == ERROR_FATAL) {
		// a fatal error, so stop parsing further.
		this->status = PARSE_FAILED;
	}
	return true;
}
bool Parser::sendError(String cd, String msg, bufferIndex ln, int s) {
	return this->addError(Error(cd, msg, ln, s));
}

// displays the errors to the console/file.
bool Parser::showErrors(ostream& out, bool clearAfterDisplay) {
	this->errors = sortErrors(this->errors);
	if (this->errors.size() > 0) {
		for (__SIZETYPE i = 0; i < this->errors.size(); i++) {
			out << this->errors[i].message() << endl;
		}
		if (clearAfterDisplay) this->errors.clear();
		return true;
	}
	return false;
}

// get a global function declared before.
Function Parser::getFunction(String id) {
	Function f;
	for (__SIZETYPE i = 0; i < this->functions.size(); i++) {
		f = this->functions[i];
		if (f.id() == id) return f;
	}
	return Function();
}

// stores the data in the hashes array,
// and returns the index where it is stored
// in a hashed format: "#index"
Token Parser::hashify(HashedData& hd) {
	long ind = this->hashes.size();
	this->hashes.pushback(hd);
	String hash("#d");
	hash += integerToString(ind);
	return Token(hash, DIRECTIVE, HASHED);
}
// get the hashed data corresponding to the given string hash.
HashedData Parser::getHashedData(String hash) {
	long ind = hash.substr(2).toNumber();
	return this->hashes[ind];
}

/**** Parsing Procedures ****/
// The main parsing procedure.
// Parses the source code, and sets the status.
// Displays errors on failure.
bool Parser::parseSource() {
	// some error checks before proceeding:
	Vector<Error> lexerErrors = this->lexer->getErrors();
	for (__SIZETYPE i = 0; i < lexerErrors.size(); i++) this->addError(lexerErrors[i]);
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
	this->validateRPN(this->output);

	lexerErrors = this->lexer->getErrors();
	for (__SIZETYPE i = 0; i < lexerErrors.size(); i++) this->addError(lexerErrors[i]);

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

	static Token eqTok = Lexer::toToken("="),
	             statementEnd = Lexer::toToken(";");

	variables.stackVariables(); // variables in this block.

	while (!lexer->ended()) {
		current = lexer->getToken();
		if (current.value() == "$null") return RPN();
		if (current.value() == "$eof") break;
		if (current.value() == "$endline") continue;
		if (current.indent() > depth) {
			// unexpected over-indentation
			this->sendError("l4", "", current.lineNumber());
		}
		if (current.indent() < depth) {
			// block ended.
			lexer->putbackToken(current);
			break;
		}
		lineBuffer = lexer->getTokensTill("$endline");

		// variable declaration(s)
		if (current.value() == "var" || current.value() == "global") {
			Token vartok;

			if (!lineBuffer.pop(vartok)) this->sendError("p3", "after var", current.lineNumber()); // expected identifier
			else if (vartok.type() != IDENTIFIER) this->sendError("p4.2", vartok.value(), vartok.lineNumber()); // invalid identifier
			else {
				bool reDec = false;
				if (current.value() == "global") {
					Vector<Variable>& globs = variables.getBaseVariables();
					for (__SIZETYPE i = 0; i < globs.size(); i++)
						if (globs[i].id() == vartok.value()) {
							this->sendError("p4.1", vartok.value(), vartok.lineNumber()); // re-declaration of global variable
							reDec = true;
						}
				}
				else if (variables.existsAtTop(vartok.value())) {
					this->sendError("p4.1", vartok.value(), vartok.lineNumber()); // re-declaration of variable
					reDec = true;
				}

				// no errors, add the variable.
				if (!reDec) {
					Variable v(vartok);
					if (current.value() == "global") variables.getBaseVariables().pushback(v);
					else variables.addVariable(v);
					vartok.setSubtype(VARIABLE);

					// now parse the declaration:
					Token tmp; RPN decl;
					if (lineBuffer.pop(tmp) && tmp.value() == "=") { // initialised.
						this->lexer->putbackToken(newlineToken);
						Stack<Token> v2;
						while (lineBuffer.pop(tmp)) v2.push(tmp);
						while (v2.pop(tmp)) this->lexer->putbackToken(tmp);

						decl = this->parseDeclaration();
						blockOutput.push(vartok);
						while (decl.pop(tmp)) blockOutput.push(tmp);
						
						eqTok.setLineNumber(current.lineNumber());
						blockOutput.push(eqTok);
						statementEnd.setLineNumber(current.lineNumber());
						blockOutput.push(statementEnd);
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
			else {
				lexer->putbackToken(tmp);
			}

			// hash and store:
			hdIf.setValues(i);
			blockOutput.push(current); // the `if` token
			blockOutput.push(this->hashify(hdIf)); // the hashed token.
		}
		else if(current.value() == "while" || current.value() == "until") {
			HashedData hdWhile;
			HashedData::csFor w;

			w.forCondition = this->expressionToRPN(lineBuffer);
			w.forStatements = this->parseBlock(depth + 1);
			this->variables.popVariables(w.forVariables);

			hdWhile.setValues(w);
			blockOutput.push(current);
			blockOutput.push(this->hashify(hdWhile));

		}
		else if (current.value() == "for") {
			// usage: 
			//   for i = 1 to N
			//   for i = N downto 1
			HashedData hdFor;
			HashedData::csFor f;
			Token tmp, eq, iter, comp;
			Infix expr; RPN res;

			// get the iterator variable
			if (!lineBuffer.pop(iter) || iter.type() != IDENTIFIER) this->sendError("p3", "identifier as iterator in `for` expression", iter.lineNumber());
			iter.setSubtype(VARIABLE);
			f.counterVariables.pushback(Variable(iter));
			// get the =
			if (!lineBuffer.pop(eq) || eq.value() != "=") this->sendError("p3", "=", tmp.lineNumber());

			expr.clear();
			while (lineBuffer.pop(tmp)) {
				if (tmp.value() == "to" || tmp.value() == "downto") break;
				expr.push(tmp);
			}
			if (tmp.value() != "to" && tmp.value() != "downto") this->sendError("p3", "to", tmp.lineNumber());
			comp = tmp;
			res = this->expressionToRPN(expr);

			f.forInitialization.push(iter);
			while (res.pop(tmp)) f.forInitialization.push(tmp);
			f.forInitialization.push(eq);

			res = this->expressionToRPN(lineBuffer);
			f.forCondition.push(iter);
			while (res.pop(tmp)) f.forCondition.push(tmp);

			tmp = Lexer::toToken((comp.value() == "to") ? "<=" : ">="); tmp.setLineNumber(current.lineNumber());
			f.forCondition.push(tmp);

			f.forUpdate.push(iter);
			tmp = Lexer::toToken("1"); tmp.setLineNumber(current.lineNumber());
			f.forUpdate.push(tmp);
			tmp = Lexer::toToken((comp.value() == "to") ? "+=" : "-="); tmp.setLineNumber(current.lineNumber());
			f.forUpdate.push(tmp);

			f.forStatements = this->parseBlock(depth + 1);
			this->variables.popVariables(f.forVariables);

			hdFor.setValues(f);
			blockOutput.push(current);
			blockOutput.push(this->hashify(hdFor));
		}
		else if (current.value() == "foreach") {
			// usage:
			//   foreach key in object
			//   foreach index in array
			HashedData hdForeach;
			HashedData::csFor f;

			Token tmp;
			lineBuffer.pop(tmp); // key/index
			if (tmp.type() != IDENTIFIER) this->sendError("p3", "identifier as iterator in `foreach` expression", current.lineNumber());
			f.counterVariables.pushback(Variable(tmp));
			
			if (!lineBuffer.pop(tmp) || tmp.value() != "in") this->sendError("p3", "keyword `in`", current.lineNumber());

			f.forInitialization = this->expressionToRPN(lineBuffer);
			f.forStatements = this->parseBlock(depth + 1);
			this->variables.popVariables(f.forVariables);

			hdForeach.setValues(f);
			blockOutput.push(current);
			blockOutput.push(this->hashify(hdForeach));
		}
		else if (current.value() == "function") {
			// `function` declaration format:
			// function <name> ( <param1>, <param2>, <param3>)
			Token funcname, tmp;
			if (!lineBuffer.pop(funcname)) this->sendError("p3", "after keyword function", current.lineNumber()); // expected
			if (funcname.type() != IDENTIFIER) this->sendError("p3", " valid function name", current.lineNumber()); // expected
			if (!!this->getFunction(funcname.value()).id()) this->sendError("p3.4", funcname.value(), current.lineNumber()); // re-declaration

			Function func(funcname.value());

			Vector<String> params;
			// extract the parameter list:
			if (!lineBuffer.pop(tmp) || tmp.value() != "(") this->sendError("p3", "( after function name", current.lineNumber()); // expected
			if (!lineBuffer.pop(tmp)) this->sendError("p3", "closing ) in function declaration", current.lineNumber()); // expected
			else if (tmp.value() != ")") { // has non-void parameter list
				// first parameter is `tmp`
				if (tmp.type() != IDENTIFIER) this->sendError("p4.2", tmp.value(), tmp.lineNumber());
				else params.pushback(tmp.value());

				while (lineBuffer.pop(tmp)) {
					if (tmp.value() == ")") break;
					if (tmp.value() != ",") this->sendError("p4.3", "", tmp.lineNumber()); // expected function arg separator ,
					else if (!lineBuffer.pop(tmp) || tmp.type() != IDENTIFIER) this->sendError("p3", " identifier after ,", tmp.lineNumber()); // expected identifier after separator ,
					else params.pushback(tmp.value());
				}
				if (tmp.value() != ")") this->sendError("p3", " closing ) in function declaration", tmp.lineNumber());
			}

			func.setParams(params);
			// set the function's statements (hashes stored in parser itself)
			RPN funcst = this->parseBlock(depth + 1);
			func.setStatements(funcst);

			// set the function's locals
			Vector<Variable> v;
			this->variables.popVariables(v);
			func.setVariables(v);

			this->functions.pushback(func);
		}
		else if (current.value() == "return") {
			RPN returnExpr = this->expressionToRPN(lineBuffer);
			Token tmp;
			while (returnExpr.pop(tmp)) blockOutput.push(tmp);
			blockOutput.push(current);
		}
		else {
			Infix i;
			i.push(current);
			bool pr = (current.value() == "print" || current.value() == "printLine");
			if (pr) i.push(Lexer::toToken("("));
			while (lineBuffer.pop(current)) i.push(current);
			if (pr) i.push(Lexer::toToken(")"));

			RPN tempout = this->expressionToRPN(i);
			while (tempout.pop(current)) blockOutput.push(current);
			blockOutput.push(Token(";", PUNCTUATOR));
		}

	}
	return blockOutput;
}

// procedure to parse variable initialisations, on declaration
// Takes care of parsing arrays and hash tables.
RPN Parser::parseDeclaration() {
	Token current;
	RPN decl;
	if (this->lexer->ended()) return decl;
	current = this->lexer->getToken();

	if (current.value() == "{") { // parse a hash-table (object primitive)
		long depth = 0, index = 0;
		Infix args, objdecl;
		Token key, tmp, tmp2;

		objdecl = this->lexer->getTokensTill("}");

		while (!objdecl.empty()) {
			if (!objdecl.pop(key)) this->sendError("p3", "key in object declaration", current.lineNumber());
			if (!(key.type() == IDENTIFIER || key.type() == LITERAL)) this->sendError("p4.4", "", key.lineNumber());

			if (key.type() == IDENTIFIER) key = Lexer::toToken(Lexer::stringToLiteral(key.value()));
			decl.push(key);

			if (!objdecl.pop(tmp) || tmp.value() != ":") this->sendError("p3", ": after key", current.lineNumber());

			args.clear();
			while (objdecl.pop(tmp)) {
				if (tmp.value() == "(" || tmp.value() == "[") depth++;
				else if (tmp.value() == ")" || tmp.value() == "]") depth--;
				else if (tmp.value() == "," && depth ==  0) break;
				else if (tmp.type() != DIRECTIVE) args.push(tmp);
			}
			RPN objValue = this->expressionToRPN(args);
			args.clear();
			while (objValue.pop(tmp2)) decl.push(tmp2);
			index++;
		}
		decl.push(Token("@init", DIRECTIVE, OBJECT));
		decl.push(this->toArgsToken(index));
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
					while (arrayElem.pop(tmp2)) decl.push(tmp2);
					index++;
			}
			if (depthB == -1) break;
			if (tmp.type() != DIRECTIVE) args.push(tmp);
		}

		while (depthB-- > 0) this->sendError("p2", "[", current.lineNumber());
		while (depthP-- > 0) this->sendError("p2", "(", current.lineNumber());
		if (decl.empty()) index = 0;
		else if (depthB > -1) this->sendError("p3", "closing ] in array declaration", current.lineNumber());
		decl.push(Token("@init", DIRECTIVE, ARRAY));
		decl.push(this->toArgsToken(index));
	}
	else { // parse an expression.
		long depth = 0;
		Infix args;
		Token tmp;
		this->lexer->putbackToken(current);

		while (!this->lexer->ended()) {
			tmp = this->lexer->getToken();
			if (tmp.value() == "(" || tmp.value() == "[") depth++;
			else if (tmp.value() == ")" || tmp.value() == "]") depth--;
			else if ((tmp.value() == ","  && depth == 0) || tmp.value() == "$endline") break;
			args.push(tmp);
		}
		decl = this->expressionToRPN(args);
	}

	return decl;
}

/**** Common Parsing Procedures ****/
// Converts a given Infix expression to RPN
// supports: variable, function calls, operators, brackets
// flags errors in case of unexpected or invalid tokens.
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
		bool expected = false;
		if (current.value() == "$endline" || current.value() == ";") {
			while (operstack.pop(current)) {
				// check for unbalanced ( and [
				if (current.value() == "(" || current.value() == "[") this->sendError("p1", current.value(), current.lineNumber());
				output.push(current);
			}
			current = statementEnd;
			output.push(statementEnd);
			expected = true;
		}
		if (current.value() == "$null") {
			expected = true;
		}

		if (current.type() == IDENTIFIER) {
			// check whether it is a function:
			if (!args.empty() && args.front().value() == "(") {
				current.setSubtype(FUNCTION);
			} else { // is a variable/property
				current.setSubtype(VARIABLE);
				output.push(current);
			}
			expected = true;
		}
		if (current.subtype() == FUNCTION) {
			operstack.push(current);
			Token tmp;
			if (!args.pop(tmp) || tmp.value() != "(") this->sendError("p3.1", "", current.lineNumber()); // function call should be succeeded by a (
			operstack.push(tmp);

			// zero argument function call: in case next token is )
			funcarglist.push((!args.empty() && args.front().value() == ")") ? 0 : 1);
			expected = true;
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
								this->sendError("p3.2", "@args", current.lineNumber());
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
					this->sendError("p2", ")", current.lineNumber()); // unbalanced )
				}
			}
			else if (current.value() == "]") {
				while (operstack.pop(tmp)) {
					if (tmp.value() == "[") break;
					output.push(tmp);
				}
				if (tmp.value() != "[") this->sendError("p2", "]", current.lineNumber()); // unbalanced ]
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
				if (tmp.value() != "?") this->sendError("p2", ": without a ?", current.lineNumber()); // unexpected : without a ?
				operstack.pop();
				operstack.push(ternaryOp);
			}
			expected = true;
		}
		if (current.type() == OPERATOR) {
			if (current.value() == "-") { // check for unary minus
				if (!prevtok.value() || prevtok.value() == "(" || prevtok.value() == "=" || prevtok.value() == ";" || prevtok.type() == OPERATOR) {
					current.setSubtype(UNARYOP);
				}
			}
			else if (current.value() == "++" || current.value() == "--") {
				if (!args.empty() && args.front().type() == IDENTIFIER) current.setSubtype(PRE);
				else current.setSubtype(POST);
			}

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
			expected = true;
		}
		if (current.type() == LITERAL) {
			output.push(current);
			expected = true;
		}
		if (!expected) {
			this->sendError("p2.2", current.value(), current.lineNumber());
		}
		prevtok = current;
	}
	while (operstack.pop(current)) {
		// check for unbalanced ( and [
		if (current.value() == "(" || current.value() == "[") this->sendError("p1", current.value(), current.lineNumber());
		output.push(current);
	}

	// finally, after parsing the infix buffer, append the pre and post RPN.
	RPN finalOutput;
	while (preChange.pop(current)) finalOutput.push(current);
	while (output.pop(current)) finalOutput.push(current);
	while (postChange.pop(current)) finalOutput.push(current);

	return finalOutput;
}

// Checks whether a given RPN expression is valid
// validation rules:
//     1. Sufficient operands for operators
//     2. valid operands for . ++ -- and assignment.
//     3. in case of a block, gets the block, and checks each statement set in the block.
bool Parser::validateRPN(RPN rpn) {
	Stack<tokenType> vals;
	Token curr;
	Vector<String> assignmentOp(strsplit("= += -= *= %= /=", ' '));
	bool success = true;

	while (rpn.pop(curr)) {
		if (curr.value() == ";") vals.clear();
		else if (curr.type() == LITERAL) vals.push(LITERAL);
		else if (curr.type() == IDENTIFIER) vals.push(curr.subtype());
		else if (curr.type() == OPERATOR) {
			if (curr.subtype() == UNARYOP) {
				if (!vals.pop()) {
					this->sendError("p3.3", curr.value(), curr.lineNumber());
					success = false;
				}
				vals.push(LITERAL);
			}
			else if (curr.subtype() == BINARYOP) {
				tokenType t1, t2;
				if (!vals.pop(t2) || !vals.pop(t1)) {
					this->sendError("p3.3", curr.value(), curr.lineNumber());
					success = false;
				}
				else if (assignmentOp.indexOf(curr.value()) >= 0) {
					if (t1 != VARIABLE) {
						this->sendError("rv1", "", curr.lineNumber());
						success = false;
					}
					vals.push(t1);
				}
				else if (curr.value() == ".") {
					if ((t1 != VARIABLE && t1 != OBJECT) || t2 != VARIABLE) {
						this->sendError("ro1", curr.value(), curr.lineNumber());
						success = false;
					}
					vals.push(t1);
				}
				else if (curr.value() == "[]") {
					if (t1 != VARIABLE && t1 != OBJECT) {
						this->sendError("ro1", curr.value(), curr.lineNumber());
						success = false;
					}
					vals.push(t1);
				}
				else {
					vals.push(LITERAL);
				}
			}
			else if (curr.subtype() == MULTINARYOP) { // only ?: (3 args)
				tokenType t1, t2, t3;
				vals.pop(t3); vals.pop(t2); vals.pop(t1);
				vals.push(t2);
			}
			else if (curr.subtype() == PRE || curr.subtype() == POST) {
				tokenType t;
				if (!vals.pop(t)) {
					this->sendError("p3.3", curr.value(), curr.lineNumber());
					success = false;
				}
				if (t != VARIABLE) {
					this->sendError("ro1", curr.value(), curr.lineNumber());
					success = false;
				}
				vals.push(LITERAL);
			}
		}
		else if (curr.type() == DIRECTIVE) {
			if (curr.value() == "@init") {
				// array/object primitive:
				Token length; tokenType t;
				rpn.pop(length);
				long len = length.value().substr(6).toInteger();
				if (curr.subtype() == ARRAY) {
					while (len--) vals.pop(t);
					vals.push(OBJECT);
				}
				else if (curr.subtype() == OBJECT) {
					while (len--) {
						vals.pop(); vals.pop();
					}
					vals.push(OBJECT);
				}
			}
			if (curr.value().substr(0, 6) == "@args|") {
				long ind = curr.value().substr(6).toInteger();
				vals.pop(); // function token
				while (ind--) vals.pop(); // args
				Token inv; tokenType t1;
				rpn.pop(inv);
				if (inv.value() == ".") {
					if (!vals.pop(t1)) {
						this->sendError("p3.3", inv.value(), inv.lineNumber());
						success = false;
						t1 = VARIABLE; // prevent further errors
					}
					else if (t1 != VARIABLE) {
						this->sendError("ro1", inv.value(), inv.lineNumber());
						success = false;
						t1 = VARIABLE; // prevent further errors
					}
					rpn.pop(inv);
				}
				vals.push(OBJECT);
			}
			else if (curr.type() == HASHED) {
				Vector<RPN> st = this->getHashedData(curr.value()).getStatements();
				RPN tmp;
				while (st.popfront(tmp)) success = success && this->validateRPN(tmp);
			}
		}
		else if (curr.type() == KEYWORD) {
			if (curr.value() == "if" || curr.value() == "while" || curr.value() == "for" || curr.value() == "foreach") {
				rpn.pop(curr);
				Vector<RPN> st = this->getHashedData(curr.value()).getStatements();
				RPN tmp;
				while (st.popfront(tmp)) success = success && this->validateRPN(tmp);
			}
		}
	}
	return success;
}

// converts a number into: "@args|<number>"
// this is used as a directive to find the number of arguments for a function call.
Token Parser::toArgsToken(__SIZETYPE num) {
	String s = "@args|";
	s += integerToString(num);
	return Token(s, DIRECTIVE, FUNCTION);
}

// sort the errors based on line numbers:
Vector<Error> Parser::sortErrors(Vector<Error> errors) {
	__SIZETYPE sz = errors.size(), i, j;
	Error tmp;
	for (i = 0; i < sz; i++) {
		for (j = i + 1; j < sz; j++) {
			if (errors[j].lineNumber() < errors[i].lineNumber()) {
				tmp = errors[i];
				errors[i] = errors[j];
				errors[j] = tmp;
			}	
		}
	}

	return errors;
}
/*** END implementation: class Parser ***/
#endif
