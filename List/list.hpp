#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef char el_t;

#define ON 1
#define OFF 0
#define DEAD 0xDEAD
#define ALIVE 0x742617000027

typedef struct _list_t {
    
    el_t* data;
    size_t head;
    size_t* prev;
    size_t* next;
    size_t size;
    size_t free;
    size_t capacity;
    int happines_flag;
    int status;
} list_t;

enum ERROR {
    
    LIST_NULL_PTR  = 1,
    DATA_ALLOC_ERR = 2,
    PREV_ALLOC_ERR = 3,
    NEXT_ALLOC_ERR = 4,
    NULL_SIZE      = 5,
    OUT_OF_MEM     = 6,
    CAP_LESSER_SZ  = 7,
    DATA_NULL_PTR  = 8,
    PREV_NULL_PTR  = 9,
    NEXT_NULL_PTR  = 10,
    HEAD_SEG_FAULT = 11,
    FREE_SEG_FAULT = 12,

};

int list_verify( list_t* list, int err );

int list_ctor( list_t* list, size_t size );

size_t list_find ( list_t* list, size_t place );

int list_add ( list_t* list, size_t place, el_t value );

int list_dump( list_t* list);

#endif