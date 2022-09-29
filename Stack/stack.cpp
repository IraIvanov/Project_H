#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.hpp"

#define ZOMBIE 0
#define ALIVE 1
#define ASSERTED == 0 || fprintf(stderr, "Error\n")
#define dump(stk, out) do_dump(&stk, __FUNCTION__, __LINE__, __FILE__, out)



int func_verify(stack* stk){

    int err = 0;
    if (!stk) return STACK_NULL_PTR;
    if (!stk->data) err |= DATA_NULL_PTR;
    if (stk->data && stk->capacity != 0 ) err |= DATA_NULL_CAP;
    if (stk->size > stk->capacity) err |= CAP_LES_SIZE;
    if(!stk->status) err |= ZOMBIE_STAT;
    return err;
}

void p_error(int err_code) {

    int i = 0;
    while (err_code) {

        if(err_code % 2) printf("err code %d", (i+1));
        err_code /= 2;
    }

    return;
}

int stack_ctor(stack* stk, size_t size) {

    int err = 0;

    if ( !(err = func_verify(stk) ) ) dump(*stk, stk->info.log);
    
    stk -> data = (el_type*) calloc(sizeof(el_type), size);
    stk -> size = 0;
    stk -> capacity = size;
    stk -> status = ALIVE;
    
    if ( !(err = func_verify(stk) ) ) dump(*stk, stk->info.log);
    
    return err;
}

int stack_resize(stack* stk) {

    int err = 0;
    
    if ( !(err = func_verify(stk)) ) dump(*stk, stk->info.log);

    void* tmp = NULL;
    tmp = realloc(stk -> data, sizeof(el_type)*(stk -> capacity) * 2); //add tmp to check null ptr 
   
    if ( !tmp ) return RESIZE_ERR;
    
    stk->data = (el_type*)tmp;
    stk -> capacity *= 2;
    return err;
}

int stack_push(stack* stk, el_type value) {

    int err = 0;

    if ( !( err = func_verify(stk)) ) dump(*stk, stk->info.log);
    
    if( stk -> size >= stk -> capacity ) stack_resize(stk);
    
    (stk -> data)[stk -> size++] = value;
    
    if ( !(err = func_verify(stk)) ) dump(*stk, stk->info.log);
    
    dump(*stk, stk->info.log);
    return err;
}
//important func to static 
static int stack_resize_down(stack* stk) {

    int err = 0;

    if ( !(err = func_verify(stk)) ) dump(*stk, stk->info.log);

    void * tmp = NULL;
    tmp = realloc(stk->data, sizeof(el_type) * (stk -> capacity) /4);
    
    if( !tmp ) return RESIZE_ERR;

    stk -> data = (el_type*)tmp;
    stk -> capacity /= 4;

    if ( ! (err = func_verify(stk))) dump(*stk, stk->info.log);

    return err;
}

int stack_pop(stack* stk, el_type* value) {

    assert(value);
    int err = 0;
    if ( !(err = func_verify(stk))) dump(*stk, stk->info.log);

    *value = (stk -> data)[stk -> size - 1];
    stk -> size--;

    if ( stk -> size <= (stk -> capacity / 4)) stack_resize_down(stk);
    
    if ( !(err = func_verify(stk)) ) dump(*stk, stk->info.log);
    
    dump(*stk, stk->info.log);
    return err; 
}

int stack_dtor(stack* stk) {

    int err = 0;
    
    if ( !(err = func_verify(stk)) ) dump(*stk, stk->info.log);
    
    free(stk -> data);
    stk -> data = (el_type*)1488; //(void*)1488
    stk -> size = 1;
    stk -> capacity = 0;
    stk -> status = ZOMBIE;
    return err;
}

void free_stack_inf(stack* stk) {
    
    free(stk->info.file);
    free(stk->info.func);
    free(stk->info.name);
    return;
}

void set_stack_inf(stack* stk, int line, const char* func, const char* file, const char* name, FILE* log) {

    assert(line && func && file && file && name && log);
    stk->info.file = strdup(file);
    stk->info.func = strdup(func);
    stk->info.name = strdup(name);
    stk->info.line = line;
    stk->info.log = log;
    return;
}

void print_stack_inf(stack* stk) {

    printf("%s ", stk->info.name);
    printf("at %s", stk->info.func);
    printf(" at %s", stk->info.file);
    printf("(%d)\n", stk->info.line);
    return;

}

static void do_dump(stack* stk, const char* func, int line, const char* file, FILE* out) {

    assert(func && line && file && out);
    fprintf(out, "%s at %s(%d):\n", func, file, line);
    fprintf(out, "%s[%p]", stk->info.name, (void*)stk);
    fprintf(out, "at %s", stk->info.func);
    fprintf(out, " at %s", stk->info.file);
    fprintf(out, "(%d)\n", stk->info.line);
    fprintf(out, "{\n size = %lu;\n cap = %lu;\n data [%p]\n", stk->size, stk->capacity, (void*)(stk->data));
    
    for(size_t i = 0; i < stk->size && i < stk->capacity; i++) {
    
        fprintf( out, "*[%lu] = %d\n", i, stk->data[i]);
    
    }
    
    for(size_t i = stk->size; i < stk->capacity; i++) {
    
        fprintf( out, "[%lu] = %d (POISON)\n", i, stk->data[i]);
    
    }
    
    fprintf(out, " }\n}\n\n");
    //fprintf( out, "[%lu] = %d")


}