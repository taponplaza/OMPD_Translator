# OMPD to MPI Translator

## Description

This repository contains the implementation of a tool designed to translate code written with OMPD (OpenMP Distributed) directives to MPI (Message Passing Interface). This project is part of a collaborative effort involving multiple final degree projects aimed at integrating parallel and distributed programming into a unified model based on the C language. The translator accepts C99 source files as input and performs the necessary lexical, syntactic, and semantic analysis to generatefiles useful for code development and debugging.

## Introduction

This project is one of four final degree projects proposed by professors from DATSI, with the objective of developing a tool that integrates parallel and distributed programming into a single model using the C language. The model uses extended OpenMP directives with additional ones related to MPI, forming what is known as OMPD (Distributed OpenMP).

## Features

- Symbol Table: Efficient storage and retrieval of variable, function, and type information.
- Lexer and Parser: Implemented using Flex and Bison based on the C99 and OpenMP grammars.
- Directive Translation: Translates OMPD data type definition directives to their MPI equivalents.
- Example Programs: Includes example programs demonstrating the translation and execution of OMPD to MPI.

## Installation

To install and set up the project locally, follow these steps:

*bash*
```
git clone https://github.com/taponplaza/Tabla_y_Tipos.git
cd OMPD-to-MPI-Translator
```

### Install Dependencies:
Ensure you have Flex, Bison, and a C compiler installed on your system. You can install them using your package manager. For example, on Ubuntu:

*bash*
```
sudo apt-get update
sudo apt-get install flex bison gcc
```

### Build the Project:

*bash*
```
make
```


## Usage
To use the translator, run the following command:
1. Open a terminal in the directory where the translator is located.
2. Compile the source code using the translator:
    ```
    ./fparse input.c [DEBUG] [output.c]
    ```
    Where:
    - `input.c`  is the input file to be processed.
    - `DEBUG` is a flag option to start debuggin the insertions on the symbols table.
    - `output.c` output file option.

When compiling, four files can be generated:
- `log.txt`: This file shows the traces of the analyzer, which can be useful for understanding the analysis process performed by the translator and for identifying possible errors or anomalies.
- `sym_tables.txt`: Here, all symbol tables generated during the compilation process are shown. These tables are crucial for understanding how identifiers are resolved and the data structures used by the translator.
- `error.txt` is the file where errors found during processing will be logged.
- `generated.txt` is the gerated translated code from the input file.
    
## Project Structure

- results/: Diferent results from the current examples.
- tests/: Test cases to verify the functionality of the translator.

## References

- OpenMP Documentation: https://www.openmp.org/specifications/
- MPI Documentation: https://www.mpi-forum.org/docs/
