#include <stdio.h>
#include "quad_eq.h"
#include "io.h"

int main(int argc, char* argv[]) {
    double a, b, c, x1, x2;
    enum EQ_RES res = NO_SOLUTION;
    input(&a, &b, &c);
    res = solv(a, b, c, &x1, &x2);
    output(x1, x2, res);
    return 0;
}