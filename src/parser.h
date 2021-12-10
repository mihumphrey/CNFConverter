/* ************************************************************************************************
 * 
 *  - FILE parser.h
 *      - Parses through input file, and converts string to Expression *
 *          - Follows the grammar defined in grammar.h
 * 
 *      - Defines functions that will be initialized in the matching .c file
 *          - parse: parses input file (command line)
 *          - buildExpression: generates an expression given some char * expression (rec)
 *          - getExpr: finds the next expression in a char * (finds matching parens)
 *          - getOp: finds the next op in a char * (finds matching spaces)
 * 
 * ***********************************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include "grammar.h"

Expression *parse(char *filename);
Expression *buildExpression(char *inputString);
void getExpr(char *input, char *buff, int *start);
void getOp(char *input, char *buff, int *counter);

#endif