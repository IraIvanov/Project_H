#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "diff.hpp"
#include <math.h>


static int _code_gen( const tree_t* tree, FILE* file ) {

    if ( !tree ) return NULL_TREE;
    if ( !file ) return NULL_VALUE;

    if ( tree -> left ) {
     
        _code_gen( tree->left, file );

    }

    if ( tree -> right ) { 

       _code_gen( tree->right, file );

    }

    if ( (tree->data)->type == NUM ) fprintf( file, "PUSH %g\n", (tree->data->data).num );
    else if ( (tree->data)->type == OP ) {
        switch ( (tree->data->data).op ) {

            case ADD: fprintf( file, "ADD\n" );
                break;
            case MUL: fprintf( file, "MUL\n" );
                break;
            case SUB: fprintf( file, "SUB\n" );
                break;
            case DIV: fprintf( file, "DIV\n" );
                break;
            case DEG: fprintf( file, "POW\n" );
                break;
            case LG: fprintf( file, "LN\n" );
                break;
            case COS: fprintf( file, "COS\n" );
                break;
            case SIN: fprintf( file, "SIN\n" );
                break;
            default: return 0;
        }
    }
    else if ( (tree->data)->type == VAR ) printf( "%c", (tree->data->data).var );

    return 0;
}

int code_gen( const node_t* node, FILE* file ) {

    int err = 0;
    
    if ( (err = node_verify( node )) != 0 ) return err;
    if ( !file ) return NULL_VALUE;

    _code_gen( node->node, file );
    fprintf( file, "OUT\nHLT\n");

    return 0;
}