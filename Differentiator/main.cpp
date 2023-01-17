#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "diff.hpp"
#include "lexer.hpp"

int main() {

    char* str = (char*)calloc( 1024, sizeof(char) ); 
    scanf( "%s", str );
    printf( "%s\n", str);
    node_t node = {};
    tree_ctor( &node );
    get_tree( str, &node );
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