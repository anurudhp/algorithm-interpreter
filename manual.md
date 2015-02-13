The AlgoFlex CLI
===
The AlgoFlex interpreter comes along with a simple, easy to use command line interface, supporting a small set of commands.

![Image](http://i.imgur.com/fJJzj3J.png)

Commands supported: `help, about, run, clear, quit`. 
Type `help` to see the usage of the commands.
### Running a program
---
Say the code is in `test.alg`

![Image  run prog](http://i.imgur.com/bR1Die9.png)

### Compilation Errors
---
If there are any compilation errors, they will be displayed after parsing. The code will not be executed in such a case.

![image: prog errors](http://i.imgur.com/vpkfP0C.png)

> Runtime errors will be discussed later.

# The AlgoFlex language:
---
## Syntax
---
The syntax of AlgoFlex is simple, and easy to use. It roughly imitates c-style pseudocode. Blocks are indent based, and there is no need for statement terminators like semicolons. Variables are untyped.

The major components of the language are:

* comments
* Literals
* Variables
* Operators
* Control Structures
* Functions

### 0) Comments
---
The comment character is # , just like in other scripting languages.
AlgoFlex only supports single line comments.
### 1) Literals
---
Literals can be of three types: String, Number and Boolean.
String literals can either be enclosed in double quotes (`"hello"`) or single quotes (`'hello'`)<br/>
Number literals are decimals like `1234` or `12.34`
Boolean literals are `true` and `false` (which are reserved words)
### 2) Variables
---
Variables are declared using the `var` keyword.
ex: `var x = 100`
Variables can change type dynamically, hence can be re-assigned with values of different types.

- Variable initialisation: `var x = 1 + 2 + 3`
- Arrays: Primitive arrays must be enclosed in [ ], with elements separated by commas. All arrays are heterogeneous
- Objects: Have properties and methods, initialised using {} or Object()

```
### sample variable declarations: ###
var num = 100 # number
var str = "hello world!" # string
var b = true # boolean
var arr = [1, 2, 3, 4] # array
var arr2 = ['a', 'b', 10, false] # heterogenous array
var arr3 = Array(10) # empty array of size 10
var obj = {a: 1, b: 1, c: 1} # object primitive (no methods)
var obj2 = Object() # empty object, {} can also be used. (no methods)
```
2.2) Accessing array elements:
```
var arr = [1, 2, 3]
var a = arr[0]
```
2.3) Accessing object properties
```
var obj = {a: 1, b: 2, c: 3}
var c = obj.a # (ie. 1) alter: obj['a']
```
### 3) Inbuilt Functions
---
#### 3.1) Output functions

The following output functions are supported: `print, printLine`

usage: `print value1[, value2[, value3,...]]`  
`printLine` works just like print, but prints an additional newline at the end.

Example:
```
printLine "Hello World!"
print "This is a number: ", 42
```
![printnum](http://i.imgur.com/ZvfsoV0.png)

Example 2: Printing arrays and objects
```
var arr = [1, 2, 3, 4, 5]
var obj = {name: "Dijkstra", algorithm: "Shunting Yard"}
printLine "The array is ", arr
printLine "The object is ", obj
```
![img_op2](http://i.imgur.com/ThRB2yX.png)

#### 3.2) Input Functions

The following input functions are supported: `readNumber, readString, readLine`
Usage:   
```
var x = readNumber("Enter a number: ") # the arguments are prompts to the user.
var str = readString("Enter a name: ")
var line = readLine("Enter a sentence: ")

print x, '\n', str, '\n', line
```
![input1](http://i.imgur.com/lRUoagx.png)

#### 3.3) Fundamental Class Constructors

The pre-defined classes are `Number, Integer, String, Boolean, Array, Object`

Usage: `var x = class_name(args)`

example:
```
var str = "1234"
str = String(Number(str) + 10)
print str
```

![class1](http://i.imgur.com/4oueOz6.png)

- Array constructor:  
	- Declare an array of given size: `Array(size)`
	- Declare an empty array: `Array()` 
- Object Constructor:
	- Declare an empty object (without any members): `var o = Object()`

Example:
```
var arr = Array(5)
for i = 0 to 4
	arr[i] = i + 1
printLine "The array is ", arr

var obj = Object()
obj.name = "code"
obj.lang = "algoflex"
obj.lines = 10
printLine "the object is ", obj
```
![img arr_obj_cons](http://i.imgur.com/UiqnZq6.png)

#### 3.4) Array functions

The following array methods are supported: `length, push, pop, shift, unshift`

- `length`: Returns the length of the array
- `push`: Pushes elements (arguments) to the end of the array
- `pop`: Deletes the last element of the array and returns it
- `shift`: Deletes the first element of the array and returns it.
- `unshift`: Pushes elements (arguments) to the front

Example:
```
var arr = [1, 2, 3, 4]
printLine "length: ", arr.length()
printLine "initial: ", arr
printLine
arr.push(5, 6, 7) # pushes the arguments one after the other
printLine "after push: ", arr

printLine "Last element: ", arr.pop()
printLine "after pop: ", arr

printLine
arr.unshift(0)
printLine "after unshift: ", arr
printLine "first element: ", arr.shift()
printLine "after shift: ", arr

```
![arr_func](http://i.imgur.com/FR75m20.png)

### 4) Operators

Supported operators:
- Math operators: `+ - * / %`
- Assignment operators: `= += -= *= /= %= ++ --`
- Logical operators: `&& ||`
- Relational operators: `== === != !== > >= < <=`

Aliases:
- `and` for `&&`
- `or` for `||`
- `equals` for `==`

All operators follow standard operation rules.

Additional rules:
- addition of strings: concatenation.
- addition of any type with string: conversion to string, followed by concatenation
- Boolean addition: conversion to number, and arithmetic addition.

Example:
```
var x = readNumber("Enter the first number: ")
var y = readNumber("Enter the second number: ")
printLine "The sum is ", x + y
printLine

var str = "Hello " + "World " + 42 + "!"
printLine str
```
![oper](http://i.imgur.com/tgndn5Z.png)

### 5) Control Structures
---
#### 5.0) Blocks
A code block is a segment of code, of the same indent (tabs only), and without any line of different indent between them (except for empty lines)
```
var x = 1 #global block: 0 indent
if x == 1
	print "ok" # indent of 1 tab

	print "done!" # indent of one tab, belongs to the if block, as line before is empty.

var y = 0
	print "??" # compilation error, over indentation (does not belong to any construct)
```
#### 5.1) `if - else if - else` construct
Syntax:
```
if <condition>
	<statements>
else if <condition>
	<statements>
else
	<statements>
```
Example:
```
var x = readNumber("Enter a number: ")
if x != Integer(x)
	print "Not an integer"
else if x % 2 == 0
	print "Even"
else
	print "Odd"
```
![img_if1](http://i.imgur.com/7BMIFb5.png)
![img_if2](http://i.imgur.com/YRETqnD.png)
![img_if3](http://i.imgur.com/iqkC6WU.png)

#### 5.2) `while` and `until`
- While Loop: executes till condition becomes true
- Until Loop: executes till condition becomes false (ie. as long as condition is true)

Syntax
```
while <condition>
	<statements>

until <condition>
	<statements>
```
Example 1:
```
var x = readNumber("enter a number: ")

while x > 5
	x = readNumber("ok? ")

print "stopped"
```
![img while](http://i.imgur.com/08t94pJ.png)

Example 2:
```
var x = readNumber("enter a number: ")

until x equals 0
	x = readNumber("0 to quit: ")

print "thank you."
```
![img until](http://i.imgur.com/g3cNna9.png)

#### 5.3) `for` loops
- Used for number looping, with an iterator increasing/decreasing by 1 with each execution.

Syntax:
```python
# loop for increasing index
for <iterator> = <start> to <end>
	<statements>

for <iterator> = <start> downto <end>
	<statements>
```
Example 1:
```
for i = 1 to 10
	print i, ' '
```
![img for to](http://i.imgur.com/ftX91n8.png)

Example 2:
```
for i = 10 downto 1
	print i, ' '
```
![img for downto](http://i.imgur.com/0d1GRLi.png)

#### 5.4) Object iteration: `foreach`
- Array: iterates through the array, with iterator as index
- Object: iterates through, with iterator as key

Syntax:
```
foreach <index> in <array>
	<statements>
foreach <key> in <object>
	<statements>
```
Example 1:
```python
var arr = ['a', 'b', 'c', 'd', 'e']

foreach index in arr
	printLine index, ': ', arr[index]
```
![img for to](http://i.imgur.com/1PU3K0x.png)

Example 2:
```python
var obj = {file: 'test.alg', lang: 'algoflex', exec: 'run test.alg', after: 'clear and quit'}
foreach prop in obj
	printLine prop, ": ", obj[prop]
```
![img for downto](http://i.imgur.com/UfJEdz1.png)

### 6) Functions
---
Features:
- As the language is untyped, there is no need to specify return type.
- Excess arguments to function are ignored
- When arguments are insufficient, the rest are set to `null`
- No forward declaration
- Recursion is supported

Declaration syntax:
```
function <name>(<parameter1>, <parameter2>, ...)
	<statements>
```
Call syntax: `<name>(<argument1>, <argument2>, ...)`

Example:
```
function square(n)
	return n * n

var x = readNumber("Enter a number: ")
printLine "Square of ", x, " is ", square(x)
```
![img func](http://i.imgur.com/Xgxn9RR.png)

#### 7) Runtime Errors
Runtime errors will appear in case the user performs an invalid operation etc. The program will show the error, and abort further execution

Example:
```
var x = 10
var y = x.a # invalid
```
![img rte](http://i.imgur.com/58J84Nr.png)

### Example Program
Merge Sort
```javascript
var merge_count = 0
var sort_count = 0

#merges 2 sorted arrays
function merge(arr1, arr2)
	merge_count++
	var res = []
	while arr1.length() > 0 and arr2.length() > 0
		if arr1[0] < arr2[0]
			res.push(arr1.shift())
		else 
			res.push(arr2.shift())
	
	while arr1.length() > 0
		res.push(arr1.shift())
	while arr2.length() > 0
		res.push(arr2.shift())
	return res

# sorts the given array
function sort(arr)
	if typeof arr != 'array'
		return Array(0)
	if arr.length() <= 1
		return arr

	sort_count++

	var mid = Integer(arr.length() / 2)
	var tmp1 = []
	for i = 0 to mid - 1
		tmp1.push(arr[i])
	var tmp2 = []
	for i = mid to arr.length() - 1
		tmp2.push(arr[i])
	return merge(sort(tmp1), sort(tmp2))

function main()
	var N = readNumber("Enter the number of numbers: ")
	var arr = Array(N)
	print "Enter the numbers: "
	for i = 0 to N-1
		arr[i] = readNumber()
	
	var res = sort(arr)
	printLine "The sorted array is: ", res
	printLine "Number of merges: ", merge_count, "\nNumber of sorts: ", sort_count

# call the main process
main()
```
![img mergesort](http://i.imgur.com/cZVs8ok.png)
