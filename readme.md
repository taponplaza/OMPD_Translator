# C COMPILER

## Description

This repository contains a C compiler for the C99 version. The compiler accepts C99 source files as input and performs the necessary lexical, syntactic, and semantic analysis to generate files useful for code development and debugging.

When compiling, three files are generated:
- `log.txt`: This file shows the traces of the analyzer, which can be useful for understanding the analysis process performed by the compiler and for identifying possible errors or anomalies.
- `sym_tables.txt`: Here, all symbol tables generated during the compilation process are shown. These tables are crucial for understanding how identifiers are resolved and the data structures used by the compiler.
- `error.txt` is the file where errors found during processing will be logged.

## Repository Branches
- **main**: This branch is used to upload changes during development. If errors or issues occur, they can be corrected in this branch before being integrated into the production branch.
- **release**: The release  branch contains code that is considered stable and ready for use in production environments. The code in this branch must have passed exhaustive tests and is expected to function correctly at all times.

## Running the Compiler
To run the compiler, follow these steps:
1. Open a terminal in the directory where the compiler is located.
2. Compile the source code using the compiler:
    ```
    ./fparse input.c [DEBUG]
    ```
    Where:
    - `input.c`  is the input file to be processed.
    - `DEBUG` is a flag option to start debuggin the insertions on the symbols table.
    

Thank you for using our C compiler!