#include "tree.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "io.hpp"
#include "stack.hpp"
#include <string.h>
#include <ctype.h>
#include "diff.hpp"
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

#define EPS 0.01 

#define MOVE_OFFSET offset += 1 + skip_spaces( str + offset + 1 )

#define RES_CTOR(dest) dest = (tree_t*)calloc( 1, sizeof( tree_t ) );\
                dest->data = (el_t)calloc( 1, sizeof(element_type) );

#define NODE_CPY( dest, src ) memcpy( dest->data, src->data, sizeof( element_type) );

#define IS_OP( tree, oper ) TYPE(tree) == OP && OP_DATA(tree) == oper 

#define IS_NUM( tree, val ) TYPE(tree) == NUM && abs(NUM_DATA(tree) - val ) < EPS 

tree_t* node_cpy( const tree_t* src ) {

    tree_t* res = NULL;
    RES_CTOR( res )
    NODE_CPY( res, src )
    if ( TYPE(src) == OP && OP_DATA(src) > DEG ) {
        if ( src->left ) res->left = node_cpy( src->left );
        if ( src->right ) res->right = node_cpy( src->right );
    } else {
        if ( src->left ) res->right = node_cpy( src->left );
        if ( src->right ) res->left = node_cpy( src->right );
    }

    return res;
}

int is_var( const int c ) {

    switch( c ) {

        case 'x': return 1;
    
        case 'y': return 1;

        case 'X': return 1;

        case 'Y': return 1;
    
        default : return 0; 

    }

    return 0;

}

int is_op( const char* str ) {

    if ( !str ) return NULL_VALUE;

    char * s = (char*)calloc( 1024, sizeof(char) );

    sscanf( str, "%[^()]", s );

    if ( !stricmp( s, "+") ) {
        free(s);
        return ADD;
    }
    if ( !stricmp( s, "-") ) {
        free(s);
        return SUB;
    }
    if ( !stricmp( s, "/") ) {
        free(s);
        return DIV;
    }
    if ( !stricmp( s, "*") ){
        free(s);
        return MUL;
    }
    if ( !stricmp( s, "^") ){
        free(s);
        return DEG;
    }
    if ( !stricmp( s, "ln") ) {
        free(s);
        return LG;
    }
    if ( !stricmp( s, "cos") ) {
        free(s);
        return COS;
    }
    if ( !stricmp( s, "sin") ) {
        free(s);
        return SIN;
    }
    free(s);
    return 0;
}

//this function has possible segfaults, be careful, it's demo  

int tree_upload( const char* str, node_t* node ) {


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

                if ( is_op( str + i ) ) {
                    
                    int temp = 0;
                    data->data.op = is_op(str + i);
                    temp = data->data.op;
                    switch ( temp ) {

                        case ADD: i += 1;
                            break;
                        case SUB: i += 1;
                            break;
                        case DIV: i += 1;
                            break;
                        case MUL: i += 1;
                            break;
                        case DEG: i += 1;
                            break;
                        case LG: i += 2;
                            break;
                        default: i+= 3; 

                    }
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
                left->right = node_cpy( node->left );
                //left->right = node->left;
                res->right = (tree_t*)calloc( 1, sizeof( tree_t ) );
                right = res->right;
                right->data = (el_t)calloc( 1, sizeof(element_type) );
                right->data->data.op = MUL;
                right->data->type = OP;
                right->left = node_cpy( node->right );
                //right->left = node->right;
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
                left->right = node_cpy( node->left ); 
                //left->right = node->left;
                right->data = (el_t)calloc( 1, sizeof(element_type) );
                right->data->data.op = MUL;
                right->data->type = OP;
                right->left = node_cpy( node->right );
                //right->left = node->right;
                right->right = _Diff( node->left );
                res->right = (tree_t*)calloc( 1, sizeof( tree_t ) );
                right = res->right;
                right->data = (el_t)calloc( 1, sizeof(element_type) );
                right->data->data.op = MUL;
                right->data->type = OP;
                right->left = node_cpy( node->left );
                right->right = node_cpy( node->left );
                //right->left = node->left;
                //right->right = node->left;
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
                res->right = node_cpy( node->right );
                //res->right = node->right ;//comms
                return res;
            case DEG: RES_CTOR( res )
                if( node->right->data->type == NUM ) {

                    res->data->data.op = MUL;
                    res->data->type = OP;
                    RES_CTOR( res->left )
                    left = res->left;
                    left->data->data.op = DEG;
                    left->data->type = OP;
                    left->left = node_cpy( node->right );
                    //left->left = node->right;
                    left->right = node_cpy( node->left );
                    //left->right = node->left;
                    RES_CTOR( res->right )
                    right = res->right;
                    right->data->data.op = LG;
                    right->data->type = OP;
                    right->right = node_cpy( node->right );
                    //right->right = node->right;

                }   else if ( node->left->data->type == NUM ) {

                    res->data->data.op = MUL;
                    res->data->type = OP;
                    RES_CTOR( res->left )
                    left = res->left;
                    left->data->data.op = MUL;
                    left->data->type = OP;
                    left->left = node_cpy( node->left );
                    //left->left = node->left;
                    left->right = _Diff( node->right );
                    RES_CTOR( res->right )
                    right = res->right;
                    right->data->data.op = DEG;
                    right->data->type = OP;
                    right->left = node_cpy( node->right );
                    //right->left = node->right;
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
                    right->right = node_cpy( node->left );
                    right->left = node_cpy( node->right );
                    //right->right = node->left;
                    //right->left = node->right;
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
                    left->right->right = node_cpy( node->right );
                    right->data->data.op = DIV;
                    right->data->type = OP;
                    right->right = node_cpy( node->right );
                    RES_CTOR( right->left );
                    left = right->left;
                    left->data->data.op = MUL;
                    left->data->type = OP;
                    left->left = _Diff( node->right );
                    left->right = node_cpy( node->left );

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
                left->right = node_cpy(node->right);
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
                left->right = node_cpy(node->right);
                return res;
            default: return NULL; 
        }


        default: return NULL;

    }

}

node_t* Diff ( const node_t* src ) {

    node_t* res = NULL;

    int err = 0;

    if ( (err = node_verify( src ))!= 0 ) return NULL;

    res = (node_t*)calloc( 1, sizeof(node_t) );
    tree_ctor( res );
    free(res->node);
    res->node = _Diff( src->node );

    return res;
}

int _const_erase( node_t* node, tree_t** tree ) {

    tree_t* temp = NULL;

    if ( IS_OP( (*tree) , MUL) && IS_NUM((*tree)->right, 1)) {

        temp = (*tree)->left;
        tree_dtor(node, (*tree)->right );
        free((*tree)->data);
        free(*tree);
        *tree = temp; 

    } else if( IS_OP( (*tree) , MUL) && IS_NUM((*tree)->left, 1)) {

        temp = (*tree)->right;
        tree_dtor(node, (*tree)->left );
        free((*tree)->data);
        free(*tree);
        *tree = temp; 

    } else if( IS_OP( (*tree) , DEG) && IS_NUM((*tree)->right, 1)) {

        temp = (*tree)->left;
        tree_dtor(node, (*tree)->right );
        free((*tree)->data);
        free(*tree);
        *tree = temp; 

    } else if( IS_OP( (*tree) , MUL) && IS_NUM((*tree)->left, 0)) {

        temp = (*tree)->left;
        tree_dtor(node, (*tree)->right );
        free((*tree)->data);
        free(*tree);
        *tree = temp; 

    } else if( IS_OP( (*tree) , MUL) && IS_NUM((*tree)->right, 0)) {

        temp = (*tree)->right;
        tree_dtor(node, (*tree)->left );
        free((*tree)->data);
        free(*tree);
        *tree = temp; 
    } else if( TYPE((*tree)) == OP && OP_DATA((*tree)) <= DEG && TYPE(((*tree)->left)) == NUM && TYPE(((*tree)->right)) == NUM  ) {

        double num = 0;
        tree_t* left = (*tree)->left, *right = (*tree)->right;

        switch (OP_DATA((*tree))) {
        
        case ADD: num = NUM_DATA(left) + NUM_DATA(right);
            tree_dtor( node, left );
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;
            (*tree)->left = NULL;
            (*tree)->right = NULL; 
            break;
        
        case SUB: num = NUM_DATA(left) - NUM_DATA(right);
            tree_dtor( node, left );
            tree_dtor( node, right );
            (*tree)->left = NULL;
            (*tree)->right = NULL;
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num; 
            break;

        case MUL: num = NUM_DATA(left) * NUM_DATA(right);
            tree_dtor( node, left );
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;
            (*tree)->left = NULL;
            (*tree)->right = NULL; 
            break;
        
        case DIV: num = NUM_DATA(left) / NUM_DATA(right);
            tree_dtor( node, left );
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;
            (*tree)->left = NULL;
            (*tree)->right = NULL; 
            break;

        case DEG: num = pow( NUM_DATA(left) , NUM_DATA(right) );
            tree_dtor( node, left );
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;
            (*tree)->left = NULL;
            (*tree)->right = NULL; 
            break;
        
        default:
            break;
        }

    } else if( TYPE((*tree)) == OP && TYPE((*tree)->right) == NUM  ) {

        double num = 0;
        tree_t* right = (*tree)->right;

        switch (OP_DATA((*tree))) {
        
        case COS: num = cos(NUM_DATA(right));
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;
            (*tree)->right = NULL; 
            break;
        
        case SIN: num = sin(NUM_DATA(right));
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;            
            (*tree)->right = NULL; 
            break;

        case LG: num = log(NUM_DATA(right));
            tree_dtor( node, right );
            TYPE((*tree)) = NUM;
            NUM_DATA((*tree)) = num;            
            (*tree)->right = NULL; 
            break;
        
        default:
            break;
        }

    }

    if ( (*tree)->left ) {
        _const_erase( node, &((*tree)->left) );
    }
    if ( (*tree)->right ) {
        _const_erase( node, &((*tree)->right) );
    }

    return 0;
}

int const_erase ( node_t* node ) {

    int err = 0;

    if ( (err = node_verify( node ))!= 0 ) return err;

    _const_erase( node, &(node->node) );
    return 0;

}

int _tree_latex( const tree_t* tree, FILE* file ) {

    int flag = 0;

     if ( IS_OP( tree, DIV ) ) {
        fprintf( file, "\\frac{");
        if ( tree -> left ) {

            if ( (tree->data->type == OP && tree->left->data->type == OP && ( (tree->data->data.op / 10) > ( tree->left->data->data.op / 10))) && OP_DATA(tree) < DEG ) {
                fprintf( file, "(" );
                _tree_latex( tree->left, file);
                fprintf( file, ")" );
            } else _tree_latex( tree->left, file);

            fprintf( file, "}{");

             if ( tree -> right ) { 

                if ( (tree->data->type == OP && tree->right->data->type == OP && ( (tree->data->data.op / 10) > ( tree->right->data->data.op / 10))) || ( tree->data->type == OP && tree->data->data.op >= DEG ))  {
                    fprintf( file, "(" );
                    _tree_latex( tree->right, file );
                    fprintf( file, ")" );
                } else _tree_latex( tree->right, file );

            }

            fprintf( file, "}" );

        }
    } else {

        if ( tree -> left ) {

            if ( (tree->data->type == OP && tree->left->data->type == OP && ( (tree->data->data.op / 10) > ( tree->left->data->data.op / 10))) || (tree->data->type == OP && tree->data->data.op > DEG) ) {
                fprintf( file, "(" );
                _tree_latex( tree->left, file);
                fprintf( file, ")" );
            } else _tree_latex( tree->left, file);
        }

        if ( (tree->data)->type == NUM ) fprintf( file, "%.2lf", (tree->data->data).num );
        else if ( (tree->data)->type == OP ) {
            //printf( "%c", (tree->data->data).op );
            switch ( (tree->data->data).op ) {

                case ADD: fprintf( file, "+" );
                    break;
                case MUL: fprintf( file, "\\cdot " ); 
                    break;
                case SUB: fprintf( file, "-" );
                    break;
                case DIV:
                    break;
                case DEG: fprintf( file, "^" );
                    flag = 1;
                    break;
                case LG: fprintf( file, "\\ln");
                    flag = 1;
                    break;
                case COS: fprintf( file, "\\cos");
                    flag = 1; 
                    break;
                case SIN: fprintf( file, "\\sin");
                    flag = 1;
                    break;
                default: return 0;
            }
        }
        else if ( (tree->data)->type == VAR ) fprintf( file, "%c", (tree->data->data).var );

        if ( tree -> right ) { 

            if ( flag == 1 ) {

                fprintf ( file, "{" );
            }

            if ( (tree->data->type == OP && tree->right->data->type == OP && ( (tree->data->data.op / 10) > ( tree->right->data->data.op / 10))) && OP_DATA(tree) < DEG)  {
                fprintf( file, "(" );
                _tree_latex( tree->right, file );
                fprintf( file, ")" );
            } else _tree_latex( tree->right, file );

            if ( flag == 1 ) {
                fprintf ( file, "}" );
            }

        }
    }

    return 0;
}

int r_tree_latex( const tree_t* tree, FILE* file ) {

    int flag = 0;

    tree_t* left = NULL, *right = NULL;

    if ( TYPE(tree) == OP && OP_DATA(tree) > DEG ) {

        left = tree->left;
        right = tree->right;
    } else {

        left = tree->right;
        right = tree->left;

    }

    if ( IS_OP( tree, DIV ) ) {
        fprintf( file, "\\frac{");
        if ( left ) {

            if ( (tree->data->type == OP && left->data->type == OP && ( (tree->data->data.op / 10) > ( left->data->data.op / 10))) && OP_DATA(tree) < DEG ) {
                fprintf( file, "(" );
                r_tree_latex( left, file);
                fprintf( file, ")" );
            } else r_tree_latex( left, file);

            fprintf( file, "}{");

             if ( right ) { 

                if ( (tree->data->type == OP && right->data->type == OP && ( (tree->data->data.op / 10) > ( right->data->data.op / 10))) || ( tree->data->type == OP && tree->data->data.op >= DEG ))  {
                    fprintf( file, "(" );
                    r_tree_latex( right, file );
                    fprintf( file, ")" );
                } else r_tree_latex( right, file );

            }

            fprintf( file, "}" );

        }
    } else {

        if ( left ) {

            if ( (tree->data->type == OP && left->data->type == OP && ( (tree->data->data.op / 10) > ( left->data->data.op / 10))) || (tree->data->type == OP && tree->data->data.op > DEG) ) {
                fprintf( file, "(" );
                r_tree_latex( left, file);
                fprintf( file, ")" );
            } else r_tree_latex( left, file);
        }

        if ( (tree->data)->type == NUM ) fprintf( file, "%.2lf", (tree->data->data).num );
        else if ( (tree->data)->type == OP ) {
            //printf( "%c", (tree->data->data).op );
            switch ( (tree->data->data).op ) {

                case ADD: fprintf( file, "+" );
                    break;
                case MUL: fprintf( file, "\\cdot " ); 
                    break;
                case SUB: fprintf( file, "-" );
                    break;
                case DIV:
                    break;
                case DEG: fprintf( file, "^" );
                    flag = 1;
                    break;
                case LG: fprintf( file, "\\ln");
                    flag = 1;
                    break;
                case COS: fprintf( file, "\\cos");
                    flag = 1; 
                    break;
                case SIN: fprintf( file, "\\sin");
                    flag = 1;
                    break;
                default: return 0;
            }
        }
        else if ( (tree->data)->type == VAR ) fprintf( file, "%c", (tree->data->data).var );

        if ( right ) { 

            if ( flag == 1 ) {

                fprintf ( file, "{" );
            }

            if ( (tree->data->type == OP && right->data->type == OP && ( (tree->data->data.op / 10) > ( right->data->data.op / 10))) && OP_DATA(tree) < DEG)  {
                fprintf( file, "(" );
                _tree_latex( right, file );
                fprintf( file, ")" );
            } else _tree_latex( right, file );

            if ( flag == 1 ) {
                fprintf ( file, "}" );
            }

        }
    }

    return 0;
}


int tree_latex( const node_t* node, const node_t* src ) {

    int err = 0;
    int pid = 0;

    if ( ( err = node_verify( node ) ) != 0  ) return err;
    if ( ( err = node_verify( src ) ) != 0  ) return err;
    
      FILE* file = fopen( "topdf.tex", "w" );
      fprintf( file, "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\begin{document}\n$ (" );
      r_tree_latex( src->node, file );
      fprintf( file, " )' = ");
      _tree_latex( node->node, file );
      fprintf( file, "$\n\\end{document}");
      fclose( file );
      system( "pdflatex topdf.tex" );
      system( "rm *.tex" );
      system( "rm *.aux" ); 
      system( "rm *.log" );

    return 0;

}
