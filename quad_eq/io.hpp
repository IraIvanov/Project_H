#include <math.h>
#include <stdio.h>

#ifndef IN_OUT_PUT_H_INCLUDED
#define IN_OUT_PUT_H_INCLUDED

/**
*   \brief EQ_RES is enum that contains an amount of solutions 
*/



enum EQ_RES{
    NO_SOLUTION = 0,
    ONE_SOLUTION = 1,
    TWO_SOLUTIONS = 2,
    INF_SOLUTIONS = 3
};

//doxygen 
//README.md + added IMG

/**
*   \brief Function is basicaly reading double type numbers from keyboard into 3 inputed double type pointers
*   \param [in] a first num ptr
*   \param [in] b second num ptr
*   \param [in] c third num ptr
*   If func get wrong input, it will show following massage and ask to write num again
*/

void input( double* a, double* b, double* c);

/**
*   \brief Function is writing two numbers into a terminal with special message that determinated by EQ_RES enum
*   \param [in] x1 first num 
*   \param [in] x2 second num 
*   \param [in] res enum that contains amount of printing vars 
*   Also enum determinates massage that will be shown with numvers
*/

void output( const double x1, const double x2, const enum EQ_RES res);


#endif //IN_OUT_PUT_H_INCLUDED