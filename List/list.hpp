#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef char el_t;

#define ON 1
#define OFF 0
#define DEAD 0xDEAD
#define ALIVE 74261727
#define LOG "log.txt"

#define DED_PTR 0xDEADBABE

typedef struct _list_t {
    
    el_t* data;
    int* prev;
    int* next;
    int size;
    int free;
    int capacity;
    int happinez_wolfanino_flag;
    int status;
} list_t;

enum ERROR {
    
    LIST_NULL_PTR  = -1,
    DATA_ALLOC_ERR = -2,
    PREV_ALLOC_ERR = -4,
    NEXT_ALLOC_ERR = -8,
    NULL_SIZE      = -16,
    OUT_OF_MEM     = -32,
    CAP_LESSER_SZ  = -64,
    DATA_NULL_PTR  = -128,
    PREV_NULL_PTR  = -256,
    NEXT_NULL_PTR  = -512,
    HEAD_SEG_FAULT = -1024,
    FREE_SEG_FAULT = -2048,
    NULL_CAP       = -4096,
    DEAD_LIST      = -8192,
    CTOR_ERR       = -16384,
    INVALID_ERR    = -32768,
    SADDNES        = -65536,

};

int list_verify( const list_t* list );

int list_ctor( list_t* list, int size );

int list_find ( list_t* list, int place );

int list_add ( list_t* list, int place, el_t value );

int list_dump( list_t* list);

int list_dtor( list_t* list );

int list_remove( list_t* list, int place );

int list_sort( const list_t* dest, list_t* res );

int show_flag( list_t* list );

int list_realloc( list_t* list, int size );

#endif