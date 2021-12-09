# Demo: Error formatting

## Example 1

### Input program

```
     var x = 6


 *      "abc"
```

### Compiler output

```
Error: mismatched types 'Int' and 'String' for operator '*'
 test/lang/example.cy:1:14
  |
1 |         var x = 6
  |  _______________^
2 | |
3 | |
4 | |   *      "abc"
  | |______________^ mismatched types 'Int' and 'String' for operator '*'
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: the offending expression is followed across multiple lines.

## Example 2

### Input program

```
func foo(a: Int, b: Int) {}

foo(1, true
or
not
false
or
false
and
not
true
and
false
)
```

### Compiler output

```
Error: mismatched argument types for 'foo': expected 'Int', found 'Bool'
 test/lang/example.cy:3:8
    |
  2 |
  3 |     foo(1, true
    |  __________^
  4 | |   or
  5 | |   not
  6 | |   false
... | |
 12 | |   and
 13 | |   false
    | |_______^ mismatched argument types for 'foo': expected 'Int', found 'Bool'
 14 |     )
    |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: output is truncated if it's too long - the lines in the middle are skipped and replaced by '...'.
