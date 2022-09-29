#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.hpp"
#include <malloc.h>

#define ZOMBIE 0
#define ALIVE 1
#define dump(stk, out, err) do_dump(&stk, __FUNCTION__, __LINE__, __FILE__, out, err)
#define ASSERTED != 0 || printf("ERROR");
#define CANARY_SIZE sizeof(unsigned long long)


unsigned int rot13 (void* ptr, size_t size) {

    unsigned int h = 0;

    for(size_t i = 0; i < size; i++) {
        
        h += (unsigned char) (*((char*)ptr + i));
        h -= (h >> 19) | (h << 13);

    }

    return h;
}


int func_verify(stack* stk){

    int err = 0;
    if (!stk) return STACK_NULL_PTR;
    if (stk -> data == POISONED_DATA) err |= BAD_DATA_PTR;
    if (!stk->data) err |= DATA_NULL_PTR;
    if (stk->data && stk->capacity == 0 ) err |= DATA_NULL_CAP;
    if (stk->size > stk->capacity) err |= CAP_LES_SIZE;
    if(!stk->status) err |= ZOMBIE_STAT;

    #ifdef CANARY_PROT
    if(stk->LEFT_CANARY != CANARY || stk->RIGHT_CANARY != CANARY ) err |= DEAD_CANARY;
    #endif

    #ifdef HASH_PROT
    unsigned long long old_hash = stk->hash;
    if(rot13(stk -> data, stk->capacity) != old_hash) err |= BAD_HASH;
    #endif

    return err;
}



void p_error(int err_code, FILE* out) {

    int i = 0;
    
    if (err_code == 1) {
        
        fprintf(out, "STACK NULL PTR ");
        return;
    }

    err_code /= 2;

    while (err_code) {

        i++;

        if(err_code % 2){   

            switch (i) {
                
            case 1:
                    
                fprintf(out, "DATA NULL PTR, ");
                break;
            
            case 2:

                fprintf(out,"CAPACITY LESS THAN SIZE, ");
                break;
            
            case 3:

                fprintf(out, "ZOMBIE STATUS, ");
                break;

            case 4:

                fprintf(out, "STACK CAPACITY IS 0, ");
                break;
            
            case 5:

                fprintf(out, "ALLOCATION ERROR, ");
                break;

            case 6:

                fprintf(out, "DATA HAS BAD PTR, ");
                break;

            case 7:

                fprintf(out, "CANARY IS DEAD, ");
                break;

            case 8:

                fprintf(out, "BAD HASH, ");
                break;
            
            default:

                fprintf(out, "UNKNOWN ERROR CODE ");
                break;
            }
    
        }
        err_code /= 2;
    }

    return;
}

int _stack_ctor(stack* stk, size_t size, int line, const char* func, const char* file, const char* name, FILE* log) {

    int err = 0;

    #ifdef NDEBUG
    if ( (err = func_verify(stk) ) == 1 ) dump(*stk, stk->info.log, err);
    #endif
    stk -> data = (el_type*) calloc(size + 2*CANARY_SIZE, sizeof(el_type));
 
    stk -> size = 0;
    stk -> capacity = size;
    stk -> status = ALIVE;

    #ifdef CANARY_PROT
    stk->LEFT_CANARY = CANARY;
    stk->RIGHT_CANARY = CANARY;
    #endif

    #ifdef HASH_PROT
    stk->hash = 0;
    #endif

    #ifdef NDEBUG
    set_stack_inf(stk, line, func, file, name, log);
    if ( (err = func_verify(stk) ) ) dump(*stk, stk->info.log, err);
    #endif
    //*(unsigned long long*)(stk -> data) = CANARY;
    //*(unsigned long long*)((char*)(stk -> data) + sizeof(el_type) * size + CANARY_SIZE) = CANARY; 
    
    return err;
}

int stack_resize(stack* stk) {

    int err = 0;
    
    #ifdef NDEBUG
    if ( (err = func_verify(stk)) ) dump(*stk, stk->info.log, err);
    #endif

    void* tmp = NULL;
    tmp = realloc(stk -> data, sizeof(el_type)*(stk -> capacity) * resize_up_const /*+ 2*CANARY_SIZE*/); //add tmp to check null ptr 
   
    if ( !tmp ) return ALLOC_ERR;
    
    stk->data = (el_type*)tmp;
    stk -> capacity *= resize_up_const;

    for(size_t i = stk->size; i < stk->capacity; i++){
        
        stk->data[i] = 0;

    }
    return err;
}

int stack_push(stack* stk, el_type value) {

    int err = 0;

    #ifdef NDEBUG
    if ( ( err = func_verify(stk)) ) dump(*stk, stk->info.log, err);
    #endif

    if( stk -> size >= stk -> capacity ) stack_resize(stk);
    
    (stk -> data)[stk -> size++ /*+ CANARY_SIZE*/] = value;
    
    #ifdef HASH_PROT
    stk->hash = rot13(stk->data, stk->capacity);
    #endif

    #ifdef NDEBUG
    if ( ( err = func_verify(stk)) ) dump(*stk, stk->info.log, err);
    #endif

    //dump(*stk, stk->info.log, err);
    return err;
}
//important func to static 
static int stack_resize_down(stack* stk) {

    int err = 0;
    #ifdef NDEBUG
    if ( (err = func_verify(stk)) ) dump(*stk, stk->info.log, err);

    for(size_t i = stk->size; i < (stk->capacity); i++ ) {

        stk -> data[i] = POISONED_EL;

    }
    #endif

    void * tmp = NULL;
    tmp = realloc(stk->data, sizeof(el_type) * (stk -> capacity) / resize_down_const /*+ 2*CANARY_SIZE*/);
    
    if( !tmp ) return ALLOC_ERR;

    stk -> data = (el_type*)tmp;
    stk -> capacity /= resize_down_const;
    #ifdef HASH_PROT
    stk->hash = rot13(stk->data, stk->capacity);
    #endif
    //*(unsigned long long*)(stk -> data + stk -> capacity + sizeof(CANARY) / sizeof(el_type)) = CANARY;
    #ifdef CANARY_PROT
    if (  (err = func_verify(stk))) dump(*stk, stk->info.log, err);
    #endif
    return err;
}

int stack_pop(stack* stk, el_type* value) {

    
    int err = 0;
    #ifdef NDEBUG
    (value)ASSERTED;
    if ( (err = func_verify(stk))) dump(*stk, stk->info.log, err);
    #endif

    *value = (stk -> data)[stk -> size - 1];
    stk ->data[stk -> size - 1] = POISONED_EL;
    stk -> size--;

    if ( stk -> size <= (stk -> capacity / 4)) stack_resize_down(stk);
    
    #ifdef HASH_PROT
    stk->hash = rot13(stk->data, stk->capacity);
    #endif

    #ifdef NDEBUG
    if ( (err = func_verify(stk)) ) dump(*stk, stk->info.log, err);
    #endif
    //dump(*stk, stk->info.log, err);
    return err; 
}

int stack_dtor(stack* stk) {

    int err = 0;
    #ifdef NDEBUG
    if ( (err = func_verify(stk)) ) dump(*stk, stk->info.log, err);
    #endif

    free(stk -> data);
    stk -> data = POISONED_DATA; //(void*)1488
    stk -> size = 1;
    stk -> capacity = 0;
    stk -> status = ZOMBIE;
    
    #ifdef HASh_PROT
    stk->hash = rot13(stk->data, stk->capacity);
    #endif

    #ifdef NDEBUG
    if ( (err = func_verify(stk)) ) dump(*stk, stk->info.log, err);
    
    fclose(stk->info.log);
    #endif
    return err;
}

void set_stack_inf(stack* stk, int line, const char* func, const char* file, const char* name, FILE* log) {

    (line && func && file && file && name && log)ASSERTED;
    stk->info.file = file;
    stk->info.func = func;
    stk->info.name = name;
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

static void do_dump(stack* stk, const char* func, int line, const char* file, FILE* out, int err) {

    assert(func && line && file && out);
    fprintf(out, "%s at %s(%d):\n", func, file, line);
    fprintf(out, "%s[%p]", stk->info.name, (void*)stk);
    fprintf(out, "(");

    if ( err == 0 ) fprintf(out, "ok");
    else p_error(err, out);
    fprintf(out, ") ");
    fprintf(out, "at %s", stk->info.func);
    fprintf(out, " at %s", stk->info.file);
    fprintf(out, "(%d)\n", stk->info.line);
    fprintf(out, "{\n \tsize = %lu;\n \tcap = %lu;\n \tdata [%p]\n", stk->size, stk->capacity, (void*)(stk->data));
    
    for(size_t i = 0; i < stk->size && i < stk->capacity; i++) {
    
        fprintf( out, " \t*[%lu] = %d\n", i, stk->data[i]);
    
    }
    
    for(size_t i = stk->size; i < stk->capacity; i++) {
        
        fprintf( out, " \t[%lu] = %d ", i, stk->data[i]);
        if (stk->data[i] == POISONED_EL) fprintf(out, "(POISON)");
        fprintf(out, "\n");
    }
    
    fprintf(out, " }\n}\n\n");
    //fprintf( out, "[%lu] = %d")


}