This is documentation for quad_eq. This program contains several functions and libraries:
io.h , which has:
input and output functions.
quad_eq.h , which has:
    discriminant, linear, quad_solv and solv func;
Let's what they do.
io.h:
    input function is basicaly reading double type numbers from keyboard into 3 inputed double type pointers;
    output function is writing two numbers into a terminal with special message that determinated by EQ_RES enum;
quad_eq.h:
    discriminant function is calculating the discriminant using 3 inputed double type numbers;
    linear function is solving linear equation using 2 inputed double type numbers;
    quad_solv function is calculating x's using a, b coef and Discriminant (all double type numbers)'\
    solv function is simply solving quadratic equation using a, b, c coeficents (all double type) and writing it into x1 and x2 double type pointers;