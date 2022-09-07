#include "io.h"
#ifndef QUAD_EQ_H_INCLUDED
#define QUAD_EQ_H_INCLUDED

const double EPS = 0.001;

double discriminant(const double a, const double b, const double c);
int double_eq_cmp( const double a, const double b);
EQ_RES linear(const double b, const double c, double* x1);
EQ_RES quad_solv(const double a, const double b, const double D, double* x1, double* x2) ;
EQ_RES solv( const double a, const double b, const double c, double* x1,  double* x2);

#endif //QUAD_EQ_H_INCLUDED