/****************************************************************************
 * bignum.c
 *
 * Computer Science 51
 * Bignum Functions
 *
 * Fundamental Data structures for project
 ***************************************************************************/

#include "bignum.h"

/**
 * Will add two bignums
 * Needed for matrix mult algorithms
 */
void add_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res) 
{
    res->val = b1->val + b2->val;
}

/**
 * Will mult two bignums
 * Needed for matrix mult algorithms
 */
void mult_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res)
{
    res->val = b1->val * b2->val;
}

/**
 * Creates a bignum from an int.
 */
void bignum_from_int(int i, BIGNUM* b)
{
    b->val = i;
}


void negate_bignums(BIGNUM* b)
{
  b->val = b->val * (-1);
}

/**
 * Will print bignum to stdout
 */
void print_bignum(BIGNUM* b)
{
    printf("%lld", b->val);
}
