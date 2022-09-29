#include <stdio.h>
#include <stdlib.h>
#include "stack.hpp"

#define stack_inf(stk, out) set_stack_inf(&stk, __LINE__, __FUNCTION__ , __FILE__ , #stk, out)
//#define dump(stk, out) do_dump(&stk, __FUNCTION__, __LINE__, __FILE__, out)

int main() {

    stack stk1 = {};
    stack_ctor(stk1, 5);
    /*stack_push(&stk1, 1);
    stack_push(&stk1, 3);
    stack_push(&stk1, 5);*/
    el_type value = 0;

    //stack_pop(&stk1, &value);
    //print_stack_inf(&stk1);
    //printf("%d\n", value);
    for(int i = 0; i < 10; i++) {
        stack_push(&stk1, i);
    }
    stk1.LEFT_CANARY = 1;
    for(int i = 0; i < 10; i++) {
        stack_pop(&stk1, &value);
        printf("poped %d\n", value);
    }
    stack_dtor(&stk1);
    return 0;
}