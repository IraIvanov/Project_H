#include <stdio.h>
#include <stdlib.h>
#ifndef MY_SORT_H_INCLUDED
#define MY_SORT_H_INCLUDED


#define dump(stk, out, err) do_dump(&stk, __FUNCTION__, __LINE__, __FILE__, out, err)
#define stack_ctor(stk, size) _stack_ctor(&stk, size, __LINE__, __FUNCTION__, __FILE__, #stk, fopen("log.txt", "a"))

const unsigned long long CANARY = 0xCAFEBABE;

typedef int el_type;

const el_type POISONED_EL = 0xD15EA5E;

#define POISONED_DATA (el_type*)3 

const unsigned resize_up_const = 2, resize_down_const = 4;



typedef struct Stack_info_t {

    const char* name;

    int line;

    const char* file;

    const char* func;

    FILE* log;

} stack_info;

enum ERRORS{
    STACK_NULL_PTR = 0b00000001,
    DATA_NULL_PTR  = 0b00000010,
    CAP_LES_SIZE   = 0b00000100,
    ZOMBIE_STAT    = 0b00001000,
    DATA_NULL_CAP  = 0b00010000,
    ALLOC_ERR      = 0b00100000,
    BAD_DATA_PTR   = 0b01000000,
    DEAD_CANARY    = 0b10000000,
    BAD_HASH       = 0b100000000,
};

/*enum CANARYS{

};*/

typedef struct Stack_t {

    unsigned long long LEFT_CANARY;

    el_type* data;

    size_t size;

    size_t capacity;

    int status;

    stack_info info;

    unsigned long long RIGHT_CANARY;
} stack;

unsigned int rot13 (void* ptr);

int _stack_ctor(stack* stk, size_t size, int line, const char* func, const char* file, const char* name, FILE* log);

int stack_resize(stack* stk);

int stack_push(stack* stk, el_type value);

static int stack_resize_down(stack* stk);

int stack_pop(stack* stk, el_type* value);

int stack_dtor(stack* stk);

void free_stack_inf(stack* stk);

static void set_stack_inf(stack* stk, int line, const char* func, const char* file, const char* name, FILE* log);

int func_verify(stack* stk);

void p_error(int err_code, FILE* out);

void print_stack_inf(stack* stk);

static void do_dump(stack* stk, const char* func, int line, const char* file, FILE* out, int err) ;

#endif