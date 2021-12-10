#include <stdio.h>
#include "grammar.h"
#include "parser.h"
#include "cnf.h"

#include <time.h>
/* ************************************************************************************************
 * 
 * - FUNCTION main
 *      - Parameters: 
 *          - argc, argv -> number/array of arguments passed
 *      - Returns: 
 *          - int -> success or failure  (0 or 1)
 *      - Methodology:
 *          - Parses input file
 *              - Generates Expression given file input
 *          - Calls iffElim to convert iffs to implies'
 *          - Calls nnf to convert expression to nnf
 *          - Calls cnf to convert expression to cnf
 *          - Tracks runtime of each of the above, and prints this at the end
 *          - Creates data structure to better hold expressions + track performance with
 * 
 * ***********************************************************************************************/
int main(int argc, char **argv) {
    char *filename = argv[1];
    double parset, ifft, impt, nnft, cnft;
    clock_t begin, clock();
    begin = clock();
    Expression *e = parse(filename);
    parset = (double) (clock() - begin) / CLOCKS_PER_SEC;
    setbuf(stdout, NULL);
    printExpression(e, false);
    PRINT("\n");

    begin = clock();
    e = iffElim(e);
    ifft = (double) (clock() - begin) / CLOCKS_PER_SEC;
    
    begin = clock();
    e = impElim(e);
    impt = (double) (clock() - begin) / CLOCKS_PER_SEC;

    begin = clock();
    e = nnf(e);
    nnft = (double) (clock() - begin) / CLOCKS_PER_SEC;

    begin = clock();
    e = cnf(e);
    cnft = (double) (clock() - begin) / CLOCKS_PER_SEC;
    printExpression(e, true);
    PRINT("\n");

    PRINT("TIME SPENT: \n");
    PRINT("\tPAR: %.10f\n", parset);
    PRINT("\tIFF: %.10f\n", ifft);
    PRINT("\tIMP: %.10f\n", impt);
    PRINT("\tNNF: %.10f\n", nnft);
    PRINT("\tCNF: %.10f\n", cnft);
    PRINT("TOTAL: %.10f\n", parset + ifft + impt + nnft + cnft);
    PRINT("TOTALNPARSE: %.10f\n", ifft + impt + nnft + cnft);

    CNF *head = NULL;
    head = makeCNF(e, &head);
    printCNF(head);
    printf("\n");
    printf("LEN: %d\n", CNFLen(head));

    freeCNF(head);
    freeExpression(e);
    return 0;
}