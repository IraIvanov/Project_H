#include <stdio.h>
#include "cmd.hpp"
#include <sys/stat.h>
#include <stdlib.h>
#include "../Stack/stack.hpp"
#include <string.h>
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

int analyse_verify( char* cmd, int* marks, size_t marks_size, int line) {
    
    char *command = (char*)calloc(sizeof(char), MAX_CMD_SIZE);
    //printf("scanning\n");
    //printf("command_ptr[%p], cmd_ptr[%p]\n", (void*)command, (void*)*cmd);
    sscanf(cmd, "%s", command);
    //printf("command[%s]\n", command);
    size_t offset = (size_t)(strstr(cmd, command) - cmd); 
    int command_num = command_verify(command);
    free(command);
    char *check_reg;
    el_type num = 0;
    size_t mark = 0; 
    switch(command_num) {
        case PUSH:
            
            if (sscanf(cmd + MAX_CMD_SIZE + offset, "%d", &num) || sscanf(cmd + MAX_CMD_SIZE + offset, "[%d]", &num)){
            return sizeof(char) + sizeof(el_type);
            }
            check_reg = (char*)calloc(sizeof(char), MAX_CMD_SIZE);
            if (sscanf(cmd + MAX_CMD_SIZE + offset, "[%s]", check_reg) || sscanf(cmd + MAX_CMD_SIZE + offset, "%s", check_reg)){
                if( register_verify(check_reg) != REG_NUN ){
                    free(check_reg);
                    return 2;
                }
            }
            return -1;
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
            
            if (sscanf(cmd + MAX_CMD_SIZE + offset, "%lu:", &mark) || sscanf(cmd + MAX_CMD_SIZE + offset, "%lu", &mark)) return sizeof(char) + sizeof(size_t);
            return -1;
        case HLT:
            return 1;
        case MARK:
            if (sscanf(cmd + MAX_CMD_SIZE + offset, ":%lu", &mark) &&  mark < marks_size ) {
                marks[mark] = line;
                return 0;
            }
            return -1;
        case DUP:
            return 1;
        case POP:
            if (sscanf(cmd + MAX_CMD_SIZE + offset, "[%d]", &num)) return 1 + sizeof(el_type);
            check_reg = (char*)calloc(sizeof(char), MAX_CMD_SIZE);
            if (sscanf(cmd + MAX_CMD_SIZE + offset, "[%s]", check_reg) || sscanf(cmd + MAX_CMD_SIZE + offset, "%s", check_reg)){
                if( register_verify(check_reg) != REG_NUN ) {
                    free(check_reg);
                    return 2;
                }
            }
            return -1;
        default:
            printf("SYNTAX ERROR, UNKNOWN COMMAND\n");
            return -1;
    }
    return 0;
}

size_t cmd_analyse(char** cmd, size_t cmd_size, int* marks, size_t marks_size) {

    size_t sum = 0;
    int tmp = 0;

    for ( int i = 0; i < (int)cmd_size; i++) {

    if ((tmp = analyse_verify(cmd[i], marks, marks_size, i) ) == -1 ){
            
            printf("SYNTAX ERROR\n");
            return 0;
        
        }
        
        sum += (size_t)tmp; 
        
        
    }
    return sum;
}

int get_args(char* command, int cmd_code, size_t *ip, int* marks, char *arr){

    size_t tmp_ip = *ip;

    if (cmd_code == PUSH) {

        el_type value = -1;
        
        char* check_reg;
        //int scanf_ret = sscanf(command, "%d", &value);
        //printf("scanf -> %d\n", scanf_ret);
                
        if (sscanf(command, "%d", &value)){
            arr[tmp_ip++] = cmd_code + IMMED_FLAG;
            *ip += 1;
            //printf("value[%d]\n",value);
            *(el_type*)(arr + tmp_ip) = value;
            *ip += sizeof(el_type);
            return 0;
        }

        if ( sscanf(command, "[%d]", &value) != 0){
            arr[tmp_ip++] = cmd_code + IMMED_FLAG + RAM_FLAG;
            *ip += 1;
            *(el_type*)(arr + tmp_ip) = value;
            *ip += sizeof(el_type);
            return 0;
        }
        check_reg = (char*)calloc(sizeof(char), MAX_CMD_SIZE);

        if (sscanf(command, "[%s]", check_reg) != 0) {
            
            arr[tmp_ip++] = cmd_code + RAM_FLAG + REG_FLAG;

            *ip += 1;
            
            if ( (*(int*)(arr + tmp_ip) = register_verify(check_reg))  == REG_NUN){
                free(check_reg);
                printf("SYNTAX ERROR, UNKNOWN REGISTER NAME\n");
                return -1;
            
            }
            free(check_reg);
            *ip += sizeof(int);
            return 0;
        }

        printf("SYNTAX ERROR, PUSH HAS WRONG ARG\n");
        return -1;
                
    }

    if (cmd_code == JMP) {

        int mark = -2;

        if (sscanf(command, "%d:", &mark)  != 0) {
            
            arr[tmp_ip++] = JMP;

            *ip += 1;
            
            if ((*(int*)(arr + tmp_ip) = marks[mark]) == -1){
                printf("SYNTAX ERROR, UNKNWON MARK");
                return -1;
            }
            
            *ip += sizeof(int);

            return 0;
        
        }
        if (sscanf(command, "%d", &mark)) {
            
            arr[tmp_ip++] = JMP;
            *ip += 1;
            
            if ((*(int*)(arr + tmp_ip) = mark) < 0){
                printf("SYNTAX ERROR, MARK IS BELOW ZERO\n");
                return -1;
            }

            *ip += sizeof(int);

            return 0;

        }

        return -1;
    
    }

    if ( cmd_code == POP ) {
        
        el_type value = 0;
        char* check_reg;
        
        if (sscanf(command, "[%d]", &value)){

            arr[tmp_ip++] = POP + RAM_FLAG + IMMED_FLAG;
            *ip += 1;
            *(el_type*)(arr + tmp_ip) = value;
            return 0;
        }
        check_reg = (char*)calloc(sizeof(char), MAX_CMD_SIZE);
        if (sscanf(command, "[%s]", check_reg)) {

            arr[tmp_ip++] = POP + RAM_FLAG + REG_FLAG;
            *ip += 1;

            if ( (*(int*)(arr + tmp_ip) = register_verify(check_reg))  == REG_NUN){
                free(check_reg);
                printf("SYNTAX ERROR, UNKNOWN REGISTER NAME\n");
                return -1;
            
            }
            free(check_reg);
            *ip += sizeof(int);
            return 0;

        }
        
        if (sscanf(command, "%s", check_reg)) {

            arr[tmp_ip++] = POP + RAM_FLAG + REG_FLAG;
            *ip += 1;

            if ( (*(int*)(arr + tmp_ip) = register_verify(check_reg))  == REG_NUN){
                
                free(check_reg);
                printf("SYNTAX ERROR, UNKNOWN REGISTER NAME\n");
                return -1;
            
            }
            free(check_reg);
            *ip += sizeof(int);
            return 0;

        }

        return -1;
    }
    printf("SYNTAX ERROR, WRONG\n");
    return -1;
}

int assamble(char** cmd, size_t cmd_size, int* marks, char* arr) {

    char * command = (char*)calloc(sizeof(char), MAX_CMD_SIZE);
    int cmd_code = 0;
    size_t ip = 0;
    size_t offset = 0;
    for( size_t i = 0; i < cmd_size; i++) {
        
        sscanf(cmd[i], "%s", command);
        offset = (size_t)(strstr(cmd[i], command) - cmd[i]);
        //printf("offset is [%lu]\n command is [%s]", offset, command);
        if( (cmd_code = command_verify(command)) != NUN ) {

            switch(cmd_code){
                case PUSH:
                    
                    if ( get_args(cmd[i] + offset + MAX_CMD_SIZE, cmd_code, &ip, marks, arr) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                case JMP:
                    
                    if ( get_args(cmd[i] + offset + MAX_CMD_SIZE, cmd_code, &ip, marks, arr) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                case POP:
                    
                    if ( get_args(cmd[i] + offset + MAX_CMD_SIZE, cmd_code, &ip, marks, arr) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                default:
                    arr[ip++] = cmd_code;

            }
        } else return -1;
    }

    free(command);

    return 0;

}