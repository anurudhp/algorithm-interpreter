#ifdef COMPONENT_PARSER_H

// included in "parser.cpp"

/*********************************
* Implementation: Class Variable
*********************************/

Variable::Variable() {}
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

String Variable::id() const {
	return this->_id;
}

tokenType Variable::type() {
	tokenType ty = this->_value.type();
	switch (ty) {
		case NUMBER: case BOOLEAN: case STRING:
		case HASHED: case ARRAY: case OBJECT:
		return ty;
	}
	if (!this->_keys.empty()) return OBJECT;
	if (!this->_values.empty()) return ARRAY;
	return UNKNOWN;
}

Token Variable::value() const { return this->_value; }

// Member/embedded value accessors
bool Variable::setValue(const Variable& v) {
	_value = v._value;
	_keys = v._keys;
	_values = v._values;
	_object = v._object;
	return true;
}

bool Variable::hasValueAt(Token key) {
	if (this->type() == ARRAY) {
		key = Operations::typecastToken(key, NUMBER);
		if (key.value() == "null" || !key.value().isInteger()) return false;
		
		__SIZETYPE index = key.value().toInteger();
		if (index < 0) index += this->_values.size();
		
		if (index < 0 || index >= this->_values.size()) return false;
		return true;
	}
	else if (this->type() == OBJECT) {
		key = Operations::typecastToken(key, STRING);
		if (key.value() == "null") return false;
		
		if (this->_keys.indexOf(key.value()) == -1) return false;
		return true;
	}
	return false;
}

Variable& Variable::valueAt(Token key) {
	if (!this->hasValueAt(key)) return nullVariableRef;
	
	__SIZETYPE index = 0;
	if (this->type() == ARRAY) {
		key = Operations::typecastToken(key, NUMBER);
		index = key.value().toInteger();
		if (index < 0) index += this->_values.size();
	}
	else if (this->type() == OBJECT) {
		key = Operations::typecastToken(key, STRING);		
		index = _keys.indexOf(key.value());
	}
	return this->_values[index];
}

bool Variable::setValueAt(Token key, Variable value) {
	if (this->type() == ARRAY) {
		key = Operations::typecastToken(key, NUMBER);
		if (key.value() == "null" || !key.value().isInteger()) return false;
		
		__SIZETYPE index = key.value().toInteger();
		if (index < 0) index += this->_values.size();
		
		if (index < 0 || index > this->_values.size()) return false;
		if (index == this->_values.size()) {
			this->_values.pushback(Variable());
		}
		this->_values[index].setValue(value);
		return true;
	}
	else if (this->type() == OBJECT) {
		key = Operations::typecastToken(key, STRING);
		if (key.value() == "null") return false;
		
		__SIZETYPE index = _keys.indexOf(key.value());
		if (index == -1) { // new key
			_keys.pushback(key.value());
			_values.pushback(Variable());
			_values[-1].setValue(value);
		}
		else { // update existing value
			_values[index].setValue(value);
		}
		return true;
	}
	return false;
}

Function Variable::getMethod(String funcId) {
	if (this->type() != OBJECT || this->_object == NULL) return Function();
	return this->_object->getPrototype(funcId);
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
VariableScope& VariableScope::operator= (const VariableScope& sc) {
	this->varstack = sc.varstack;
	return *this;
}

// adds an empty vector of variables to the top.
bool VariableScope::stackVariables() {
	this->varstack.pushback(Vector<Variable>());
	return true;
}
// adds the passed vector to the top.
bool VariableScope::stackVariables(Vector<Variable>& vars) {
	this->varstack.pushback(vars);
	return true;
}

// adds a variable to the top-most vector of the scope
bool VariableScope::addVariable(Variable& var) {
	if (this->varstack.size() == 0) this->stackVariables();
	this->varstack[-1].pushback(var);
	return true;
}
// deletes the topmost variable vector.
bool VariableScope::popVariables() {
	return this->varstack.popback();
}
// deletes the topmost variable vector, and assigns it to the reference
bool VariableScope::popVariables(Vector<Variable>& vars) {
	return this->varstack.popback(vars);
}

// checks whether a variable of identifier `id` exists in the scope.
bool VariableScope::exists(String id) {
	__SIZETYPE i, j, d = this->depth();
	for (i = d - 1; i >= 0; i--) {
		Vector<Variable>& vars = this->varstack[i];
		for (j = 0; j < vars.size(); j++) {
			if (vars[j].id() == id) return true;
		}
	}
	return false;
}
bool VariableScope::existsAtTop(String id) {
	Vector<Variable>& vars = this->varstack[-1];
	for (__SIZETYPE j = 0; j < vars.size(); j++) {
		if (vars[j].id() == id) return true;
	}
	return false;
}
// finds a variable of identifier `id` and returns it.
// returns a null variable, if not found.
Variable& VariableScope::resolve(String id) {
	__SIZETYPE i, j, d = this->depth();
	for (i = d - 1; i >= 0; i--) {
		Vector<Variable>& vars = this->varstack[i];
		for (j = 0; j < vars.size(); j++) {
			if (vars[j].id() == id) {
				return vars[j];
			}
		}
	}
	return nullVariableRef;
}

Vector<Variable>& VariableScope::getBaseVariables() {
	if (this->varstack.size() == 0) this->stackVariables();
	return this->varstack[0];
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
	this->parameters = params;
	this->statements = st;
}
Function::Function(const Function& f) {
	this->_id = f._id;
	this->parameters = f.parameters;
	this->statements = f.statements;
}

__SIZETYPE Function::paramsSize() const { return this->parameters.size(); }

String Function::id() const { return this->_id; }
RPN Function::getStatements() const { return this->statements; }

bool Function::setParams(Vector<String> p) {
	this->parameters = p;
	return true;
}
bool Function::setStatements(RPN st) {
	this->statements = st;
	return true;
}

bool validate() {
	return true;
}
Token Function::evaluate(Vector<Token> parameters, Evaluator& eval) {
	for (__SIZETYPE i = 0; i < functionVariables.size(); i++) {
		if (i > parameters.size()) parameters.pushback(nullvalToken);
		functionVariables[i].setValue(Variable(parameters[i]));
	}
	VariableScope scope;
	scope.stackVariables(eval.getGlobals());
	scope.stackVariables(functionVariables);

	Token ret = eval.evaluateRPN(this->statements, scope);
	
	// update the globals in the evaluator.
	eval.getGlobals() = scope.getBaseVariables();
	
	if (!ret.value()) ret = nullvalToken;
	return ret;
}
/*******************************
* End Implementation: Function 
********************************/

/*******************************
* Implementation: class Object 
********************************/

String Object::id() { return this->_id; }

Function Object::getConstructor() {
	return this->constructor;
}

bool Object::hasPrototype(String funcid) {
	for (__SIZETYPE i = 0; i < this->prototypes.size(); i++) {
		if (this->prototypes[i].id() == funcid) return true;
	}
	return false;
}
Function Object::getPrototype(String funcid) {
	for (__SIZETYPE i = 0; i < this->prototypes.size(); i++) {
		if (this->prototypes[i].id() == funcid) return this->prototypes[i];
	}
	return Function();
}

Variable Object::construct(Vector<Variable> initArgs) {
	Variable v;
	v._object = this;
	return v;
}

#endif /* COMPONENT_PARSER_H */
