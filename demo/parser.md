# Demo: Parser errors

## Example 1

### Input program

```
var x = (1 + 2 return
```

### Compiler output

```
Error: expected ')', found keyword 'return'
 test/lang/example.cy:1:16
  |
1 | var x = (1 + 2 return
  |                ^^^^^^ expected ')', found keyword 'return'
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: 'return' is not valid in an arithmetic expression.

## Example 2

### Input program

```
func sum(first: Int second: Int) -> Int
{
    return first + second
}
```

### Compiler output

```
Error: expected ')' or ',', found 'second'
 test/lang/example.cy:1:21
  |
1 | func sum(first: Int second: Int) -> Int
  |                     ^^^^^^ expected ')' or ',', found 'second'
2 | {
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: a comma is missing between the parameters.
