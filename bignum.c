/****************************************************************************
 * bignum.c
 *
 * Computer Science 51
 * Bignum Functions
 *
 * Fundamental Data structures for project
 *
 * Consulted http://www.cs.sunysb.edu/~skiena/392/programs/bignum.c
 * - Steven Skiena's implementation as a
 * guide for our own bignums code
 *
 * NOTE: Must initialize BIGNUMS coeffs to 0 with bignum_from_int 
 * before calling these functions
 ***************************************************************************/

#include <math.h>
#include "bignum.h"

 /**
 * NAME: subtract_bignums
 * INPUT: BIGNUM b1, BIGNUM b2, BIGNUM res
 * USAGE: helper function for add_bignums
 *
 * NOTES: assumes inputs are initialized
 */
void subtract_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res)
{
  int borrow=0;
  
  for (int i=0; i < ((res->lastIndex) + 1); i++)
  {
    int digit = ((b1->coeffs[i]) - (b2->coeffs[i]) - borrow);

    if((b1->coeffs[i]) > 0)
    {
      borrow = 0;
    }
    if (digit < 0)
    {
      digit += BASE;
      borrow = 1;
    }
    
    res->coeffs[i] = digit % BASE;
  }
}

 /**
 * NAME: help_subtract_bignums
 * INPUT: BIGNUM b1, BIGNUM b2, BIGNUM res
 * USAGE: comparison function for add_bignum
 *
 * NOTES: assumes inputs are initialized
 */
void help_subtract_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res)
{
  // check if b1 > b2.
  if ((b1->lastIndex) > (b2->lastIndex))
  {
    subtract_bignums(b1, b2, res);
    res->neg = false;
  }
  else if ((b1->lastIndex) < (b2->lastIndex))
  {
    subtract_bignums(b2, b1, res);
    res->neg = true;
  }
  else
  {
    for(int i = b1->lastIndex; i >= 0; i--)
    {
        if ((b1->coeffs[i]) > (b2->coeffs[i]))
        {
            subtract_bignums(b1, b2, res);
            res->neg = false;
            return;
        }
        else if((b1->coeffs[i]) < (b2->coeffs[i]))
        {
            subtract_bignums(b2, b1, res);
            res->neg = true;
            return;
        }
        else if (i == 0)
            bignum_from_int(0, res);        
     }
  }
}

 /**
 * NAME: trim_bignum
 * INPUT: BIGNUM b
 * USAGE: adjust lastIndex (helper for add_bignums)
 *
 * NOTES: assumes b is initialized
 */
void trim_bignum(BIGNUM* b)
{
  while ( ((b->lastIndex) > 0) && ((b->coeffs[b-> lastIndex]) == 0) )
		b->lastIndex --;
}

 /**
 * NAME: add_bignums
 * INPUT: BIGNUM b1, BIGNUM b2, BIGNUM res
 * USAGE: adds two bignums
 *
 * NOTES: assumes inputs are initialized. 
 * If one input is negative it 
 * calls subtract_bignums
 */
void add_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res) 
{
    bignum_from_int(0, res); 
    int carry = 0;
   
   // assume that the b1/b2/res coeffs that arent used 
   // have been initialized to 0
   
   // figure lastIndex for temporary calculations
   if ((b1->lastIndex) > (b2->lastIndex))
   {
     res->lastIndex = (b1->lastIndex) + 1;
   }
   else
   {
     res->lastIndex = (b2->lastIndex) + 1; 
   }
   
   // consider signs
   if ((b1 -> neg) == (b2 -> neg))
   {
     (b1->neg) ? (res->neg = true) : (res->neg = false);
     
     // carrying
     for (int i = 0; i < ((res->lastIndex) + 1); i++)
     {
       res->coeffs[i] = ((b1->coeffs[i]) + (b2-> coeffs[i]) + carry) % BASE;
       carry = ((b1->coeffs[i]) + (b2-> coeffs[i]) + carry) / BASE;
     }
   }
   else if ((b1-> neg) == true )
   {
     help_subtract_bignums(b2, b1, res);
   }
   else
   {
     help_subtract_bignums(b1, b2, res);
   }
   
   trim_bignum(res);
}

 /**
 * NAME: mult_bignums
 * INPUT: BIGNUM b1, BIGNUM b2, BIGNUM res
 * USAGE: Multiplies two bignums
 *
 * NOTES: necessary for matrix multiplication
 */
void mult_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res)
{
    BIGNUM* storeB1 = malloc(sizeof(BIGNUM));
    *storeB1 = *b1;
    
    BIGNUM* tmp = malloc(sizeof(BIGNUM));
 
    for (int i=0; i<=b2->lastIndex; i++) {

        // count all tens properly
		for (int j=1; j<=b2->coeffs[i]; j++) {

			add_bignums(res,storeB1,tmp);

			*res = *tmp;
		}
		
		// multiply b1 by ten
		//if ((b1->lastIndex != 0) && (b1->coeffs[0] != 0))
		//{
		  for (int k = storeB1->lastIndex; k >= 0; k--)
		  {
		    storeB1->coeffs[k+1] = storeB1->coeffs[k];
		  }
		  storeB1->coeffs[0] = 0;
		  storeB1->lastIndex ++;
		//}
	}
	
	// fix signs
	((b1->neg) == (b2->neg)) ? (res->neg=false) : (res->neg=true);
	
	// fix indexing
	trim_bignum(res);
	free(tmp);
	free(storeB1);
}

 /**
 * NAME: bignum_from_int
 * INPUT: int n, BIGNUM b
 * USAGE: converts n into a bignum b
 *
 * NOTES: assumes b is initialized
 */
void bignum_from_int(int n, BIGNUM* b)
{
    // raw number to convert
    int raw = abs(n);

    // fix signs first
    (n >= 0) ? (b->neg = false) : (b->neg = true);
  
    // default coeffs for bignums- CRITICAL
    for (int i =0; i < LIMIT; i++)
    {
        b->coeffs[i] = 0;
    }
    b -> lastIndex = -1;
  
    while (raw > 0)
    {
        b->lastIndex ++;
        b->coeffs[b->lastIndex] = (raw % 10);
        raw = raw / 10;
    }
  
    // if the number was 0, we still need to fix indexing
    if (n == 0)
    {
        b -> lastIndex = 0;
    }
}

/**
 * NAME: negate_bignums
 * INPUT: BIGNUM b
 * USAGE: flip the sign of the bignum
 *
 * NOTES: assumes b is initialized
 */
void negate_bignums(BIGNUM* b)
{
    b->neg = !( b->neg );
}

 /**
 * NAME: print_bignum
 * INPUT: BIGNUM b
 * USAGE: prints b to stdout.
 *
 * NOTES: assumes b is initialized
 */
void print_bignum(BIGNUM* b)
{
    if (b->neg)
        printf("-");
    for (int i= (b->lastIndex); i >= 0; i--)
    {
        printf("%i", b->coeffs[i]);
    }
}
