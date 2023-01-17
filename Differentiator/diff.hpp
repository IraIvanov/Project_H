#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

#define TYPE( node ) node->data->type

#define OP_DATA( node ) node->data->data.op

#define NUM_DATA( node ) node->data->data.num

#define RES_CTOR(dest) dest = (tree_t*)calloc( 1, sizeof( tree_t ) );\
                dest->data = (el_t)calloc( 1, sizeof(element_type) );

#define NODE_CPY( dest, src ) memcpy( dest->data, src->data, sizeof( element_type) );

    enum operations {
        
        ADD = 1,
        SUB = 2,
        MUL = 20,
        DIV = 21,
        DEG = 30,
        LG  = 31,
        SIN = 32,
        COS = 33,

    };

    int is_var( const int c );
    int is_op( const char* str );
    int tree_upload( const char* str, node_t* node );
    node_t* Diff ( const node_t* src );
    int const_erase ( node_t* node );
    int tree_latex( const node_t* node, const node_t* src );

#endif