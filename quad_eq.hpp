#include "io.hpp"
#ifndef QUAD_EQ_H_INCLUDED
#define QUAD_EQ_H_INCLUDED

//const for comparing double type num

const double EPS = 0.001;

/**
*   \brief Function is calculating the discriminant using 3 inputed double tupe numbers
*   \param [in] a a first coeficent 
*   \param [in] b second coeficent
*   \param [in] c third coeficent
*   Result is sqrt(discriminant) if it's >= 0 and discriminant if it's below 0  
*
*/

double discriminant(const double a, const double b, const double c);

/**
*   \brief Sidefunction that compares two numbers with double type
*   \param [in] a first num
*   \param [in] b second num
*   Result is an 0 if a != b and 0 if a == b 
*/

int double_eq_cmp( const double a, const double b);

/**
*   \brief Function is solving linear equation using 2 inputed double type numbers
*   \param [in] a a first coeficent 
*   \param [in] b second coeficent
*   \param [out] x1 pointer to a var that contains solution
*   Result is enum EQ_RES type, that contains amount of solution (0, 1, 2 and 3 if eq has infinite roots)   
*/


EQ_RES linear(const double b, const double c, double* x1);


/**
*   \brief Function is calculating x's using a, b coef and Discriminant (all double type numbers)
*   \param [in] a a first coeficent 
*   \param [in] b second coeficent
*   \param [in] D Discriminant
*   \param [out] x1 pointer to a var that contains first solution
*   \param [out] x2 pointer to a var that contains second solution
*   Result is enum EQ_RES type, that contains amount of solution (0, 1, 2 and 3 if eq has infinite roots)   
*/


EQ_RES quad_solv(const double a, const double b, const double D, double* x1, double* x2) ;

/**
*   \brief Function is simply solving quadratic equation using a, b, c coeficents (all double type) and writing it into x1 and x2 double type pointers
*   \param [in] a a first coeficent 
*   \param [in] b second coeficent
*   \param [in] c third coeficent
*   \param [out] x1 pointer to a var that contains first solution
*   \param [out] x2 pointer to a var that contains second solution
*   Result is enum EQ_RES type, that contains amount of solution (0, 1, 2 and 3 if eq has infinite roots)   
*/

EQ_RES solv( const double a, const double b, const double c, double* x1,  double* x2);



#endif //QUAD_EQ_H_INCLUDED