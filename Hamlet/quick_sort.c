#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

int num_cmp(char *s1, char* s2){
    int v1, v2;
    v1 = atoi( s1 );
    v2 = atoi( s2 );

}

int min(const int a, const int b){

    if (a < b) return a;

    else return b;

}

int my_strcmp(const char *s1, const char *s2) {
    assert( s1 && s2 );

    int i;

    for( i = 0; s1[i] == s2[i]; i++){
        
        if ( s1[i] == '\0') return 0;

    }

    return s1[i] - s2[i];

}

void quick_sort( void * array[], int left, int right, int (*comp)(void *, void *)) {

    int 
    

}