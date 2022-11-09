#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include <string.h>
#include "akinator.hpp"
#include "io.hpp"
#include  "stack.hpp"

int upload ( node_t* node, FILE* file ) {

    stack leaves;
    stack_ctor( leaves, DEF_STK_SIZE);

    int err = 0;
    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !file ) return NULL_FILE;

    char* text = NULL;
    size_t text_sz = 0, i = 0;
    buf_read( &text, file, &text_sz);
    char* data = (char*)calloc(MAX_LINE, sizeof(char) );
    size_t offset = 2;
    sscanf( text + offset, "%[^\n]", data);
    offset += strlen(data);
    free(node->node->data);
    (node->node)->data = strdup(data); 
    //stack_push( &leaves, node->node);
    tree_t* leave = node->node;
    stack_push( &leaves, leave );
    i = offset+1;

    while ( i < (text_sz ) && text[i] != '\0' ) {
        
        if ( text[i] == '{'){
            stack_push( &leaves, leave );
            i++;
            sscanf( text + i, "%[^\n]", data );
            //printf("%s\n", data);
            i+=strlen( data ) + 1;
            if ( !leave->left ){
                tree_append_left( node, leave, data );
                leave = leave->left;
            }
            else {
                tree_append_right( node, leave, data );
                leave = leave->right;
            }

        }

        if ( text[i] == '}' ){
            i+=2;
            stack_pop(&leaves, &leave);
        }
        
    }
    stack_dtor;
    free( data );
    free( text );

    return 0;

}