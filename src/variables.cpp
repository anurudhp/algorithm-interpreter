#ifdef COMPONENT_PARSER_H

// included in "parser.cpp"

/*********************************
* Implementation: Class Variable
*********************************/

Variable::Variable() {
	this->_type = UNKNOWN;
	this->_object = NULL;
}
Variable::Variable(String id) {
	this->_id = id;
	this->_type = UNKNOWN;
	this->_object = NULL;
}

Variable::Variable(Token val) {
	this->_type = UNKNOWN;
	if (val.type() == IDENTIFIER) {
		_id = val.value();
	}
	else if (val.type() == LITERAL) {
		this->_value = val;
		this->_type = val.subtype();
	}
	this->_object = NULL;
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
	return _type;
}
void Variable::setType(tokenType t) {
	this->_type = t;
	if (t == ARRAY || t == OBJECT) this->_value.setType(t);
}

Token Variable::value() const { return this->_value; }

__SIZETYPE Variable::length() const {
	if (_type == STRING) return (this->_value.value().length() - 2);
	if (_type == ARRAY || _type == OBJECT) return this->_values.size();
	return -1;
}

// Member/embedded value accessors
bool Variable::setValue(const Variable& v) {
	if (_id.charAt(0) == ';') {
		_value = nullvalToken;
		return false;
	}
	_type = v._type;
	_value = v._value;
	_keys = v._keys;
	_values = v._values;
	_object = v._object;
	return true;
}

bool Variable::hasValueAt(Token key) {
	if (this->type() == ARRAY) {
		key = Operations::typecastToken(key, NUMBER);
		if (key.value() == "null") return false;
		
		if (!key.value().isInteger()) return false;
		__SIZETYPE index = key.value().toInteger();
		if (index < 0) index += this->_values.size();
		
		if (index < 0 || index >= this->_values.size()) return false;
		return true;
	}
	else if (this->type() == OBJECT) {
		key = Operations::typecastToken(key, STRING);
		if (key.value() == "null") return false;
		
		if (this->_keys.indexOf(key.value()) == -1) {
			this->_keys.pushback(key.value());
			this->_values.pushback(Variable());
			this->_values[-1].setValue(nullvalToken);
		}
		return true;
	}
	return false;
}

Variable& Variable::valueAt(Token key) {
	if (!this->hasValueAt(key)) {
		return nullVariableRef;
	}

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

bool Variable::pushValue(Variable v, bool isFront) {
	if (this->_type != ARRAY) return false;
	if (isFront) this->_values.pushfront(v);
	else this->_values.pushback(v);
	return true;
}
bool Variable::popValue(Variable& v, bool isFront) {
	if (this->_type != ARRAY) return false;
	if (isFront) this->_values.popfront(v);
	else this->_values.popback(v);
	return true;
}
// methods for objects:
bool Variable::addPair(Token key, Variable val) {
	if (this->_type != OBJECT) return false;
	return this->setValueAt(key, val);
}
bool Variable::deletePair(Token key, Variable& ref) {
	if (this->_type != OBJECT) return false;
	for (__SIZETYPE i = 0; i < this->_keys.size(); i++) {
		if (key.value() == this->_keys[i]) {
			this->_keys.remove(i);
			ref = this->_values[i];
			this->_values.remove(i);
			return true;
		}
	}
	return false;
}
String Variable::getKey(__SIZETYPE index) {
	if (this->_type != OBJECT || index >= this->length() || index < 0) return "";
	return this->_keys[index];
}

Function Variable::getMethod(String funcId) {
	if (this->type() != OBJECT || this->_object == NULL) return Function();
	return this->_object->getPrototype(funcId);
}

Token Variable::printValues(ostream& out) {
	if (_type == STRING || _type == BOOLEAN || _type == NUMBER || _type == CONSTANT) {
		return InbuiltFunctions::write(_value);
	}
	
	Token res = trueToken, tmp;
	if (_type == ARRAY) {
		out << "[ ";
		bool first = true;
		for (__SIZETYPE i = 0; i < _values.size(); i++) {
			if (first) first = false;
			else out << ", ";
			tmp = _values[i].printValues(out);
			res = Operations::logical("&&", res, tmp);
		}
		out << " ]";
		return res;
	}
	if (_type == OBJECT) {
		out << "{ ";
		bool first = true;
		for (__SIZETYPE i = 0; i < _values.size(); i++) {
			if (first) first = false;
			else out << ", ";
			out << _keys[i] << ": ";
			tmp = _values[i].printValues(out);
			res = Operations::logical("&&", res, tmp);
		}
		out << " }";
		return res;
	}
	return falseToken;
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
	hasRet = false;
}

Function::Function(String id) {
	this->_id = id;
	hasRet = false;
}
Function::Function(String id, Vector<String> params, RPN st) {
	this->_id = id;
	this->parameters = params;
	this->statements = st;
	hasRet = false;
}
Function::Function(const Function& f) {
	this->_id = f._id;
	this->parameters = f.parameters;
	this->statements = f.statements;
	this->functionVariables = f.functionVariables;
	hasRet = false;
}

__SIZETYPE Function::paramsSize() const { return this->parameters.size(); }
String Function::id() const { return this->_id; }
RPN Function::getStatements() const { return this->statements; }

void Function::setParams(Vector<String> p) { this->parameters = p; }
void Function::setStatements(RPN st) { this->statements = st; }
void Function::setVariables(Vector<Variable> fv) { this->functionVariables = fv; }

bool Function::hasReturned() const { return this->hasRet; }
bool Function::setReturn(Token ret) {
	if (this->hasRet) return false;
	this->hasRet = true;
	this->returnVal = ret;
	return true;
}

Token Function::evaluate(Vector<Variable> args, Evaluator& eval) {
	Vector<Variable> func_args;
	Variable nullVar(nullvalToken);
	for (__SIZETYPE i = 0; i < this->parameters.size(); i++) {
		if (i > args.size()) args.pushback(nullVar);
		Variable arg(this->parameters[i]);
		arg.setValue(args[i]);
		func_args.pushback(arg);
	}

	VariableScope scope;
	scope.stackVariables(eval.getGlobals());
	scope.stackVariables(func_args);
	scope.stackVariables(functionVariables);

	eval.evaluateRPN(this->statements, scope);
	// update the globals in the evaluator.
	eval.getGlobals() = scope.getBaseVariables();

	Token ret = this->returnVal;
	if (!this->hasReturned()) ret = nullvalToken;
	this->hasRet = false;

	if (ret.subtype() == VARIABLE && scope.exists(ret.value())) {
		// local variable returned. cache it in the evaluator
		Variable& v = scope.resolve(ret.value());
		String hash = eval.cacheVariable(v);
		ret = Token(hash, DIRECTIVE, VARIABLE);
	}
	return ret;
}
/*******************************
* End Implementation: Function 
********************************/

/*******************************
* Implementation: class Object 
********************************/
Object::Object() { isFundamental = false; }

Object::Object(String name, bool f) {
	this->_id = name;
	this->isFundamental = f;
}

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
