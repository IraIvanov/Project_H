#include <stdio.h>
#include <stdlib.h>
#include "list.hpp"

#ifndef _TREE_H_INCLUDED_
#define _TREE_H_INCLUDED_

#define DEAD 0xDEAD
#define ALIVE 74261727
#define PSN_DATA ""
#define DEF_DATA "Nobody"
#define LOG "log.txt"
#define DUMP "dump.txt"
#define MAX_LINE 1024

typedef char* el_t;

typedef struct _tree_t {

    el_t data;
    struct _tree_t* left;
    struct _tree_t* right;

} tree_t;

typedef struct _node_t {

    tree_t* node;
    int size;
    int status;

} node_t;

enum ERROR {
    
    NULL_NODE  = -1,
    NULL_TREE  = -2,
    CTOR_ERR   = -4,
    BAD_SIZE   = -8,
    BAD_STAT   = -16,
    BAD_DATA   = -32,
    ADD_ERR    = -64,
    ALLOC_ERR  = -128,
    LEAV_ERR   = -256,
    NULL_VALUE = -512,

};

int tree_ctor( node_t* node/*, tree_t* tree*/ );

int node_verify( node_t* node );

int tree_append_left( node_t* node, tree_t* tree, el_t data );

int tree_append_right( node_t* node, tree_t* tree, el_t data );

int tree_dtor ( node_t* node, tree_t* tree );

int leave_free ( node_t* node, tree_t* tree );

int graph_dump ( node_t* node );

int do_dump ( node_t* node );

int tree_seek( node_t* node, tree_t* tree, int* i, el_t value, list_t *list, int n );

#endif