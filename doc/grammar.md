# Grammar

```
# main
program = statement*
statement = declaration-statement
          | expression-statement

declaration-statement = variable-definition
                      | function-definition

expression-statement = expression


# declaration statements
variable-definition = "var" IDENTIFIER type-annotation|("=" expression)

function-definition = "func" IDENTIFIER "(" (parameter ("," parameter)*)? ")" ("->" type)? "{" statement* "}"


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

if-expression = "if" expression block ("else" block)?

while-expression = "while" expression block

literal = NUMBER
        | STRING
        | BOOLEAN


# general
block = "{" statement* "}"
parameter = IDENTIFIER type-annotation
type-annotation = ":" type
type = IDENTIFIER
```
