/****************************************************************************
 * matrix.h
 *
 * Computer Science 51
 * Matrix and Bignum Structure
 *
 * Fundamental Data structures for project
 ***************************************************************************/
 
#include "bignum.h" 
#include <stdint.h>
#include <sys/resource.h>

// matrix structure
typedef struct 
{
    // will malloc size upon construction
    BIGNUM** matrix;
    
    // number of columns
    int numCols;
    
    // number of rows
    int numRows;
    
}
MATRIX;

/**
 * NAME: zero_matrix
 * INPUT: int rowSize, int colSize, MATRIX* m
 * USAGE: initializes m to become a rowSize by colSize matrix.
 *          fills all values with zero.
 *
 * NOTES: assumes m is malloced.
 */
void zero_matrix(int rowSize, int colSize, MATRIX* m);

/**
 * NAME: initialize_matrix
 * INPUT: int rowSize, int colSize, MATRIX* m
 * USAGE: initializes m to become a rowSize by colSize matrix.
 *          fills all values with random values <= 10000.
 *
 * NOTES: assumes m is malloced.
 */
void initialize_matrix(int rowSize, int colSize, MATRIX* m);

/**
 * NAME: print_matrix
 * INPUT: MATRIX* m
 * USAGE: prints the contents of m to stdout.
 */
void print_matrix(MATRIX* m);

/**
 * NAME: free_matrix
 * INPUT: MATRIX* m
 * USAGE: free memory of m
 *
 * NOTES: m must be initialize or memory issues will occur.
 */
void free_matrix(MATRIX* m);

/* HELPER FUNCTIONS */

/**
 * NAME: add_matrices
 * INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
 * USAGE: add matrices m1 and m2 together, stores the result in res.
 *
 * NOTES: res must already be initialized.
 */
void add_matrices(MATRIX* m1, MATRIX* m2, MATRIX* res);

/**
 * NAME: subtract_matrices
 * INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
 * USAGE: subtract matrix m2 from m1, stores the result in res.
 *
 * NOTS: res must already be initialized.
 */
void subtract_matrices(MATRIX* m1, MATRIX* m2, MATRIX* res);

/*
*Borrowed from CS50 Staff Code from fall 2012 pset5
*Helper function to calculate time
*/
double calculate(const struct rusage* b, const struct rusage* a);


