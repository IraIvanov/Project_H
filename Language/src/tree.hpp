#include <stdio.h>
#include <stdlib.h>

#ifndef _TREE_H_INCLUDED_
#define _TREE_H_INCLUDED_

#define DEAD 0xDEAD
#define ALIVE 74261727
#define PSN_DATA ""
#define DEF_DATA "Nobody"
#define LOG "log.txt"
#define DUMP "dump.txt"
#define MAX_LINE 1024

enum types{

    NUM = 0,
    VAR = 1,
    OP  = 2,

};

typedef union _data_t { 

    double num;
    char var;
    char op;

} data_t;

typedef struct _element_type {

    data_t data;
    int type;

} element_type;

typedef element_type* el_t; 

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

int node_verify( const node_t* node );

int tree_append_left( node_t* node, tree_t* tree, const el_t data );

int tree_append_right( node_t* node, tree_t* tree, const el_t data );

int tree_dtor ( node_t* node, tree_t* tree );

int leave_free ( node_t* node, tree_t* tree );

int graph_dump ( const node_t* node );

int do_dump ( const node_t* node );

int tree_print ( const tree_t* tree, FILE* file, int* i);

int tree_pprint( const tree_t* tree );

int tree_rprint( const tree_t* tree );

int get_hight( tree_t* node, int i );

#endif