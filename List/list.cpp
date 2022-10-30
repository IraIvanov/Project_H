#include <stdio.h>
#include "list.hpp"
#include <stdlib.h>

int list_verify( list_t* list, int err ){

    if ( !list ) return LIST_NULL_PTR;
    if ( list->capacity < list->size) return CAP_LESSER_SZ;
    if ( !list->data ) return DATA_NULL_PTR;
    if ( !list->prev ) return PREV_NULL_PTR;
    if ( !list->next ) return NEXT_NULL_PTR;
    if ( !list->head > list->capacity ) return HEAD_SEG_FAULT;
    if ( !list->free > list->capacity ) return FREE_SEG_FAULT; 

    return -1;
}

int list_ctor( list_t* list, size_t size ) {

    if ( size == 0) return NULL_SIZE;
    
    if ( !list ) return LIST_NULL_PTR;
    
    list -> data = (el_t*)calloc( size, sizeof(el_t));
    
    if ( !(list->data) ) return DATA_ALLOC_ERR;

    list -> prev = (size_t*)calloc( size, sizeof(size_t));

    if ( !(list->prev) ) return PREV_ALLOC_ERR;

    list -> next = (size_t*)calloc( size, sizeof(size_t));

    if ( !(list->next) ) return NEXT_ALLOC_ERR;

    list->size = 0;
    list->capacity = size;
    list->head = 1;
    list->free = 1;
    list->happines_flag = OFF; //change after adding sort 

    (list->next)[size - 1] = 0;

    for (size_t i = 0; i < (list->capacity); i++){
    
        (list->next)[i] = i + 1;
        (list->prev)[i] = -1;
    } 
    (list->next)[size - 1] = 0;
    (list->next)[0] = 0;
    (list->prev)[1] = 0;
    (list->prev)[0] = 0;
    //(list->prev)[size - 1] = -1;

    return 0;
}

size_t list_find ( list_t* list, size_t place ) {

    if ( !list ) return 0; //list_ver , p_err 

    if ( place > list->size ) return 0;
    
    size_t i = 0;
    size_t plc = list->head;

    while( i < place ) {

        plc = (list->next)[plc];
        i++;
    }

    return plc;
}


int list_add ( list_t* list, size_t place, el_t value ) {

    //replace all returns with verify
    if ( !list ) return LIST_NULL_PTR;

    if ( list->free == 0 || list->size >= list->capacity ) return OUT_OF_MEM;
    
    size_t free_plc = list->free, tmp_prev_plc = 0;
    list->free = (list->next)[free_plc];
    (list->data)[free_plc] = value;
    //list_dump(list);
    tmp_prev_plc = (list->next)[place];
    (list->prev)[free_plc] = place;
    (list->next)[free_plc] = tmp_prev_plc;
    (list->next)[place] = free_plc;
    (list->prev)[tmp_prev_plc] = free_plc;
    /*
    tmp_prev_plc = (list->prev)[place];
    (list->prev)[free_plc] = tmp_prev_plc;
    (list->next)[tmp_prev_plc] = free_plc;
    (list->prev)[place] = free_plc;
    (list->next)[free_plc] = place;*/
    //if ( )
    list->size += 1;
    return 0;
}

int list_dump( list_t* list) {

    if ( !list ) return LIST_NULL_PTR;

    for ( size_t i = 0; i < list->size; i++ ){

        printf("[%lu] prev <- [%c] -> next [%lu]\n", (list->prev)[i], (list->data)[i] ,(list->next)[i]);

    }
    

    return 0;
}

int list_dtor( list_t* list) {

    return 0;
}




