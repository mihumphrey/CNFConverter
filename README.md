# CNF Converter
This program takes a string first-order logic proposition, and converts it to Conjunctive Normal Form (CNF). It is written entirely in C as a course project for CSCI598: Program Verification and Synthesis.
<hr><hr>

<pre>
Author:       Michael Humphrey
Professor:    Dr. Jedidiah McClurg
Course:       CSCI598
Date:         Fall 2021</pre>
<hr><hr>

## Contents
- [CNF Converter](#cnf-converter)
  - [Contents](#contents)
  - [Overview](#overview)
  - [How to Use](#how-to-use)
    - [Compilation](#compilation)
    - [Running](#running)
  - [Discussion](#discussion)
<hr><hr>

## Overview
This program converts a string representation of a first-order logic expression into Conjunctive Normal Form. This program is written entirely in C, with 100% memory efficiency and 0 memory leaks. 

The program expects one command line input `filename`, which is the path to an input file that will be read as the input string.

The string version of the expression contained in `filename` must be fully parenthesized in order to be parsed correctly. For example:

```bash
'((A) AND (B))'       # VALID expression
'((A) AND (NOT (B)))' # VALID expression

'(A AND B)'   # INVALID expression
'((A) AND B)' # INVALID expression
'(A) AND (B)' # INVALID expression
```

There are three possible verbosity levels for this program:

- 0 --> no output (including expression in cnf form)
- 1 --> no debug or performance output
- 2 --> performance output, but no debugging
- 3 --> performance output, and debugging (WARNING: may output **millions** of lines)
<hr><hr>

## How to Use
### Compilation
This program may be compiled simply by running `make` on a unix machine, while in the base top-level-directory. This compiles the C code using gcc. Additionally, to clear any binaries, `make clean` may be run. 
<hr><hr>

### Running

This `make` command creates a binary file `./cnf` in the same base directory.

This binary file expects one command-line input: `filename`.
This filename must contain only a fully-parenthesized first-order logic expression.

One additional command-line input may be used: `verbosity`. This must be the second passed argument.

The following verbosity levels are accepted:
- 0 --> no output (including expression in cnf form)
- 1 --> no debug or performance output
- 2 --> performance output, but no debugging
- 3 --> performance output, and debugging (WARNING: may output **millions** of lines)

Example runs may be seen below:
```bash
make
./cnf input/1   # runs cnf on 'input/1' with default verbosity level: 1
./cnf input/1 0 # runs cnf on 'input/1' with verbosity level 0
./cnf input/2 1 # runs cnf on 'input/2' with verbosity level 1
./cnf input/3 2 # runs cnf on 'input/3' with verbosity level 2
./cnf input/4 3 # runs cnf on 'input/4' with verbosity level 3
./cnf input/5 4 # runs cnf on 'input/5' with verbosity level 3
```
<hr><hr>

## Discussion
Extra care was taken to improve the runtime of this application. As such, the code is as simple as possible, while maintaining a high enough level construct to stay efficient.

After developing the program, I tried parallelizing the algorithm. No matter how I attempted to parallelize it, the parallel version was *always* slower than the sequential version. I tried:

- 1 thread = 1 recursive call
- 1 thread = 1 recursive call up to some predefined depth

Neither of these succeeded in any runs to be faster than the sequential version.


Besides the performance side of this program, the memory management received special care as well. For very large inputs (> 100,000 expressions), this program may use hundreds of gigabytes of memory. As such, it is critical that this memory is managed correctly, without the possibility of a memory leak. Memory is freed as it is allocated, and there is precisely one free per malloc, each of which is handled in wrapper functions.
<hr><hr>

