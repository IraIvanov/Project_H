#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.hpp"
#include <malloc.h>
#include "tree.hpp"

#define ZOMBIE 0
#define S_ALIVE 1
#define RED     "\x1b[31m"
#define ESC     "\x1b[0m"
#define ASSERTED != 0 || printf("ERROR")




#define DEBUG(stk) if ( ( err = func_verify(stk)) ){ \
        dump(stk, S_LOG, err);\
        p_error(err, stdout);\
        fprintf(stdout, "\n");\
        return err; \ 
    } 


#ifdef NDEBUG


static void set_stack_inf(stack* stk, int line, const char* func, const char* file, const char* name/*, FILE* log CHANGED*/) {

    (line && func && file && file && name/* && log CHANGED*/)ASSERTED;
    stk->info.file = file;
    stk->info.func = func;
    stk->info.name = name;
    stk->info.line = line;
    //stk->info.log = log; WAS CHANGED     
    return;
}

void print_stack_inf(stack* stk) {

    printf("%s ", stk->info.name);
    printf("at %s", stk->info.func);
    printf(" at %s", stk->info.file);
    printf("(%d)\n", stk->info.line);
    return;

}

#endif
void do_dump(stack* stk, const char* func, int line, const char* file, FILE* out, int err) {

    assert(func && line && file && out);
    fprintf(out, "%s at %s(%d):\n", func, file, line);
    if(!stk) {
        fprintf(out, "[%p]\n", (void*)stk);
        return;
    }
    fprintf(out, "%s[%p]", stk->info.name, (void*)stk);
    fprintf(out, "(");


    if ( err == 0 ) fprintf(out, "ok");
    else p_error(err, out);
    fprintf(out, ") ");
    fprintf(out, "at %s", stk->info.func);
    fprintf(out, " at %s", stk->info.file);
    fprintf(out, "(%d)\n", stk->info.line);
    fprintf(out, "{\n \tsize = %lu;\n \tcap = %lu;\n \tdata [%p]\n", stk->size, stk->capacity, (void*)(stk->data));
    
    if( err == DATA_NULL_PTR || err == BAD_DATA_PTR){
        printf("}");
        return;
    }

    for(size_t i = 0; i < stk->size && i < stk->capacity; i++) {
    
        fprintf( out, " \t*[%lu] = %d\n", i, ((el_type*)((char*)stk->data + CANARY_SIZE))[i]);
    
    }
    
    for(size_t i = stk->size; i < stk->capacity; i++) {
        
        fprintf( out, " \t[%lu] = %d ", i, ((el_type*)((char*)stk->data + CANARY_SIZE))[i]);
        if (((el_type*)((char*)stk->data + CANARY_SIZE))[i] == POISONED_EL) fprintf(out, "(POISON)");
        fprintf(out, "\n");
    }
    
    fprintf(out, " }\n}\n\n");
    //fprintf( out, "[%lu] = %d") DEBUG
    fclose(out);


}



#define dump(stk, out, err) do_dump(stk, __FUNCTION__, __LINE__, __FILE__, out, err)


static int stack_resize_down(stack* stk) {

    int err = 0;
    //dump(stk, LOG, err);
    #ifdef NDEBUG
        DEBUG(stk)
    #endif
    //dump(stk, LOG, err); DEBUG
    /*for(size_t i = stk->size; i < (stk->capacity); i++ ) {

        ((el_type*)((char*)stk -> data + CANARY_SIZE))[i] = POISONED_EL;

    }*/

    void * tmp = NULL;
    #ifdef CANARY_PROT
    *(unsigned long long*)((char*)(stk->data) + CANARY_SIZE + stk->capacity*sizeof(el_type)) = 0;
    #endif
    //dump(stk, LOG, err);
    tmp = realloc(stk->data, sizeof(el_type) * (stk -> capacity) / resize_up_const /*resize_down_const DEBUG*/ + 2 * CANARY_SIZE );
    
    if( !tmp ) return ALLOC_ERR;

    stk -> data = (el_type*)tmp;
    stk -> capacity /= resize_up_const/*resize_down_const DEBUG*/;
    #ifdef CANARY_PROT

    //memcpy((char*)(stk->data) + CANARY_SIZE + stk->capacity*sizeof(el_type), &CANARY, CANARY_SIZE);CHANGED  
    *(unsigned long long*)(((char*)(stk->data) + sizeof(el_type)*(stk->capacity)) + CANARY_SIZE) = CANARY;
    #endif
    #ifdef HASH_PROT
    stk->hash = rot13((char*)stk->data + CANARY_SIZE, stk->capacity); 
    #endif
    #ifdef NDEBUG
        DEBUG(stk)
    #endif
    return err;
}

static int stack_resize(stack* stk) {

    int err = 0;
    
    #ifdef NDEBUG
        DEBUG(stk)
    #endif

    void* tmp = NULL;
    tmp = realloc(stk -> data, sizeof(el_type)*(stk -> capacity) * resize_up_const + 2*CANARY_SIZE); //add tmp to check null ptr
   
    if ( !tmp ) return S_ALLOC_ERR;
    
    stk->data = (el_type*)tmp;
    stk -> capacity *= resize_up_const;

    for(size_t i = stk->size; i < stk->capacity; i++){
        
        ((el_type*)((char*)stk->data + CANARY_SIZE))[i] = 0;

    }
    #ifdef CANARY_PROT
    //memcpy((char*)(stk->data) + CANARY_SIZE + stk->capacity*sizeof(el_type), &CANARY, CANARY_SIZE); CHANGED
    *(unsigned long long*)((char*)(stk->data + stk->capacity) + CANARY_SIZE) = CANARY; //too
    #endif
    return err;
}



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
    if (stk->size > stk->capacity) err |= CAP_LES_SIZE;
    if(stk->status == ZOMBIE) err |= ZOMBIE_STAT;
    if (!stk->data) return err |= DATA_NULL_PTR;
    if (stk->data && stk->capacity == 0 ) return err |= DATA_NULL_CAP;

    #ifdef CANARY_PROT
    if(stk->LEFT_CANARY != CANARY || stk->RIGHT_CANARY != CANARY 
    || *(unsigned long long*)stk->data != CANARY || *(unsigned long long*)((char*)stk->data + sizeof(el_type)*stk->capacity + CANARY_SIZE ) != CANARY) err |= DEAD_CANARY;
    #endif

    #ifdef HASH_PROT
    unsigned long long old_hash = stk->hash;
    if(rot13((char*)(stk -> data) + CANARY_SIZE, stk->capacity) != old_hash) err |= BAD_HASH;
    #endif

    return err;
}



void p_error(int err_code, FILE* out) {

    int i = 0;
    char *red = "", *esc = "";

    if ( out == stdout ) {
    
        red = "\x1b[31m";
        esc = "\x1b[0m";
    }

    if (err_code == 1) {
        
        fprintf(out, "%sSTACK NULL PTR%s ",RED, ESC);
        return;
    }

    err_code /= 2;

     while (err_code) {

        i++;

        if(err_code % 2){   

            switch (i) {
                
            case 1:
                    
                fprintf(out, "%sDATA NULL PTR%s, ", red, esc);
                break;
            
            case 2:

                fprintf(out,"%sCAPACITY LESS THAN SIZE%s, ", red, esc);
                break;
            
            case 3:

                fprintf(out, "%sZOMBIE STATUS%s, ", red, esc);
                break;

            case 4:

                fprintf(out, "%sSTACK CAPACITY IS 0%s, ", red, esc);
                break;
            
            case 5:

                fprintf(out, "%sALLOCATION ERROR%s, ", red, esc);
                break;

            case 6:

                fprintf(out, "%sDATA HAS BAD PTR%s, ", red, esc);
                break;

            case 7:

                fprintf(out, "%sCANARY IS DEAD%s, ", red, esc);
                break;

            case 8:

                fprintf(out, "%sBAD HASH%s, ", red, esc);
                break;
            
            default:

                fprintf(out, "%sUNKNOWN ERROR CODE%s ", red, esc);
                break;
            }
    
        }
        err_code /= 2;
    }

    return;
}

int _stack_ctor(stack* stk, size_t size, int line, const char* func, const char* file, const char* name/*, FILE* log CHANGED*/) {

    int err = 0;

    #ifdef NDEBUG
    if ( ( err = func_verify(stk)) == STACK_NULL_PTR || stk->status == S_ALIVE){ 
        dump(stk, S_LOG, err);
        return err; 
    } 
    #endif
    stk -> data = (el_type*) calloc(size*sizeof(el_type) + 2*CANARY_SIZE, sizeof(char));
 
    stk -> size = 0;
    stk -> capacity = size;
    stk -> status = S_ALIVE;

    #ifdef CANARY_PROT
    stk->LEFT_CANARY = CANARY;
    stk->RIGHT_CANARY = CANARY;
    #endif

    #ifdef NDEBUG
    set_stack_inf(stk, line, func, file, name/*, log CHANGED*/);
        if ( ( err = func_verify(stk)) == 2){ 
        dump(stk, S_LOG, err);
        return err; 
    } 
    #endif

    #ifdef CANARY_PROT
    //memcpy(stk->data, &CANARY, CANARY_SIZE); CHANGED
    *(unsigned long long*)((char*)(stk->data)) = CANARY;
    //memcpy((char*)(stk->data) + CANARY_SIZE + stk->capacity*sizeof(el_type), &CANARY, CANARY_SIZE); CHANGED
    *(unsigned long long*)((char*)(stk->data + stk->capacity) + CANARY_SIZE) = CANARY; //too
    #endif

    #ifdef HASH_PROT
    stk->hash = rot13((char*)stk->data + CANARY_SIZE, stk->capacity);
    #endif

    return err;
}



int stack_push(stack* stk, el_type value) {

    int err = 0;

    #ifdef NDEBUG
        DEBUG(stk)
    #endif

    if( stk -> size >= stk -> capacity ) stack_resize(stk);
    
    ((el_type*)((char*)(stk -> data) + CANARY_SIZE))[stk -> size++] = value;
    
    #ifdef HASH_PROT
    stk->hash = rot13((char*)stk->data + CANARY_SIZE, stk->capacity);
    #endif

    #ifdef NDEBUG
    DEBUG(stk)
    #endif

    //dump(stk, S_LOG, err); DEBUG
    return err;
}
//important func to static 


int stack_pop(stack* stk, el_type* value) {

    
    int err = 0;
    #ifdef NDEBUG
    (value)ASSERTED;
        DEBUG(stk)
    #endif
//Print errors to cmd
    if(stk->size == 0) return printf("ERROR NOTHING TO POP\n");

    *value = ((el_type*)((char*)stk -> data + CANARY_SIZE))[stk -> size - 1];
    ((el_type*)((char*)stk ->data + CANARY_SIZE))[stk -> size - 1] = POISONED_EL;
    stk -> size--;
    #ifdef HASH_PROT
    stk->hash = rot13((char*)stk->data + CANARY_SIZE, stk->capacity);
    #endif

    if ( stk -> size <= (stk -> capacity / resize_down_const) && (stk->capacity/resize_down_const) >= 1) stack_resize_down(stk);
    
    #ifdef HASH_PROT
    stk->hash = rot13((char*)stk->data + CANARY_SIZE, stk->capacity);
    #endif

    #ifdef NDEBUG
        DEBUG(stk)
    #endif
    //dump(stk, stk->info.log, err); DEBUG
    return err; 
}

int stack_dtor(stack* stk) {

    int err = 0;
    #ifdef NDEBUG
        DEBUG(stk)  
    #endif

    free(stk -> data);
    stk -> data = POISONED_DATA; //(void*)1488
    stk -> size = 1;
    stk -> capacity = 0;
    stk -> status = ZOMBIE;
    
    /*#ifdef HASH_PROT CHANGED 
    stk->hash = rot13(stk->data, stk->capacity);
    #endif

    #ifdef NDEBUG
    if ( (err = func_verify(stk)) ) dump(stk, S_LOG, err);
    
    fclose(stk->info.log);
    #endif*/
    return err;
}
