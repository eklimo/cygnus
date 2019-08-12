# Grammar

```
# main
program = statement*
statement = variable-definition
          | variable-assignment
          | function-definition
          | return-statement
          | if-statement
          | expression-statement

# statements
variable-definition = "var" IDENTIFIER ("=" expression)|type-annotation
variable-assignment = IDENTIFIER "=" expression

function-definition = "func" IDENTIFIER "(" (parameter ("," parameter)*)? ")" ("->" type)? "{" statement* "}"
return-statement = "return" expression?

if-statement = "if" expression block ("else" block)?

expression-statement = expression

# expressions
expression = value POSTFIX-OPERATOR* (INFIX-OPERATOR expression)?
value = "(" expression ")"
      | PREFIX-OPERATOR+ expression
      | IDENTIFIER ("(" (expression ("," expression)*)? ")")?
      | literal
literal = NUMBER
        | STRING
        | BOOLEAN

# general
block = "{" statement* "}"
      | statement
parameter = IDENTIFIER type-annotation
type-annotation = ":" type
type = IDENTIFIER
```
