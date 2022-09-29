#include <stdio.h>
#include <stdlib.h>
#ifndef MY_SORT_H_INCLUDED
#define MY_SORT_H_INCLUDED



typedef int el_type;

typedef struct Stack_info_t {

    char* name;

    int line;

    char* file;

    char* func;

    FILE* log;

} stack_info;

enum ERRORS{
    STACK_NULL_PTR = 0b00000001,
    DATA_NULL_PTR  = 0b00000010,
    CAP_LES_SIZE   = 0b00000100,
    ZOMBIE_STAT    = 0b00001000,
    DATA_NULL_CAP  = 0b00010000,
    RESIZE_ERR     = 0b00100000
};

/*enum CANARYS{

};*/

typedef struct Stack_t {
    el_type* data;

    size_t size;

    size_t capacity;

    int status;

    stack_info info;

} stack;

int stack_ctor(stack* stk, size_t size);

int stack_resize(stack* stk);

int stack_push(stack* stk, el_type value);

static int stack_resize_down(stack* stk);

int stack_pop(stack* stk, el_type* value);

int stack_dtor(stack* stk);

void free_stack_inf(stack* stk);

void set_stack_inf(stack* stk, int line, const char* func, const char* file, const char* name, FILE* log);

int func_verify(stack* stk);

void p_error(int err_code);

void print_stack_inf(stack* stk);

static void do_dump(stack* stk, const char* func, int line, const char* file, FILE* out);

#endif