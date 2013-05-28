/*************************************************************************
 * matrix.c
 *
 * Implements some simple helper funtions for both Winograd's and Strassen's
 * algorithms. Also includes an initializer function for a matrix.
 ************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

/**
 * NAME: zero_matrix
 * INPUT: int rowSize, int colSize, MATRIX* m
 * USAGE: initializes m to become a rowSize by colSize matrix.
 *          fills all values with zero.
 *
 * NOTES: assumes m is malloced.
 */
void zero_matrix(int rowSize, int colSize, MATRIX* m)
{
    m->matrix = (BIGNUM**) malloc(rowSize * sizeof(BIGNUM*));
    for(int i = 0; i < rowSize; i++)
    {
        m->matrix[i] = (BIGNUM*) malloc(colSize * sizeof(BIGNUM));
        for(int j = 0; j < colSize; j++)
            bignum_from_int(0, &m->matrix[i][j]);
    }
      
    // Fill in information
    m->numRows = rowSize;
    m->numCols = colSize;
}

/**
 * NAME: initialize_matrix
 * INPUT: int rowSize, int colSize, MATRIX* m
 * USAGE: initializes m to become a rowSize by colSize matrix.
 *          fills all values with random values <= 10000.
 *
 * NOTES: assumes m is malloced.
 */
void initialize_matrix(int rowSize, int colSize, MATRIX* m)
{
   // Allocate memory for the matrix and initialize values to random.
    m->matrix = (BIGNUM**) malloc(rowSize * sizeof(BIGNUM*));
    for(int i = 0; i < rowSize; i++)
    {
        m->matrix[i] = (BIGNUM*) malloc(colSize * sizeof(BIGNUM));
        for(int j=0; j<colSize; j++)
        {
            int value = rand()%10000;
            bignum_from_int(value,&m->matrix[i][j]);
        }
    }   
        
    // Fill in information
    m->numCols = colSize;
    m->numRows = rowSize;
    
}

/**
 * NAME: print_matrix
 * INPUT: MATRIX* m
 * USAGE: prints the contents of m to stdout.
 */
void print_matrix(MATRIX* m)
{
    int a = m->numRows;
    int b = m->numCols;
    for(int i = 0; i < a; i++)
    {    
        printf("\n");
        for(int j = 0; j < b; j++)
        {
            print_bignum(&(m->matrix[i][j]));
            printf("\t");
        }
    }
    printf("\n");
}

/**
 * NAME: free_matrix
 * INPUT: MATRIX* m
 * USAGE: free memory of m
 *
 * NOTES: m must be initialize or memory issues will occur.
 */
void free_matrix(MATRIX* m)
{
    int numRows = m->numRows;
    // Deallocate the matrix values.
    for(int i = 0; i < numRows; i++)
        free(m->matrix[i]);
    
    // Free the stored matrix
    free(m->matrix);
    free(m);
}
    

/* HELPER FUNCTIONS */

/**
 * NAME: add_matrices
 * INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
 * USAGE: add matrices m1 and m2 together, stores the result in res.
 *
 * NOTES: res must already be initialized.
 */
void add_matrices(MATRIX* m1, MATRIX* m2, MATRIX* res){
    int len = m1->numRows;
    for(int i=0; i<len; i++)
    {
        for(int j=0; j<len; j++)
            add_bignums(&m1->matrix[i][j], &m2->matrix[i][j], &res->matrix[i][j]);
    }
}

/**
 * NAME: subtract_matrices
 * INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
 * USAGE: subtract matrix m2 from m1, stores the result in res.
 *
 * NOTS: res must already be initialized.
 */
void subtract_matrices(MATRIX* m1, MATRIX* m2, MATRIX* res){
    int len = m1->numRows;
    for(int i=0; i<len; i++)
    {
        for(int j=0; j<len; j++)
        {
            negate_bignums(&m2->matrix[i][j]);
            add_bignums(&m1->matrix[i][j], &m2->matrix[i][j], &res->matrix[i][j]);
            negate_bignums(&m2->matrix[i][j]);            
        }
    }
}

/*
 * Borrowed from CS50 Staff Code from fall 2012 pset5
 * Helper function to calculate time
 */
double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}

