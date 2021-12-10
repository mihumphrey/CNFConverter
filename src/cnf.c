#include "cnf.h"

/* ************************************************************************************************
 * 
 * - FUNCTION iffElim
 *      - Parameters: 
 *          - Expression * -> Expression to eliminate double implication from
 *      - Returns: 
 *          - Expression * -> Expression with iffs converted to implies (see below)
 *      - Methodology:
 *          - Recursively checks BOPEXPRESSIONS for iff
 *              - ((A) IFF (B)) -> (((A) IMPLIES (B)) AND ((B) IMPLIES (A)))
 * 
 * ***********************************************************************************************/
Expression *iffElim(Expression *expression) {
    Expression *left;
    Expression *right;
    Expression *e;

    switch (expression->expressionType) {
        case TERMINAL:
            return expression;
        case UOPEXPRESSION:
            expression->right = iffElim(expression->right);
            return expression;
        case BOPEXPRESSION:
            switch (expression->operator) {
            case IFF:
                e = initExpression(BOPEXPRESSION, AND);

                left = initExpression(BOPEXPRESSION, IMPLIES);
                right = initExpression(BOPEXPRESSION, IMPLIES);

                left->left = copyExpression(expression->left);
                left->right = copyExpression(expression->right);

                right->left = copyExpression(expression->right);
                right->right = copyExpression(expression->left);

                freeExpression(expression);

                e->left = iffElim(left);
                e->right = iffElim(right);

                return e;
            
            default:
                expression->left = iffElim(expression->left);
                expression->right = iffElim(expression->right);

                return expression;
            }
        default:
            // This *should* be impossible to reach on a well-formed input, thus return null
            printExpression(expression, false);
            return NULL;
    }
    return expression;
}

/* ************************************************************************************************
 * 
 * - FUNCTION impElim
 *      - Parameters: 
 *          - Expression * -> Expression to eliminate implication from
 *      - Returns: 
 *          - Expression * -> Expression with implies converted to ors (see below)
 *      - Methodology:
 *          - Recursively checks BOPEXPRESSIONS for implies
 *              - ((A) IMPLIES (B)) -> ((NOT (A)) OR (B))
 * 
 * ***********************************************************************************************/
Expression *impElim(Expression *expression) {
    Expression *left;
    Expression *e;
    switch (expression->expressionType) {
        case TERMINAL:
            return expression;
        case UOPEXPRESSION:
            expression->right = impElim(expression->right);
            return expression;

        case BOPEXPRESSION:
            switch (expression->operator) {
            case IMPLIES:

                e = initExpression(BOPEXPRESSION, OR);
                left = initExpression(UOPEXPRESSION, NOT);

                e->right = copyExpression(expression->right);
                left->right = copyExpression(expression->left);

                e->left = left;

                e->left = impElim(e->left);
                e->right = impElim(e->right);
                
                freeExpression(expression);
                return e;
            
            default:

                expression->left = impElim(expression->left);
                expression->right = impElim(expression->right);

                return expression;
            }
        default:
            break;
    }
    return expression;
}

/* ************************************************************************************************
 * 
 * - FUNCTION nnf
 *      - Parameters: 
 *          - Expression * -> Expression to convert to nnf
 *      - Returns: 
 *          - Expression * -> Expression in nnf
 *      - Methodology:
 *          - Recursively converts expression (which contains only AND, OR, NOT) to nnf
 *              -   (NOT (NOT (A))) -> (A)
 *              -   (NOT ((A) AND (B))) -> ((NOT (A)) OR (NOT (B)))
 *              -   (NOT ((A) OR (B))) -> ((NOT (A)) AND (NOT (B)))
 * 
 * ***********************************************************************************************/
Expression *nnf(Expression *expression) {
    Expression *left, *right, *e;
    switch (expression->expressionType) {
        case TERMINAL:
            return expression;

        case BOPEXPRESSION:
            expression->left = nnf(expression->left);
            expression->right = nnf(expression->right);
            return expression;

        case UOPEXPRESSION:
            if (expression->right->operator == NOT) {
                e = copyExpression(expression->right->right);
                freeExpression(expression);
                return nnf(e);

            } else if (expression->right->operator == AND) {

                e = initExpression(BOPEXPRESSION, OR);
                left = initExpression(UOPEXPRESSION, NOT);
                right = initExpression(UOPEXPRESSION, NOT);

                left->right = copyExpression(expression->right->left);
                right->right = copyExpression(expression->right->right);

                freeExpression(expression);
                
                e->left = left;
                e->right = right;

                e->left = nnf(e->left);
                e->right = nnf(e->right);
                return e;

            } else if (expression->right->operator == OR) {
                e = initExpression(BOPEXPRESSION, AND);
                left = initExpression(UOPEXPRESSION, NOT);
                right = initExpression(UOPEXPRESSION, NOT);

                left->right = copyExpression(expression->right->left);
                right->right = copyExpression(expression->right->right);

                freeExpression(expression);

                e->left = left;
                e->right = right;

                e->left = nnf(e->left);
                e->right = nnf(e->right);
                return e;
            }
        case NONEEXP:
        default:
            break;
    }
    return expression;
}

/* ************************************************************************************************
 * 
 * - HELPER FUNCTION dist
 *      - Parameters: 
 *          - Expression * -> First expression to distribute
 *          - Expression * -> Second expression to distribute
 *      - Returns: 
 *          - Expression * -> Expression with e1 distributed into e2
 *      - Methodology:
 *          - Recursively distributes one expression to another in order to avoid nested ANDS
 *              -   ((A) OR ((B) AND (C))) -> (((A) OR (B)) AND ((A) OR (C)))
 * 
 * ***********************************************************************************************/
Expression *dist(Expression *e1, Expression *e2) {
    Expression *e, *left, *right, *temp;

    if (e1->operator == AND) {
        e = initExpression(BOPEXPRESSION, AND);

        e->left = dist(copyExpression(e1->left), copyExpression(e2));
        e->right = dist(copyExpression(e1->right), copyExpression(e2));

    } else if (e2->operator == AND) {
        e = initExpression(BOPEXPRESSION, AND);
        e->left = dist(copyExpression(e1), copyExpression(e2->left));
        e->right = dist(copyExpression(e1), copyExpression(e2->right));

    } else {
        e = initExpression(BOPEXPRESSION, OR);
        e->left = copyExpression(e1);
        e->right = copyExpression(e2);

    }
    freeExpression(e1);
    freeExpression(e2);
    return e;
}
/* ************************************************************************************************
 * 
 * - FUNCTION cnf
 *      - Parameters: 
 *          - Expression * -> Expression to convert from nnf to cnf
 *      - Returns: 
 *          - Expression * -> Expression in cnf form
 *      - Methodology:
 *          - Recursively convert an nnf expression into cnf
 *              - ((A) OR ((B) AND (C))) -> (((A) OR (B)) AND ((A) OR (C)))
 * 
 * ***********************************************************************************************/
Expression *cnf(Expression *expression) {
    Expression *e, *left, *right, *temp;

    if (expression->expressionType == TERMINAL || expression->expressionType == UOPEXPRESSION) {
        e = copyExpression(expression);

    } else if (expression->operator == AND) {
        e = initExpression(BOPEXPRESSION, AND);
    
        e->left = cnf(copyExpression(expression->left));
        e->right = cnf(copyExpression(expression->right));

    } else if (expression->operator == OR) {
        expression->left = cnf(expression->left);
        expression->right = cnf(expression->right);
   
        e = dist(copyExpression(expression->left), copyExpression(expression->right));
    }
    freeExpression(expression);
    return e;
}