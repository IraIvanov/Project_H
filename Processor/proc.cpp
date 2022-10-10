#include "cmd.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "../Stack/stack.hpp"

int main(int argc, char *argv[]) {
    
    if ( argc < 2) {
        
        fprintf(stderr, "\x1b[31m Execution error, file was not inputed\x1b[0m\n");
        return -1;
    
    } 

    FILE* input = fopen(argv[1], "r");
    
    if ( !input ) {

        fprintf(stderr, "\x1b[31m ERROR WHILE READING THE FILE \x1b[0m\n");
        fclose(input);
        return -1;

    }

    el_type* code;
    int version = 0;
    short signature = 0;
    size_t code_size;
    
    fread(&version, sizeof(int), 1, input);
    
    if (version != VERSION) {
        fprintf(stderr, "\x1b[31m ERROR, WRONG VERSION \x1b[0m\n");
        fclose(input);
        return -1;
    }

    fread(&signature, sizeof(short), 1, input);

    //printf("%d", signature);
    if(signature != *(short*)SIGNATURE) {
        fprintf(stderr, "\x1b[31m ERROR, WRONG SIGNATURE \x1b[0m\n");
        fclose(input);
        return -1;
    }

    fread(&code_size, sizeof(size_t), 1, input);
    code_read(&code, input, NULL);
    arr_print(code, code_size);
    int cmd = 0;
    el_type var1 = 0;
    el_type var2 = 0;
    stack stk = { };
    stack_ctor(stk, 64);

    for(size_t i = 0; i < code_size; i++) {
        if( (cmd = code_verify(code[i])) == NUN){
            
            fprintf(stderr, "\x1b[31m ERROR UNKNOWN COMMAND \x1b[0m\n");
            fclose(input);
            free(code);
            return -1;
        }

        switch (cmd)
        {
        case PUSH:
            stack_push(&stk, code[++i]);
            printf("pushing[%d]\n", code[i]);
            break;
        case ADD:
            stack_pop(&stk, &var1);
            stack_pop(&stk, &var2);
            printf("add var1[%d]  var2[%d]\n", var1, var2);
            stack_push(&stk, var1 + var2);
            break;
        case SUB:
            stack_pop(&stk, &var1);
            stack_pop(&stk, &var2);
            printf("sub var1[%d]  var2[%d]\n", var1, var2);
            stack_push(&stk, var2 - var1);
            break;
        case MUL:
            stack_pop(&stk, &var1);
            stack_pop(&stk, &var2);
            printf("mul var1[%d]  var2[%d]\n", var1, var2);
            stack_push(&stk, var1*var2);
            break;
        case DIV:
            stack_pop(&stk, &var1);
            stack_pop(&stk, &var2);
            printf("div var1[%d]  var2[%d]\n", var1, var2);
            stack_push(&stk, var2 / var1);
            break;
        case OUT:
            stack_pop(&stk, &var1);
            printf("%d\n", var1);
            break;
        case HLT:
            stack_dtor(&stk);
            printf("stack destroied\n");
            break;
        default:
            printf("dumping\n");
            //dump_proc(code, i);
            break;
        }
    }
    fclose(input);
    free(code);
    return 0;
}