#include <stdio.h>
#include "list.hpp"
#include <stdlib.h>
#include <string.h>

int list_verify( const list_t* list ){

    int err = 0;
    if ( !list ) return LIST_NULL_PTR;
    if ( list->capacity < list->size) err += CAP_LESSER_SZ;
    if ( !list->data ) err += L_DATA_NULL_PTR;
    if ( !list->prev ) err += PREV_NULL_PTR;
    if ( !list->next ) err += NEXT_NULL_PTR;
    if ( list->free > list->capacity ) err += FREE_SEG_FAULT;
    if ( list->size < 0 ) err += NULL_SIZE;
    if ( list->capacity <= 0) err += NULL_CAP;
    if ( list->status != ALIVE ) err += DEAD_LIST;
    if ( list->prev == (int*)DED_PTR || list->next == (int*)DED_PTR || list->data == (list_el_t*)DED_PTR ) err += INVALID_ERR;

    return err;
}

int list_ctor( list_t* list, int size ) {

    if ( size <= 0) return NULL_SIZE;
    
    if ( !list ) return LIST_NULL_PTR;

    if ( list->status == ALIVE ) {
        
        fprintf( stderr, "%s\n", "ERROR, TRYING TO RECREATE LIST" );
        return L_CTOR_ERR;

    } 
    
    list -> data = (list_el_t*)calloc( size, sizeof(list_el_t));
    
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
        (list->prev)[i] = FREE;
        (list->data)[i] = POISON;
    } 
    (list->next)[size - 1] = 0;
    (list->next)[0] = 0;
    (list->prev)[1] = 0;
    (list->prev)[0] = 0;
    (list->data)[0] = null_data;
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


int list_add ( list_t* list, int place, list_el_t value ) {

    //replace all returns with verify
    int err = 0;

    if ( place < 0 ) return NULL_SIZE;

    if ( (err = list_verify( list ) ) != 0 ) return err;

    if ( list->free == 0 || list->size >= list->capacity ) return OUT_OF_MEM;
    
    if ( (list->prev)[place] == FREE ) {

        fprintf(stderr,  "Root element is empty!\n");
        return INVALID_ERR;

    }

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

int list_dump( list_t* list, const char* name) {

    int err = 0;

    if ( !name ) return INVALID_ERR;

    if ( (err = list_verify(list)) < 0) return err;

    int pos = (list->next)[0];

    FILE* log = fopen(L_LOG, "w");

    fprintf( log, "digraph G {\n\trankdir=LR;\ngraph [dpi = 100];\n spline = ortho\n");
    //fprintf ( log, "size is %d\n capacity is %d\n data ptr is %p\n prev ptr is %p\n next ptr is %p\n"\
    , list->size, list->capacity, (void*)(list->data), (void*)(list->prev), (void*)(list->next));
    for ( int i = 0; i < list->capacity; i++){
        
        if ( (list-> data)[i] == POISON) fprintf( log, "\t node%d [shape=\"record\",  style=\"rounded, filled\", fillcolor = \"#fb45a7\" label=\" index %d | value PSN | { prev %d | next %d } \" ];\n", i, i,\
        (list->prev)[i], (list->next)[i]);

        else fprintf( log, "\t node%d [shape=\"record\", style=\"rounded, filled\", fillcolor = \"#9d4cef\" label=\" index %d | value %s | { prev %d | next %d } \" ];\n", i, i,\
        (list->data)[i]->data, (list->prev)[i], (list->next)[i]);

    }

    for ( int i = 0; i < list->capacity /*&& pos != 0*/ ; i++ ){

        if ( (list->prev)[i] == FREE )  fprintf(log, "\tnode%d->node%d [color = red];\n ", i, (list->next)[i]);
        else fprintf(log, "\tnode%d->node%d [color = blue];\n", i, (list->next)[i]);
        //fprintf(log, "[%d] prev <- [%c] -> next [%d]\n", (list->prev)[pos], (list->data)[pos] ,(list->next)[pos]);
        //pos = (list->next)[pos];

    }
    fprintf(log, "}\n");

    fclose(log);

    char *command = (char*)calloc(MAX_LINE, sizeof(char));
    strcpy(command, "dot -Tsvg log.txt > ");
    command = strcat(command, name);
    command = strcat(command, ".svg");
    system(command);
    free(command);
    return 0;
}

int list_print( list_t* list ) {

    int err = 0;
    if ( (err = list_verify(list)) < 0) return err;

    int pos = (list->next)[0];
    while ( pos != 0 ) {

        if ( (list->data)[pos]->flag == 0) fprintf( stdout, "not ");
        fprintf( stdout, "%s\n", (list->data)[pos]->data );
        pos = (list->next)[pos];

    }

    return 0;

}

int list_dtor( list_t* list ) {

    int err = 0;
    err = list_verify( list );

    if ( err & LIST_NULL_PTR || err & L_DATA_NULL_PTR || err & PREV_NULL_PTR || err & NEXT_NULL_PTR || err & DEAD_LIST || err & INVALID_ERR ) return err;

    free(list->data);
    free(list->prev);
    free(list->next);

    list->data = (list_el_t*)DED_PTR;
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

    if ( (list->prev)[place] == FREE ) {

        fprintf(stderr,  "Root element is empty!\n");
        return INVALID_ERR;

    }

    (list->data)[place] = POISON;
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

    if ( list_ctor( res, size ) < 0 ) return L_CTOR_ERR;

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

    list_el_t* tmp_data = (list_el_t*)realloc(list->data, size*sizeof(list_el_t));

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

int list_free( list_t* list ){

    int err = 0;
    if ( (err = list_verify( list )) < 0 ) return err;

    int pos = (list->prev)[0];
    
    while ( pos != 0 ) {

        free((list->data)[pos]);
        list_remove( list, pos );
        pos = (list->prev)[0];

    }

    return 0;

}