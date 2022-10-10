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
    cmd_print(cmd, cmd_size);
    
    el_type *arr = (el_type*)calloc( sizeof(el_type), ARR_CAP);
    
    if ( !arr ){
        
        printf("\x1b[31m CAN NOT CREAT ARRAY\x1b[0m");
        fclose(input);
        free(code);
        return -1;
    }
    /*FILE* log = fopen("log.txt", "w"); <----------- DEBUG
    fwrite(code, sizeof(char), code_size, log);
    fclose(log);*/ 

    char* command = (char*)calloc(sizeof(char), ARR_CAP);
    if ( !command ){
        printf("\x1b[31m CMD BUFF\x1b[0m");
        fclose(input);
        free(arr);
        free(code);
        return -1;
    }

    long unsigned int offset = 0;
    int var = 0, cmd_code = 0;
    size_t arr_size = 0, arr_capacity = ARR_CAP;

    while ( offset < code_size && code[offset] != '\0' ) {
    
        sscanf(code + offset, "%s", command);
        offset = (size_t)(strstr(code + offset, command) - code) + strlen(command);
        //printf("%ld\n", offset); <----------------DEBUG

        if ( (cmd_code = command_verify( command ) ) != NUN) {

            //printf("%d\n", cmd_code); <-----------DEBUG
        
            if (cmd_code == PUSH) {

                if ( !sscanf(code + offset, "%d", &var) ){
                    
                    printf("SYNTAX ERROR, EXPECTED VALUE OF PUSH");
                    fclose(input);
                    free(arr);
                    free(code);
                    free(command);
                    return -1;

                }   else {

                    if ( arr_size >=( arr_capacity - 1)){

                        void* tmp;
                        tmp = realloc(arr, sizeof(el_size)*2*arr_capacity);
                        
                        if (!tmp){
                            printf(" OUT OFF MEMORY ");
                            free(command);
                            free(code);
                            fclose(input);
                            return - 1;
                        }

                        arr = (el_type*)tmp;
                        arr_capacity *= 2;

                    }

                    arr[arr_size++] = cmd_code;
                    arr[arr_size++] = var;

                    //arr_print(arr, arr_size-1); <------ DEBUG

                    while(code[offset] != '\n' && code[offset] != '\0') offset++;

                }

            } else {

                 if ( arr_size >= arr_capacity){

                        void* tmp;
                        tmp = realloc(arr, sizeof(el_size)*2*arr_capacity);
                        
                        if (!tmp){
                            printf(" OUT OFF MEMORY ");
                            free(command);
                            free(code);
                            fclose(input);
                            return - 1;
                        }

                        arr = (el_type*)tmp;
                        arr_capacity *= 2;

                }
                arr[arr_size++] = cmd_code;

                while(code[offset] != '\n' && code[offset] != '\0') offset++;
            
            }
        
        } else {

            printf("SYNTAX ERROR, UNKNOWN COMMAND!\n");
            fclose(input);
            free(arr);
            free(code);
            free(command);
            return -1;
        
        }
        /*printf("%d\n", offset); <----- DEBUG
        printf("%c", code[offset]);*/
    }

    FILE* output = fopen("a.out", "w");

    if ( !output ) {

        fprintf(stderr, "\x1b[31m ERROR WHILE CREATING a.out FILE \x1b[0m\n");
        fclose(input);
        free(arr);
        free(code);
        free(command);
        return -1;

    }

    fwrite(&VERSION, sizeof(int), 1, output);
    fwrite(SIGNATURE, sizeof(short), 1, output);
    fwrite(&arr_size, sizeof(size_t), 1, output);
    fwrite(arr, sizeof(el_type), arr_size, output);
    fwrite(arr, sizeof(el_type), arr_size, output);
    fclose(output);
    fclose(input);
    free(arr);
    free(code);
    free(command);

    return 0;

}
