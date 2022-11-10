#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include <string.h>
#include "io.hpp"
#include "akinator.hpp"
#include "list.hpp"

int main() {

    node_t node = { };
    list_t seek_list = { };
    list_t seek_list2 = { };
    tree_ctor(&node);
    list_ctor( &seek_list, DEF_LIST_SZ);
    list_ctor( &seek_list2, DEF_LIST_SZ);
    FILE* dump = fopen( DUMP, "r" );
    upload( &node, dump );
    fclose(dump);
    char* string = (char*)calloc( MAX_LINE, sizeof(char) );

    while ( 1 ) {

        printf( "What do you wanna do?\n[P]lay, [S]how tree, [C]ompare, [G]et definition, [E]xit.\n");
        system ( " echo \"What do you wanna do?\n\" | festival --tts ");
        scanf( "%s", string );
        while( stricmp(string, "P") && stricmp(string, "S") && stricmp(string, "C") && stricmp(string, "G") && stricmp(string, "E") ) {
        
            printf( "Sorry, I don't understand\n");
            system ( " echo \"Sorry, I don't understand\n\" | festival --tts ");
            scanf( "%s", string);
        
        }

        if ( !stricmp( string, "P") ) {
            
            game( &node, &seek_list );
        
        } else {

            if ( !stricmp( string, "S") ) {
            
                show_tree( &node );
            
            } else {
 
                if ( !stricmp( string, "C") ) {
            
                    compare( &node, &seek_list, &seek_list2 );
            
                }

                else {

                    if ( !stricmp( string, "G") ) {
            
                        show_def( &node, &seek_list );
            
                    } else {

                        break;

                    }

                }

            }

        }

        if ( !stricmp( string, "N") ) {
            
            free(string);
            return 0;
            
        }

    }

    do_dump( &node );
    list_dtor( &seek_list );
    list_dtor( &seek_list2 );
    tree_dtor( &node, node.node );
    free( string );
    return 0;

}