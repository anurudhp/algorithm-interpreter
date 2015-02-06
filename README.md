A simple interpretive programming language.
============================
Paradigms:
---
- Simple and minimal. Mainly for writing algorithms.
- Indent-based block control. Statements terminated by newlines or ;
- Loosely typed.
- Object Oriented.

Testing:
---
`g++ src/interface.cpp -o src/exec`  
`./src/exec`  
Debug modes:- >2: print all messages, 1: print only log messages, 0: no debug messages. 

---
Basic Syntax
---
```python
# this is a comment
var x = 5
var y = x++ + 1 - 4
var str = 'hello' # or "hello"
var array = [] # or = Array()
var array2 = [1, 2, 3, 'a', 'b']
var obj = {} # or Object()
var obj2 = {a: 1, b: 2, c: 3}

if x equals 5
    print ("Yaay it works")
else
    var y = 0
    print y, '\n' # prints 0 : scope based variables

for i = 1 to 5
    print i, ' '
for i = 5 downto 1
    print i, ' '

while x < 10
    print x++, '\n'

function find(arr, elem)
    if typeof arr != 'array'
        return 0
    
    for i = 0 to arr.length() - 1
        if arr[i] == elem
            return i
    return -1

arr.push(1)
arr.push(2)
print find(arr, 5)

```
Note: The project is still under development, but does support the basic features described above. Feel free to report any bugs.

---
Credits
---
Staff:
- Codelegend 
- VThejas
