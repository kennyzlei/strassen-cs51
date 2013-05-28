/*************************************************************************
 * winograd.c
 *
 * Implements Winograd multiplication algorithm, a slight practical
 * imporvement from the naive implementation.
 * Can be run using command "winograd" (which uses bignums) and 
 * "intwinograd" (which uses 64bit ints if available).
 * "make winograd" and "make intwinograd" will compile the required files. 
 ************************************************************************/
 
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "bignum.h"


/* WINOGRAD HELPER FUNCTIONS */

/**
 * NAME: winograd_preprocess
 * INPUT: MATRIX* m1, MATRIX* m2, BIGNUM* row, BIGNUM* col
 * USAGE: Calculates the rowFactors and columnFactors of m1 and m2
 *           as part of winograd's algorithm
 * 
 * NOTES: m1, m2, row, col must all be malloced before using this function.
 */
void winograd_preprocess(MATRIX* m1, MATRIX* m2, BIGNUM* row, BIGNUM* col)
{
    // get matrices info for indexing
    int a = m1->numRows;
    int b = m1->numCols;
    int c = m2->numCols;
    int d = b/2;

    // get row factors from m1
    for (int i = 0; i < a; i++)
    {
        mult_bignums (&m1->matrix[i][0], &m1->matrix[i][1], &row[i]);
        for (int j = 1; j < d; j++)
	    {
	        // We must initialize temporary variables whenever we use bignums.
	        BIGNUM* temp1 = malloc(sizeof(BIGNUM));
	        bignum_from_int(0, temp1);
	        mult_bignums(&m1->matrix[i][2*j], &m1->matrix[i][2*j+1], temp1);
	        
	        BIGNUM* temp2 = malloc(sizeof(BIGNUM));
	        bignum_from_int(0, temp2);
	        add_bignums(&row[i], temp1, temp2);
	        
	        row[i] = *temp2;
	        
	        // Free temporary memory.
	        free(temp1);
	        free(temp2);
        }
    }

    // get column factors from m2
    for (int i = 0; i < c; i++)
    {
        mult_bignums(&m2->matrix[0][i], &m2->matrix[1][i], &col[i]);
        for (int j = 1; j < d; j++)
	    {
	        // We must initialize temporary variables whenever we use bignums.	    
	        BIGNUM* temp1 = malloc(sizeof(BIGNUM));
	        bignum_from_int(0, temp1);
            mult_bignums(&m2->matrix[(2*j)][i], &m2->matrix[(2*j+1)][i], temp1);
            
            BIGNUM* temp2 = malloc(sizeof(BIGNUM));
            bignum_from_int(0, temp2);
            add_bignums(&col[i], temp1, temp2);
            
            col[i] = *temp2;    
            
            // Free temporary memory.
	        free(temp1);
	        free(temp2);            
	    }
    }
}

/* WINOGRAD ALGORITHM */

/**
 * NAME: winograd_mult
 * INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
 * USAGE: Multiplies m1 and m2 using winograd's algorithm and stores
 *           the result in res
 * 
 * NOTES: m1, m2, m3 must all be malloced before using this function.
 */
void winograd_mult(MATRIX* m1, MATRIX* m2, MATRIX* res)
{
    // Get matrices info for indexing
    int m1RowSize = m1->numRows;
    int m1ColSize = m1->numCols;
    int m2ColSize = m2->numCols;
    int d = m1ColSize/2;
    
    // Checks to see whether m1 and m2 are valid matrices.
    if (m1->numCols != m2->numRows)
    {
        printf("Error: Matrices cannot be multiplied");
        return;
    }
    
    // Prepocess the matrices
    BIGNUM* rowFactor = malloc(m1RowSize * sizeof(BIGNUM));
    BIGNUM* columnFactor = malloc(m2ColSize * sizeof(BIGNUM));    
    winograd_preprocess(m1, m2, rowFactor, columnFactor);

    // Fill in matrix information
    res->numRows = m1RowSize;
    res->numCols = m2ColSize;

    // Allocate memory for the rows of the matrix.
    res->matrix = (BIGNUM**) malloc(m1RowSize * sizeof(BIGNUM*));
    
    // Apply winograd's algorithm
    for (int i = 0; i < m1RowSize; i++)
    {
        // Allocate memory for each entry in the matrix.
        res->matrix[i] = (BIGNUM*) malloc(m2ColSize * sizeof(BIGNUM));
            
        for (int j = 0; j < m2ColSize; j++)
        {
            // Note we must negate the bignums first.
            negate_bignums(&rowFactor[i]);
            negate_bignums(&columnFactor[j]);
            
            add_bignums(&rowFactor[i], &columnFactor[j], &res->matrix[i][j]);
            for (int k = 0; k < d; k++)
            {
                // Initialize temp variables for bignum operations.
                BIGNUM* temp1 = malloc(sizeof(BIGNUM));
                bignum_from_int(0, temp1);
	            add_bignums(&m1->matrix[i][(2*k)], &m2->matrix[(2*k+1)][j], temp1);
	            
                BIGNUM* temp2 = malloc(sizeof(BIGNUM));
                bignum_from_int(0, temp2);	            
	            add_bignums(&m1->matrix[i][(2*k+1)], &m2->matrix[(2*k)][j], temp2);
	            
	            BIGNUM* temp3 = malloc(sizeof(BIGNUM));
                bignum_from_int(0, temp3);
	            mult_bignums(temp1, temp2, temp3);
	            
  	            BIGNUM* temp4 = malloc(sizeof(BIGNUM));
                bignum_from_int(0, temp4);
                add_bignums(&res->matrix[i][j], temp3, temp4);
                res->matrix[i][j] = *temp4;
                
                // Free temp variables when done.
                free(temp1);
                free(temp2);
                free(temp3);
                free(temp4);
            }
            
            // We renegate the bignums to return them back to their original sign.
            negate_bignums(&rowFactor[i]);
            negate_bignums(&columnFactor[j]);
        }
    }

    // make room for odd shared values
    if (m1ColSize%2 != 0)
    {
        for (int i = 0; i < m1RowSize; i++)
	    {
	        for (int j = 0; j < m2ColSize; j++)
	        {
	            // Initialize temp variables for bignum operations.
	            BIGNUM* temp1 = malloc(sizeof(BIGNUM));
	            bignum_from_int(0,temp1);
	            mult_bignums(&m1->matrix[i][m1ColSize-1], &m2->matrix[m1ColSize-1][j], temp1);
	            
	            BIGNUM* temp2 = malloc(sizeof(BIGNUM));
	            bignum_from_int(0,temp2);	            
	            add_bignums(&res->matrix[i][j], temp1, temp2);
	            res->matrix[i][j] = *temp2;
	            
	            // Free temporary variables.
	            free(temp1);
	            free(temp2);
	        }
	    }
    }
    
    // Free row and column factors.
    free(rowFactor);
    free(columnFactor);    
}

int main(void)
{
   // Structs for timing data.
    struct rusage before, after;
    double ti_multiply=0.0;
    
    // Seed random number generator.
	srand(time(NULL));
	
	// Initalize matrixes. Change values here for different size matrices.
	// Note that for winograd, m1->numCols and m2->numRows have to be > 1
    MATRIX* m1 = malloc(sizeof(MATRIX));
    MATRIX* m2 = malloc(sizeof(MATRIX));
    initialize_matrix(10,10,m1);
    initialize_matrix(10,10,m2);
    
    MATRIX* m3 = malloc(sizeof(MATRIX));
    
    // Calculate time while multiplying.
    getrusage(RUSAGE_SELF, &before);
    winograd_mult(m1,m2,m3);
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
