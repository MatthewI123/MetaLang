# MetaLang


## Introduction

**MetaLang** is a scripting language that is parsed and interpreted entirely at compile-time using C++'s template metaprogramming features.

The **MetaLang** parser expects a `type_list` of `token`s and generates a parse tree from the top-down (via recursive descent).

It is inconvinient to manually create the list of tokens, so a program will tokenize a **MetaLang** source file and genereate files consisting of tokens and lexemes.


## Building

1. Edit [source.metalang](./tests/source.metalang).
2. Execute `$ make`
3. Execute `$ tests/tests.out`


## To-do

1. Complete the parser
2. Extend the language.
3. Create the interpreter.
4. Write more documentation.
5. Write more tests.
