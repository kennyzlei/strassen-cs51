/*************************************************************************
 * regularMult.c
 *
 * Implements regular multiplication algorithm.
 * Regular multiplication can be done by running
 * "regular" (which uses bignums) and "intregular" (which uses 64bit ints).
 * "make regular" and "make intregular" will compile the required files.
 ************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bignum.h"
#include "matrix.h"


/**
 * NAME: regular_mult
 * INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
 * USAGE: Multiplies m1 and m2 naively and stores the result in res.
 * 
 * NOTES: pointers for m1, m2, m3 must all be malloced before using this function.
 */
void regular_mult(MATRIX* m1, MATRIX* m2, MATRIX* res)
{
    // Checks to see whether m1 and m2 can be multiplied.
    if (m1->numCols != m2->numRows)
    {
        printf("Error: Matrices cannot be multiplied");
        return;
    }
    
    // Define numRows and numCols of res
    int rowSize = m1->numRows;
    int colSize = m2->numCols;
    
    // Fill in matrix information
    res->numRows = rowSize;
    res->numCols = colSize;
    
    // Allocate memory for the rows of the matrix.
    res->matrix = (BIGNUM**) malloc(rowSize * sizeof(BIGNUM*));
    for(int i = 0; i < rowSize; i++)
    {
        // Allocate memory for each entry in the matrix.
        res->matrix[i] = (BIGNUM*) malloc(colSize * sizeof(BIGNUM));
        
        // Go across the columns of m2
        for (int j=0; j< colSize; j++)
        {
            // Initialize a temporary bignum to store a sum.
            BIGNUM* sum = malloc(sizeof(BIGNUM));
            bignum_from_int(0,sum);
            
            // Go down the rows of m1 and across the columns of m2.
            // Naive multiplication is performed here.
            for (int k = 0; k < m1->numCols; k++)
            {
                // Initialize a temporary bignum to store the result of multiplication.
                BIGNUM* multRes = malloc(sizeof(BIGNUM));
                bignum_from_int(0,multRes);
                mult_bignums(&m1->matrix[i][k], &m2->matrix[k][j], multRes);
                
                // Initialize a temporary bignum for addition.
                BIGNUM* addRes = malloc(sizeof(BIGNUM));
                bignum_from_int(0,addRes);
                add_bignums(multRes, sum, addRes);
                
                // Store the addition and free varibles.
                *sum = *addRes;
                free(multRes);
                free(addRes);
            }
            
            // Store the sum into the matrix and free temp bignum.
            res->matrix[i][j] = *sum;
            free(sum);
        }
    }
}

int main(void)
{
    // Structs for timing data.
    struct rusage before, after;
    double ti_multiply=0.0;
    
    // Seed random number generator.
	srand(time(NULL));
	
	// Initalize matrixes. Change values here for different size matrices.
    MATRIX* m1 = malloc(sizeof(MATRIX));
    MATRIX* m2 = malloc(sizeof(MATRIX));
    initialize_matrix(10,10,m1);
    initialize_matrix(10,10,m2);
    
    MATRIX* m3 = malloc(sizeof(MATRIX));
    
    // Calculate time while multiplying.
    getrusage(RUSAGE_SELF, &before);
    regular_mult(m1,m2,m3);
    getrusage(RUSAGE_SELF, &after);
    ti_multiply = calculate(&before, &after);
    
    // Print out matrices to stdout.  Comment this section out for large matrices.
    print_matrix(m1);
    print_matrix(m2);
    print_matrix(m3);
    
    // Print out computation time.
    printf("\nTime Spent (in sec): %f\n", (ti_multiply));
    
    // Free matrices when done with them.
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(m3);
}
