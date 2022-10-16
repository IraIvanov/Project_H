#include <stdio.h>
#include "cmd.hpp"
#include <sys/stat.h>
#include <stdlib.h>
#include "stack.hpp"
#include <string.h>
#include "io.hpp"
#include <unistd.h>
/*
    SKIP REPLACE WITH TWO FUNC'S SKIP COMMENTS AND CHECK_SYNT
    ADD NAMED LABELS 
    ADD RAM AND JMP'S


*/
#include <math.h>

#define CMD_BYTES 0x1F
#define ARG_BYTES 0
double const EPSILON = 0.00000001;

int is_equal(el_type var1, el_type var2) {

    if ( abs(var1 - var2) < EPSILON ) return 0;

    return 1;

}

int execute (char *code, size_t code_size, stack* stk, stack* stk_addr, el_type* ram, size_t ram_size, el_type *regs ) {

    el_type var1 = 0, var2 = 0;
    el_type val = 0;
    int ip = 0;
    int tmp = 0;
    //printf("debug\n");
    while ( ip < code_size && ip >= 0) {
        var1 = 0;
        var2 = 0;
        val = 0;
        //printf("command is%0x %0x \n", code[ip], code[ip] & CMD_BYTES);
        switch (code[ip] & CMD_BYTES){
            
            case PUSH:
                //printf("pushing!\n");
                
                if( code[ip] & RAM_FLAG ){
                    if ( code[ip] & REG_FLAG ) {
                        ip++;
                        int arg = (int)regs[code[ip++]];
                        val = ram[arg];
                        //printf("pushing[%lf]\n", val);
                        //printf("RAM %lf\n", ram[arg]); 
                        stack_push(stk, val);
                        printf("pushing[%lf]\n", val);
                        break;
                    }
                    if (code[ip] & IMMED_FLAG) {
                        ip++;
                        int arg = *(int*)(code + ip);
                        ip += sizeof(int);
                        val = ram[arg];
                        stack_push(stk, val);
                        printf("pushing[%lf]\n", val);
                        break;
                        //printf("RAM %lf\n", ram[arg]);
                    }
                    
                }
                //printf("wtf %0x \n", code[ip] & REG_FLAG);
                if ( code[ip] & REG_FLAG ) {
                        ip++;
                        int arg = (int)code[ip++];
                        val = regs[arg];
                        stack_push(stk, val);
                        printf("pushing[%lf]\n", val);
                        break;
                }
                //printf("wtf %0x \n", code[ip] & IMMED_FLAG);
                if (code[ip] & IMMED_FLAG) {
                    ip++;
                    val = *(el_type*)(code + ip);
                    ip += sizeof(el_type);
                    stack_push(stk, val);
                    printf("pushing[%lf]\n", val);
                    break;
                }
                stack_push(stk, val);
                printf("pushing[%lf]\n", val);
                break;
            case ADD:

                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                printf("add var1[%lf]  var2[%lf]\n", var1, var2);
                stack_push(stk, var1 + var2);
                ip++;
                break;
            case SUB:

                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                printf("sub var1[%lf]  var2[%lf]\n", var1, var2);
                stack_push(stk, var2 - var1);
                ip++;
                break;
            case MUL:
                
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                printf("mul var1[%lf]  var2[%lf]\n", var1, var2);
                stack_push(stk, var1*var2);
                ip++;
                break;
            case DIV:

                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( is_equal( var1, 0 ) ){
                    printf("EXECUTING ERROR, DIVISION BY ZERO");
                    return -1;
                }
                printf("div var1[%lf]  var2[%lf]\n", var1, var2);
                stack_push(stk, var2 / var1);
                ip++;
                break;
            case OUT:
                
                stack_pop(stk, &var1);
                printf("%lf\n", var1);
                ip++;
                printf("next cmd is %c\n", code[ip]);
                break;
            case DUMP:
                dump(stk, LOG, 0);
                printf("dumping...\n");
                //dump_cpu(code, ip);
                ip++;
                break;
            case HLT:
                
                stack_dtor(stk);
                stack_dtor(stk_addr);
                printf("stack destroied\n");
                return 0;
                break;
            case JMP:
                ip++;
                //printf("%d\n", ip);
                ip = *((int*)(code + ip));
                //printf("%d\n", ip);
                //printf("%c\n", code[ip]);
                sleep(1);
                break;
            case POP:
                
                stack_pop(stk, &var1);

                if( code[ip] & 0x80 ){
                    if ( code[ip] & 0x40 ) {
                        ip++;
                        char arg = regs[code[ip++]];
                        ram[arg] = var1; 
                        //printf("RAM %lf\n", ram[arg]);
                    }
                    if (code[ip] & 0x20) {
                        ip++;
                        int arg = *(int*)(code + ip);
                        ip += sizeof(int);
                        ram[arg] = var1;
                        //printf("RAM %lf\n", ram[arg]);
                    }
                }

                if ( code[ip] & 0x40 ) {
                        ip++;
                        regs[code[ip++]] = var1;
                }
                //stack_push(stk, val);
                printf("popping[%lf]\n", var1);
                break;
            case DUP:
                stack_pop(stk, &var1);
                stack_push(stk, var1);
                stack_push(stk, var1);
                printf("duplicating %lf\n", var1);
                ip++;
                break;
            case CALL:
                ip++;
                //printf("%d\n", ip);
                tmp = ip + sizeof(int);
                memcpy(&val,&tmp, sizeof(int));
                //printf("%d", val);
                stack_push(stk_addr, val);
                ip = *((int*)(code + ip));
                //printf("%d\n", ip);
                //printf("%c\n", code[ip]);
                printf("calling function\n");
                //sleep(1);
                break;
            case RET:
                
                stack_pop(stk_addr, &val);
                memcpy(&ip, &val, sizeof(int));
                break;
            case JB:

                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) < 0  && is_equal(var1, var2) ) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
            case JBE:

                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) <= 0 || !is_equal(var1, var2)) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;    
            case JA:

                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) > 0 && is_equal(var1, var2)) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
            case JAE:

                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) >= 0 ||!is_equal(var1, var2) ) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
            case JE:

                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) == 0 || !is_equal(var1, var2 )) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
            case JNE:

                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) != 0 && is_equal(var1, var2) ) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;  
            default:
                printf("UNKNOWN COMMAND\n");
                return -1;
                break;
        }
    }

    return 0;
}


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
    if ( !stricmp(command, "JA\0")) return JA;
    if ( !stricmp(command, "JAE\0")) return JAE;
    if ( !stricmp(command, "JB\0")) return JB;
    if ( !stricmp(command, "JBE\0")) return JBE;
    if ( !stricmp(command, "JE\0")) return JE;
    if ( !stricmp(command, "JNE\0")) return JNE;
    if ( !stricmp(command, "CALL\0")) return CALL;
    if ( !stricmp(command, "RET\0")) return RET;
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
                    //printf("we are here : %d is int\n", num);
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
        case JA:

            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JA_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case JB:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JB_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case JAE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JAE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case JBE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JBE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case JE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case JNE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + JNE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case CALL:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( sscanf( cmd + CALL_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR\n");
            return -1;
        case RET:

            if ( check_synt(cmd + RET_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER RET\n");   
                    return -1;
            }
            return 1;
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
                        marks[i].line = line;
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
            int tmp_reg = -1;
            //printf("here may be problem %s\n", check_reg);
            //printf("here may be problem %s\n", check_reg + check_reg_offset);
            if( ( (tmp_reg = register_verify(check_reg + check_reg_offset) ) != REG_NUN )){
                //printf("tmp_reg is %d\n", tmp_reg);
                //printf("tmp_reg is %d\n", REG_NUN);
                //printf("here may be problem %s\n", check_reg + check_reg_offset);
                arr[tmp_ip++] += REG_FLAG;
                *ip += 1;
                *(arr + tmp_ip) = (char)tmp_reg;
                free(check_reg);
                *ip += sizeof(char);
                return 0;
            }
            //printf("here may be problem %s\n", check_reg + check_reg_offset);
            if ( sscanf( check_reg + check_reg_offset, "%d", &num) ) 
            {
                arr[tmp_ip++] += IMMED_FLAG;
                *ip += 1;
                //printf("we are here : %d is int\n", num);
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

    if (cmd_code >= JB && cmd_code <= JNE) {

        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

        if (sscanf(cmd, "%s", check_reg)) {
            
            arr[tmp_ip++] = cmd_code;
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

    if (cmd_code == CALL ) {

        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

        if (sscanf(cmd, "%s", check_reg)) {
            
            arr[tmp_ip++] = cmd_code;
            *ip += 1;
            
            for(size_t i = 0; i < marks_size; i++){

                if (!strcmp(marks[i].name, check_reg + 1)){
                    *(int*)(arr + tmp_ip) = marks[i].line;
                    //printf("%d\n", *(int*)(arr + tmp_ip));
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
                 case JB:
                    
                    if ( get_args(cmd[i] + offset + JB_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                 case JBE:
                    
                    if ( get_args(cmd[i] + offset + JBE_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                 case JA:
                    
                    if ( get_args(cmd[i] + offset + JA_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                 case JAE:
                    
                    if ( get_args(cmd[i] + offset + JAE_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                 case JE:
                    
                    if ( get_args(cmd[i] + offset + JE_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                 case JNE:
                    
                    if ( get_args(cmd[i] + offset + JNE_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
                        printf ("GETtTING ARGS ERROR\n");
                        return -1;
                    }
                    break;
                case CALL:

                    if ( get_args(cmd[i] + offset + CALL_S, cmd_code, &ip, marks, arr, marks_size) == -1) {
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