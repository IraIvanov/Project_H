#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "stack.hpp"
#include "io.hpp"

#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED
#define DEF_CMD(name, num, length, arg, exec_code)\
    name = num,
enum CMD {
    #include "cmd_info.hpp"
};
#undef DEF_CMD

#define DEF_CMD(name, num, length, arg, exec_code)\
    name##_S = length,

enum CMD_SIZE {
    #include "cmd_info.hpp"
};
#undef DEF_CMD

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
    REG_NUN = 255,

};
//100 11111
//765 43210
enum flags {
    RAM_FLAG   = 0b10000000,
    REG_FLAG   = 0b01000000,
    IMMED_FLAG = 0b00100000,
};

enum ASSEMBLE_ERRORS {
    NULL_PTR       = -1,
    SYNT_ERROR     = -1,
    LENGTH_ERR     = -1,
    AS_ALLOC_ERR   = -1,
    LABEL_MEM_ERR  = -1,
    LABEL_NAME_ERR = -1,
    GET_ARGS_ERR   = -1,
    ASSEMBLE_ERR   = -1,
};
#define SIGNATURE "SP"

const char VERSION = 3;
const size_t MIN_STACK_SIZE = 64;
const size_t MARKS_SIZE = 256; 
const size_t MAX_CMD_SIZE = 5;
const size_t MAX_LINE= 1024;
const int OFF = 0;
const int ON = 1;

const size_t REG_SIZE = 4; 
const size_t RAM_SIZE = 1024;

int code_read( char** code, FILE* input, size_t* size);

void arr_print(const char* arr, const size_t size);

int command_verify ( const char* command);

int code_verify ( const el_type code);

//void dump_proc(el_type* code, )
enum REGS register_verify ( const char* buf );

int analyse_verify( char* cmd, label* marks, const size_t marks_size, const int ip, const int line);

int cmd_analyse(char** cmd, const size_t cmd_size, label* marks, const size_t marks_size);

int get_args(char* cmd, const int cmd_code, size_t *ip, const label* marks, char *arr, const size_t marks_size);

int assemble( char** cmd,const size_t cmd_size, const label* marks, char* arr, const size_t marks_size);

int check_synt(const char* str);

int skip_comments(char *str);

int skip_sqares(char *str);

int labels_ctor(label** marks, const size_t MARKS_SIZE);

int labels_dtor(label* marks);

int execute (const char *code, const size_t code_size, stack* stk, stack* stk_addr, el_type* ram, const size_t ram_size, el_type *regs );

int is_equal(const el_type var1, const el_type var2);



#endif