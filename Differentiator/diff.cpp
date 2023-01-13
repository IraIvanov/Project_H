#include "tree.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "io.hpp"
#include "stack.hpp"
#include <string.h>
#include <ctype.h>
#include "diff.hpp"

#define MOVE_OFFSET offset += 1 + skip_spaces( str + offset + 1 )
#define RES_CTOR(dest) dest = (tree_t*)calloc( 1, sizeof( tree_t ) );\
                dest->data = (el_t)calloc( 1, sizeof(element_type) );

int is_var( int c ) {

    switch( c ) {

        case 'x': return 1;
    
        case 'y': return 1;
    
        default : return 0; 

    }

    return 0;

}

int is_op( int c ) {

    switch ( c ) {

        case '+': return ADD;

        case '-': return SUB;

        case '*': return MUL;

        case '/': return DIV;

        case '^': return DEG;

        case 'l': return LG;

        case 's': return SIN;

        case 'c': return COS;

        default: return 0;
    
    }

    return 0;
}

//this function has possible segfaults, be careful, it's demo  

int tree_upload( char* str, node_t* node ) {


    int err = 0, i = 0;
    double num = 0;

    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !str ) return NULL_VALUE;

    stack leaves;
    stack_ctor( leaves, DEF_STK_SIZE );
    tree_t* leave = node->node;
    el_t data = NULL;
    if ( str[i] == '(' ) {
        
        data = (el_t)calloc( 1, sizeof(element_type) );
        leave->data = data;
        stack_push ( &leaves, leave );
        i += 1;
    }

    else return -1;

    while ( str[i] != '\0' ) {

        if ( str[i] == '(' ) {

            i += 1;
            stack_push( &leaves, leave );
            
            if ( !leave->right ) {
                
                data = (el_t)calloc( 1, sizeof(element_type) );
                tree_append_right( node, leave, data );
                leave = leave->right;
            
            } else if ( !leave->left ) {

                data = (el_t)calloc( 1, sizeof(element_type) );
                tree_append_left( node, leave, data );
                leave = leave->left;

            }


        } else if ( str[i] != ')') {

            data = leave->data;
            //printf( " kill me %s\n", str + i );

            if ( sscanf( str + i, "%lf", &((data->data).num) ) == 1 ) {

                char *end = NULL;
                strtod( str + i, &end );
                data->type = NUM;
                i = 0;
                str = end;
                //printf("%lf", data->data.num);

            } else {

                if ( is_op( str[i] ) ) {

                    data->data.op = is_op(str[i]);
                    i += 1;
                    data->type = OP;
                    //printf("%c", data->data.op); 

                } else if ( is_var( str[i]) ) {

                    data->data.var = str[i];
                    //printf("%c", data->data.var);
                    data->type = VAR;
                    i += 1;

                } else return -1;

                
            }
    

        }

        if ( str[i] == ')' ) {

            stack_pop( &leaves, &leave );
            i += 1;

        }

    }

    stack_dtor( &leaves );

    return 0;
}


tree_t* _Diff ( const tree_t* node ) {

    tree_t* res = NULL, *left = NULL, *right = NULL;

    switch( node->data->type ) {

        case NUM: res = (tree_t*)calloc( 1, sizeof( tree_t ) );
            res->data = (el_t)calloc( 1, sizeof(element_type) );
            res->data->data.num = 0;
            res->data->type = NUM;
            res->left = NULL;
            res->right = NULL;
            return res;
        case VAR: res = (tree_t*)calloc( 1, sizeof( tree_t ) );
            res->data = (el_t)calloc( 1, sizeof(element_type) );
            res->data->data.num = 1;
            res->data->type = NUM;
            res->left = NULL;
            res->right = NULL;    
            return res;
        case OP: switch( node->data->data.op ) {

            case ADD: res = (tree_t*)calloc( 1, sizeof( tree_t ) );
                res->data = (el_t)calloc( 1, sizeof(element_type) );
                res->data->data.op = ADD;
                res->data->type = OP;
                res->left = _Diff( node->right );
                res->right = _Diff( node->left );    
                return res;                    
            case SUB: res = (tree_t*)calloc( 1, sizeof( tree_t ) );
                res->data = (el_t)calloc( 1, sizeof(element_type) );
                res->data->data.op = SUB;
                res->data->type = OP;
                res->left = _Diff( node->right );
                res->right = _Diff( node->left ); 
                return res;
            case MUL: res = (tree_t*)calloc( 1, sizeof( tree_t ) );
                res->data = (el_t)calloc( 1, sizeof(element_type) );
                res->data->data.op = ADD;
                res->data->type = OP;
                res->left = (tree_t*)calloc( 1, sizeof( tree_t ) );
                left = res->left;
                left->data = (el_t)calloc( 1, sizeof(element_type) );
                left->data->data.op = MUL;
                left->data->type = OP;
                left->left = _Diff( node->right );
                left->right = node->left;
                res->right = (tree_t*)calloc( 1, sizeof( tree_t ) );
                right = res->right;
                right->data = (el_t)calloc( 1, sizeof(element_type) );
                right->data->data.op = MUL;
                right->data->type = OP;
                right->left = node->right;
                right->right = _Diff( node->left );
                return res;
            case DIV: res = (tree_t*)calloc( 1, sizeof( tree_t ) );
                res->data = (el_t)calloc( 1, sizeof(element_type) );
                res->data->data.op = DIV;
                res->data->type = OP;
                res->left = (tree_t*)calloc( 1, sizeof( tree_t ) );
                left = res->left;
                left->data = (el_t)calloc( 1, sizeof(element_type) );
                left->data->data.op = SUB;
                left->data->type = OP;
                left->left = (tree_t*)calloc( 1, sizeof( tree_t ) );
                left->right = (tree_t*)calloc( 1, sizeof( tree_t ) );
                right = left->right;
                left = left->left;
                left->data = (el_t)calloc( 1, sizeof(element_type) );
                left->data->data.op = MUL;
                left->data->type = OP;
                left->left = _Diff( node->right );
                left->right = node->left;
                right->data = (el_t)calloc( 1, sizeof(element_type) );
                right->data->data.op = MUL;
                right->data->type = OP;
                right->left = node->right;
                right->right = _Diff( node->left );
                res->right = (tree_t*)calloc( 1, sizeof( tree_t ) );
                right = res->right;
                right->data = (el_t)calloc( 1, sizeof(element_type) );
                right->data->data.op = MUL;
                right->data->type = OP;
                right->left = node->left;
                right->right = node->left;
                return res;
            case LG: RES_CTOR ( res )
                res->data->data.op = DIV;
                res->data->type = OP;     
                RES_CTOR( res->left )
                left = res->left;
                left->data->data.op = MUL;
                left->data->type = OP;
                RES_CTOR( left->left )
                left->right = _Diff( node->right );//comms
                left = left->left;
                left->data->data.num = 1;
                left->data->type = NUM;
                res->right = node->right ;//comms
                return res;
            case DEG: RES_CTOR( res )
                if( node->right->data->type == NUM ) {

                    res->data->data.op = MUL;
                    res->data->type = OP;
                    RES_CTOR( res->left )
                    left = res->left;
                    left->data->data.op = DEG;
                    left->data->type = OP;
                    left->left = node->right;
                    left->right = node->left;
                    RES_CTOR( res->right )
                    right = res->right;
                    right->data->data.op = LG;
                    right->data->type = OP;
                    right->right = node->right;

                }   else if ( node->left->data->type == NUM ) {

                    res->data->data.op = MUL;
                    res->data->type = OP;
                    RES_CTOR( res->left )
                    left = res->left;
                    left->data->data.op = MUL;
                    left->data->type = OP;
                    left->left = node->left;
                    left->right = _Diff( node->right );
                    RES_CTOR( res->right )
                    right = res->right;
                    right->data->data.op = DEG;
                    right->data->type = OP;
                    right->left = node->right;
                    RES_CTOR( right->right );
                    right = right->right;
                    right->data->data.num = node->left->data->data.num - 1;
                    right->data->type = NUM;


                }   else {

                    res->data->data.op = MUL;
                    res->data->type = OP;
                    RES_CTOR( res->right )
                    right = res->right;
                    right->data->data.op = DEG;
                    right->data->type = OP;
                    right->right = node->left;
                    right->left = node->right;
                    RES_CTOR( res->left )
                    left = res->left;
                    left->data->data.op = ADD;
                    left->data->type = OP;
                    RES_CTOR( left->left )
                    RES_CTOR( left->right )
                    right = left->right;
                    left = left->left;
                    left->data->data.op = MUL;
                    left->data->type = OP;
                    left->left = _Diff( node->left );
                    RES_CTOR( left->right )
                    left->right->data->data.op = LG;
                    left->right->data->type = OP;
                    left->right->right = node->right;
                    right->data->data.op = DIV;
                    right->data->type = OP;
                    right->right = node->right;
                    RES_CTOR( right->left );
                    left = right->left;
                    left->data->data.op = MUL;
                    left->data->type = OP;
                    left->left = _Diff( node->right );
                    left->right = node->left;

                }

                return res;
            case SIN:
                RES_CTOR( res )
                res->data->data.op = MUL;
                res->data->type = OP;
                res->right = _Diff( node->right );
                RES_CTOR( res->left )
                left = res->left;
                left->data->data.op = COS;
                left->data->type = OP;
                left->right = node->right;
                return res;
            case COS:
                RES_CTOR( res )
                res->data->data.op = SUB;
                res->data->type = OP;
                RES_CTOR( res->left )
                left = res->left;
                left->data->data.num = 0;
                left->data->type = NUM;
                RES_CTOR( res->right )
                right = res->right;
                right->data->data.op = MUL;
                right->data->type = OP;
                right->right = _Diff( node->right );
                RES_CTOR( right->left )
                left = right->left;
                left->data->data.op = SIN;
                left->data->type = OP;
                left->right = node->right;
                return res;
            default: return NULL; 
        }


        default: return NULL;

    }

}

node_t* Diff ( const node_t* src ) {

    node_t* res = NULL;
    res = (node_t*)calloc( 1, sizeof(node_t) );
    tree_ctor( res );
    //free(res->node);
    res->node = _Diff( src->node );

    return res;
}
