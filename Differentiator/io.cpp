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

    *code = (char*)calloc(sizeof(char), code_size + 1);

    if( !code ) return CMD_ALLOC_ERR;

    if ( code_size != fread(*code, sizeof(char), code_size, input)) return READ_ERR;

    (*code)[code_size ] = '\0';
    //(*code)[code_size + 1] = '\0';
    
    if ( size ) {
        *size = code_size;
    }

    return 0;

}

int cmd_get(char *code, char***cmd, size_t* size, const size_t code_size) {

    if( !code ) return BUFF_NULL_PTR;
    if( !cmd ) return CMD_NULL_PTR;

    size_t cmd_size = 0;

    size_t offset = 0;

    while(isspace(code[offset])) offset++;

    //printf("%lu\n", offset);


    for(size_t i = offset; i < code_size; i++) {

        if(code[i] == '\n'){

            while(code[i] == '\n' && i < code_size) i++;
            cmd_size++;

        }

    }

    *cmd = (char**)calloc(sizeof(char*), cmd_size);

    if ( !cmd ) return CMD_ALLOC_ERR;

    *size = cmd_size;
    size_t cmd_place = offset, cmd_i = 0;

    for(size_t i = offset; code[i] != '\0'; i++) { 

        if ( code[i] == '\n') {
            code[i++] = '\0';
            while(isspace(code[i]) && code[i+1]!= '\0') i++;
            (*cmd)[cmd_i] = code + cmd_place;
            cmd_place = i;
            cmd_i++;
             
        }

    }

    return 0;
}

int cmd_print(const char** cmd, const size_t cmd_size){
    
    if ( !cmd ) return CMD_NULL_PTR;

    for(size_t i = 0; i < cmd_size; i++) {
        /*for(size_t j = 0; cmd[i][j] != '\n'; j++){
            printf("%c", cmd[i][j]);
        }*/
        printf("%s", cmd[i]);
        printf("\n");
    }
    return 0;
}

int stricmp(const char* str1, const char* str2) {

    size_t i = 0, j = 0;
    while ( str1[i] != '\0' && str2[j] != '\0' ) {

        while ( isspace(str1[i]) != 0 ) i++;
        while ( isspace(str2[j]) != 0 ) j++; 
        if ( tolower(str1[i]) != tolower(str2[j])) return tolower(str1[i]) - tolower(str2[j]);

        i += 1;
        j += 1;

    }

    while ( isspace(str1[i]) != 0 ) i++;
    while ( isspace(str2[j]) != 0 ) j++;

    if ( str1[i] != str2[j] ) return str1[i] - str2[j];

    return 0;
}

int skip_spaces( const char *str){
    
    int i = 0;
    while( str[i] != '\0' && isspace(str[i])) i++;

    return i;
}