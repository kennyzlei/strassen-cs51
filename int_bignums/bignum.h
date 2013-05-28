/****************************************************************************
 * matrix.h
 *
 * Computer Science 51
 * Bignum Functions
 *
 * Fundamental Data structures for project
 ***************************************************************************/
#ifndef _BIGNUM_H
#define _BIGNUM_H 

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Bignum structure 
typedef struct
{
    // simple implementation of bignum.
    long long val;
}
BIGNUM;

/**
* NAME: print_bignum
* INPUT: MATRIX* m1, MATRIX* m2, MATRIX* res
* USAGE: Multiplies m1 and m2 naively and stores the result in res.
* 
* NOTES: m1, m2, m3 must all be malloced before using this function.
*/
void print_bignum(BIGNUM* b);

/**
 * Will add two bignums
 * Needed for matrix mult algorithms
 */
void add_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res);

/**
 * Will mult two bignums
 * Needed for matrix mult algorithms
 */
void mult_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res);

/**
 * Creates a bignum from an int.
 */
void bignum_from_int(int i, BIGNUM* b);

/**
 * Will negate a Bignum 
 */
void negate_bignums(BIGNUM* b);



#endif

