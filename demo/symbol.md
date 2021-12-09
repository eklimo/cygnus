# Demo: Symbol resolution errors

## Example 1

### Input program

```
if 3 < 4
{
    var x = 123
}

var y = x
```

### Compiler output

```
Error: symbol 'x' is not defined
 test/lang/example.cy:6:9
  |
5 |
6 | var y = x
  |         ^ symbol 'x' is not defined
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: variable 'x' went out of scope.

## Example 2

### Input program

```
var a = 1
a = 2
var a = 3
```

### Compiler output

```
Error: symbol 'a' is already defined
 test/lang/example.cy:3:5
  |
2 | a = 2
3 | var a = 3
  |     ^ symbol 'a' is already defined
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: variables cannot be re-declared.
