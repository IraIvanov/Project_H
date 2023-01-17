#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include <string.h>
#include "io.hpp"
#include "diff.hpp"

int node_verify( const node_t* node ) {

    int err = 0;

    if ( !node ) return NULL_NODE;
    if ( !(node -> node) ) return NULL_TREE;
    //if ( node->size <= 0 ) err += BAD_SIZE;
    if ( node->status != ALIVE ) err += BAD_STAT;

    return err;
}

 
int tree_ctor( node_t* node/*, tree_t* tree*/ ) {

    if ( !node ) return NULL_NODE;
    //if ( !tree ) return NULL_TREE;
    if ( node->status == ALIVE ) return CTOR_ERR;

    tree_t* tree = (tree_t*)calloc( 1, sizeof(tree_t));

    if ( !tree ) return ALLOC_ERR;

    tree -> data = NULL;
    tree -> left = NULL;
    tree -> right = NULL;
    node -> node = tree;
    node -> size = 1;
    node -> status = ALIVE;

    return 0;
}

int tree_append_left( node_t* node, tree_t* tree, const el_t data ) {

    int err = 0;

    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !tree ) return NULL_TREE;
    if ( data == NULL ) return BAD_DATA;

    if ( (tree -> left) != NULL ) return ADD_ERR;

    tree -> left = (tree_t*) calloc(1, sizeof(tree_t));

    if ( !(tree->left) ) return ALLOC_ERR;

    (tree -> left) -> data = data;
    node->size += 1;

    return 0;

}

int tree_append_right( node_t* node, tree_t* tree, const el_t data ) {

    int err = 0;

    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !tree ) return NULL_TREE;
    if ( data == NULL ) return BAD_DATA;

    if ( (tree -> right) != NULL ) return ADD_ERR;

    tree -> right = (tree_t*) calloc(1, sizeof(tree_t));

    if ( !(tree->right) ) return ALLOC_ERR;

    (tree -> right) -> data = data;
    node->size += 1;

    return 0;

}

int tree_dtor ( node_t* node, tree_t* tree ) {

    int err = 0;

    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !tree ) return NULL_TREE;

    free(tree -> data);
    if ( tree -> left ) tree_dtor( node, tree->left );
    if ( tree -> right ) tree_dtor( node, tree->right );
    free(tree);
    //node -> size -= 1; 

    return 0;
}

int leave_free ( node_t* node, tree_t* tree ) {

    int err = 0; 
    
    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !tree ) return NULL_TREE;
    if ( tree -> left ) return LEAV_ERR;
    if ( tree -> right ) return LEAV_ERR;

    free(tree->data);
    free(tree);
    //node -> size -= 1;

    return 0;
}



int graph_dump ( const node_t* node ) {

    int err = 0; 
    
    if ( (err = node_verify( node ))!= 0 ) return err;

    FILE* log = fopen( LOG, "w" );
    fprintf( log, "digraph G {\ngraph [dpi = 100];\n spline = ortho\n");//\trankdir=LR;\n
    int i = 1;
    tree_print( node->node, log, &i );
    fprintf(log, "}\n");
    fclose( log );
    system("dot -Tsvg log.txt > tree.svg");
    system( "rm log.txt" ); 
    return 0;

}

static int tree_dump ( const tree_t* tree, FILE* dump ) ;

int do_dump ( const node_t* node ) {

    int err = 0;

    if ( (err = node_verify( node )) != 0 ) return err;

    FILE* dump = fopen( DUMP, "w" );
    tree_dump( node->node , dump );
    fclose ( dump );
    return 0;

}

 
int tree_print ( const tree_t* tree, FILE* file, int* i ) {

    if ( !tree ) return NULL_TREE;
    if ( !i ) return NULL_VALUE;

    if ( file ) {

        char c = 0;

        switch( TYPE(tree) ){

            case OP: 

                    switch ( (tree->data->data).op ) {

                        case ADD: c = '+';
                            break;
                        case MUL: c = '*';
                            break;
                        case SUB: c = '-';
                            break;
                        case DIV: c = '/';
                            break;
                        case DEG: c = '^';
                            break;
                        case LG: c = 'l';
                            break;
                        case COS: c = 'c';
                            break;
                        case SIN: c = 's';
                            break;
                        default: return 0;
                    }

                    fprintf( file, "\t node%d [shape=\"record\", style=\"rounded, filled\", fillcolor = \"#9d4cef\" label=\" %c \" ];\n", *i,\
                    c);
                    break;
            case VAR:
                    c = tree->data->data.var; 
                    fprintf( file, "\t node%d [shape=\"record\", style=\"rounded, filled\", fillcolor = \"#9d4cef\" label=\" %c \" ];\n", *i,\
                    c);
                    break;
            case NUM: fprintf( file, "\t node%d [shape=\"record\", style=\"rounded, filled\", fillcolor = \"#9d4cef\" label=\" %.2lf \" ];\n", *i,\
                    (tree->data->data.num));
                    break;
            default: return 0;
        }

        //fprintf( file, "\t node%d [shape=\"record\", style=\"rounded, filled\", fillcolor = \"#9d4cef\" label=\" %s \" ];\n", *i,\
        (tree->data));
        int tmp = *i;
        if ( tree->left ) {
            fprintf( file, "\tnode%d->node%d ;\n", tmp, ++(*i));
            tree_print( tree-> left, file, i);
        }
        if ( tree->right ) {
            fprintf( file, "\tnode%d->node%d ;\n", tmp, ++(*i));
            tree_print( tree-> right, file, i);
        }

        return 0;

    }

    if ( tree -> left ) tree_print( tree->left, file, i);
    if ( tree->data->type == NUM ) printf( "%lf", tree->data->data.num );
    else printf( "%c", tree->data->data.var );
    if ( tree->right ) tree_print( tree->right, file, i);

    return 0;

}

int tree_pprint( const tree_t* tree ) {

    //int op1 = 0, op2 = 0;
    if ( !tree ) return NULL_TREE;

    if ( tree -> left ) {

        if ( (tree->data->type == OP && tree->left->data->type == OP && ( (tree->data->data.op / 10) > ( tree->left->data->data.op / 10))) || (tree->data->type == OP && tree->data->data.op > DEG) ) {
            printf( "(" );
            tree_pprint( tree->left);
            printf( ")" );
        } else tree_pprint( tree->left);
    }

    if ( (tree->data)->type == NUM ) printf( "%.2lf", (tree->data->data).num );
    else if ( (tree->data)->type == OP ) {
        //printf( "%c", (tree->data->data).op );
        switch ( (tree->data->data).op ) {

            case ADD: printf("+");
                break;
            case MUL: printf("*");
                break;
            case SUB: printf("-");
                break;
            case DIV: printf("/");
                break;
            case DEG: printf("^");
                break;
            case LG: printf("ln");
                break;
            case COS: printf("cos");
                break;
            case SIN: printf("sin");
                break;
            default: return 0;
        }
    }
    else if ( (tree->data)->type == VAR ) printf( "%c", (tree->data->data).var );

    if ( tree -> right ) { 

        if ( (tree->data->type == OP && tree->right->data->type == OP && ( (tree->data->data.op / 10) > ( tree->right->data->data.op / 10))) || ( tree->data->type == OP && tree->data->data.op >= DEG ))  {
            printf( "(" );
            tree_pprint( tree->right );
            printf( ")" );
        } else tree_pprint( tree->right );

    }

    return 0;
}

int tree_rprint( const tree_t* tree ) {

    //int op1 = 0, op2 = 0;
    if ( !tree ) return NULL_TREE; 

    if ( TYPE(tree) == OP && OP_DATA(tree) <= DEG ){

        if ( tree -> right ) { 

            if (( tree->data->type == OP && tree->right->data->type == OP && ( (tree->data->data.op / 10) > ( tree->right->data->data.op / 10)) ) || ( tree->data->type == OP && tree->data->data.op > DEG ))  {
                printf( "(" );
                tree_rprint( tree->right );
                printf( ")" );
            } else tree_rprint( tree->right );

        }
        
        if ( (tree->data)->type == NUM ) printf( "%.2lf", (tree->data->data).num );
        else if ( (tree->data)->type == OP ) {
            //printf( "%c", (tree->data->data).op );
            switch ( (tree->data->data).op ) {

                case ADD: printf("+");
                    break;
                case MUL: printf("*");
                    break;
                case SUB: printf("-");
                    break;
                case DIV: printf("/");
                    break;
                case DEG: printf("^");
                    break;
                case LG: printf("ln");
                    break;
                case COS: printf("cos");
                    break;
                case SIN: printf("sin");
                    break;
                default: return 0;
            }
        }
        else if ( (tree->data)->type == VAR ) printf( "%c", (tree->data->data).var );



        if ( tree -> left ) {

            if (( tree->data->type == OP && tree->left->data->type == OP && ( (tree->data->data.op / 10) > ( tree->left->data->data.op / 10)) ) || ( tree->data->type == OP && tree->data->data.op >= DEG )) {
                printf( "(" );
                tree_rprint( tree->left);
                printf( ")" );
            } else tree_rprint( tree->left);
        }

    } else {
        
        if ( tree -> left ) {

            if (( tree->data->type == OP && tree->left->data->type == OP && ( (tree->data->data.op / 10) > ( tree->left->data->data.op / 10)) ) || ( tree->data->type == OP && tree->data->data.op > DEG )) {
                printf( "(" );
                tree_rprint( tree->left);
                printf( ")" );
            } else tree_rprint( tree->left);
        }
       
        
        if ( (tree->data)->type == NUM ) printf( "%.2lf", (tree->data->data).num );
        else if ( (tree->data)->type == OP ) {
            //printf( "%c", (tree->data->data).op );
            switch ( (tree->data->data).op ) {

                case ADD: printf("+");
                    break;
                case MUL: printf("*");
                    break;
                case SUB: printf("-");
                    break;
                case DIV: printf("/");
                    break;
                case DEG: printf("^");
                    break;
                case LG: printf("ln");
                    break;
                case COS: printf("cos");
                    break;
                case SIN: printf("sin");
                    break;
                default: return 0;
            }
        }
        else if ( (tree->data)->type == VAR ) printf( "%c", (tree->data->data).var );

        if ( tree -> right ) { 

            if (( tree->data->type == OP && tree->right->data->type == OP && ( (tree->data->data.op / 10) > ( tree->right->data->data.op / 10)) ) || ( tree->data->type == OP && tree->data->data.op >= DEG ))  {
                printf( "(" );
                tree_rprint( tree->right );
                printf( ")" );
            } else tree_rprint( tree->right );

        }

       
    }

    return 0;
}

static int tree_dump ( const tree_t* tree, FILE* dump ) {

    if ( !tree ) {

        fprintf( dump, "\n}");
        return 0;

    }   

    int i = 0;

    fprintf( dump, "\n{%s", tree->data);

    if ( tree -> left ) {

        tree_dump ( tree -> left, dump );
        i++;
    }

    if ( tree -> right ) {

        tree_dump ( tree -> right, dump );
        i++;
    }

    if ( i != 0 ) fprintf( dump, "\n}");

    else fprintf( dump, "\n}");
    
    return 0;

}

int get_hight( tree_t* node, int i ) {

    if ( !node ) return NULL_TREE;
    
    int left = i, right = i; 

    if ( node->left ) left = get_hight( node->left, i++ ); 
    if ( node->right ) right = get_hight( node->right, i++ ); 
    if ( left > right ) return left;
    else return right;

}