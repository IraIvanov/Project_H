#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include <string.h>
#include "io.hpp"
#include "akinator.hpp"
#include "list.hpp"

int main() {

    tree_t* tree = NULL;
    node_t node = { };
    list_t seek_list = { };
    tree_ctor(&node);
    list_ctor( &seek_list, DEF_LIST_SZ);
    tree = node.node;
    FILE* dump = fopen( DUMP, "r" );
    upload( &node, dump );
    fclose(dump);
    do_dump( &node );
    char* string = (char*)calloc(MAX_LINE, sizeof(char)), *swap_str;
    while( 1 ) {

        if ( tree ){        
            
            printf( "It's %s ?\n [Y]es/[N]o?\n", tree->data);
            scanf( "%s", string);
            
            while( stricmp(string, "Y") && stricmp(string, "N") ) {
            
                printf(" Sorry, I don't understand, please type Y or N\n");
                scanf( "%s", string);
            
            }

            if ( !stricmp( string, "Y") ) {
                
                if ( !tree->left ) {
                    printf( "Hahaha, as I expected\n");
                    tree = NULL;
                }

                else tree = tree->left;    
                //tree = tree->left;
            }

            if ( !stricmp( string, "N") ) {
                
                if ( !tree->right ) {

                    printf( "So, who it's then ?\n");
                    while(getchar() != '\n' ){};
                    scanf( "%[^\n]", string);
                    int i = 0;
                    tree_seek( &node, node.node, &i, string, &seek_list, 0);

                    if ( i == 1 ){
                        printf( "This object already in database, here it's defenition:\n");
                        list_print(&seek_list);
                        tree = NULL;
                    }
                    else {
                        swap_str = tree->data;
                        tree_append_right( &node, tree, swap_str);
                        tree_append_left(&node, tree, string);
                        printf( " How %s is different from %s.\nIt's ...", string, swap_str );
                        free ( swap_str );
                        while(getchar() != '\n' ){};
                        scanf( "%[^\n]", string );
                        tree->data = strdup(string);
                        tree = NULL;
                    }

                }

                else {

                    tree = tree->right;
                }

                //tree = tree->right;
            }

        }

        else {

            printf( "Play again?\n[Y]es/[N]o\n");
            scanf( "%s", string);
            
            while( stricmp(string, "Y") && stricmp(string, "N") ) {
            
                printf(" Sorry, I don't understand, please type Y or N\n");
                scanf( "%s", string);
            
            }

            if ( !stricmp( string, "Y") ) tree = node.node;
            if ( !stricmp( string, "N") ) break;

        }

    }
    graph_dump( &node );
    do_dump( &node );
    list_dtor( &seek_list );
    tree_dtor( &node, node.node);
    free(string);
    return 0;
}