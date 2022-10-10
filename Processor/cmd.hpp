#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../Stack/stack.hpp"
#include "io.hpp"

#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

enum CMD {
    HLT  = 0,
    PUSH = 1,
    ADD  = 2,
    SUB  = 3,
    MUL  = 4,
    DIV  = 5,
    OUT  = 6,
    DUMP = 7,
    JMP  = 8,
    MARK = 9,
    POP  = 10,
    DUP  = 11,
    NUN  = 1488,
};

enum REGS {
    
    rip = 0,
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4,
    REG_NUN = 1488,

};
#define SIGNATURE "SP"

const int VERSION = 1;
const size_t MIN_STACK_SIZE = 64;

int code_read( el_type** code, FILE* input, size_t* size);

void arr_print(el_type* arr, size_t size);

enum CMD command_verify ( char* command);

enum CMD code_verify ( el_type code);

//void dump_proc(el_type* code, )


#endif