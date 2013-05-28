/****************************************************************************
 * bignum.h
 *
 * Computer Science 51
 * Bignum Functions
 *
 * Fundamental Data structures for project
 ***************************************************************************/
#ifndef _BIGNUM_H
#define _BIGNUM_H 
#define LIMIT 100
#define BASE 10

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Bignum structure 
typedef struct
{
    // simple for now
    //int val;
    
    int coeffs[LIMIT];
    bool neg;
    int lastIndex;
    
}
BIGNUM;

 /**
 * NAME: print_bignum
 * INPUT: BIGNUM b
 * USAGE: prints b to stdout.
 *
 * NOTES: assumes b is initialized
 */
void print_bignum(BIGNUM* b);

 /**
 * NAME: add_bignums
 * INPUT: BIGNUM b1, BIGNUM b2, BIGNUM res
 * USAGE: adds two bignums
 *
 * NOTES: assumes inputs are initialized. 
 * If one input is negative it 
 * calls subtract_bignums
 */
void add_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res);

 /**
 * NAME: mult_bignums
 * INPUT: BIGNUM b1, BIGNUM b2, BIGNUM res
 * USAGE: Multiplies two bignums
 *
 * NOTES: necessary for matrix multiplication
 */
void mult_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res);

 /**
 * NAME: bignum_from_int
 * INPUT: int n, BIGNUM b
 * USAGE: converts n into a bignum b
 *
 * NOTES: assumes b is initialized
 */
void bignum_from_int(int i, BIGNUM* b);

/**
 * NAME: negate_bignums
 * INPUT: BIGNUM b
 * USAGE: flip the sign of the bignum
 *
 * NOTES: assumes b is initialized
 */
void negate_bignums(BIGNUM* b);

#endif

