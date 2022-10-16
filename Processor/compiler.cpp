#include <stdio.h>
#include "io.hpp"
#include "stack.hpp"
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
    //cmd_print(cmd, cmd_size);
    label* marks;
    labels_ctor(&marks, MARKS_SIZE);
    size_t arr_size = cmd_analyse(cmd, cmd_size, marks, MARKS_SIZE);
    
    if ( !arr_size) {
        printf ("\x1b[31m ASSEMBLING ERROR \x1b[0m \n");
        labels_dtor(marks);
        free(marks);
        return -1;
    }

    char *arr = (char*)calloc( sizeof(char), arr_size);

    
    if ( !arr ){
        
        printf("\x1b[31m CAN NOT CREAT ARRAY\x1b[0m \n");
        fclose(input);
        free(code);
        labels_dtor(marks);
        free(marks);
        free(cmd);
        return -1;
    }

    if ( assamble(cmd, cmd_size, marks, arr, MARKS_SIZE) == -1) {
        printf( "SYNTAX ERROR\n");
        fclose(input);
        free(arr);
        free(code);
        free(cmd);
        labels_dtor(marks);
        free(marks);
        return -1;
    }

    FILE* output = fopen("a.out", "w");

    if ( !output ) {

        fprintf(stderr, "\x1b[31m ERROR WHILE CREATING a.out FILE \x1b[0m\n");
        fclose(input);
        free(arr);
        free(code);
        free(cmd);
        labels_dtor(marks);
        free(marks);
        return -1;

    }
    fwrite(&VERSION, sizeof(char), 1, output);
    fwrite(SIGNATURE, sizeof(short), 1, output);
    fwrite(&arr_size, sizeof(size_t), 1, output);
    fwrite(arr, sizeof(char), arr_size, output);
    fclose(output);
    fclose(input);
    free(arr);
    free(code);
    free(cmd);
    labels_dtor(marks);
    free(marks);
    //free(command);

    return 0;

}
