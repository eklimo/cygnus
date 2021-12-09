# Cygnus

_Cygnus_ is an in-progress compiler for a hypothetical programming language inspired by Swift and Rust. Although the eventual goal is compilation to machine code, only its front-end is currently complete, meaning that it is capable of fully analyzing input programs and discarding invalid inputs.

## Compiler features

- [Lexer](demo/lexer.md) - breaks up input program into series of tokens that have individual meaning
  - Flexible keyword/operator/separator specification including any ASCII characters
- [Parser](demo/parser.md) - constructs abstract syntax tree that represents the structure of the program
  - Top-down [Pratt](https://en.wikipedia.org/wiki/Operator-precedence_parser#Pratt_parsing)/recursive descent style
  - Able to assign arbitrary precedence to any operator in the language
- [Symbol resolution](demo/symbol.md) - determines which symbol in the program every identifier refers to
- [Type checker](demo/type.md) - verifies correct operations between types in the program
- Detailed and visual [error reporting](demo/error.md), in the style of the Rust compiler

## Language features

- Standard arithmetic and logical operators
- Statements and blocks
- Variable definitions
- Function definitions and calls
- `if` expressions
- `while` expressions

## Demo

For further information on what Cygnus is capable of, see the examples in the [`demo`](demo) directory, or download this project and try your own programs!

## Usage

After building, execute `cygnus` with the path to a `.cy` file, or add `--help` for more information on CLI options. Optionally, add `--debug` to see the compiler's debug output.

e.g.

```bash
$ cygnus "test/lang/fizzbuzz.cy" --debug
```

## Building

### Dependencies

- C++17-compatible compiler
- CMake 3.1+

### Linux

```bash
$ git clone https://github.com/eklimo/cygnus.git && cd cygnus
Cloning into 'cygnus'...

$ mkdir build && cd build

$ cmake .. && make
...
[100%] Built target cygnus
```

The executable `cygnus` will be located in `build/bin`.

## Technologies

- C++17
- Python
