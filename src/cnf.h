/* ************************************************************************************************
 * 
 *  - FILE cnf.h
 *      - This header file, and its matching .c file exist to implement the major functions
 *        that are used to convert a first-order logic expression into conjunctive normal form.
 *        
 *      - Every function expects to take either one or two Expressions as inputs
 *      - Every function returns an expression, which has been modified in some way.
 *      - Every function is recursive in nature, and is 100% memory efficient
 *          - Memory is freed whenever possible
 *          - The Expression structure stores pointers wherever possible, thus reducing the 
 *            amount of memory allocated.
 * 
 *      - An expression has three major forms:
 *          - BOPEXPRESSION (Binary Operator Expression)
 *              - Has a left and a right expression (will never be NULL)
 *              - Expression type is BOPEXPRESSION
 *              - Operator may be one of: {AND, OR, IMPLIES, IFF}
 *              - Term is undefined
 * 
 *          - UOPEXPRESSION (Unary Operator Expression)  
 *              - Has a right expression (will never be NULL) and no left expression (always NULL)
 *              - Expression type is UOPEXPRESSION
 *              - Operator may be one of: {NOT}
 *              - Term is undefined
 * 
 *          - TERMINAL
 *              - Has neither a left or right expression (always NULL)
 *              - Expression type is NONEEXP
 *              - Operator is NONEOP
 *              - Term is a terminal/atomic character/string
 * 
 *      - This file defines 5 functions:
 *          - iffElim: converts expressions with IFFs to an expression without IFFs
 *          - impElim: converts expressions with IMPLIES to an expression without IMPLIESs
 *          - nnf: converts expression to negation normal form (assuming it has no implies)
 *          - cnf: converts nnf expression to cnf
 *          - dist: distributes one expression onto another
 *  
 * ***********************************************************************************************/

#ifndef CNF_H
#define CNF_H

#include "grammar.h"

Expression *iffElim(Expression *expression);
Expression *impElim(Expression *expression);
Expression *nnf(Expression *expression);
Expression *dist(Expression *e1, Expression *e2);
Expression *cnf(Expression *expression);

#endif