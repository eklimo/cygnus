# Demo: Type check errors

## Example 1

### Input program

```
func foo() -> Bool
{
    return 5 / 2
}
```

### Compiler output

```
Error: body return type 'Int' does not match signature return type 'Bool'
 test/lang/example.cy:1:6
  |
1 | func foo() -> Bool
  |      ^^^ body return type 'Int' does not match signature return type 'Bool'
2 | {
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: '5 / 2' is not a boolean.

## Example 2

### Input program

```
var x = 6 * "abc"
```

### Compiler output

```
Error: mismatched types 'Int' and 'String' for operator '*'
 test/lang/example.cy:1:9
  |
1 | var x = 6 * "abc"
  |         ^^^^^^^^^ mismatched types 'Int' and 'String' for operator '*'
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: '6' and 'abc' are invalid operands for the multiplication operator.
