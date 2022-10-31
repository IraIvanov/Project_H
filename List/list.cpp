#include <stdio.h>
#include "list.hpp"
#include <stdlib.h>

int list_verify( const list_t* list ){

    int err = 0;
    if ( !list ) return LIST_NULL_PTR;
    if ( list->capacity < list->size) err += CAP_LESSER_SZ;
    if ( !list->data ) err += DATA_NULL_PTR;
    if ( !list->prev ) err += PREV_NULL_PTR;
    if ( !list->next ) err += NEXT_NULL_PTR;
    if ( list->free > list->capacity ) err += FREE_SEG_FAULT;
    if ( list->size < 0 ) err += NULL_SIZE;
    if ( list->capacity <= 0) err += NULL_CAP;
    if ( list->status != ALIVE ) err += DEAD_LIST;
    if ( list->prev == (int*)DED_PTR || list->next == (int*)DED_PTR || list->data == (el_t*)DED_PTR ) err += INVALID_ERR;

    return err;
}

int list_ctor( list_t* list, int size ) {

    if ( size <= 0) return NULL_SIZE;
    
    if ( !list ) return LIST_NULL_PTR;

    if ( list->status == ALIVE ) {
        
        fprintf( stderr, "%s\n", "ERROR, TRYING TO RECREATE LIST" );
        return CTOR_ERR;

    } 
    
    list -> data = (el_t*)calloc( size, sizeof(el_t));
    
    if ( !(list->data) ) return DATA_ALLOC_ERR;

    list -> prev = (int*)calloc( size, sizeof(int));

    if ( !(list->prev) ) return PREV_ALLOC_ERR;

    list -> next = (int*)calloc( size, sizeof(int));

    if ( !(list->next) ) return NEXT_ALLOC_ERR;

    list->size = 0;
    list->status = ALIVE;
    list->capacity = size;
    list->free = 1;
    list->happinez_wolfanino_flag = ON; //change after adding sort 

    (list->next)[size - 1] = 0;

    for (int i = 0; i < (list->capacity); i++){
    
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

int list_find ( list_t* list, int place ) {

    if ( !list ) return 0; //list_ver , p_err 

    if ( place > list->size ) return 0;
    
    int i = 0;
    int plc = (list->next)[0];

    while( i < place ) {

        plc = (list->next)[plc];
        i++;
    }

    return plc;
}


int list_add ( list_t* list, int place, el_t value ) {

    //replace all returns with verify
    int err = 0;

    if ( place < 0 ) return NULL_SIZE;

    if ( (err = list_verify( list ) ) != 0 ) return err;

    if ( list->free == 0 || list->size >= list->capacity ) return OUT_OF_MEM;
    
    if ( place != 0 && place != (list->prev)[0] ) list->happinez_wolfanino_flag = OFF;

    int free_plc = list->free, tmp_next_plc = 0;
    list->free = (list->next)[free_plc];
    (list->data)[free_plc] = value;
    //list_dump(list);
    tmp_next_plc = (list->next)[place];
    (list->prev)[free_plc] = place;
    (list->next)[free_plc] = tmp_next_plc;
    (list->next)[place] = free_plc;
    (list->prev)[tmp_next_plc] = free_plc;
    list->size += 1;
    return 0;
}

int list_dump( list_t* list) {

    int err = 0;

    if ( !list ) return LIST_NULL_PTR;

    if ( (err = list_verify(list)) < 0) return err;

    int pos = (list->next)[0];

    FILE* log = fopen(LOG, "a");

    fprintf ( log, "size is %d\n capacity is %d\n data ptr is %p\n prev ptr is %p\n next ptr is %p\n", list->size, list->capacity, (void*)(list->data), (void*)(list->prev), (void*)(list->next));
    for ( int i = 0; i < list->size && pos != 0 ; i++ ){

        fprintf(log, "[%d] prev <- [%c] -> next [%d]\n", (list->prev)[pos], (list->data)[pos] ,(list->next)[pos]);

        pos = (list->next)[pos];

    }
    
    fclose(log);

    return 0;
}

int list_dtor( list_t* list ) {

    int err = 0;
    err = list_verify( list );

    if ( err & LIST_NULL_PTR || err & DATA_NULL_PTR || err & PREV_NULL_PTR || err & NEXT_NULL_PTR || err & DEAD_LIST || err & INVALID_ERR ) return err;

    free(list->data);
    free(list->prev);
    free(list->next);

    list->data = (el_t*)DED_PTR;
    list->next = (int*)DED_PTR;
    list->prev = (int*)DED_PTR;

    list->capacity = -1;
    list->size = -1;
    list->free = 0;
    list->status = DEAD;

    return 0;
}

int list_remove( list_t* list, int place ) {

    int err = 0;

    if ( place != (list->next)[0] && place != (list->prev)[0] ) list->happinez_wolfanino_flag = OFF;

    if ( (err = list_verify( list ) ) != 0 ) return err;

    if ( place <= 0 || list->size == 0 ) return NULL_SIZE;

    int tmp_free = list->free, tmp_next = list->next[place], prev = (list->prev)[place];
    list->free = place;
    (list->next)[place] = tmp_free;
    (list->next)[prev]  = tmp_next;
    (list->prev)[tmp_next] = prev;
    (list->prev)[place] = -1;
    list->size -= 1;

    return 0;
}

int show_flag( list_t* list ) {

    int err = 0;

    if ( (err = list_verify( list ) ) != 0 ) return err;

    return list->happinez_wolfanino_flag;

}

int list_sort( const list_t* dest, list_t* res ) {

    int err = 0;

    if ( (err = list_verify( dest ) ) != 0 ) return err;

    int size = dest->capacity, range = dest->size, place = (dest->next)[0];
    
    if ( !res ) return LIST_NULL_PTR;

    if ( list_ctor( res, size ) < 0 ) return CTOR_ERR;

    for ( int i = 0 ; i < range && place != 0; i++) {

        if ( (err = list_add( res, i, (dest->data)[place])) < 0 ){
            
            return err;

        }

        place = (dest->next)[place];

    }

    return 0;

}

int list_realloc( list_t* list, int size ){

    int err = 0;

    if ( (err = list_verify( list ) ) < 0) return err;

    if ( size < list->size ) return NULL_SIZE;

    if ( list->happinez_wolfanino_flag != ON ){

    fprintf( stderr, "%s\n", "ERROR, LIST HAVE TO BE SORTED");
    return SADDNES;

    }

    el_t* tmp_data = (el_t*)realloc(list->data, size*sizeof(el_t));

    if ( !tmp_data ) return DATA_ALLOC_ERR;

    int *tmp_next = (int*)realloc(list->next, size*sizeof(int));

    if ( !tmp_next ) return NEXT_ALLOC_ERR;

    int *tmp_prev = (int*)realloc(list->prev, size*sizeof(int));

    if ( !tmp_prev ) return PREV_ALLOC_ERR;

    list->data = tmp_data;

    list->next = tmp_next;

    list->prev = tmp_prev;

    list->capacity = size;

    for ( int i = (list->size + 1); i < size; i++) {

        (list->prev)[i] = -1;
        (list->next)[i] = list->free;
        list->free = i; 

    }

    return 0;

}