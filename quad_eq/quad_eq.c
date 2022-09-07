#include <stdio.h>
#include <math.h>
#include "quad_eq.h"
#include "io.h"
#include <assert.h>

const double EPS = 0.001;

double discriminant(const double a, const double b, const double c){

    double D;

    D = b*b - 4*a*c;

    if ( D < 0){

        return -1;

    } else {

        return sqrt(D);

    }

}

EQ_RES linear(const double b, const double c, double* x1){

    assert( x1 ); 

    if ( fabs(b) < EPS && fabs(c) >= EPS) 

        return NO_SOLUTION;

    else {

        if ( fabs(b) < EPS && fabs(c) < EPS) 

            return INF_SOLUTIONS;

        else{

            *x1 = -c / b;

            return ONE_SOLUTION; 

        }

    }

} 

EQ_RES quad_solv(const double a, const double b, const double D, double* x1, double* x2) {

    assert( !( !x1 || !x2) ); 

    if ( fabs(D + 1) < EPS){

        *x1 = NAN;

        *x2 = NAN;
        
        return NO_SOLUTION;

    } else{ 

        if ( fabs(D) < EPS ) {

            *x1 = -b  / (2 * a);

            *x2 = NAN; 

            return ONE_SOLUTION;

        } else {

            *x1 = (-b + D)  / (2 * a);

            *x2 = (-b - D)  / (2 * a);

            return TWO_SOLUTIONS;
        }

    }

}



EQ_RES solv( const double a, const double b, const double c, double* x1,  double* x2){
    
    assert( !( !x1 || !x2) ); 

    if ( fabs(a) < EPS  ){ 

        *x2 = NAN;

        return linear(b, c, x1);

    } else {

        double D = discriminant(a, b, c);

        return quad_solv(a, b, D, x1, x2);
    }

}