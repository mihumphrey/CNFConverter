#include "parser.h"

/* ************************************************************************************************
 * 
 * - FUNCTION parse
 *      - Parameters: 
 *          - char * -> filename to read from
 *      - Returns: 
 *          - Expression * -> Expression version of the contents of the filename
 *      - Methodology:
 *          - Opens + reads data from inputfile
 *          - Calls buildExpression on entire contents
 * 
 * ***********************************************************************************************/
Expression *parse(char *filename) {
    FILE *input = fopen(filename, "r");
    if (input == NULL) printf("NULL\n");
    char current;
    char inputString[MAX_FILE_LEN];
    int i = 0;

    while ((current = fgetc(input)) != EOF) {
        inputString[i++] = current;
    }

    inputString[i] = '\0';
    fclose(input);

    return buildExpression(inputString);
}

/* ************************************************************************************************
 * 
 * - FUNCTION buildExpression
 *      - Parameters: 
 *          - char * -> input (or substring) from inputfile 
 *      - Returns: 
 *          - Expression * -> Expression version of the contents of the inputString
 *      - Methodology:
 *          - Creates an expression
 *          - Determines if expression should be (B/U)OPEXPRESSION / TERMINAL (see grammar)
 *          - Recursively calls itself on each side of the top-level expression
 *              - "((A) AND (B))"
 *                    |------>                  ((BOPEXPRESSION) (AND))
 *                    |------>                 /                       \
 *                    |------>     ((TERM) (A))                         ((TERM) (B))
 * 
 * ***********************************************************************************************/
Expression *buildExpression(char *inputString) {
    Expression *expression = initExpression(NONEEXP, NONEOP);

    int counter = 0;
    char *left = calloc(1, MAX_LEN), *right = calloc(1, MAX_LEN), *op = calloc(1, MAX_OP_LEN);
    switch (getType(inputString)) {
        case TERMINAL: 
            expression->expressionType = TERMINAL;
            memcpy(expression->term, inputString, TERM_LEN);
            break;
                    
        case BOPEXPRESSION:
            getExpr(inputString, left, &counter);
            getOp(inputString, op, &counter);
            getExpr(inputString, right, &counter);

            expression->operator = getOperator(op);
            expression->left = buildExpression(left);
            expression->right = buildExpression(right);

            expression->expressionType = BOPEXPRESSION;
            break;
        case UOPEXPRESSION:
            getOp(inputString, op, &counter);
            getExpr(inputString, right, &counter);

            expression->expressionType = UOPEXPRESSION;
            expression->operator = getOperator(op);
            expression->right = buildExpression(right);
            break;
        default:
            printf("PANIC!!!!!");
            break;
    }   
    free(left);
    free(right);
    free(op);
    return expression;
}

/* ************************************************************************************************
 * 
 * - FUNCTION getExpr
 *      - Parameters: 
 *          - char * -> input string
 *          - char * -> buff to hold next expression
 *          - int * -> how many chars have been iterated thru
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Search input string for matching parens
 *              - Everything between matching parens is the expression we are looking for
 *              - ((A) AND (B)) -> (A)
 * 
 * ***********************************************************************************************/
void getExpr(char *input, char *buff, int *counter) {
    int buffCount = 0, numOpens = 0;
    for (int i = (*counter) + 1; i < strlen(input); i++) {
        (*counter)++;
        char current = input[i];
        buff[buffCount++] = current;
        if (current == OPEN)
            numOpens++;
        if (current == CLOSE)
            numOpens--;
        if (numOpens == 0) {
            (*counter)++;
            break;
        }
    }
    buff[buffCount] = '\0';
}

/* ************************************************************************************************
 * 
 * - FUNCTION getExpr
 *      - Parameters: 
 *          - char * -> input string
 *          - char * -> buff to hold next op
 *          - int * -> how many chars have been iterated thru
 *      - Returns: 
 *          - void
 *      - Methodology:
 *          - Search input string for matching spaces
 *              - Everything between matching spaces is the operator we are looking for
 *              - " AND (B)" -> "AND"
 * 
 * ***********************************************************************************************/
void getOp(char *input, char *buff, int *counter) {
    int buffCount = 0;
    for (int i = (*counter) + 1; i < strlen(input); i++) {
        char current = input[i];
        (*counter)++;
        if (current == ' ')
            break;
        buff[buffCount++] = current;
    }
    buff[buffCount] = '\0';
}