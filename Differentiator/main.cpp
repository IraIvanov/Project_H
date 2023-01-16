#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "diff.hpp"

int main() {

    char* str = (char*)calloc( 1024, sizeof(char) ); 
    scanf( "%s", str );
    printf( "%s\n", str);
    node_t node = {};
    tree_ctor( &node );
    tree_upload( str, &node );
    tree_rprint( node.node );
    printf("\n");
    node_t* res = Diff( &node );
    Diff( NULL );
    tree_pprint ( res->node );
    printf("\n");
    graph_dump( res );
    const_erase( res );
    const_erase( res );
    const_erase( res );
    const_erase( res );
    tree_pprint ( res->node );
    printf("\n");
    tree_latex( res, &node );
    //printf("\n");
    graph_dump( res );
    tree_dtor( res, res->node );
    tree_dtor( &node, node.node);
    free(res);
    free(str);
    return 0;

}