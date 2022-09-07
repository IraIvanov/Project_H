#include <math.h>
#include <stdio.h>

#ifndef IN_OUT_PUT_H_INCLUDED
#define IN_OUT_PUT_H_INCLUDED

enum EQ_RES{
    NO_SOLUTION,
    ONE_SOLUTION,
    TWO_SOLUTIONS,
    INF_SOLUTIONS
};

void input( double* a, double* b, double* c);
void output( const double x1, const double x2, const enum EQ_RES res);

#endif //IN_OUT_PUT_H_INCLUDED