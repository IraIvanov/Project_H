#include <stdio.h>
#include "io.hpp"
#include <assert.h>

/** @file */ 

void input( double* a, double* b, double* c){
    assert( !( !a || !b || !c) );

    while(scanf("%lf %lf %lf", a, b, c) != 3 || getchar() !='\n'){

        while(getchar() != '\n'){}

        printf("%s\n", "Incorrect input, try again!");

    }

    return;

}



void output( const double x1, const double x2, const enum EQ_RES res){

    switch(res) {
        case NO_SOLUTION:

            printf("%s", "No solutions\n");

            break;

        case ONE_SOLUTION:

            printf("%s %.2lf\n", "Equation has only one solution:", x1);

            break;
        
        case TWO_SOLUTIONS:

            printf("%s %.2lf %.2lf\n", "Equation has two solutions: ", x1, x2);

            break;
        
        case INF_SOLUTIONS:

            printf("%s", "Infinite amount of solutions\n");

            break;

        default:
            printf("%s", "ERROR\n");
            break;
    }

    return;

}

