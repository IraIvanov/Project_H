#include <stdio.h>
#include <stdlib.h>
#include "./src/tree.hpp"
#include "./src/diff.hpp"
#include "./src/lexer.hpp"
#include "./src/lang.hpp"

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
    FILE* file = fopen( "code.txt", "w" );
    graph_dump( &node );
    tree_pprint( node.node );
    code_gen( &node, file );
    printf("\n");
    tree_dtor( &node, node.node);
    free(str);
    return 0;

}