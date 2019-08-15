# Grammar

```
# main
program = statement*
statement = declaration-statement
          | expression-statement

declaration-statement = function-definition
                      | variable-definition

expression-statement = expression


# declaration statements
function-definition = "func" IDENTIFIER "(" (parameter ("," parameter)*)? ")" ("->" type)? "{" statement* "}"

variable-definition = "var" IDENTIFIER type-annotation|("=" expression)


# expressions
expression = infix-operator-expression
           | prefix-operator-expression
           | postfix-operator-expression
           | group-expression
           | call-expression
           | return-expression
           | if-expression
           | while-expression
           | IDENTIFIER
           | literal

infix-operator-expression = expression INFIX-OPERATOR expression

prefix-operator-expression = PREFIX-OPERATOR expression

postfix-operator-expression = expression POSTFIX-OPERATOR

group-expression = "(" expression ")"

call-expression = IDENTIFIER "(" (expression ("," expression)*)? ")"

return-expression = "return" expression?

if-expression = "if" expression block ("else" block)?

while-expression = "while" expression block

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
