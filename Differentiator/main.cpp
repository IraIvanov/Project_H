#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "diff.hpp"
#include "lexer.hpp"

int main() {

    char* str = (char*)calloc( MAX_SIZE, sizeof(char) ); 
    fgets( str, MAX_SIZE, stdin );
    printf( "%s\n", str);
    node_t node = {};
    tree_ctor( &node );
    int err = get_tree( str, &node );
    
    if ( err ) {

        free( str );
        tree_dtor( &node, node.node );    
        return 0;

    }

    tree_rprint( node.node );
    printf("\n");
    node_t* res = Diff( &node );
    Diff( NULL );
    graph_dump( res );
    const_erase( res );
    tree_pprint ( res->node );
    printf("\n");
    tree_latex( res, &node );
    tree_dtor( res, res->node );
    tree_dtor( &node, node.node);
    free(res);
    free(str);
    return 0;

}