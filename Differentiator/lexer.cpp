/*

priority: 
    +, -
    /, *
    ^ ---- ? ( maybe with next functions )
    ln, cos, sin


{
    G ::= E \0 
    E ::= T {[+-]T}*
    T ::= D{['/]D}*
    D ::= F{[^]F}*
    F ::= {["ln","cos","sin"]P}|P
    P ::= '('E')'|N|V
    N ::= [0 - 9]+
    V ::= [x]
}

*/
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.hpp"
#include "diff.hpp"
#include "io.hpp"

#define MAX_SIZE 1024

#define OK 0

#define BAD 1

int flag = OK;

static tree_t* GetE( const char** s );

static tree_t* GetP( const char** s);

static tree_t* GetT( const char** s );

static tree_t* GetN( const char** s );

static tree_t* GetD( const char** s );

static tree_t* GetF( const char** s );

static tree_t* GetG( const char** s ) {

    if ( !s ) {

        flag = BAD;
        printf( "error, NULL ptr\n" );
        return NULL;
    }

    tree_t* res = GetE( s );
    
    if ( **s != '\0' ) {

        flag = BAD;
        printf( "bad data\n" );
    }
    
    return res;

}

tree_t* GetE( const char** s ) {

    printf( "%s\n", *s );
    tree_t* res = GetT( s );

    (*s) += skip_spaces( *s );

    if( **s == '+' || **s == '-' ) {

        const char op = **s;
        (*s)++;
        tree_t* left = GetE( s );
        tree_t* right = NULL;
        RES_CTOR( right )
        NODE_CPY( right, res )
        right->left = res->left;
        right->right = res->right;
        int op_data = 0;

        if ( op == '+' ) op_data = ADD; 
        else op_data = SUB;
        
        TYPE(res) = OP;
        OP_DATA(res) = op_data;
        res->left = left;
        res->right = right;
        (*s) += skip_spaces( *s );

    }

    return res;

}

tree_t* GetT( const char** s ) {

    //skip_spaces( s );
    tree_t* res = GetD( s );
    (*s) += skip_spaces( (*s) );
    if( **s == '*' || **s == '/' ) {

        const char op = **s;
        int op_data = 0;
        (*s)++;
        tree_t* left = GetT( s ), *right = NULL;
        
        if ( op == '*' ) op_data = MUL;
        else op_data = DIV;
        RES_CTOR( right )
        NODE_CPY( right, res )
        right->left = res->left;
        right->right = res->right;
        TYPE(res) = OP;
        OP_DATA(res) = op_data;
        res->right = right;
        res->left = left;
        (*s) += skip_spaces( *s );
    }

    return res;

}

/*

    T ::= D{['/]D}*
    D ::= F{[^]F}*
    F ::= {["ln","cos","sin"]P}|P

*/

tree_t* GetD( const char** s) {

    tree_t* res = NULL;
    (*s) += skip_spaces( *s );
    res = GetF( s );
    if( **s == '^' ) {
        (*s)++;
        tree_t* left = GetD( s ), *right = NULL;
        RES_CTOR( right )
        NODE_CPY( right, res )
        right->left = res->left;
        right->right = res->right;
        TYPE(res) = OP;
        OP_DATA(res) = DEG;
        res->left = left;
        res->right = right;        
        (*s) += skip_spaces( *s );

    }

    return res;

}

int func_verify( const char** s) {

    assert( s );
    (*s) += skip_spaces( *s );
    if ( !strncmp( *s, "ln", 2 ) ) return LG;
    if ( !strncmp( *s, "cos", 3) ) return COS;
    if ( !strncmp( *s, "sin", 3 ) ) return SIN;
    return 0;

}

tree_t* GetF( const char** s ) {

    int op = 0;
    tree_t* res = NULL, *right = NULL;
    (*s) += skip_spaces( *s );

    if ( (op = func_verify( s )) ) {

        
        switch ( op )
        {
        case LG: (*s) += 2; 
            right = GetP( s );
            RES_CTOR( res )
            TYPE(res) = OP;
            OP_DATA( res ) = LG;
            res->right = right;
            break;
        case COS: (*s) += 3; 
            right = GetP( s );
            RES_CTOR( res )
            TYPE(res) = OP;
            OP_DATA( res ) = COS;
            res->right = right;
            break;
        case SIN: (*s) += 3; 
            right = GetP( s );
            RES_CTOR( res )
            TYPE(res) = OP;
            OP_DATA( res ) = SIN;
            res->right = right; 
            break;
        default:
            break;
        }

    } else res = GetP( s );

    return res;

}   

tree_t* GetP( const char** s ) {

    tree_t* res = NULL;
    (*s) += skip_spaces( *s );

    if ( **s == '(' ){

        (*s)++;
        res = GetE( s );
        (*s) += skip_spaces( *s );
        assert( **s == ')' );
        (*s)++;

    } else res = GetN( s ); 

    return res;

}

tree_t* GetN( const char** s ) {

    double val = 0;
    tree_t* res = NULL; 
    (*s) += skip_spaces( *s );
    char* end = NULL;
    const char* old = *s;

    if( sscanf( *s, "%lf", &val ) == 1 ) {

        val = strtod( *s, &end );
        RES_CTOR( res )
        TYPE(res) = NUM;
        NUM_DATA(res) = val;
        (*s) = end;

    } else {

        if ( is_var( **s ) ){

            RES_CTOR( res )
            TYPE(res) = VAR;
            res->data->data.var = **s;
            (*s)++;

        }

    }
    //printf( "%s\n", s );
    if ( old == (*s) ) {

        flag = BAD;
        printf( "bad data\n" );
        return NULL;

    }
    (*s) += skip_spaces( *s );


    return res;

}

int get_tree( const char* str, node_t* node ) {

    int err = 0;

    if ( (err = node_verify( node ))!= 0 ) return err;
    if ( !str ) return NULL_VALUE;

    if ( node->node ) free( node->node );

    node->node = GetG( (const char**)&str );

    return 0;

}
