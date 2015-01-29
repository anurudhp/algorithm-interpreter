#ifdef COMPONENT_PARSER_H

// included in "parser.cpp"

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

// Member/embedded value accessors

tokenType Variable::hasMember(Token ref) {
	if (ref.type() != LITERAL) return false;
	if (this->type() == ARRAY) {
		if (ref.subtype() == NUMBER && ref.value().isInteger()) {
			long index = ref.value().toInteger();
			if (index >= 0 && index <= this->_values.size()) return true;
			return false;
		}
		return false;
	}
	else if (this->type() == OBJECT) {
		if (this->_keys.indexOf(ref.value()) >= 0) return true;
		return false;
	}
	return false;
}

Variable& Variable::valueAt(Token) {
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

/*******************************
* End Implementation: Variable 
********************************/

/*********************************
* Implementation: Class Variable
*********************************/

VariableScope::VariableScope() {
	this->varstack.clear();
}
VariableScope::VariableScope(const VariableScope& sc) {
	this->varstack = sc.varstack;
}

// adds an empty vector of variables to the top.
bool VariableScope::stackVariables() {
	this->varstack.pushback(Vector<Variable>());
}
// adds the passed vector to the top.
bool VariableScope::stackVariables(Vector<Variable>& vars) {
	this->varstack.pushback(vars);
}

// adds a variable to the top-most vector of the scope
bool VariableScope::addVariable(Variable& var) {
	this->varstack[-1].pushback(var);
}
// deletes the topmost variable vector.
bool VariableScope::popVariables() {
	return this->varstack.popback();
}
// deletes the topmost variable vector, and assigns it to the reference
bool VariableScope::popVariables(Vector<Variable>& vars) {
	return this->varstack.popback(vars);
}

// finds a variable of identifier `id` and returns it.
// returns a null variable, if not found.
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

// checks whether a variable of identifier `id` exists in the scope.
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

// scope depth.
__SIZETYPE VariableScope::depth() const { return this->varstack.size(); }

/*******************************
* End Implementation: VariableScope 
********************************/

/*********************************
* Implementation: Class Function
*********************************/

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
/*******************************
* End Implementation: Function 
********************************/

#endif /* COMPONENT_PARSER_H */
