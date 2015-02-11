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
---
The following output functions are supported: `print, printLine`

usage: `print value1[, value2[, value3,...]]`  
`printLine` works just like print, but prints an additional newline at the end.

Example:
```
printLine "Hello World!"
print "This is a number: ", 42
```
![i1](http://i.imgur.com/ZvfsoV0.png)

#### 3.2) Input Functions
---
The following input functions are supported: `readNumber, readString, readLine`  
Usage:   
`var x = readNumber("Enter a number: ")` 

