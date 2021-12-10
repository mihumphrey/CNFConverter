#include "grammar.h"
/* ************************************************************************************************
 * 
 * - FUNCTION getType
 *      - Parameters: 
 *          - char * -> expression in string form to find the type of
 *      - Returns: 
 *          - ExpressionType -> Type of the string expression
 *      - Methodology:
 *          - Checks to see if (U/B)OPEXPRESSION or TERMINAL
 *              - Any string starting with "(" is a BOPEXPRESSION
 *              - Any string not starting with "(" and whose first word is "NOT" is a UOPEXPRESSION
 *              - Any other string is interpreted as a terminal
 * 
 *              - ((A) AND (B)) -> BOPEXPRESSION
 *                 ^
 *              - (NOT (A)) -> UOPEXPRESSION
 *                 ^^^
 *              - (A) -> TERMINAL
 * 
 * ***********************************************************************************************/
ExpressionType getType(char *element) {
    char temp[MAX_FILE_LEN];
    strcpy(temp, element);
    if (element[1] == OPEN)
        return BOPEXPRESSION;

    char *token;
    token = strtok(temp, " ");
    int count = 0;
    while (element[count] == OPEN)
        count++;
    if (getOperator(&token[count]) == NOT)
        return UOPEXPRESSION;

    token = strtok(NULL, " ");
    if (token == NULL)
        return TERMINAL;

    return NONEEXP;
}

/* ************************************************************************************************
 * 
 * - FUNCTION getOperator
 *      - Parameters: 
 *          - char * -> operator in string form
 *      - Returns: 
 *          - Operator -> operator in enum form
 *      - Methodology:
 *          - Checks to see if string matches any versions of each operator
 *              - "AND" -> AND (enum)
 *              - "∧"   -> AND (enum)
 *              - ...
 * 
 * ***********************************************************************************************/
Operator getOperator(char *element) {
    for (int i = 0; i < NUM_ANDS; i++) {
        if(strcmp(element, ANDS[i]) == 0)
            return AND;
    }
    for (int i = 0; i < NUM_ORS; i++) {
        if(strcmp(element, ORS[i]) == 0)
            return OR;
    }
    for (int i = 0; i < NUM_IMPLICATIONS; i++) {
        if(strcmp(element, IMPLICATIONS[i]) == 0)
            return IMPLIES;
    }
    for (int i = 0; i < NUM_IFFS; i++) {
        if(strcmp(element, IFFS[i]) == 0)
            return IFF;
    }
    for (int i = 0; i < NUM_NOTS; i++) {
        if(strcmp(element, NOTS[i]) == 0)
            return NOT;
    }
    return NONEOP;

}

/* ************************************************************************************************
 * 
 * - FUNCTION printOpFromEnum
 *      - Parameters: 
 *          - Operator -> operator in enum form
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Prints operator to stdout in typical form
 * 
 * ***********************************************************************************************/
void printOpFromEnum(Operator op) {
    switch (op) {
        case AND:
            printf(" ∧ ");
            break;
        case OR: 
            printf(" ∨ ");
            break;
        case IMPLIES:
            printf(" ⇒ ");
            break;
        case IFF:
            printf(" ⇔  ");
            break;
        case NOT:
            printf("¬");
            break;
        default:
            printf(" NOP ");
    }
}

/* ************************************************************************************************
 * 
 * - FUNCTION printExpression
 *      - Parameters: 
 *          - Expression * -> the expression to print
 *          - bool -> whether to put parens around AND expressions
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Prints expression in human readable form
 *          - Removes parens around terms and uops by default
 *          - Has option to remove parens from AND bops
 * 
 * ***********************************************************************************************/
void printExpression(Expression *e, bool cnf) {
    if (e->expressionType == TERMINAL) {
        for (int i = 0; i < strlen(e->term); i++) {
            if (e->term[i] != OPEN && e->term[i] != CLOSE) {
                printf("%c", e->term[i]);
            }
        } 
    } else if (e->expressionType == BOPEXPRESSION) {
        if (!cnf)
            printf("("); 
        printExpression(e->left, cnf);
        printOpFromEnum(e->operator);
        printExpression(e->right, cnf);
        if (!cnf)
            printf(")");
    } else if (e->expressionType == UOPEXPRESSION) {
        printOpFromEnum(e->operator);
        printExpression(e->right, cnf);
    }
}

/* ************************************************************************************************
 * 
 * - FUNCTION initExpression
 *      - Parameters: 
 *          - ExpressionType -> Type of expression to create
 *          - Operator -> Operator to give to the expression
 *      - Returns: 
 *          - Expression * -> address of new Expression
 *      - Methodology:
 *          - Mallocs space on HEAP
 *          - sets left and right to NULL
 *          - sets operator and type given parameters
 *          - does not zero out term
 * 
 * ***********************************************************************************************/
Expression *initExpression(ExpressionType type, Operator operator) {
    Expression *e = malloc(sizeof(Expression));
    e->expressionType = type;
    e->operator = operator;
    e->left = NULL;
    e->right = NULL;
    
    return e;
}

/* ************************************************************************************************
 * 
 * - FUNCTION freeExpression
 *      - Parameters: 
 *          - Expression * -> expression to free
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Recursively frees memory allocated to ENTIRE expression tree
 * 
 * ***********************************************************************************************/
void freeExpression(Expression *e) {
    if (e->expressionType == UOPEXPRESSION) {
            freeExpression(e->right);
            e->right = NULL;

    } else if (e->expressionType == BOPEXPRESSION) {
            freeExpression(e->left);
            freeExpression(e->right);
            e->left = NULL;
            e->right = NULL;
    }
    free(e);
    e = NULL;
}

/* ************************************************************************************************
 * 
 * - FUNCTION copyExpression
 *      - Parameters: 
 *          - Expression * -> expression to copy from
 *      - Returns: 
 *          - Expression * -> expression to copy into
 *      - Methodology:
 *          - Recursively copies expression from src to dest
 * 
 * ***********************************************************************************************/
Expression *copyExpression(Expression *src) {
    Expression *dest = initExpression(src->expressionType, src->operator);

    if (src->left != NULL) {
        dest->left = copyExpression(src->left);
    } else {
        src->left = NULL;
    }
    if (src->right != NULL) {
        dest->right = copyExpression(src->right);
    } else {
        src->right = NULL;
    }
    if (src->expressionType == TERMINAL) {
        memcpy(dest->term, src->term, TERM_LEN);
    }
    return dest;
}

/* ************************************************************************************************
 * 
 * - FUNCTION CNFLen
 *      - Parameters: 
 *          - CNF * -> linked list to compute length of
 *      - Returns: 
 *          - int -> number of expressions in CNF form
 *      - Methodology:
 *          - Iterates thru linked list, counting each time
 * 
 * ***********************************************************************************************/
int CNFLen(CNF *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

/* ************************************************************************************************
 * 
 * - FUNCTION makeCNF
 *      - Parameters: 
 *          - Expression * -> expression to convert to linked list
 *          - CNF ** -> head of linked list
 *      - Returns: 
 *          - CNF * -> linked list version of cnf expression (flattened)
 *      - Methodology:
 *          - Recursively iterates on AND left, right, ORS, NOTS, others added straight
 * 
 * ***********************************************************************************************/
CNF *makeCNF(Expression *e, CNF **head) {
    CNF *a;
    switch(e->operator) {
        case AND:
            makeCNF(e->left, head);
            makeCNF(e->right, head);
            break;
        default:
            a = malloc(sizeof(CNF));
            a->data = e;
            a->next = *head;
            *head = a;
    }
    return *head;
}

/* ************************************************************************************************
 * 
 * - FUNCTION printCNF
 *      - Parameters: 
 *          - CNF * -> head of linked list
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Iterates thru linked list, printing each expression as it goes
 * 
 * ***********************************************************************************************/
void printCNF(CNF *c) {
    while (c != NULL) {
        printf("(");
        printExpression(c->data, true);
        printf(")");
        if (c->next != NULL)
            printf(" ∧ ");
        c = c->next;
    }
}

/* ************************************************************************************************
 * 
 * - FUNCTION freeCNF
 *      - Parameters: 
 *          - CNF * -> head of linked list
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Iterates thru linked list, freeing each expression as it goes
 * 
 * ***********************************************************************************************/
void freeCNF(CNF *head) {
    CNF *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

