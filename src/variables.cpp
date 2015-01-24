#ifdef COMPONENT_PARSER_H

/*********************************
* Implementation: Class Variable
*********************************/
Variable::Variable() {
	_id = "";
	_value = Token();
}
Variable::Variable(Token val) {
	_id = val.value();
	_value = val;
}

Variable::Variable(const Variable& v) {
	_id = v._id;
	this->setValue(v);
}
Variable& Variable::operator= (const Variable& v) {
	_id = v._id;
	this->setValue(v);
	return *this;
}

String Variable::id() const { return this->_id; }

tokenType Variable::type() {
	tokenType ty = this->_value.type();
	if (ty == NUMBER || ty == STRING || ty == BOOLEAN) return ty;
	if (!this->_keys.empty()) return OBJECT;
	if (!this->_values.empty()) return ARRAY;
	return UNKNOWN;
}

Token Variable::value() const { return this->_value; }

Variable& Variable::valueAt (Token) {
	return *this;
}

bool Variable::setValue(const Variable& v) {
	_value = v._value;
	_keys = v._keys;
	_values = v._values;
	return true;
}
bool Variable::setValueAt(Token, Variable) {
	return false;
}

/*** End variable ***/

/** Var Scope **/

VariableScope::VariableScope() {
	this->varstack.clear();
}
VariableScope::VariableScope(const VariableScope& sc) {
	this->varstack = sc.varstack;
}

bool VariableScope::stackVariables() {
	this->varstack.pushback(Vector<Variable>());
}
bool VariableScope::stackVariables(Vector<Variable>& vars) {
	this->varstack.pushback(vars);
}
bool VariableScope::addVariable(Variable& var) {
	this->varstack[-1].pushback(var);
}
bool VariableScope::popVariables() {
	return this->varstack.popback();
}
bool VariableScope::popVariables(Vector<Variable>& vars) {
	return this->varstack.popback(vars);
}

Variable VariableScope::resolve(String id) {
	__SIZETYPE i, j, d = this->depth();
	for (i = d - 1; i >= 0; i--) {
		Vector<Variable>& vars = this->varstack[i];
		for (j = 0; j < vars.size(); j++) {
			if (vars[j].id() == id) return vars[j];
		}
	}
	return Variable();
}
__SIZETYPE VariableScope::exists(String id) {
	__SIZETYPE i, j, d = this->depth();
	for (i = d - 1; i >= 0; i--) {
		Vector<Variable>& vars = this->varstack[i];
		for (j = 0; j < vars.size(); j++) {
			// cerr << ">> " << vars[j].id() << endl;
			if (vars[j].id() == id) return true;
		}
	}
	return false;
}
__SIZETYPE VariableScope::depth() const { return this->varstack.size(); }

// function

Function::Function() {
	this->_id = "";
}
Function::Function(String id) {
	this->_id = id;
}
Function::Function(String id, Vector<String> params, RPN st) {
	this->_id = id;
	this->params = params;
	this->statements = st;
}
Function::Function(const Function& f) {
	this->_id = f._id;
	this->params = f.params;
	this->statements = f.statements;
}

__SIZETYPE Function::paramsSize() const { return this->params.size(); }

String Function::id() const { return this->_id; }
RPN Function::getStatements() const { return this->statements; }

bool Function::setParams(Vector<String> p) { this->params = p; return true; }
bool Function::setStatements(RPN st) { this->statements = st; return true; }

bool validate() {
	return false;
}
Token Function::evaluate(Vector<Token>, Evaluator&) {
	return Token();
}

#endif /* COMPONENT_PARSER_H */