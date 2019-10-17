# Grammar

```
# main
program = statement-list
statement = declaration-statement
          | expression-statement
          | block

declaration-statement = variable-definition
                      | function-definition

expression-statement = expression


# declaration statements
variable-definition = "var" IDENTIFIER (type-annotation ("=" expression)?)|("=" expression)

function-definition = "func" IDENTIFIER "(" (parameter ("," parameter)*)? ")" ("->" type)? block


# expressions
expression = prefix-operator-expression
           | infix-operator-expression
           | postfix-operator-expression
           | group-expression
           | call-expression
           | return-expression
           | if-expression
           | while-expression
           | IDENTIFIER
           | literal

prefix-operator-expression = PREFIX-OPERATOR expression

infix-operator-expression = expression INFIX-OPERATOR expression

postfix-operator-expression = expression POSTFIX-OPERATOR

group-expression = "(" expression ")"

call-expression = IDENTIFIER "(" (expression ("," expression)*)? ")"

return-expression = "return" expression?

if-expression = "if" expression statement ("else" statement)?

while-expression = "while" expression statement

literal = NUMBER
        | STRING
        | boolean-literal
        | unit-literal
boolean-literal = "true" | "false"
unit-literal = "(" ")"


# general
statement-list = (statement ("\n"|";" statement)*)?
block = "{" statement-list "}"
parameter = IDENTIFIER type-annotation
type-annotation = ":" type
type = IDENTIFIER
     | "(" ")"
```
