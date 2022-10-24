#include <stdio.h>
#include "cmd.hpp"
#include <sys/stat.h>
#include <stdlib.h>
#include "stack.hpp"
#include <string.h>
#include "io.hpp"
#include <unistd.h>

#include <math.h>

#define CMD_BYTES 0x1F
#define ARG_BYTES 0
double const EPSILON = 0.00000001;

int is_equal(const el_type var1, const el_type var2) {

    if ( abs(var1 - var2) < EPSILON ) return 0;

    return 1;

}
//#define COMMA ,
#define DEF_CMD(name, num, length, arg, exec_code)\
    case num:\
    exec_code


int execute (const char *code, const size_t code_size, stack* stk, stack* stk_addr, el_type* ram, const size_t ram_size, el_type *regs ) {

    if ( !code || !stk || !stk_addr || !ram || !regs ){
        printf("EXEC ERROR, NULL PTR\n");
        return NULL_PTR;
    }
    el_type var1 = 0, var2 = 0;
    el_type val = 0;
    int ip = 0;
    int tmp = 0;

    while ( ip < code_size && ip >= 0) {
        var1 = 0;
        var2 = 0;
        val = 0;

        switch (code[ip] & CMD_BYTES){
            #include "cmd_info.hpp"  
            default:
                printf("UNKNOWN COMMAND\n");
                stack_dtor(stk);
                stack_dtor(stk_addr);
                return -1;
                break;
        }
    }
    stack_dtor(stk);
    stack_dtor(stk_addr);
    return 0;
}
#undef DEF_CMD

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

int check_synt(const char* str) {
    
    if ( !str ) {
        printf("ERROR WASN'T INPUTED IN CHECK_SYNT!!!!\n");
        return NULL_PTR;
    }

    for(size_t i = 0; str[i] != '\0'; i++) {
        
        if (str[i] != ' ') {
            return SYNT_ERROR;
        }

    }

    return 0;
}

int skip_comments(char *str) {
    
    if ( !str ) {
        printf("ERROR WASN'T INPUTED IN SKIP_COMMENTS!!!!\n");
        return NULL_PTR;
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
        printf("ERROR WASN'T INPUTED IN SKIP_SQUARES!!!!\n");
        return NULL_PTR;
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

void arr_print(const char* arr, const size_t size) {

    for(size_t i = 0; i < size; i++) {
        printf("%c\n", arr[i]);
    }

    return;
}

#define DEF_CMD(name, num, length, arg, exec_code)\
    if ( num == MARK) {\
    if ( command[0] == ':') return num;\
    }\
    if ( !stricmp(command, #name)) return num;

int command_verify ( const char* command) {
    
    if ( !command ){

        printf("ERROR, COMAND_VERIFY NULL PTR\n");
        return NUN;
    }

    #include "cmd_info.hpp"
    
    return NUN;
}
#undef DEF_CMD

#define DEF_CMD(name, num, length, arg, exec_code)\
    if ( code == num ) return num;

int code_verify ( const el_type code) {

    if ( code == MARK) return NUN;

    #include "cmd_info.hpp"

    return NUN;
}

#undef DEF_CMD

enum REGS register_verify ( const char* buf ) {

    if ( !buf ){

        printf("ERROR, NULL PTR IN REG_VERIFY\n");
        return REG_NUN;
    }

    if ( !stricmp(buf, "rax\0")) return rax;
    if ( !stricmp(buf, "rbx\0")) return rbx;
    if ( !stricmp(buf, "rcx\0")) return rcx;
    if ( !stricmp(buf, "rdx\0")) return rdx;

    return REG_NUN;
}

int labels_ctor(label** marks, const size_t MARKS_SIZE){
    
    if(!marks) {
        printf("MAKRS HAVE NULL PTR IN LABEL_CTOR\n");
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
        printf("MAKRS HAVE NULL PTR IN LABEL_DTOR\n");
        return -1;
    }
    for(size_t i = 0; i < MARKS_SIZE; i++) {
        if(strcmp(marks[i].name, "\0")){
            free(marks[i].name);
        }
    }
    return 0;
}

int analyse_verify( char* cmd, label* marks, const size_t marks_size, const int ip, const int line) {
    
    if( !cmd || !marks ){

        printf("ERROR, NULL PTR IN ANALYSE_VERIFY\n");
        return NULL_PTR;
    }

    skip_comments(cmd);
    if(cmd[0] == '\0') return 0;
    int cmd_len = strlen(cmd);
    char *command = (char*)calloc(sizeof(char), MAX_LINE);

    if ( !command ) return AS_ALLOC_ERR;
    if ( cmd_len > MAX_LINE) {
        printf("ERROR, COMMAND LENGTH MORE THEN MAX_LINE CONST IN LINE %d\n", line);
        return LENGTH_ERR;
    }

    sscanf(cmd, "%s", command);

    int offset = strstr(cmd, command) - cmd; 
    
    int command_num = command_verify(command);
    free(command);
    char *check_reg;
    el_type value = 0;
    int num;
    int mark = 0; 
    int offset2 = 0;
    switch(command_num) {
        case PUSH:
            offset += PUSH_S;
            if (sscanf(cmd + offset, "%lf %n", &value, &offset2)) {
                if ( check_synt(cmd + offset + offset2)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION %d\n", line);   
                    return SYNT_ERROR;
                }
                return sizeof(char) + sizeof(el_type);
            }          

            offset += skip_spaces(cmd + offset);
            offset2 = skip_sqares(cmd + offset);
            offset += skip_spaces(cmd + offset + offset2);
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);
            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }
            if ( offset2 == 0){
                if (sscanf(cmd + offset + offset2, "%s", check_reg)) {
                    //printf("%s\n", check_reg);
                    int check_reg_len = strlen(check_reg);
                    if( register_verify(check_reg) != REG_NUN ){
                        //printf("%s\n", check_reg);
                        if ( check_synt(cmd + offset + offset2 + check_reg_len)== -1){
                        printf("SYNRAX ERROR, UNNOWN CONSTRACTION USED REGISRETS %d\n", line);
                        free(check_reg);  
                        return SYNT_ERROR;
                    }
                        free(check_reg);
                        
                        return 2;
                    }
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION USED REGISTERS %d\n", line);
                    free(check_reg);  
                    return SYNT_ERROR;
                }
            printf("SYNRAX ERROR, UNNOWN CONSTRACTION %d\n", line);
            free(check_reg);  
            return SYNT_ERROR;
            }

            //offset += offset2;
            //printf("%s\n", cmd + offset);
            if ( sscanf( cmd + offset++ + offset2, "%d %n", &num, &offset2)) {
                    //printf("we are here : %d is int\n", num);
                    //printf("we are here : %s\n", cmd + offset + offset2);

                    if ( check_synt(cmd + offset )== -1) {

                    offset += offset2 + skip_spaces(cmd + offset + offset2);

                    if ( cmd[offset] == '\0') {
                        free(check_reg);
                        return sizeof(char) + sizeof(int);
                    }


                    if ( cmd[offset] != '+' ) {
                        printf("%s\n", cmd + offset);
                        printf("SYNRAX ERROR, UNNOWN CONSTRACTION USING RAM\n");
                        free(check_reg);  
                        return SYNT_ERROR;
                    }

                    offset++;
                    offset +=skip_spaces(cmd + offset);
                    //printf("%s\n", cmd + offset);
                    if (sscanf( cmd + offset, "%s", check_reg)) {
                        if( register_verify(check_reg) != REG_NUN ){
                        //printf("%s\n", check_reg);
                        int check_reg_len = strlen(check_reg);
                        if ( check_synt(cmd + offset + check_reg_len)== -1){
                            //printf("%s\n", cmd + check_reg_len + offset);
                            printf("SYNRAX ERROR, UNNOWN CONSTRACTION  USING RAM %d\n", line);
                            free(check_reg);  
                            return SYNT_ERROR;
                        }
                            free(check_reg);
                            
                            return sizeof(char) + sizeof(char) + sizeof(int);
                        }
                        printf("SYNRAX ERROR, UNNOWN CONSTRACTION USING RAM %d\n", line);
                        free(check_reg);  
                        return SYNT_ERROR;
                    }
                    
                    printf("SYNRAX ERROR, GENERAL UNNOWN CONSTRACTION\n");
                    free(check_reg);  
                    return SYNT_ERROR;
                }
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }

            if (sscanf( cmd + offset, "%s", check_reg)) {
                if( register_verify(check_reg) != REG_NUN ){
                //printf("%s\n", check_reg);
                int check_reg_len = strlen(check_reg);
                if ( check_synt(cmd + offset + check_reg_len)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION REGISTER AND RAM IN LINE %d\n", line);
                    free(check_reg);  
                    return SYNT_ERROR;
                }
                    free(check_reg);
                    
                    return sizeof(char) + sizeof(char);
                }
                printf("%s\n", check_reg);
                printf("SYNRAX ERROR, UNNOWN REGISTER NAME %d\n", line);
                free(check_reg);  
                return SYNT_ERROR;
            }

            printf("SYNRAX ERROR,GENERAL UNNOWN CONSTRACTION %d\n", line);
            free(check_reg);  
            return SYNT_ERROR;
        case ADD:
            if ( check_synt(cmd + ADD_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER ADD %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case SUB:
            if ( check_synt(cmd + SUB_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER SUB %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case MUL:
            if ( check_synt(cmd + MUL_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER MUL %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case DIV:
            if ( check_synt(cmd + DIV_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER DIV %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case OUT:
            if ( check_synt(cmd + OUT_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER OUT %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case DUMP:
            if ( check_synt(cmd + DUMP_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER DUMP %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case JMP:
            
            if ( sscanf(cmd + JMP_S + offset, "%d", &mark)) return sizeof(char) + sizeof(int);
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }
            if ( sscanf( cmd + JMP_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case JA:

            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }
            if ( sscanf( cmd + JA_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case JB:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }
            if ( sscanf( cmd + JB_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case JAE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if ( sscanf( cmd + JAE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case JBE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if ( sscanf( cmd + JBE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case JE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if ( sscanf( cmd + JE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case JNE:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if ( sscanf( cmd + JNE_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("JUMP ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case CALL:
        
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if ( sscanf( cmd + CALL_S + offset, "%s", check_reg)){
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }
            printf("CALL ERROR %d\n", line);
            free(check_reg);
            return SYNT_ERROR;
        case RET:

            if ( check_synt(cmd + RET_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER RET %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case HLT:
            if ( check_synt(cmd + HLT_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER HLT %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case IN:
            if ( check_synt(cmd + IN_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER IN %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case MARK:
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if (sscanf(cmd + MARK_S + offset, "%s", check_reg)) {
                
                for(size_t i = 0 ; i < marks_size; i++) {
                    if( (marks[i]).line == -1 ) {
                        //printf("line %d", line);
                        marks[i].line = ip;
                        marks[i].name = check_reg;
                        return 0;
                    }
                }
                printf("ERROR, DON'T HAVE PLACE FOR ANOTHER LABEL");
                free(check_reg);
                return LABEL_MEM_ERR;
            }
            printf("LABEL ERROR, LABEL HAVE NO NAME %d\n", line);
            free(check_reg);
            return LABEL_NAME_ERR;
        case DUP:
            if ( check_synt(cmd + DUP_S)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION, VALUE AFTER DUP %d\n", line);   
                    return SYNT_ERROR;
            }
            return 1;
        case POP:
            offset += POP_S;
            offset += skip_spaces(cmd + offset);
            offset2 = skip_sqares(cmd + offset);
            offset += skip_spaces(cmd + offset + offset2);
            check_reg = (char*)calloc(sizeof(char), MAX_LINE);

            if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN ANALYSE_VERIFY\n");    
                return AS_ALLOC_ERR;
            }

            if ( offset2 == 0){
                if (sscanf(cmd + offset + offset2, "%s", check_reg)) {
                    //printf("%s\n", check_reg);
                    int check_reg_len = strlen(check_reg);
                    if( register_verify(check_reg) != REG_NUN ){
                        //printf("%s\n", check_reg);
                        if ( check_synt(cmd + offset + offset2 + check_reg_len)== -1){
                        printf("SYNRAX ERROR, UNNOWN CONSTRACTION %d\n", line);
                        free(check_reg);  
                        return SYNT_ERROR;
                    }
                        free(check_reg);
                        
                        return 2;
                    }
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION %d\n", line);
                    free(check_reg);  
                    return SYNT_ERROR;
                }
            printf("SYNRAX ERROR, UNNOWN CONSTRACTION %d\n", line);
            free(check_reg);  
            return SYNT_ERROR;
            }
            
            //offset += offset2;
            if ( sscanf( cmd + offset++ + offset2, "%d %n", &num, &offset2)) {
                    //printf("we are here : %d is int\n", num);
                    //printf("we are here : %s\n", cmd + offset + offset2);

                    if ( check_synt(cmd + offset )== -1) {

                    offset += offset2 + skip_spaces(cmd + offset + offset2);

                    if ( cmd[offset] == '\0') {
                        free(check_reg);
                        return sizeof(char) + sizeof(int);
                    }


                    if ( cmd[offset] != '+' ) {
                        printf("%s\n", cmd + offset);
                        printf("SYNRAX ERROR, UNNOWN CONSTRACTION USING RAM\n");
                        free(check_reg);  
                        return SYNT_ERROR;
                    }


                    offset++;
                    offset +=skip_spaces(cmd + offset);
                    //printf("%s\n", cmd + offset);

                    if (sscanf( cmd + offset, "%s", check_reg)) {
                        if( register_verify(check_reg) != REG_NUN ){
                        //printf("%s\n", check_reg);
                        int check_reg_len = strlen(check_reg);
                        if ( check_synt(cmd + offset + check_reg_len)== -1){
                            printf("SYNRAX ERROR, UNNOWN CONSTRACTION  USING RAM %d\n", line);
                            free(check_reg);  
                            return SYNT_ERROR;
                        }
                            free(check_reg);
                            
                            return sizeof(char) + sizeof(char) + sizeof(int);
                        }
                        printf("SYNRAX ERROR, UNNOWN CONSTRACTION USING RAM %d\n", line);
                        free(check_reg);  
                        return SYNT_ERROR;
                    }
                    
                    printf("SYNRAX ERROR, GENERAL UNNOWN CONSTRACTION\n");
                    free(check_reg);  
                    return SYNT_ERROR;
                }
                free(check_reg);
                return sizeof(char) + sizeof(int);
            }

            if (sscanf( cmd + offset, "%s", check_reg)) {
                if( register_verify(check_reg) != REG_NUN ){
                //printf("%s\n", check_reg);
                int check_reg_len = strlen(check_reg);
                if ( check_synt(cmd + offset + check_reg_len)== -1){
                    printf("SYNRAX ERROR, UNNOWN CONSTRACTION REGISTER AND RAM IN LINE %d\n", line);
                    free(check_reg);  
                    return SYNT_ERROR;
                }
                    free(check_reg);
                    
                    return sizeof(char) + sizeof(char);
                }
                printf("%s\n", check_reg);
                printf("SYNRAX ERROR, UNNOWN REGISTER NAME %d\n", line);
                free(check_reg);  
                return SYNT_ERROR;
            }

            printf("SYNRAX ERROR, UNNOWN CONSTRACTION IN LINE%d\n", line);
            free(check_reg);  
            return SYNT_ERROR;
        default:
            printf("SYNTAX ERROR, UNKNOWN COMMAND %d\n", line);
            return SYNT_ERROR;
    }
    return 0;
}

int cmd_analyse(char** cmd, const size_t cmd_size, label* marks, const size_t marks_size) {

    if(!cmd || !marks) {
        printf("ERROR, NULL PTR IN CMD_ANALYSE");
        return 0;
    }

    int sum = 0;
    int tmp = 0;

    for ( int i = 0; i < (int)cmd_size; i++) {

    if ((tmp = analyse_verify(cmd[i], marks, marks_size, sum, i) ) == -1 ){
            
            printf("SYNTAX ERROR IN FIRST ANALYSIS\n");
            return 0;
        
        }
        //printf("sum %d\n", sum);
        sum += tmp; 
        
        
    }
    return sum;
}

int get_args(char* cmd, const int cmd_code, size_t *ip, const label* marks, char *arr, const size_t marks_size){
    
    if( !cmd || !ip || !marks || !arr) {
        printf("ERROR, NULL PTR IN GET_ARGS\n");
        return NULL_PTR;
    }

    size_t tmp_ip = *ip;

    char* check_reg;
    if (cmd_code == PUSH) {

        el_type value = -1;
        arr[tmp_ip] = 0;
        int num = 0;
        int offset = 0;
        int squares = 0;
        int offset2 = 0;
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

        if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN GET_ARGS\n");    
                return AS_ALLOC_ERR;
            }

        offset += skip_spaces(cmd);
        if ( ( cmd[offset] == '[' ) ){
            offset += 1;
            arr[tmp_ip] += RAM_FLAG;
        } 
        arr[tmp_ip] += cmd_code;

        if ( sscanf( cmd + offset, "%d %n", &num, &offset2)) {
            arr[tmp_ip] += IMMED_FLAG;
            *ip += 1;
            //printf("we are here : %d is int\n", num);
            *(int*)(arr + tmp_ip + 1) = num;
            *ip += sizeof(int);

            offset += offset2+ skip_spaces(cmd + offset + offset2);

            if ( cmd[offset] == '\0') {
                free(check_reg);
                return 0;
            }


            if ( cmd[offset] != '+' ) {
                printf("%s\n", cmd + offset);
                printf("SYNRAX ERROR, UNNOWN CONSTRACTION USING RAM\n");
                free(check_reg);  
                return SYNT_ERROR;
            }
            
            offset++;
            offset += skip_spaces(cmd + offset);

            if (sscanf( cmd + offset, "%s", check_reg)) {
                int tmp_reg = -1;
                if ( (tmp_reg = register_verify(check_reg )) != REG_NUN ) {
                //printf("%s\n", check_reg);
                    arr[tmp_ip] += REG_FLAG;
                    arr[tmp_ip + 1 + sizeof(int)] = (char)tmp_reg;
                    *ip += sizeof(char);
                    free(check_reg);
                    
                    return 0;
                }
                printf("%s\n", check_reg);
                printf("SYNTAX ERROR, PUSH HAS WRONG REGISTER NAME\n");
                free(check_reg);  
                return SYNT_ERROR;
            }
            free(check_reg);
            return 0;
        }


        if (sscanf(cmd + offset, "%s", check_reg)) {

            //arr[tmp_ip] += cmd_code;
            int tmp_reg = -1;
            //printf("here may be problem %s\n", check_reg);
            //printf("here may be problem %s\n", check_reg + check_reg_offset);
            if( ( (tmp_reg = register_verify(check_reg )) != REG_NUN )){
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

        }
        //printf("check reg is %s\n cmd is %s\n", check_reg, cmd);
        free(check_reg);

        printf("SYNTAX ERROR, PUSH HAS WRONG ARG \n");
        return SYNT_ERROR;
                
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

         if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN GET_ARGS\n");    
                return AS_ALLOC_ERR;
        }

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
            return LABEL_NAME_ERR;

        }
        free(check_reg);
        return SYNT_ERROR;
    
    }

    if ( cmd_code == POP ) {
        
        el_type value = 0;
        int num = 0;
        char* check_reg;
        arr[tmp_ip] = 0;
        int offset = 0;
        int squares = 0;
        int offset2 = 0;
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

         if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN GET_ARGS\n");    
                return AS_ALLOC_ERR;
        }


        offset += skip_spaces(cmd);
        if ( ( cmd[offset] == '[' ) ){
            offset += 1;
            arr[tmp_ip] += RAM_FLAG;
        } 
        arr[tmp_ip] += cmd_code;

        if ( sscanf( cmd + offset, "%d %n", &num, &offset2)) {
            arr[tmp_ip] += IMMED_FLAG;
            *ip += 1;
            //printf("we are here : %d is int\n", num);
            *(int*)(arr + tmp_ip + 1) = num;
            *ip += sizeof(int);

            offset += offset2+ skip_spaces(cmd + offset + offset2);

            if ( cmd[offset] == '\0') {
                free(check_reg);
                return 0;
            }


            if ( cmd[offset] != '+' ) {
                printf("%s\n", cmd + offset);
                printf("SYNRAX ERROR, UNNOWN CONSTRACTION USING RAM\n");
                free(check_reg);  
                return SYNT_ERROR;
            }
            
            offset++;
            offset += skip_spaces(cmd + offset);

            if (sscanf( cmd + offset, "%s", check_reg)) {
                int tmp_reg = -1;
                if ( (tmp_reg = register_verify(check_reg )) != REG_NUN ) {
                //printf("%s\n", check_reg);
                    arr[tmp_ip] += REG_FLAG;
                    arr[tmp_ip + 1 + sizeof(int)] = (char)tmp_reg;
                    *ip += sizeof(char);
                    free(check_reg);
                    
                    return 0;
                }
                printf("%s\n", check_reg);
                printf("SYNTAX ERROR, PUSH HAS WRONG REGISTER NAME\n");
                free(check_reg);  
                return SYNT_ERROR;
            }
            free(check_reg);
            return 0;
        }


        if (sscanf(cmd + offset, "%s", check_reg)) {

            int tmp_reg = -1;
            if( ( (tmp_reg = register_verify(check_reg )) != REG_NUN )){
                arr[tmp_ip++] += REG_FLAG;
                *ip += 1;
                *(arr + tmp_ip) = (char)tmp_reg;
                free(check_reg);
                *ip += sizeof(char);
                return 0;
            }

        }
        free(check_reg);

        printf("SYNTAX ERROR, POP HAS WRONG ARG \n");
        return SYNT_ERROR;


        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

         if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN GET_ARGS\n");    
                return AS_ALLOC_ERR;
        }


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
        return SYNT_ERROR;    
    }

    if (cmd_code >= JB && cmd_code <= JNE) {

        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

         if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN GET_ARGS\n");    
                return AS_ALLOC_ERR;
        }


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
            return LABEL_NAME_ERR;

        }
        free(check_reg);
        return SYNT_ERROR;
    
    }

    if (cmd_code == CALL ) {

        check_reg = (char*)calloc(sizeof(char), MAX_LINE);

         if ( !check_reg ){ 

                printf("ALLOCATION ERROR IN GET_ARGS\n");    
                return AS_ALLOC_ERR;
        }

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
            return LABEL_NAME_ERR;

        }
        free(check_reg);
        return SYNT_ERROR;

    }



    printf("SYNTAX ERROR, WRONG FUNC\n");
    return SYNT_ERROR;
}
    
#define DEF_CMD(name, num, length, arg, ...)\
    case num:\
        if (num == MARK) break;\
        if (arg == 1 ) {\
            if ( get_args(cmd[i] + offset + length, num, &ip, marks, arr, marks_size) == -1) {\
                            printf ("GETtTING ARGS ERROR\n");\
                            free(command);\
                            return GET_ARGS_ERR;\
                        }\
                        break;\
        }\
            arr[ip++] = cmd_code;\
            break;

int assemble( char** cmd,const size_t cmd_size, const label* marks, char* arr, const size_t marks_size) {

    char * command = (char*)calloc(sizeof(char), MAX_LINE);
    int cmd_code = 0;
    size_t ip = 0;
    size_t offset = 0;
    for( size_t i = 0; i < cmd_size; i++) {
        
        if ( cmd[i][0] != '\0'){
            sscanf(cmd[i], "%s", command);
            offset = (size_t)(strstr(cmd[i], command) - cmd[i]);
            //printf("offset is [%lu]\n command is [%s]", offset, command);
            if( (cmd_code = command_verify(command)) != NUN ) {
                
                switch(cmd_code){

                    #include "cmd_info.hpp"
                    default:
                        arr[ip++] = cmd_code;

                }
            } else {
                free(command);
                return ASSEMBLE_ERR;
            }
        }
    }
    free(command);
    return 0;

}

#undef DEF_CMD