#ifdef COMPONENT_EVALUATOR_H

Evaluator::Evaluator(Parser *pr, String args) {
	this->parser = pr;
	this->failed = false;
}

bool Evaluator::runProgram() {
	// initialise the HEAP
	this->variables = this->parser->variables;

	RPN primarySource = this->parser->output;
	Vector<Token> rets;

	this->evaluateRPN(primarySource, this->variables, &rets);
	if (this->failed) {
		return this->showErrors(cerr);
	}
	return true;
}

/*** interface ***/
bool Evaluator::sendError(Error e) {
	this->errors.pushback(e);
	this->failed = true;
	return true;
}
bool Evaluator::sendError(String cd, String msg, bufferIndex ln, int s) {
	return this->sendError(Error(cd, msg, ln, s));
}

bool Evaluator::showErrors(ostream& out, bool clearAfterDisplay) {
	if (this->errors.size() > 0) {
		for (__SIZETYPE i = 0; i < errors.size(); i++) {
			out << errors[i].message() << endl;
		}
		if (clearAfterDisplay) errors.clear();
		return true;
	}
	return false;
}

/*** Variable Caches and management ***/
Variable& Evaluator::getCachedVariable(String id) {
	if (id.substr(0, 2) == "#c") {
		long index = id.substr(2).toInteger();
		if (index < 0 || index >= this->cache.size()) return nullVariableRef;
		return *(this->cache[index]);
	}
	return nullVariableRef;
}

String Evaluator::cacheVariable(Variable v) {
	Variable *ref = new Variable();
	if (ref == NULL) forcequit();
	ref->setValue(v);
	return this->cacheVariableRef(ref);
}
String Evaluator::cacheVariable() {
	Variable *ref = new Variable();
	return this->cacheVariableRef(ref);
}
String Evaluator::cacheVariableRef(Variable *ref) {
	if (ref == NULL) return "";
	this->cache.pushback(ref);
	long index = this->cache.size() - 1;
	String hash = "#c";
	hash += integerToString(index);
	return hash;
}

Variable& Evaluator::getVariable(String id, VariableScope& scope, bool searchCache) {
	if (scope.exists(id)) {
		return scope.resolve(id);
	}
	if (searchCache) {
		Variable& v = this->getCachedVariable(id);
		return v;
	}
	return nullVariableRef;
}
Token Evaluator::getVariableValue(Token var, VariableScope& scope, bool searchCache) {
	if (var.subtype() == VARIABLE) {
		return this->getVariable(var.value(), scope, searchCache).value();
	}
	if (var.subtype() == LITERAL) return var;
	return nullvalToken;
}

Vector<Variable>& Evaluator::getGlobals() {
	return this->variables.getBaseVariables();
}

Token Evaluator::evaluateRPN(RPN source, VariableScope& scope, Vector<Token>* statementValues) {
	if (this->failed) return Token();
	Token current;
	Stack<Token> valuestack;

	Vector<String> assignmentOp(strsplit("= += -= *= /= %=", ' '));
	Variable nullvalVariable("$");
	nullvalVariable.setValue(nullvalToken);
	nullvalVariable.setType(UNKNOWN);

	while (source.pop(current)) {
		if (this->failed) return Token();
		if (!this->functionStack.empty()) {
			if (this->functionStack.top().hasReturned()) return Token();
		}

		if (current.type() == LITERAL || current.subtype() == VARIABLE) valuestack.push(current);
		else if (current.value() == ";") {
			Token t;
			// if (valuestack.pop(t) && statementValues != NULL) statementValues->pushback(t);
		}
		else if (current.type() == DIRECTIVE) {
			// directives that reach here: array and object initialisers
			if (current.value() == "@init") {
				Token dataSize, res, key, val;
				source.pop(dataSize);
				long length = dataSize.value().substr(6).toInteger();

				if (current.subtype() == ARRAY) {
					String hash = this->cacheVariable();
					Variable& v = this->getVariable(hash, scope, true);
					v.setType(ARRAY);

					for (__SIZETYPE i = 0; i < length; i++) {
						valuestack.pop(val);
						v.pushValue((val.subtype() == VARIABLE) ? this->getVariable(val.value(), scope, true) : Variable(val), true);
					}
					res = Token(hash, DIRECTIVE, VARIABLE);
				}
				else if (current.subtype() == OBJECT) {
					String hash = this->cacheVariable();
					Variable& v = this->getVariable(hash, scope, true);
					v.setType(OBJECT);

					for (__SIZETYPE i = 0; i < length; i++) {
						valuestack.pop(val);
						valuestack.pop(key);
						v.addPair(key, (val.subtype() == VARIABLE) ? this->getVariable(val.value(), scope, true) : Variable(val));
					}
					res = Token(hash, DIRECTIVE, VARIABLE);
				}
				valuestack.push(res);
			}
		}
		else if (current.type() == OPERATOR) {
			if (current.value() == ".") {
				Token vartok, prop;
				valuestack.pop(prop);
				valuestack.pop(vartok);
				
				Variable &var = this->getVariable(vartok.value(), scope, true);
				if (var.type() != OBJECT) this->sendError("ro1", ". : LHS must be an object", vartok.lineNumber());
				else {
					prop = Operations::typecastToken(prop, STRING);
					String hash = this->cacheVariableRef(&var.valueAt(prop));
					valuestack.push(Token(hash, DIRECTIVE, VARIABLE));
				}
			}
			else if (current.value() == "[]") {
				// subscripting operator:
				// string[index] or array[index] or object[key]
				Token a, b;
				valuestack.pop(b);
				valuestack.pop(a);

				if (b.subtype() == VARIABLE) {
					b = this->getVariable(b.value(), scope, true).value();
				}
				bool valid = false;
				if (a.type() == LITERAL) {
					if (a.subtype() == STRING && b.value().isInteger()) {
						long ind = b.value().toInteger();
						String str = Lexer::tokenToString(a);
						if (ind < 0 || ind >= str.length()) {
							this->sendError(Error("r")); // invalid index
							valuestack.push(nullvalToken);
						}
						else {
							str = Lexer::stringToLiteral(str.substr(ind, 1));
							valuestack.push(Lexer::toToken(str));
						}
						valid = true;
					}
				}
				else if (a.subtype() == VARIABLE) {
					Variable& var = this->getVariable(a.value(), scope, true);
					if (var.type() == STRING) {
						if (b.value().isInteger()) {
							a = var.value();
							long ind = b.value().toInteger();
							String str = Lexer::tokenToString(a);
							if (ind < 0 || ind >= str.length()) {
								this->sendError(Error("r")); // invalid index
								valuestack.push(nullvalToken);
							}
							else {
								str = Lexer::stringToLiteral(str.substr(ind, 1));
								valuestack.push(Lexer::toToken(str));
							}
							valid = true;
						}
					}
					else if (var.type() == ARRAY || var.type() == OBJECT) {
						if (var.hasValueAt(b)) {
							Variable& prop = var.valueAt(b);
							String hash = this->cacheVariableRef(&prop);
							valuestack.push(Token(hash, DIRECTIVE, VARIABLE));
							valid = true;
						}
					}
				}

				if (!valid) {
					// only display types for operands.
					this->sendError(Error("ro1", current.value() + " : " + a.value() + " and " + b.value())); // invalid types for []
				}
			}
			else if (current.subtype() == UNARYOP) {
				Token a;
				valuestack.pop(a);
				if (current.value() == "typeof") {
					tokenType ty;
					if (a.subtype() == VARIABLE) {
						ty = this->getVariable(a.value(), scope, true).type();
					}
					else if (a.type() == LITERAL) {
						ty = a.subtype();
					}
					String typeStr = Lexer::typeToString(ty);
					valuestack.push(Lexer::toToken(Lexer::stringToLiteral(typeStr)));
				}
				else {
					a = this->getVariableValue(a, scope, true);
					valuestack.push(Operations::unaryOperator(current.value(), a));
				}
			}
			else if (current.subtype() == BINARYOP) {
				// check if it is assignment operator:
				String oper = current.value();

				bool isAssign = false;

				if (assignmentOp.indexOf(current.value()) >= 0) {
					oper = current.value().substr(0, 1);
					isAssign = true;
				}

				Token a, b;
				valuestack.pop(b);
				valuestack.pop(a);
				Token va = a, vb = b, res;

				if (a.subtype() == VARIABLE) {
					Variable& v1 = this->getVariable(a.value(), scope, true);
					va = v1.value();
				}
				if (b.subtype() == VARIABLE) {
					Variable& v2 = this->getVariable(b.value(), scope, true);
					vb = v2.value();
				}

				if (oper == "=") res = b;
				else res = Operations::binaryOperator(oper, va, vb);

				if (isAssign) {
					Variable& v = this->getVariable(a.value(), scope, true);
					if (v.id() == ";") {
						this->sendError("rv1", a.value());
					}
					else { // a = res
						if (res.subtype() == VARIABLE) {
							v.setValue(this->getVariable(res.value(), scope, true));
						} else {
							v.setValue(Variable(res));
						}
					}
				}
				valuestack.push(res);
			}
			else if (current.subtype() == PRE || current.subtype() == POST) {
				Token tok, val, res;
				valuestack.pop(tok);

				Variable& v = this->getVariable(tok.value(), scope, true);
				if (v.type() != NUMBER) this->sendError("ro2", current.value()); // ++ and -- work only on numbers

				val = v.value(); // the old value.
				res = Operations::binaryOperator(current.value().substr(0, 1), val, Lexer::toToken("1")); // after inc/dec
				v.setValue(res);

				valuestack.push((current.subtype() == PRE) ? res : val);
			}
		}
		else if (current.type() == KEYWORD) {
			if (current.value() == "return") {
				// function stack shouldn't be empty.
				// set the return for the top of the stack, and break the evaluation.
				if (!this->functionStack.empty()) {
					Token ret;
					valuestack.pop(ret);
					this->functionStack.top().setReturn(ret);
					break;
				}
			}
			else if (current.value() == "if") {
				Token hashtok;
				source.pop(hashtok);
				HashedData hd = this->parser->getHashedData(hashtok.value());
				HashedData::csIf ifSet = hd.getIf();

				Token val = evaluateRPN(ifSet.ifCondition, scope);
				if (val.subtype() == VARIABLE) val = this->getVariable(val.value(), scope, true).value();
				val = Operations::typecastToken(val, BOOLEAN);

				if (val.value() == "true") {
					scope.stackVariables(ifSet.ifVariables);
					evaluateRPN(ifSet.ifStatements, scope);
					scope.popVariables();
				}
				else {
					scope.stackVariables(ifSet.elseVariables);
					evaluateRPN(ifSet.elseStatements, scope);
					scope.popVariables();
				}
			}
			else if (current.value() == "while" || current.value() == "until") {
				Token hashtok;
				source.pop(hashtok);
				HashedData hd = this->parser->getHashedData(hashtok.value());
				HashedData::csFor whileSet = hd.getFor();

				Token val = evaluateRPN(whileSet.forCondition, scope);
				if (val.subtype() == VARIABLE) val = this->getVariable(val.value(), scope, true).value();
				val = Operations::typecastToken(val, BOOLEAN);

				String canContinue = (current.value() == "while") ? "true" : "false";

				while (val.value() == canContinue) {
					scope.stackVariables(whileSet.forVariables);
					evaluateRPN(whileSet.forStatements, scope);
					scope.popVariables();
					val = evaluateRPN(whileSet.forCondition, scope);
					if (val.subtype() == VARIABLE) val = this->getVariable(val.value(), scope, true).value();
					val = Operations::typecastToken(val, BOOLEAN);
				}
			}
			else if (current.value() == "for") {
				Token hashtok;
				source.pop(hashtok);
				HashedData hd = this->parser->getHashedData(hashtok.value());
				HashedData::csFor forSet = hd.getFor();

				scope.stackVariables(forSet.counterVariables);
				this->evaluateRPN(forSet.forInitialization, scope);

				Token val = this->evaluateRPN(forSet.forCondition, scope);
				if (val.subtype() == VARIABLE) val = this->getVariable(val.value(), scope, true).value();
				val = Operations::typecastToken(val, BOOLEAN);

				while (val.value() == "true") {
					scope.stackVariables(forSet.forVariables);
					this->evaluateRPN(forSet.forStatements, scope);
					scope.popVariables();
					this->evaluateRPN(forSet.forUpdate, scope);
					val = this->evaluateRPN(forSet.forCondition, scope);
					if (val.subtype() == VARIABLE) val = this->getVariable(val.value(), scope, true).value();
					val = Operations::typecastToken(val, BOOLEAN);
				}
				scope.popVariables();
			}
		}
		else if (current.type() == IDENTIFIER) {
			if (current.subtype() == FUNCTION) {
				Token argtok, inv, tmp;
				source.pop(argtok); source.pop(inv);

				long numOfArgs = argtok.value().substr(6).toInteger(); // argtok.value(): @args|<int>
				Vector<Token> args;
				while (numOfArgs--) {
					if (valuestack.pop(tmp)) args.pushfront(tmp);
				}

				if (inv.value() == ".") { // dereference the method.
					Token vartok;
					valuestack.pop(vartok);
					Token val = this->getVariableValue(vartok, scope, true);

					if (val.type() == LITERAL) {
						if (val.subtype() == STRING) {
							// possible methods: length, substr
							String str = Lexer::tokenToString(val);
							if (current.value() == "length") {
								valuestack.push(Lexer::toToken(integerToString(str.length())));
							}
						}
					}
					else { // for a variable:
						Variable& v = this->getVariable(vartok.value(), scope, true);

						if (v.type() == ARRAY) {
							if (current.value() == "push" || current.value() == "unshift") {
								Variable pushvar; Token arg;
								while (!args.empty()) {
									args.popfront(arg);
									if (arg.subtype() == VARIABLE) pushvar.setValue(this->getVariable(arg.value(), scope, true));
									else pushvar.setValue(Variable(arg));

									v.pushValue(pushvar, (current.value() == "unshift"));
								}
								valuestack.push(Lexer::toToken(integerToString(v.length())));
							}
							else if (current.value() == "pop" || current.value() == "shift") {
								Variable popvar;
								v.popValue(popvar, (current.value() == "shift"));
								String pophash = this->cacheVariable(popvar);
								valuestack.push(Token(pophash, DIRECTIVE, VARIABLE));
							}
							else if (current.value() == "length") {
								valuestack.push(Lexer::toToken(integerToString(v.length())));
							}
							else {

							}
						}
					}
				}
				else { // global function:
					if (InbuiltFunctionList.indexOf(current.value()) >= 0) {
						Token res;
						if (current.value() == "print" || current.value() == "printLine") {
							for (__SIZETYPE i = 0; i < args.size(); i++) {
								if (args[i].type() == LITERAL) InbuiltFunctions::write(args[i]);
								else if (args[i].subtype() == VARIABLE) {
									this->getVariable(args[i].value(), scope, true).printValues(cout);
								}
							}
							if (current.value() == "printLine") InbuiltFunctions::write(Lexer::toToken("\"\\n\""));
							res = trueToken;
						}
						else if (current.value().substr(0, 4) == "read") { // readInteger, readString, readLine
							if (current.value() == "readLine") {
								res = InbuiltFunctions::readLine();
							}
							else if (current.value() == "readString") {
								res = InbuiltFunctions::read(STRING);
							}
							else if (current.value() == "readNumber") {
								res = InbuiltFunctions::read(NUMBER);
							}
						}
						// all primary class constructors
						else if (current.value() == "Array") {
							String hash = this->cacheVariable();
							Variable& v = this->getVariable(hash, scope, true);
							v.setType(ARRAY);

							Token arrSize; args.popfront(arrSize);
							if (arrSize.subtype() == VARIABLE) arrSize = this->getVariable(arrSize.value(), scope, true).value();
							long asize = arrSize.value().toInteger();

							for (__SIZETYPE i = 0; i < asize; i++)
								v.setValueAt(Lexer::toToken(integerToString(i)), nullvalVariable);
							res = Token(hash, DIRECTIVE, VARIABLE);
						}
						else if (current.value() == "Object") {
							String hash = this->cacheVariable();
							Variable& v = this->getVariable(hash, scope, true);
							v.setType(OBJECT);
							res = Token(hash, DIRECTIVE, VARIABLE);
						}
						else if (current.value() == "String" || current.value() == "Number" || current.value() == "Boolean") {
							tokenType ty = UNKNOWN;
							if (current.value() == "String") ty = STRING;
							else if (current.value() == "Number") ty = NUMBER;
							else if (current.value() == "Boolean") ty = BOOLEAN;
							Token val;
							if (!args.empty()) {
								val = args[0];
								if (val.subtype() == VARIABLE) {
									val = this->getVariable(val.value(), scope, true).value();
								}
								if (val.type() == LITERAL) {
									res = Operations::typecastToken(val, ty);
								}
								else {
									res = nullvalToken;
								}
							}
						}
						valuestack.push(res);
					}
					else {
						Function func = this->parser->getFunction(current.value());
						if (!!func.id()) {
							Vector<Variable> argVars;
							for (__SIZETYPE i = 0; i < args.size(); i++) {
								if (args[i].type() == LITERAL) argVars.pushback(Variable(args[i]));
								else if (args[i].subtype() == VARIABLE) argVars.pushback(this->getVariable(args[i].value(), scope, true));
							}
							this->functionStack.push(func);
							tmp = this->functionStack.top().evaluate(argVars, *this);
							this->functionStack.pop();
						} else {
							this->sendError("rf1", current.value()); // unable to find function.
						}
						valuestack.push(tmp);
					}
				}
			}
			else if (current.subtype() == VARIABLE) valuestack.push(current);
		}
	}

	Token ret;
	if (!valuestack.pop(ret) && statementValues) {
		statementValues->popback(ret);
	}
	while (valuestack.pop(current) && statementValues) statementValues->pushback(current);
	return ret;
}

#endif // COMPONENT_EVALUATOR_H