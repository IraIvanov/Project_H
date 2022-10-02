#include <stdio.h>
#include <stdlib.h>
#include "stack.hpp"
//#define dump(stk, out) do_dump(&stk, __FUNCTION__, __LINE__, __FILE__, out)

int main() {

    stack stk1 = {};
    stack_ctor(stk1, 5);
    el_type value = 0;

    //stack_pop(&stk1, &value);
    //print_stack_inf(&stk1);
    //printf("%d\n", value);
    for(int i = 0; i < 100; i++) {
        stack_push(NULL, i);
    }
    for(int i = 0; i < 100; i++) {

        stack_pop(NULL, &value);
        printf("poped %d\n", value);
    }
    stack_dtor(NULL);
    stack_dtor(&stk1);
    return 0;
}