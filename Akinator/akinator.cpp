#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include <string.h>
#include "akinator.hpp"
#include "io.hpp"
#include "stack.hpp"
//#include </home/shorteststraw/libs/festival/src/include/festival.h>
#define cat( x, y ) x ## y

int upload ( node_t* node, FILE* file ) {

    stack leaves;
    stack_ctor( leaves, DEF_STK_SIZE);

    int err = 0, data_len = 0;
    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !file ) return NULL_FILE;

    char* data = (char*)calloc( MAX_LINE, sizeof(char) ), c = 0;
    fgetc(file);
    c = fgetc(file);
    fgets( data, MAX_LINE, file);
    data_len = strlen(data) - 1;
    data[ data_len ] = '\0';
    //printf( "%s\n", data );
    free( (node->node)->data );
    (node->node)->data = strdup( data );
    tree_t* leave = node->node;
    stack_push( &leaves, leave );
    while ( (c = fgetc(file)) != EOF ) {

        fgets( data, MAX_LINE, file);
        data_len = strlen(data) - 1;
        data[ data_len ] = '\0';

        if ( c == '{'){

            stack_push( &leaves, leave );
            //printf( "%s\n", data);

            if ( !leave->left ){

                tree_append_left( node, leave, data );
                leave = leave->left;
            
            }
            else {

                tree_append_right( node, leave, data);
                leave = leave->right;

            }

        }

        if ( c == '}' ){

            stack_pop(&leaves, &leave);
        
        }
 
    }

    stack_dtor( &leaves );
    free( data );

    return 0;

}

int game ( node_t* node/*, tree_t* tree*/, list_t* seek_list ){ 
    
    int err = 0;
    if ( (err = node_verify( node )) < 0 ) return err;
    if ( (err = list_verify( seek_list )) < 0 ) return err;

    tree_t* tree = node->node;
    char* string = (char*)calloc(MAX_LINE, sizeof(char)), *swap_str;
    char* help_string = NULL;

    while ( tree ) {        
        
        printf( "It's %s ?\n[Y]es/[N]o?\n", tree->data);
        help_string = (char*)calloc( 2*MAX_LINE, sizeof(char));
        strcpy(help_string, " echo \"It's ");
        strcat(help_string, tree->data);
        strcat(help_string,  "?\n\" | festival --tts ");
        system(help_string);
        free(help_string);
        //system ( " echo \"It's %s ?\n [Y]es/[N]o?\n\" | festival -tts ");
        scanf( "%s", string);
        
        while( stricmp(string, "Y") && stricmp(string, "N") ) {
        
            printf("Sorry, I don't understand, please type Y or N\n");
            system ( " echo \"Sorry, I don't understand, please type Y or N\n\" | festival --tts ");
            scanf( "%s", string);
        
        }

        if ( !stricmp( string, "Y") ) {
            
            if ( !tree->left ) {
                printf( "Hahaha, as I expected\n");
                system ( " echo \"Hahaha, as I expected\n\" | festival --tts ");
                tree = NULL;
            }

            else tree = tree->left;    
            //tree = tree->left;
        }

        if ( !stricmp( string, "N") ) {
            
            if ( !tree->right ) {

                printf( "So, who it's then ?\n");
                system ( " echo \"So, who it's then ?\n\" | festival --tts ");
                while(getchar() != '\n' ){};
                scanf( "%[^\n]", string);
                int i = 0;
                i = tree_seek( node, node->node, /*&i,*/ string, seek_list, 0);//printf( "%d\n", i = tree_seek( node, node->node, /*&i,*/ string, seek_list, 0));

                if ( i == 1 ){
                    printf( "This object already in database, here it's defenition:\n");
                    system ( " echo \"This object already in database, here it's defenition:\n\" | festival --tts ");
                    list_print(seek_list);
                    list_free( seek_list );
                    tree = NULL;
                }
                else {
                    swap_str = tree->data;
                    tree_append_right( node, tree, swap_str);
                    tree_append_left(node, tree, string);
                    printf( "How %s is different from %s.\nIt's ...", string, swap_str );
                    help_string = (char*)calloc( 2*MAX_LINE, sizeof(char));
                    strcpy(help_string, " echo \"How ");
                    strcat(help_string, string);
                    strcat(help_string,  " is different from?\n\" | festival --tts ");
                    strcat(help_string, string);
                    system(help_string);
                    free(help_string);
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

        }

    }

    free( string );

    return 0;

}

int show_def( node_t* node, list_t* seek_list ) {

    int err = 0;
    if ( (err = node_verify( node )) < 0 ) return err;
    if ( (err = list_verify( seek_list )) < 0 ) return err;

    char* string = (char*)calloc( MAX_LINE, sizeof(char) );
    printf( "Who do you want to see?\n");
    system ( " echo \"Who do you want to see?\n\" | festival --tts ");
    while(getchar() != '\n' ){};
    scanf( "%[^\n]", string);
    int i = 0;
    i = tree_seek( node, node->node, /*&i,*/ string, seek_list, 0);//printf( "%d\n", i = tree_seek( node, node->node, /*&i,*/ string, seek_list, 0));

    if ( i == 1 ) {
        char *help_string = NULL;    
        int pos1 = (seek_list->next)[0];
        printf( "Here it's defenition:\n");
        system ( " echo \"Here it's defenition:\n\" | festival --tts ");
        //list_print(seek_list);
        while ( pos1 != 0 ){
            
            help_string = (char*)calloc( 2*MAX_LINE, sizeof(char) );
            strcpy(help_string, "  echo \"");
            if ( (seek_list->data)[pos1]->flag == 0) { 
                printf( "not ");
                strcat(help_string, "not ");
            }
            strcat(help_string, (seek_list->data)[pos1]->data);
            strcat( help_string, "\n\" | festival --tts ");
            printf( "%s\n", (seek_list->data)[pos1]->data);
            system( help_string );
            free(help_string);
            pos1 = (seek_list->next)[pos1];

        }

        list_free( seek_list );

    } else printf( "Sorry, I don't know him\n");

    free(string);
    return 0;

}

int show_tree( node_t* node ) {

    int err = 0;
    if ( (err = node_verify( node )) < 0 ) return err;

    graph_dump( node );
    system( "xdg-open tree.svg");
    return 0;

}

int compare( node_t* node, list_t* seek_list1, list_t* seek_list2 ) {

    int err = 0;
    if ( (err = node_verify( node )) < 0 ) return err;
    if ( (err = list_verify( seek_list1 )) < 0 ) return err;
    if ( (err = list_verify( seek_list2 )) < 0 ) return err;

    char* string = (char*)calloc( MAX_LINE, sizeof(char) );
    char* cmp_string = (char*)calloc( MAX_LINE, sizeof(char) );
    printf( "Who do you want to compare?\n");
    while(getchar() != '\n' ){};
    scanf( "%[^\n]", string);
    int i = 0, j = 0, pos1 = 0, pos2 = 0;
    i = tree_seek( node, node->node, /*&i,*/ string, seek_list1, 0);//printf( "%d\n", i = tree_seek( node, node->node, /*&i,*/ string, seek_list, 0));
    
    if ( i != 1 ) {

        printf( "Sorry I don't know who it is :(\n");
        return 0;

    }

    printf( "compare with...\n");
    while(getchar() != '\n' ){};
    scanf( "%[^\n]", cmp_string);
    j = tree_seek( node, node->node, /*&i,*/ cmp_string, seek_list2, 0);

    if ( j != 1 ) {

        printf( "Sorry I don't know who it is :(\n");
        return 0;
        
    }

    pos1 = (seek_list1->next)[0];
    pos2 = (seek_list2->next)[0];

    list_print( seek_list1 );
    list_print( seek_list2 );

    printf( "Similarities between %s and %s:\n", string, cmp_string );
    while ( pos1 != 0 && pos2 != 0 ){

        if ( stricmp((seek_list1->data)[pos1]->data, (seek_list2->data)[pos2]->data) || (seek_list1->data)[pos1]->flag != (seek_list2->data)[pos2]->flag ){
            
            printf( "But %s:\n", string );
            break;
        
        }
        if ( (seek_list1->data)[pos1]->flag == 0) printf( "not ");
        printf( "%s\n", (seek_list1->data)[pos1]->data);
        pos1 = (seek_list1->next)[pos1];
        pos2 = (seek_list2->next)[pos2];

    }

    while ( pos1 != 0 ){
        
        if ( (seek_list1->data)[pos1]->flag == 0) printf( "not ");
        printf( "%s\n", (seek_list1->data)[pos1]->data);
        pos1 = (seek_list1->next)[pos1];

    }

    printf( "And %s:\n", cmp_string );
    while ( pos2 != 0 ){
        
        if ( (seek_list2->data)[pos2]->flag == 0) printf( "not ");
        printf( "%s\n", (seek_list2->data)[pos2]->data);
        pos2 = (seek_list2->next)[pos2];

    }

    list_free( seek_list1 );
    list_free( seek_list2 );
    free(string);
    free(cmp_string);
    return 0;

}
