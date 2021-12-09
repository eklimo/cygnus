# Demo: Lexer errors

## Example 1

### Input program

```
func &print(s: String) {} @
?
```

### Compiler output

```
Error: unexpected symbol '&'
 test/lang/example.cy:1:6
  |
1 | func &print(s: String) {} @
  |      ^ unexpected symbol '&'
2 | ?
  |

Error: unexpected symbol '@'
 test/lang/example.cy:1:27
  |
1 | func &print(s: String) {} @
  |                           ^ unexpected symbol '@'
2 | ?
  |

Error: unexpected symbol '?'
 test/lang/example.cy:2:1
  |
1 | func &print(s: String) {} @
2 | ?
  | ^ unexpected symbol '?'
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: the characters are not defined in the language.

## Example 2

### Input program

```
var x = "hello, world
var y = 2
```

### Compiler output

```
Error: unterminated string literal
 test/lang/example.cy:1:9
  |
1 | var x = "hello, world
  |         ^^^^^^^^^^^^^ unterminated string literal
2 | var y = 2
  |

Error: terminating compilation for file 'test/lang/example.cy'
```

Explanation: the string is missing a closing quotation mark.
