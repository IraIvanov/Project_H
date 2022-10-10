#include "io.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

int buf_read( char** code, FILE* input, size_t* size){

    if ( !code ) return BUFF_NULL_PTR;
    if ( !input ) return FILE_NULL_PTR;

    struct stat code_stat = { };

    if ( fstat ( fileno(input), &code_stat) == -1 ) return STATS_ERR;

    size_t code_size = (size_t)code_stat.st_size;

    *code = (char*)calloc( sizeof(char), code_size + 2);

    if( !code ) return CMD_ALLOC_ERR;

    if ( code_size != fread(*code, sizeof(char), code_size, input)) return READ_ERR;

    (*code)[code_size ] = '\n';
    (*code)[code_size + 1] = '\0';
    
    if ( size ) {
        *size = code_size + 1;
    }

    return 0;

}

int cmd_get(char *code, char***cmd, size_t* size, size_t code_size) {

    if( !code ) return BUFF_NULL_PTR;
    if( !cmd ) return CMD_NULL_PTR;

    size_t cmd_size = 0;

    for(size_t i = 0; i < code_size; i++) {

        if(code[i] == '\n'){

            while(code[i] == '\n' && i < code_size) i++;
            cmd_size++;

        }

    }

    *cmd = (char**)calloc(sizeof(char*), cmd_size);

    if ( !cmd ) return CMD_ALLOC_ERR;

    *size = cmd_size;
    size_t cmd_place = 0, cmd_i = 0;

    for(size_t i = 0; code[i] != '\0'; i++) { 

        if ( code[i] == '\n') {

            while(code[i] == '\n' && code[i+1]!= '\0') i++;
            (*cmd)[cmd_i] = code + cmd_place;
            cmd_place = i;
            cmd_i++;
             
        }

    }

    return 0;
}

void cmd_print(char** cmd, size_t cmd_size){
    
    for(size_t i = 0; i < cmd_size; i++) {
        for(size_t j = 0; cmd[i][j] != '\n'; j++){
            printf("%c", cmd[i][j]);
        }
        printf("\n");
    }
    return;
}

int stricmp(const char* str1, const char* str2) {

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    if ( len1 > len2) return 1;
    if ( len1 < len2) return -1;  
    for(size_t i = 0; i < len1 && i < len2; i++) {
        if ( tolower(str1[i]) != tolower(str2[i])) return tolower(str1[i]) - tolower(str2[i]);
    }

    return 0;
}