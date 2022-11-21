/*

{
    G ::= E \0 
    E ::= T {[+-]T}*
    T ::= P{['/]P}*
    P ::= '('E')'|N
    N ::= [0 - 9]+
}

*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 1024

const char* s = NULL;

int skip_spaces( const char* str ){

    if( !str ) return -1;
    
    int i = 0;

    while( isspace( str[i] ) ) i++;

    return i; 

}

int GetE();

int GetP();

int GetT();

int GetN();

int GetG( const char* str ) {

    int val = 0;
    s = str;
    val = GetE();
    printf( "%s\n", s);
    assert( *s == '\0' );
    return val;

}

int GetE() {

    //s += skip_spaces( s );
    int val = 0;
    val = GetT();

    s += skip_spaces( s );

    while( *s == '+' || *s == '-' ) {

        const char op = *s;
        s++;
        int val2 = GetT();
        
        if ( op == '+' ) val += val2;
        else val -= val2;
        
        s += skip_spaces( s );

    }

    return val;

}

int GetT() {

    //skip_spaces( s );
    int val = 0;
    val = GetP();
    s += skip_spaces( s );
    while( *s == '*' || *s == '/' ) {

        const char op = *s;
        s++;
        int val2 = GetP();
        
        if ( op == '*' ) val *= val2;
        else val /= val2;
        s += skip_spaces( s );
    }

    return val;

}

int GetP() {

    int val = 0;
    s += skip_spaces( s );

    if ( *s == '(' ){

        s++;
        val = GetE();
        skip_spaces( s );
        assert( *s == ')' );
        s++;

    } else val = GetN(); 

    return val;

}

int GetN() {

    int val = 0;
    s += skip_spaces( s );
    const char* old = s;

    while ( *s >= '0'&& *s <= '9' ){

        val = val*10 + *s - '0';
        s++;

    }
    
    assert( old != s );
    s += skip_spaces( s );


    return val;

}

int main() {

    char* string  = (char*)calloc( MAX_SIZE, sizeof(char) );
    printf( "enter the expression\n");
    scanf( "%[^\n]", string);
    //fprintf(stderr, "%s\n", string);
    int res = GetG( string );
    printf("here is the result\n%d\n", res);
    free( string );
    return 0;

}
