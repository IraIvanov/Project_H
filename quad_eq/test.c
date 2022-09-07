#include <stdio.h>
#include <math.h>
#include "io.h"
#include "quad_eq.h"
#include "quad_test.h"


    void test_programm() {
        
        FILE* test = fopen("quad_test.txt", "r");

        enum EQ_RES res = NO_SOLUTION;

        int key = 0, ans = 0;

        void* ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *px1 = NULL, *px2 = NULL;

        double a = NAN, b = NAN, c = NAN, ans_x1 = NAN, ans_x2 = NAN, x1 = NAN, x2 = NAN;

        while(fscanf(test, "%d", &key) ) {

            switch(key){

                case TYPE_1:

                    fscanf(test, "%p %p %p", &ptr1, &ptr2, &ptr3);

                    input( (double*)ptr1, (double*)ptr2, (double*)ptr3);

                    break;

                case TYPE_2:

                    fscanf(test, "%lf %lf %lf %p %p", &a, &b, &c, &px1, &px2);

                    solv(a, b, c, (double*)px1, (double*)px2);

                    break;

                case TYPE_3:

                    fscanf(test, "%lf %lf %lf %d %lf %lf", &a, &b, &c, &ans, &ans_x1, &ans_x2);

                    res = solv(a, b, c, &x1, &x2);

                    switch(ans){

                        case ANS_0:

                            if( res == ans ) printf("%s\n", "Approved");

                                else printf("%s\n", "Wrong answer");

                                break;

                            case ANS_1:

                                if( res == ans && fabs(x1 - ans_x1) < EPS) printf("%s\n", "Approved");

                                else printf("%s\n", "Wrong answer");

                                break;

                            case ANS_2:

                                if( res == ans && fabs(x1 - ans_x1) < EPS && fabs(x2 - ans_x2) < EPS) printf("%s\n", "Approved");

                                else printf("%s\n", "Wrong answer");

                                break;

                            case ANS_3:

                                if( res == ans ) printf("%s\n", "Approved");

                                else printf("%s\n", "Wrong answer");

                                break;

                            default: printf("%s\n", "Unknown test");

                    }

                    break;

                default:
                            
                    printf("%s\n", "Unknown test key");
                            
                    break;
                    
                }
                
            }
            
        fclose(test);

        return ;

    }
    