# MetaLang


## Introduction

**MetaLang** is a scripting language that is parsed and interpreted entirely at compile-time using C++'s template metaprogramming features.

The **MetaLang** [parser](./include/compiler.paraser.hpp) expects a [type_list](./include/type_list.hpp) of [tokens](./include/compiler/token.hpp) and generates a parse tree from the top-down (via recursive descent).

It is inconvinient to manually create the list of tokens, so a program will tokenize **MetaLang** [source file](./source.metalang) and genereate files consisting of tokens and lexemes.


## Building

1. Edit [source.metalang](./source.metalang).
2. Execute `$ make parser`
3. Execute `$ build/parser.out`

The generated tokens will be in `build/tokens.hpp`.


## To-do

1. Complete the [parser](./include/compiler/parser.hpp).
2. Extend the [language](./doc/grammar.ebnf).
3. Create the interpreter.
4. Write more documentation.
5. Write more tests.
