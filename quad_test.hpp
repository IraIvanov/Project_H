#include <math.h>
#include <stdio.h>

#ifndef QUAD_TEST_H_INCLUDED
#define QUAD_TEST_H_INCLUDED

/**
*   \brief TEST_TYPE KEY conatians types of tests that are int num
*/

enum TEST_TYPE_KEY{
    TYPE_1 = 1,
    TYPE_2 = 2,
    TYPE_3 = 3
};

/**
*   \brief ANS KEY conatians types of ANS that are int num
*/

enum ANS_KEY{
    ANS_0 = 0,
    ANS_1 = 1,
    ANS_2 = 2,
    ANS_3 = 3
};

//doxygen 
//README.md + added IMG

/**
*   \brief test_programm is testing solv func taking the data from quad_test.txt file
*/

void test_programm();

#endif //QUAD_TEST_H_INCLUDED