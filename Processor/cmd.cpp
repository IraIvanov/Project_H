#include <stdio.h>
#include "cmd.hpp"
#include <sys/stat.h>
#include <stdlib.h>
#include "../Stack/stack.hpp"
#include <string.h>
#include "io.hpp"
/*
    SKIP REPLACE WITH TWO FUNC'S SKIP COMMENTS AND CHECK_SYNT
    ADD NAMED LABELS 
    ADD RAM AND JMP'S


*/
int code_read( char** code, FILE* input, size_t* size){

    if ( !code ) return BUFF_NULL_PTR;
    if ( !input ) return FILE_NULL_PTR;

    struct stat code_stat = { };

    if ( fstat ( fileno(input), &code_stat) == -1 ) return STATS_ERR;

    size_t code_size = (size_t)code_stat.st_size;

    *code = (char*)calloc( sizeof(char), code_size + 1);

    if ( code_size != fread(*code, sizeof(el_type), code_size, input)) return READ_ERR;

    (*code)[code_size] = 0;

    if ( size ) {
        *size = code_size + 1;
    }

    return 0;

}

int check_synt( char* str) {
    
    if ( !str ) {
        printf("ERROR WASN'T INPUTED TI CHO EBLAN BLYAT !!!!\n");
        return -1;
    }

    for(size_t i = 0; str[i] != '\0'; i++) {
        
        if (str[i] != ' ') {
            return -1;
        }

    }

    return 0;
}

int skip_comments(char *str) {
    
    if ( !str ) {
        printf("ERROR WASN'T INPUTED TI CHO EBLAN BLYAT !!!!\n");
        return -1;
    }

    for(size_t i = 0; str[i] != '\0'; i++) {
        
        if (str[i] == ';') {
            str[i] = '\0';
            return 0;
        }

    }

    return 0;
}

int skip_sqares(char *str) {
    
    if ( !str ) {
        printf("ERROR WASN'T INPUTED TI CHO EBLAN BLYAT !!!!\n");
        return -1;
    }

    if ( str[0] != '[') return 0;

    str += 1;
    size_t len = strlen(str);
    for(size_t i = len; i > 0; i--) {

        if ( str[i] == ']'){
            str[i] = '\0';
            return 1;
        }
    }

    return 0;
}

void arr_print(char* arr, size_t size) {

    for(size_t i = 0; i < size; i++) {
        printf("%c\n", arr[i]);
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
    if ( command[0] == ':') return MARK;
    if ( !stricmp(command, "POP\0")) return POP;
    if ( !stricmp(command, "OUT\0")) return OUT;
    if ( !stricmp(command, "DUMP\0")) return DUMP;
    if ( !stricmp(command, "HLT\0")) return HLT;
    if ( !stricmp(command, "DUP\0")) return DUP;
    
    return NUN;
}

enum CMD code_verify ( el_type code) {
    
    if( code == PUSH) return PUSH;
    if( code == ADD) return ADD;
    if( code == SUB) return SUB;
    if( code == MUL) return MUL;
    if( code == DIV) return DIV;
    if( code == OUT) return OUT;
    if (code == DUP) return DUP;
    if (code == JMP) return JMP;
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

int labels_ctor(label** marks, size_t MARKS_SIZE){
    
    if(!marks) {
        printf("MAKRS HAVE NULL PTR TI CHO EBLAN&\n");
        return -1;
    }
    *marks = (label*)calloc(sizeof(label), MARKS_SIZE);
    for(size_t i = 0; i < MARKS_SIZE; i++) {
        (*marks)[i].name = "\0";
        (*marks)[i].line = -1;
    }
    return 0;
}

int labels_dtor(label* marks){
    
    if(!marks) {
        printf("MAKRS HAVE NULL PTR TI CHO EBLAN&\n");
        return -1;
    }
    for(size_t i = 0; i < MARKS_SIZE; i++) {
        if(strcmp(marks[i].name, "\0")){
            free(marks[i].name);
        }
    }
    return 0;
}

int analyse_verify( char* cmd, label* marks, size_t marks_size, int line) {
    
    char *command = (char*)calloc(sizeof(char), MAX_LINE);
    //printf("scanning\n");
    //printf("command_ptr[%p], cmd_ptr[%p]\n", (void*)command, (void*)*cmd);
    sscanf(cmd, "%s", command);
    //printf("command[%s]\n", command);
    int offset = strstr(cmd, command) - cmd; 
    skip_comments(cmd);
    int command_num = command_verify(command);
    free(command);
    char *check_reg;
    el_type value = 0;
    int num;
    int mark = 0; 
    int offset2 = 0;
    switch(command_num) {
        case PUSH:

            if (sscanf(cmd + PUSH_S + offset, "%lf %n", &value, &offset2)) {
                if ( check_synt(cmd + PUSH_S + offset + offset2)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION\n");   
                    return -1;
                }
                return sizeof(char) + sizeof(el_type);
            }
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if (sscanf(cmd + PUSH_S + offset, "%s", check_reg)) {
                int check_reg_offset = skip_sqares(check_reg);
                //printf("%s\n", check_reg);
                if( register_verify(check_reg + check_reg_offset) != REG_NUN ){
                    //printf("%s\n", check_reg);
                    //printf("%s\n", cmd + MAX_CMD_SIZE + offset);
                    free(check_reg);
                    
                    return 2;
                }
                if ( sscanf( check_reg + check_reg_offset, "%d %n", &num, &offset2) ) {
                    if ( check_synt(check_reg + check_reg_offset + offset2)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION\n");   
                    return -1;
                }

                free(check_reg);
                return sizeof(char) + sizeof(int);
                }
                printf("PUSH ERROR with registers or RAM\n");
                return -1;
            }
            printf("%s\n", cmd + PUSH_S + offset);
            printf("PUSH ERROR\n");
            return -1;
        case ADD:
            if ( check_synt(cmd + ADD_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER ADD\n");   
                    return -1;
            }
            return 1;
        case SUB:
            if ( check_synt(cmd + SUB_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER SUB\n");   
                    return -1;
            }
            return 1;
        case MUL:
            if ( check_synt(cmd + MUL_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER MUL\n");   
                    return -1;
            }
            return 1;
        case DIV:
            if ( check_synt(cmd + DIV_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER DIV\n");   
                    return -1;
            }
            return 1;
        case OUT:
            if ( check_synt(cmd + OUT_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER OUT\n");   
                    return -1;
            }
            return 1;
        case DUMP:
            if ( check_synt(cmd + DUMP_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER DUMP\n");   
                    return -1;
            }
            return 1;
        case JMP:
            
            if ( sscanf(cmd + JMP_S + offset, "%d", &mark)) return sizeof(char) + sizeof(int);
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JMP_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case HLT:
            if ( check_synt(cmd + HLT_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER HLT\n");   
                    return -1;
            }
            return 1;
        case MARK:
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if (sscanf(cmd + MARK_S + offset, "%s", check_reg)) {
                
                for(size_t i = 0 ; i < marks_size; i++) {
                    if( (marks[i]).line == -1 ) {
                        //printf("line %d", line);
                        marks[i].line = line + 1;
                        marks[i].name = check_reg;
                        return 0;
                    }
                }
                printf("ERROR, DON'T HAVE PLACE FOR ANOTHER LABEL");
                return -1;
            }
            printf("LABEL ERROR, LABEL HAVE NO NAME\n");
            return -1;
        case DUP:
            if ( check_synt(cmd + DUP_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER DUP\n");   
                    return -1;
            }
            return 1;
        case POP:
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if (sscanf(cmd + POP_S + offset, "%s", check_reg)){
                int check_reg_offset = skip_sqares(check_reg);
                if( register_verify(check_reg + check_reg_offset) != REG_NUN ) {
                    free(check_reg);
                    return 2;
                }
                 if ( sscanf( check_reg + check_reg_offset, "%d %n", &num, &offset2) ) {
                    if ( check_synt(check_reg + check_reg_offset + offset2)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION\n");   
                    return -1;
                }

                free(check_reg);
                return sizeof(char) + sizeof(int);
                }
                printf("POP ERROR with registers\n");
                return -1;
            }
            printf("POP ERROR\n");
            return -1;
        default:
            printf("SYNTAX ERROR, UNKNOWN COMMAND\n");
            return -1;
    }
    return 0;
}

int cmd_analyse(char** cmd, size_t cmd_size, label* marks, size_t marks_size) {

    int sum = 0;
    int tmp = 0;

    for ( int i = 0; i < (int)cmd_size; i++) {

    if ((tmp = analyse_verify(cmd[i], marks, marks_size, (int)sum) ) == -1 ){
            
            printf("SYNTAX ERROR IN FIRST ANALYSIS\n");
            return 0;
        
        }
        //printf("sum %d\n", sum);
        sum += tmp; 
        
        
    }
    return sum;
}

int get_args(char* cmd, int cmd_code, size_t *ip, label* marks, char *arr, size_t marks_size){

    size_t tmp_ip = *ip;

    char* check_reg;
    if (cmd_code == PUSH) {

        el_type value = -1;

        int num = 0;
        
        
        //int scanf_ret = sscanf(command, "%d", &value);
        //printf("scanf -> %d\n", scanf_ret);

        if (sscanf(cmd, "%lf", &value)){
            arr[tmp_ip++] = cmd_code + IMMED_FLAG;
            *ip += 1;
            //printf("value[%d]\n",value);
            *(el_type*)(arr + tmp_ip) = value;
            *ip += sizeof(el_type);
            return 0;
        }
        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

        if (sscanf(cmd, "%s", check_reg)) {
            int check_reg_offset = skip_sqares(check_reg);
            if ( check_reg_offset != 0 ) arr[tmp_ip] = cmd_code + RAM_FLAG;
            else arr[tmp_ip] = cmd_code;
            char tmp_reg = -1;
            if( ( (tmp_reg = (char)register_verify(check_reg + check_reg_offset) ) != REG_NUN )){
                
                arr[tmp_ip++] += REG_FLAG;
                *ip += 1;
                *(arr + tmp_ip) = tmp_reg;
                free(check_reg);
                *ip += sizeof(char);
                return 0;
            }
            
            if ( sscanf( check_reg + check_reg_offset, "%d", &num) ) 
            {
                arr[tmp_ip++] += IMMED_FLAG;
                *ip += 1;
                *(int*)(arr + tmp_ip) = num;
                *ip += sizeof(int);
                free(check_reg);
                return 0;
            }

        }
        free(check_reg);
        printf("SYNTAX ERROR, PUSH HAS WRONG ARG\n");
        return -1;
                
    }

    if (cmd_code == JMP) {

        int mark = -2;

        if (sscanf(cmd, "%d", &mark)  != 0) {
            
            arr[tmp_ip++] = JMP;
            *ip += 1;
            *(int*)(arr + tmp_ip) = mark;
            *ip += sizeof(int);

            return 0;
        
        }  

        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

        if (sscanf(cmd, "%s", check_reg)) {
            
            arr[tmp_ip++] = JMP;
            *ip += 1;
            
            for(size_t i = 0; i < marks_size; i++){

                if (!strcmp(marks[i].name, check_reg + 1)){
                    *(int*)(arr + tmp_ip) = marks[i].line;
                    *ip += sizeof(int);
                    free(check_reg);
                    return 0;
                }
            }

            printf("ERROR, UNKNOWN LABEL\n");
            free(check_reg);
            return -1;

        }

        return -1;
    
    }

    if ( cmd_code == POP ) {
        
        el_type value = 0;
        int num = 0;
        char* check_reg;
        



        check_reg = (char*)calloc(sizeof(char), MAX_LINE);
        if (sscanf(cmd, "%s", check_reg)){
            int check_reg_offset = skip_sqares(check_reg);
            if (check_reg_offset != 0) arr[tmp_ip] = cmd_code + RAM_FLAG;
            else  arr[tmp_ip] = cmd_code;
            int tmp_reg = -1;
            if( (tmp_reg = register_verify(check_reg + check_reg_offset)) != REG_NUN ) {
                
                arr[tmp_ip++] += REG_FLAG;
                *ip += 1;
                *(arr + tmp_ip) = (char)tmp_reg;
                free(check_reg);
                *ip += sizeof(char);
                return 0;
            }
            if ( sscanf( check_reg + check_reg_offset, "%d", &num) ) {
                
                arr[tmp_ip++] += IMMED_FLAG;
                *ip += 1;
                *(int*)(arr + tmp_ip) = num;
                *ip += sizeof(int);
                free(check_reg);
                return 0;
            }

            free(check_reg);
            return 0;
        }
         free(check_reg);
        printf("SYNTAX ERROR POP, WRONG ARGS\n");
        return -1;    
    }
    printf("SYNTAX ERROR, WRONG FUNC\n");
    return -1;
}
    


int assamble(char** cmd, size_t cmd_size, label* marks, char* arr, size_t marks_size) {

    char * command = (char*)calloc(sizeof(char), MAX_LINE);
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
                    
                    if ( get_args(cmd[i] + offset + PUSH_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                case JMP:
                    
                    if ( get_args(cmd[i] + offset + JMP_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                case POP:
                    
                    if ( get_args(cmd[i] + offset + POP_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                case MARK:
                    break;
                default:
                    arr[ip++] = cmd_code;

            }
        } else return -1;
    }

    free(command);

    return 0;

}