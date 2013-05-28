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
 * Subtract bignums- a helper function for
 * add_bignums
 */
void subtract_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res)
{
  int borrow=0;
  
  // borrowing
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
 * Comparison to help subtract bignums- a helper function for
 * add_bignums
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
 * Trim bignum to adjust lastIndex- ls
 * a helper function for
 * add_bignums
 */
void trim_bignum(BIGNUM* b)
{
  while ( ((b->lastIndex) > 0) && ((b->coeffs[b-> lastIndex]) == 0) )
		b->lastIndex --;
		
   /* int i = LIMIT - 1;
    while (b->coeffs[i] == 0)
      i--;
    b->lastIndex = i;*/
}

/**
 * Will add two bignums
 * If one bignum is negative, will call 
 * helper funtion subtract_bignums
 * Needed for matrix mult algorithms
 */
void add_bignums(BIGNUM* b1, BIGNUM* b2, BIGNUM* res) 
{
    bignum_from_int(0,res); 
    int carry=0;
   
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
     for (int i=0; i < ((res->lastIndex) + 1); i++)
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
 * Will mult two bignums
 * Needed for matrix mult algorithms
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
 * Creates a bignum from an int.
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


void negate_bignums(BIGNUM* b)
{
    b->neg = !( b->neg );
}

/**
 * Will print bignum to stdout
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

/* For testing */
int main(void)
{
  BIGNUM* n1 = malloc(sizeof(BIGNUM));
  bignum_from_int(90,n1);
  
  BIGNUM* n2 = malloc(sizeof(BIGNUM));
  bignum_from_int(15,n2);
  
  BIGNUM* n3 = malloc(sizeof(BIGNUM));
  bignum_from_int(0,n3);
  print_bignum(n3);
  mult_bignums(n1, n2, n3);
  
  print_bignum(n3);
  printf("= %i\n",(90*15));

  bignum_from_int(-1500,n1);

  bignum_from_int(100,n2);

  bignum_from_int(0,n3);
  add_bignums(n1, n2, n3);
  
  print_bignum(n3);
  printf(" = %i\n",(-1500+100));
  
  bignum_from_int(-1540780,n1);

  
  bignum_from_int(265,n2);

  
  bignum_from_int(0,n3);
  mult_bignums(n1, n2, n3);
  
  print_bignum(n3);
  printf(" = %i\n", (-1540780*265));
  free(n1);
  free(n2);
  free(n3);
}