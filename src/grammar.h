/* ************************************************************************************************
 * 
 *  - FILE grammar.h
 *      - Defines a grammar + vocabulary that will be used in this project
 *      - Defines how each connective may be represented
 *      - Defines constants to limit the length of a proposition
 * 
 *      - Defines data structure to represent large expressions succinctly
 * 
 *      - Defines functions that will be initialized in the matching .c file
 *          - getOperator: gets operator (enum) given operator (char *)
 *          - printOpFromEnum: prints operator given operator(enum)
 *          - getType: determines the Expression Type of an expression (char *)
 *          - makeCNF: another data structure to represent cnf formula: linked list of expressions
 *          - printCNF: prints CNF linked list
 *          - printExpression: prints an expression in human-readable form (rec)
 *          - initExpression: allocates room on heap for a new expression, and returns the addr
 *          - copyExpression: copy constructor to copy one expression into another (new) one
 *          - freeExpression: frees an entire expression tree (rec)
 *          - freeCNF: frees the cnf linked list
 *          - CNFLen: computes the number of expressions in CNF form (used for benchmarking)
 * 
 * ***********************************************************************************************/

#ifndef GRAMMAR_H
#define GRAMMAR_H

/* **********************************************
 * 
 *                   INCLUDES                    
 * 
 * *********************************************/
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* **********************************************
 * 
 *            CONSTANT DEFINITIONS
 * 
 * *********************************************/
#define NUM_ANDS 3
#define NUM_ORS 3
#define NUM_NOTS 3
#define NUM_IMPLICATIONS 3
#define NUM_IFFS 3

#define OPEN '('
#define CLOSE ')'

#define MAX_LEN 10240
#define MAX_FILE_LEN 10240
#define TERM_LEN 8
#define MAX_OP_LEN 16
/* **********************************************
 * 
 *              MACRO DEFINITIONS
 * 
 * *********************************************/
#define PRINT(...) {printf(__VA_ARGS__);}

#define ASSERT(arg, err) \
              if (!(arg)) {\
                  fprintf(stderr,"\033[31mError, %s: \n\t", err);\
                  perror("");\
                  fprintf(stderr, "exiting...\n");\
                  printf("\033[0m");\
                  exit(1);\
              }

/* **********************************************
 * 
 *              ENUM DEFINITIONS
 * 
 * *********************************************/
typedef enum {
    TERMINAL, BOPEXPRESSION, UOPEXPRESSION, NONEEXP
} ExpressionType;

typedef enum {
    AND, OR, IMPLIES, IFF, NOT, NONEOP
} Operator;

/* **********************************************
 * 
 *              STRUCT DEFINITIONS
 * 
 * *********************************************/
typedef struct __Expression__ {
    struct __Expression__ *left;
    struct __Expression__ *right;
    char term[TERM_LEN];
    ExpressionType expressionType;
    Operator operator;
} Expression;

typedef struct __CNF__ {
    Expression *data;
    struct __CNF__ *next;
} CNF;

/* **********************************************
 * 
 *             OPERATOR DEFINITIONS
 * 
 * *********************************************/
static const char *ANDS[NUM_ANDS] = {"AND", "&", "∧"};
static const char *ORS[NUM_ORS] = {"OR", "|", "∨"};
static const char *IMPLICATIONS[NUM_IMPLICATIONS] = {"IMPLIES", "=>", "⇒"};
static const char *IFFS[NUM_IFFS] = {"IFF", "<=>", "⇔"};
static const char *NOTS[NUM_NOTS] = {"NOT", "!", "¬"};

/* **********************************************
 * 
 *              FUNCTION DEFINITIONS
 * 
 * *********************************************/
Operator getOperator(char *element);
void printOpFromEnum(Operator op);
ExpressionType getType(char *element);

CNF *makeCNF(Expression *e, CNF **head);
void printExpression(Expression *e, bool cnf);
void printCNF(CNF *c);

Expression *initExpression(ExpressionType type, Operator operator);
Expression *copyExpression(Expression *src);
void freeExpression(Expression *e);
void freeCNF(CNF *c);
int CNFLen(CNF *head);

#endif