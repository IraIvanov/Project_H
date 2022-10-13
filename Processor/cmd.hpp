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

enum CMD_SIZE {
    HLT_S  = 3,
    PUSH_S = 4,
    ADD_S  = 3,
    SUB_S  = 3,
    MUL_S  = 3,
    DIV_S  = 3,
    OUT_S  = 3,
    DUMP_S = 4,
    JMP_S  = 3,
    POP_S  = 3,
    MARK_S = 1,
    DUP_S  = 3,
    NUN_S  = 1488,
};

typedef struct label_t {
    int line;
    char* name;
} label;

enum REGS {
    
    rip = 0,
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4,
    REG_NUN = 1488,

};
//100 11111
//765 43210
enum flags {
    RAM_FLAG   = 0x40,
    REG_FLAG   = 0x30,
    IMMED_FLAG = 0x20,
};

#define SIGNATURE "SP"

const char VERSION = 2;
const size_t MIN_STACK_SIZE = 64;
const size_t MARKS_SIZE = 256; 
const size_t MAX_CMD_SIZE = 5;
const size_t MAX_LINE= 1024;
const int OFF = 0;
const int ON = 1;

int code_read( el_type** code, FILE* input, size_t* size);

void arr_print(char* arr, size_t size);

enum CMD command_verify ( char* command);

enum CMD code_verify ( el_type code);

//void dump_proc(el_type* code, )
enum REGS register_verify ( char* buf );

int analyse_verify( char* cmd, label* marks, size_t marks_size, int line);

int cmd_analyse(char** cmd, size_t cmd_size, label* marks, size_t marks_size);

int get_args(char* cmd, int cmd_code, size_t *ip, label* marks, char *arr, size_t marks_size);

int assamble(char** cmd, size_t cmd_size, label* marks, char* arr, size_t marks_size);

int check_synt( char* str);

int skip_comments(char *str);

int skip_sqares(char *str);

int labels_ctor(label** marks, size_t MARKS_SIZE);

int labels_dtor(label* marks);


#endif