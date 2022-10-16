#include "cmd.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "stack.hpp"

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

    char* code;
    char version = 0;
    short signature = 0;
    size_t code_size;
    el_type regs[REG_SIZE] = {0};
    el_type ram[RAM_SIZE] = {0};    
    fread(&version, sizeof(char), 1, input);
    
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
    int cmd = 0;
    el_type var1 = 0;
    el_type var2 = 0;
    stack stk = { };
    stack_ctor(stk, 64);
    size_t i = 0;
    FILE *log =fopen("log", "w");
    fwrite(code, sizeof(char), code_size, log);
    fclose(log);

    if ( execute(code, code_size, &stk, ram, RAM_SIZE, regs) == -1) {
        printf("EXEC ERROR\n");
        return -1;
    }
    printf("we are here\n");
    /*for(0; i < code_size; i++) {
        if(  (cmd = code_verify(code[i])) == NUN){
            
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
    }*/
    fclose(input);
    free(code);
    return 0;
}