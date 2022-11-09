#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#define CANARY_PROT
#define NDEBUG
#ifdef NDEBUG
#define S_LOG fopen("s_log.txt", "a")
#else
#define S_LOG
#endif

#ifdef CANARY_PROT
#define CANARY_SIZE sizeof(unsigned long long)
#else
#define CANARY_SIZE 0
#endif

#define stack_ctor(stk, size) _stack_ctor(&stk, size, __LINE__, __FUNCTION__, __FILE__, #stk/*, fopen("log.txt", "w")*/)


const unsigned long long CANARY = 0xCAFEBABE;

typedef tree_t* el_type;

const el_type POISONED_EL = (el_type)0xD15EA5E;

#define el_size sizeof(el_type)
#define POISONED_DATA (el_type*)3 
#define DEF_STK_SIZE 64

const unsigned resize_up_const = 2, resize_down_const = 4;



typedef struct Stack_info_t {

    const char* name;

    int line;

    const char* file;

    const char* func;

    //FILE* log;

} stack_info;

enum ERRORS{
    STACK_NULL_PTR = 0b00000001,
    DATA_NULL_PTR  = 0b00000010,
    CAP_LES_SIZE   = 0b00000100,
    ZOMBIE_STAT    = 0b00001000,
    DATA_NULL_CAP  = 0b00010000,
    S_ALLOC_ERR      = 0b00100000,
    BAD_DATA_PTR   = 0b01000000,
    DEAD_CANARY    = 0b10000000,
    BAD_HASH       = 0b100000000,
};

/*enum CANARYS{

};*/

typedef struct Stack_t {
    #ifdef CANARY_PROT
    unsigned long long LEFT_CANARY;
    #endif
    
    el_type* data;

    size_t size;

    size_t capacity;

    int status;

    #ifdef NDEBUG
    stack_info info;
    #endif

    #ifdef HASH_PROT
    unsigned long long hash;
    #endif

    #ifdef CANARY_PROT
    unsigned long long RIGHT_CANARY;
    #endif
} stack;


unsigned int rot13 (void* ptr, size_t size);

int _stack_ctor(stack* stk, size_t size, int line, const char* func, const char* file, const char* name/*, FILE* log*/);


int stack_push(stack* stk, el_type value);

int stack_pop(stack* stk, el_type* value);

int stack_dtor(stack* stk);

void free_stack_inf(stack* stk);


int func_verify(stack* stk);

void p_error(int err_code, FILE* out);

void print_stack_inf(stack* stk);

void do_dump(stack* stk, const char* func, int line, const char* file, FILE* out, int err);

#define dump(stk, out, err) do_dump(stk, __FUNCTION__, __LINE__, __FILE__, out, err)

#endif