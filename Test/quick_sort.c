#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"my_sort.h"


/*int num_cmp(char *s1, char* s2){
    int v1, v2;
    v1 = atoi( s1 );
    v2 = atoi( s2 );

}*/



int my_strcmp(void *s1ptr,void *s2ptr) {
    
    assert( s1ptr && s2ptr );

    char * s1 = *(char **)s1ptr;

    char * s2 = *(char **)s2ptr;


    return strcmp( s1, s2);

}

int int_cmp(void *aptr, void *bptr) {

    int *a = (int *)aptr;

    int *b = (int *)bptr;

    return *a - *b;

}

int int_cpy(void *a_ptr, void *b_ptr, int left, int right) {

    int *a = (int *)a_ptr;

    int *b = (int *)b_ptr;

    *(a + left) = *(b + right);

    return 1; 
    
}

int str_cpy(void *s1ptr, void * s2ptr, int left, int right) {

    //char *s1 = calloc(128, sizeof(char));
    //s1 = *((char **)s1ptr + left);
    
    //har s2 = calloc(128, sizeof(char));
    
    //s2 = *((char **)s2ptr + right);

    printf("why are we still here\n");

    //printf("%s, %s\n", s1, s2);

    *((char **)s1ptr + left) = strdup(*((char **)s2ptr + right));

    return 1;

}



void merge( void * array, size_t el_size, int left, int right, int mid, int (*comp)(void * lv, void * rv),

 int (*cpy)(void * array1, void * array2, int left_index, int right_index));

void merge_sort( void * array, size_t el_size, int left, int right, 

int (*comp)(void * lv, void * rv), int (*cpy)( void * array1, void * array2, int left_index, int right_index)) {

    if ( right > left ){
    
        int mid = (left+right) / 2;

        merge_sort( array, el_size, left, mid, (*comp), (*cpy));
        
        merge_sort( array, el_size, mid + 1, right, (*comp), (*cpy));
    
    printf("merging\n");

    merge(array, el_size, left, right, mid, (*comp) , (*cpy));

    }

}

void merge( void * array, size_t el_size, int left, int right, int mid, int (*comp)(void * lv, void * rv),

 int (*cpy)(void *array1, void *array2, int left_index, int right_index)) {

    int num1 = mid - left + 1;

    int num2 = right - mid;

    int i = 0, j = 0, k = left;

    //void *left_arr[num1], *right_arr[num2];

    void *left_arr = calloc(num1, el_size);
    void *right_arr = calloc(num2, el_size);

    for( ; i < num1; i++) {

        cpy( left_arr, array, i, left + i);

    }

        for( ; j < num2; j++) {

        cpy( right_arr, array, j , mid + 1 + j);

    }

    i = 0;

    j = 0;

    while ( i < num1 && j < num2 ) {

       if ( comp ( right_arr + j * el_size, left_arr + i * el_size) >= 0) {

           cpy( array, left_arr, k, i);

           i++;

       } else {

           cpy( array, right_arr, k, j);

           j++;

       }

       k++;

    }

    while ( i < num1 ) {
        
        cpy(array, left_arr, k, i);

        i++;

        k++;

    } 

    while ( j < num2 ) {
        
        cpy(array, right_arr, k, j);

        j++;

        k++;
        
    }

    free(left_arr);

    free(right_arr); 

}
