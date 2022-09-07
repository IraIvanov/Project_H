#include <stdio.h>
#include <string.h>
#include "quad_eq.h"
#include "io.h"
#include "quad_test.h"

int main(int argc, char* argv[]) {

    enum EQ_RES res = NO_SOLUTION;

    if ( argc == 1){
        double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;
        
        input(&a, &b, &c);
        res = solv(a, b, c, &x1, &x2);
        output(x1, x2, res);
    }

    else {
        if (argc == 2 && !strcmp(argv[1],"-t") ) {

            test_programm();

        } else {
            printf("%s\n", "UNKNOWN KEY_WORD");
        }
    }
    return 0;
}