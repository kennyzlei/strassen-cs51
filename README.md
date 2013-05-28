Matrix Multiplication: Comparison of Strassen, Winograd, and Naive Algorithms
=============================================================================
By: David Boone, Kenny Lei, George Lok, Varun Sriram

Overview
========
We implemented the above 3 algorithms in the language of C, upon CS51 Professor Greg Morrisett's recommendation. The naive algorithm is what we call regular multiplication – it runs in O(n^3) time. Winograd algorithm is a slight improvement from this naive algorithm – it performs less multiplication operations, by performing a preprocessing step that calculates approximately half of what is necessary before actual multiplication occurs. It is also O(n^3), but is supposedly faster in real-time. One goal of this project was to determine if this is really true. (Our initial/technical specs have further detail). Finally, Strassen algorithm is considered the second fastest multiplication algorithm in term of complexity – it runs in O(n^2.8074) time, in exchange for more use of memory – each matrix must have dimensions that are powers of 2 (hence, the need for padding of most matrices). Strassen supposedly runs faster, and part of the goal of this project was to determine at what point that is for what matrix size Strassen actually outperforms Winograd and regular naive multiplication.

Since our goal was to measure up how the algorithms performed against one another, we implemented a method that will initialize matrices with random values, that can be adjusted for different bounds. This way, we can focus on measuring the accuracy and time each algorithm takes. We also have the entries of our matrices be bignum structures, and thus, reached one of our extensions.

Current Subdirectory (.) ::
  Contains the .h and .c files relating to the matrix multiplication project.
  Note that these files contain our bignums extension and 3 algorithms - Winograd's, Strassen's, and naive "regular" multiplication.
  
Instructions
============
1. Run "make" to compile everything
2. Run "./regular" for naive multiplication with bignums.
3. Run "./winograd" for Winograd multiplication algorithm with bignums.
4. Run "./strassen" for Strassen multiplation algorithm with bignums.
5. Run "./intregular" for naive multiplication without bignums.
6. Run "./intwinograd" for Winograd multiplication algorithm without bignums.
7. Run "./intstrassen" for Strassen multiplation algorithm without bignums.
   
Steps 2, 3, and 4 will output to the screen the 2 randomly generated matrices, and the result matrix of the multiplication.  Finally, it will output the time taken to multiply.  This is important for time comparisons.

Note: You will get a "Killed" error when too much memory has been used. Remember that Strassen's uses much more memory than the other two, so it will exit earlier. Since the multiplication without bignums uses about 400 bytes fewer memory per matrix cell than multiplication with bignums, it was particularly useful in timing the 3 algorithms for comparison.
