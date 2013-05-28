/*************************************************************************
 * strassen.c
 *
 * 
 * Implements Strassen multiplication algorithm, which is closer to the
 * ideal O(n^2). 
 * Can be run using command "strassen" (which uses bignums) and 
 * "intstrassen" (which uses 64bit ints if available).
 * "make strassen" and "make intstrassen" will compile the required files.
 ************************************************************************/
 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "bignum.h"

/* STRASSEN HELPER FUNCTIONS */

/**
 * NAME: next_power
 * INPUT: int n
 * OUTPUT: int
 * USAGE: Finds the next smallest power of 2 bigger than n and output that value.
 * 
 * NOTES: Code borrowed from the following link:
 * http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
 */
int next_power(int n)
{    
    n--;
    n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
    n |= n >> 2;   // and then or the results.
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;           // The result is a number of 1 bits equal to the number
               // of bits in the original number, plus 1. That's the
               // next highest power of 2.
    return n;
}

/**
 * NAME: pad_matrix
 * INPUT: MATRIX* mOrig, MARIX* mNew
 * USAGE: Converts a matrix to the smallest 2^n by 2^n square matrix.
 *          Pads missing values with 0.  Stores the result in mNew.
 *          Original Input is unmodified.
 * 
 * NOTES: Assumes mOrig and mNew are already initalized.
 */
void pad_matrix(MATRIX* mOrig, MATRIX* mNew)
{
    // Calculate the new dimensions.
    int origDims;
    if(mOrig->numRows > mOrig->numCols)
        origDims = mOrig->numRows;
    else
        origDims = mOrig->numCols;
    int newDims = next_power(origDims);

    // Allocate memory for the new matrix and add zeros.
    zero_matrix(newDims, newDims, mNew);
    for(int i = 0; i < newDims; i++)
    {
        for (int j=0; j< newDims; j++)
            // Grab entries if they are in the original matrix.
            // Otherwise, pad with 0.
            if(i < mOrig->numRows && j < mOrig->numCols)
                mNew->matrix[i][j] = mOrig->matrix[i][j];   
    }
}

/**
 * NAME: strassen_preprocess
 * INPUT: MATRIX* mOrig1, MATRIX* mOrig2, MARIX* mNew1, MARIX* mNew1
 * USAGE: Checks to see if mOrig1 and mOrig2 can be multiplied.
 *          If so, pads both matrices and stores the new matrices into mNew1 and mNew2.
 * 
 * NOTES: Assumes all inputs are already initialized.
 */
void strassen_preprocess(MATRIX* mOrig1, MATRIX* mOrig2, MATRIX* mNew1, MATRIX* mNew2)
{
    // Checks to see whether m1 and m2 are valid matrices.
    if (mOrig1->numCols != mOrig2->numRows)
    {
        printf("Error: Matrices cannot be multiplied");
        free(mNew1);
        free(mNew2);
        mNew1=NULL;
        mNew2=NULL;
        return;
    }
    
    // Pad matrices.
    pad_matrix(mOrig1, mNew1);
    pad_matrix(mOrig2, mNew2);   
}

/**
 * NAME: strassen_postprocess
 * INPUT: MATRIX* mOrig, MATRIX* mNew2, int colSize, int rowSize
 * USAGE: Strips zeros from mOrig and outputs a new rowSize by colSize matrix.
 * 
 * NOTES: Assumes all inputs are already initialized.
 */
void strassen_postprocess(MATRIX* mOrig, MATRIX* mNew, int colSize, int rowSize)
{
    // Allocate memory for the new matrix and strip zeros
    zero_matrix(rowSize, colSize, mNew);
    for(int i = 0; i < rowSize; i++)
    {
         // Goes across the columns of m2
        for (int j=0; j< colSize; j++)
            // Grab entries from original matrix.
            mNew->matrix[i][j] = mOrig->matrix[i][j];
    }
}

/* STRASSEN ALGORITHM FUNCTIONS */
/**
 * NAME: strassen_postprocess
 * INPUT: MATRIX* mOrig, MATRIX* mNew2, int colSize, int rowSize
 * USAGE: Strips zeros from mOrig and outputs a new rowSize by colSize matrix.
 * 
 * NOTES: Assumes all inputs are already initialized.
 */
void strassen_helper(MATRIX* m1, MATRIX* m2, MATRIX* res)
{
    // base case
    if (m1->numRows <= 1)
    {
        mult_bignums(&m1->matrix[0][0], &m2->matrix[0][0], &res->matrix[0][0]);
    }
    else
    {
        // split mOrig1 & mOrig2 into a 2x2 of submatrices
        int n = m1->numRows/2;
        

           
        // Initialize submatrices    
        MATRIX* a11 = malloc(sizeof(MATRIX));
        MATRIX* a12 = malloc(sizeof(MATRIX));
        MATRIX* a21 = malloc(sizeof(MATRIX));
        MATRIX* a22 = malloc(sizeof(MATRIX));
        MATRIX* b11 = malloc(sizeof(MATRIX));
        MATRIX* b12 = malloc(sizeof(MATRIX));
        MATRIX* b21 = malloc(sizeof(MATRIX));
        MATRIX* b22 = malloc(sizeof(MATRIX));
        zero_matrix(n,n,a11);
        zero_matrix(n,n,a12);
        zero_matrix(n,n,a21);
        zero_matrix(n,n,a22);
        zero_matrix(n,n,b11);
        zero_matrix(n,n,b12);
        zero_matrix(n,n,b21);
        zero_matrix(n,n,b22);
        
        // Strassens algorithm
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // First matrix
                a11->matrix[i][j] = m1->matrix[i][j];          
                a12->matrix[i][j] = m1->matrix[i][j+n];   
                a21->matrix[i][j] = m1->matrix[i+n][j];    
                a22->matrix[i][j] = m1->matrix[i+n][j+n];
             
                // Second matrix
                b11->matrix[i][j] = m2->matrix[i][j];            
                b12->matrix[i][j] = m2->matrix[i][j+n];    
                b21->matrix[i][j] = m2->matrix[i+n][j];    
                b22->matrix[i][j] = m2->matrix[i+n][j+n]; 
            }
        }

        // Create two temporary matrices
        MATRIX* temp1 = malloc(sizeof(MATRIX));
        MATRIX* temp2 = malloc(sizeof(MATRIX));
        zero_matrix(n,n,temp1);
        zero_matrix(n,n,temp2);
        
        // The 7 important variable matrices of Strassens algorithm
        MATRIX* x1 = malloc(sizeof(MATRIX));
        MATRIX* x2 = malloc(sizeof(MATRIX));
        MATRIX* x3 = malloc(sizeof(MATRIX));
        MATRIX* x4 = malloc(sizeof(MATRIX));
        MATRIX* x5 = malloc(sizeof(MATRIX));
        MATRIX* x6 = malloc(sizeof(MATRIX));
        MATRIX* x7 = malloc(sizeof(MATRIX));
        zero_matrix(n,n,x1);
        zero_matrix(n,n,x2);
        zero_matrix(n,n,x3);
        zero_matrix(n,n,x4);
        zero_matrix(n,n,x5);
        zero_matrix(n,n,x6);
        zero_matrix(n,n,x7);
        
        // Fill those 7 matrices with the correct values
        add_matrices(a11, a22, temp1);
        add_matrices(b11, b22, temp2);
        strassen_helper(temp1, temp2, x1); 

        add_matrices(a21, a22, temp1);
        strassen_helper(temp1, b11, x2);

        subtract_matrices(b12, b22, temp2);
        strassen_helper(a11, temp2, x3);

        subtract_matrices(b21, b11, temp2);
        strassen_helper(a22, temp2, x4);

        add_matrices(a11, a12, temp1);
        strassen_helper(temp1, b22, x5);  

        subtract_matrices(a21, a11, temp1);
        add_matrices(b11, b12, temp2);      
        strassen_helper(temp1, temp2, x6);
        
        subtract_matrices(a12, a22, temp1);
        add_matrices(b21, b22, temp2);
        strassen_helper(temp1, temp2, x7);

        // 4 temporary result submatrices
        MATRIX* res11 = malloc(sizeof(MATRIX));
        MATRIX* res12 = malloc(sizeof(MATRIX));
        MATRIX* res21 = malloc(sizeof(MATRIX));
        MATRIX* res22 = malloc(sizeof(MATRIX));
        zero_matrix(n, n, res11);
        zero_matrix(n, n, res12);
        zero_matrix(n, n, res21);
        zero_matrix(n, n, res22);

        add_matrices(x3, x5, res12);
        add_matrices(x2, x4, res21);

        add_matrices(x1, x4, temp1);
        add_matrices(temp1, x7, temp2);
        subtract_matrices(temp2, x5, res11);

        add_matrices(x1, x3, temp1);
        add_matrices(temp1, x6, temp2);
        subtract_matrices(temp2, x2, res22);


        // Group submatrices
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                // first matrix
                res->matrix[i][j] = res11->matrix[i][j];
                res->matrix[i][j+n] = res12->matrix[i][j];
                res->matrix[i+n][j] = res21->matrix[i][j];
                res->matrix[i+n][j+n] = res22->matrix[i][j];
            }
        }
        
        // Free all matrices.
        free_matrix(a11);
        free_matrix(a12);
        free_matrix(a21);
        free_matrix(a22);
        free_matrix(b11);
        free_matrix(b12);
        free_matrix(b21);
        free_matrix(b22);
        free_matrix(temp1);
        free_matrix(temp2);
        free_matrix(x1);
        free_matrix(x2);
        free_matrix(x3);
        free_matrix(x4);
        free_matrix(x5);
        free_matrix(x6);
        free_matrix(x7);
        free_matrix(res11);
        free_matrix(res12);
        free_matrix(res21);
        free_matrix(res22);
    }
}

// Helper function to multiply two matrices smartly
void strassen_mult(MATRIX* mOrig1, MATRIX* mOrig2, MATRIX* res)
{
    
    // Preprocess original matrices for multiplication
    MATRIX* m1 = malloc(sizeof(MATRIX));
    MATRIX* m2 = malloc(sizeof(MATRIX));
    strassen_preprocess(mOrig1,mOrig2,m1,m2);
    
    if(m1 == NULL || m2 == NULL)
        return;
    
    MATRIX* m3 = malloc(sizeof(MATRIX));
    zero_matrix(m1->numRows, m2->numCols, m3);

    strassen_helper(m1, m2, m3);
    
    // Grab original dimensions.
    int origNumRows = mOrig1->numRows;
    int origNumCols = mOrig2->numCols;
    
    // Strip final result matrix.
    strassen_postprocess(m3, res, origNumRows, origNumCols);

    // Free memory
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(m3);
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
    strassen_mult(m1,m2,m3);
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
