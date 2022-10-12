#include <stdio.h>
#include "io.hpp"
#include "../Stack/stack.hpp"
#include <string.h>
#include "cmd.hpp"

const size_t ARR_CAP = 1024;





int main(int argc, char *argv[]) {
    
    if ( argc < 2) {
        
        fprintf(stderr, "\x1b[31m Compilation error, file was not inputed\x1b[0m\n");
        return -1;
    
    } 

    FILE* input = fopen(argv[1], "r");
    
    if ( !input ) {

        fprintf(stderr, "\x1b[31m ERROR WHILE READING THE FILE \x1b[0m\n");
        fclose(input);
        return -1;

    }

    char* code = NULL;
    size_t code_size = 0;
    buf_read( &code, input, &code_size);
    
    char**cmd = NULL;
    size_t cmd_size = 0;
    cmd_get(code, &cmd, &cmd_size, code_size);
    int marks[MARKS_SIZE] = {-1};
    size_t arr_size = cmd_analyse(cmd, cmd_size, marks, MARKS_SIZE);
    
    if ( !arr_size) {
        printf ("\x1b[31m ASSEMBLING ERROR \x1b[0m");
        return -1;
    }

    //printf("%lu\n", arr_size);
    char *arr = (char*)calloc( sizeof(char), arr_size);

    //printf("ARRAY SIZE : [%lu]\n", arr_size);
    
    if ( !arr ){
        
        printf("\x1b[31m CAN NOT CREAT ARRAY\x1b[0m");
        fclose(input);
        free(code);
        return -1;
    }

    if ( assamble(cmd, cmd_size, marks, arr) == -1) {
        printf( "SYNTAX ERROR\n");
        return -1;
    }

    FILE* output = fopen("a.out", "w");

    if ( !output ) {

        fprintf(stderr, "\x1b[31m ERROR WHILE CREATING a.out FILE \x1b[0m\n");
        fclose(input);
        free(arr);
        free(code);
        //free(command);
        return -1;

    }
    //printf("ARRAY SIZE : [%lu]\n", arr_size);
    fwrite(&VERSION, sizeof(char), 1, output);
    fwrite(SIGNATURE, sizeof(short), 1, output);
    fwrite(&arr_size, sizeof(size_t), 1, output);
    //printf("ARRAY SIZE : [%lu]\n", arr_size);
    fwrite(arr, sizeof(char), arr_size, output);
    fclose(output);
    fclose(input);
    free(arr);
    free(code);
    free(cmd);
    //free(command);

    return 0;

}
