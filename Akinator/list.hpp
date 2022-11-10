#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef struct _list_el_t_data{

    char* data;
    int flag;

}list_el_t_data;

typedef list_el_t_data* list_el_t;

#define ON 1
#define OFF 0
#define DEAD 0xDEAD
#define ALIVE 74261727
#define L_LOG "l_log.txt"
#define POISON  (list_el_t)0xDEAD

#define el_specify "%c"
#define null_data (list_el_t)0xBABE
#define MAX_LINE 1024
#define FREE -1
#define DEF_LIST_SZ 64

#define DED_PTR 0xDEADBABE

typedef struct _list_t {
    
    list_el_t* data;
    int* prev;
    int* next;
    int size;
    int free;
    int capacity;
    int happinez_wolfanino_flag;
    int status;
} list_t;

enum L_ERROR {
    
    LIST_NULL_PTR  = -1,
    DATA_ALLOC_ERR = -2,
    PREV_ALLOC_ERR = -4,
    NEXT_ALLOC_ERR = -8,
    NULL_SIZE      = -16,
    OUT_OF_MEM     = -32,
    CAP_LESSER_SZ  = -64,
    L_DATA_NULL_PTR  = -128,
    PREV_NULL_PTR  = -256,
    NEXT_NULL_PTR  = -512,
    HEAD_SEG_FAULT = -1024,
    FREE_SEG_FAULT = -2048,
    NULL_CAP       = -4096,
    DEAD_LIST      = -8192,
    L_CTOR_ERR     = -16384,
    INVALID_ERR    = -32768,
    SADDNES        = -65536,

};

int list_verify( const list_t* list );

int list_ctor( list_t* list, int size );

int list_find ( list_t* list, int place );

int list_add ( list_t* list, int place, list_el_t value );

int list_dump( list_t* list, const char* name);

int list_dtor( list_t* list );

int list_remove( list_t* list, int place );

int list_sort( const list_t* dest, list_t* res );

int show_flag( list_t* list );

int list_realloc( list_t* list, int size );

int list_print( list_t* list );

int list_free( list_t* list );

#endif