#include <stdio.h>
#include <math.h>
#include "quad_eq.h"
#include "io.h"
#include <assert.h>

int double_eq_cmp( const double a, const double b) {

    return fabs(a - b) < EPS;

}

double discriminant(const double a, const double b, const double c){

    assert( isfinite(a) && isfinite(b) && isfinite(c) );

    double D = 0;
    
    D = b*b - 4*a*c;

    if ( D < 0){

        return D;

    } else {

        return sqrt(D);

    }

}

EQ_RES linear(const double b, const double c, double* x1){

    assert( x1 && isfinite(b) && isfinite(c) ); 

    if ( double_eq_cmp(b, 0) && !double_eq_cmp(c, 0)) 

        return NO_SOLUTION;

    else {

        if ( double_eq_cmp(b, 0) && double_eq_cmp(c, 0) ) 

            return INF_SOLUTIONS;

        else{

            if (double_eq_cmp(c, 0)){
                
                *x1 = 0;

                return ONE_SOLUTION;
            }
            
            else {
                
                *x1 = -c / b;

                return ONE_SOLUTION;

            } 

        }

    }

} 

EQ_RES quad_solv(const double a, const double b, const double D, double* x1, double* x2) {

    assert( !( !x1 || !x2) && isfinite(a) && isfinite(b) && isfinite(D)); 

    if ( D < 0){

        *x1 = NAN;

        *x2 = NAN;
        
        return NO_SOLUTION;

    } else{ 

        if ( double_eq_cmp(D, 0) ) {

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
    
    assert( !( !x1 || !x2) && isfinite(a) && isfinite(b) && isfinite(c) ); 

    if ( double_eq_cmp(a, 0)  ){ 

        *x2 = NAN;

        return linear(b, c, x1);

    } else {
        
        if ( double_eq_cmp(c, 0) ){
            *x1 = 0;

            *x2 = -b / a;
            
            return TWO_SOLUTIONS;
        }

        else{ 
         
            double D = discriminant(a, b, c);

        
            return quad_solv(a, b, D, x1, x2);
        }

    }

}