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
 * Will print bignum to stdout
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

/**
 * Will print bignum to stdout
 */
void print_bignum(BIGNUM* b);

#endif


