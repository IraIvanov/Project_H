#include <stdio.h>
#include "cmd.hpp"
#include <sys/stat.h>
#include <stdlib.h>
#include "../Stack/stack.hpp"
#include "io.hpp"

int code_read( el_type** code, FILE* input, size_t* size){

    if ( !code ) return BUFF_NULL_PTR;
    if ( !input ) return FILE_NULL_PTR;

    struct stat code_stat = { };

    if ( fstat ( fileno(input), &code_stat) == -1 ) return STATS_ERR;

    size_t code_size = (size_t)code_stat.st_size;

    *code = (el_type*)calloc( sizeof(el_type), code_size + 1);

    if ( code_size != fread(*code, sizeof(el_type), code_size, input)) return READ_ERR;

    (*code)[code_size] = 0;

    if ( size ) {
        *size = code_size + 1;
    }

    return 0;

}

void arr_print(el_type* arr, size_t size) {

    for(size_t i = 0; i < size; i++) {
        printf("%d\n", arr[i]);
    }

    return;
}

enum CMD command_verify ( char* command) {
    
    if ( !stricmp(command, "PUSH\0")) return PUSH;
    if ( !stricmp(command, "ADD\0")) return ADD;
    if ( !stricmp(command, "SUB\0")) return SUB;
    if ( !stricmp(command, "MUL\0")) return MUL;
    if ( !stricmp(command, "DIV\0")) return DIV;
    if ( !stricmp(command, "JMP\0")) return JMP;
    if ( !stricmp(command, ":\0")) return MARK;
    if ( !stricmp(command, "POP\0")) return POP;
    if ( !stricmp(command, "OUT\0")) return OUT;
    if ( !stricmp(command, "DUMP\0")) return DUMP;
    if ( !stricmp(command, "HLT\0")) return HLT;
    
    return NUN;
}

enum CMD code_verify ( el_type code) {
    
    if( code == PUSH) return PUSH;
    if( code == ADD) return ADD;
    if( code == SUB) return SUB;
    if( code == MUL) return MUL;
    if( code == DIV) return DIV;
    if( code == OUT) return OUT;
    if( code == DUMP) return DUMP;
    if( code == HLT) return HLT;

    return NUN;
}

enum REGS register_verify ( char* buf ) {

    if ( !stricmp(buf, "rax\0")) return rax;
    if ( !stricmp(buf, "rbx\0")) return rbx;
    if ( !stricmp(buf, "rcx\0")) return rcx;
    if ( !stricmp(buf, "rdx\0")) return rdx;

    return REG_NUN;
}

int analyse_verify( char* cmd ){
    
    char *command;
    sscanf(cmd, "%s", command);
    int command_num = command_verify(command);
    switch(command_num) {
        case PUSH:
            char *check_reg;
            int num;
            if (sscanf(cmd, "%d", &num)) return 2;
            if (sscanf(cmd, "[%d]", &num)) return 2;
            if (sscanf(cmd, "%s", check_reg)){
                if( register_verify(check_reg)) ;
            }
            break;
        case ADD:
            return 1;
        case SUB:
            return 1;
        case MUL:
            return 1;
        case DIV:
            return 1;
        case OUT:
            return 1;
        case DUMP:
            return 1;
        case JMP:

            break;
        case HLT:
            return 1;
        case MARK:

            break;
        case DUP:
            return 1;
        case POP:
            break;
        default:
            printf("SYNTAX ERROR, UNKNOWN COMMAND\n");
            return -1;
            break;
    }

}

int cmd_analyse(char** cmd, size_t cmd_size) {

    for ( size_t i = 0; i < cmd_size; i++) {

    }
}