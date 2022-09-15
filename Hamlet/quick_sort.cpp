#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"my_sort.hpp"


void free_text(S_S_Pair* text){
    free(text->str);
    text->str_len = 0;
    free(text);
}


int my_strcmp(const void *s1ptr, const void *s2ptr) {
    
    assert( s1ptr && s2ptr );

    char * s1 = *(char **)s1ptr;


    char * s2 = *(char **)s2ptr;

    //printf("comparing [%s] and [%s]\n, their length are [%lu] and [%lu]\n", s1, s2, strlen(s1), strlen(s2));

    size_t i_s1 = 0, j_s2 = 0;

    while( i_s1 < strlen(s1) && j_s2 < strlen(s2) ) {
        
        while( !isalpha( s1[i_s1] ) ) i_s1 ++;
        while( !isalpha( s2[j_s2] ) ) j_s2 ++;
        if ( s1[i_s1] != s2[j_s2] ) return s1[i_s1] - s2[j_s2];
        i_s1++;
        j_s2++;
    
    }

    return s1[i_s1] - s2[j_s2];

}

int my_comp(const void *s1ptr, const void *s2ptr) {

    assert( s1ptr && s2ptr);

    //printf("compairing\n");

    char * s1 = ((S_S_Pair *)s1ptr) -> str;

    char * s2 = ((S_S_Pair *)s2ptr) -> str;

    //printf("s1 is [%s] and s2 is [%s], their length are : %lu and %lu \n", s1, s2, strlen(s1), strlen(s2));

    size_t i_s1 = 0, j_s2 = 0;

    //printf("skipping punctuation marks\n");

    while( i_s1 < strlen(s1) && j_s2 < strlen(s2) ) {
        
        //printf("why are we still here?\n");

        while( !isalpha( s1[i_s1] ) && i_s1 < strlen(s1) ) i_s1 ++;

        while( !isalpha( s2[j_s2] ) && j_s2 < strlen(s2)) j_s2 ++;
        
        if ( s1[i_s1] != s2[j_s2] ) {

            //printf("returning result\n");

            return s1[i_s1] - s2[j_s2];
        
        }

        i_s1++;
        
        j_s2++;
    
    }

    //printf("returning result\n");

    return s1[i_s1] - s2[j_s2];

}

int my_comp_revers(const void *s1ptr, const void *s2ptr) {

    assert( s1ptr && s2ptr);

    //printf("compairing\n");

    char * s1 = ((S_S_Pair *)s1ptr) -> str;

    char * s2 = ((S_S_Pair *)s2ptr) -> str;

    //printf("s1 is [%s] and s2 is [%s], their length are : %lu and %lu \n", s1, s2, strlen(s1), strlen(s2));

    size_t i_s1 = strlen(s1) - 1, j_s2 = strlen(s2)- 1;

    //printf("skipping punctuation marks\n");

    while( i_s1 > 0 && j_s2 > 0 ) {
        
        //printf("why are we still here?\n");

        while( !isalpha( s1[i_s1] ) && i_s1 > 0 ) i_s1 --;

        while( !isalpha( s2[j_s2] ) && j_s2 > 0) j_s2 --;
        
        if ( s1[i_s1] != s2[j_s2] ) {

            //printf("returning result\n");

            return s1[i_s1] - s2[j_s2];
        
        }

        i_s1--;
        
        j_s2--;
    
    }

    //printf("returning result\n");

    return s1[i_s1] - s2[j_s2];

}

int my_cpy ( void *struc1_ptr,const void *struc2_ptr, size_t left, size_t right) {

    assert(struc1_ptr && struc2_ptr);

    S_S_Pair *struc1 = (S_S_Pair *)struc1_ptr + left;

    S_S_Pair *struc2 = (S_S_Pair *)struc2_ptr + right;

    //printf("Copying...\n");

    struc1 -> str = strdup(struc2 -> str);

    struc1 -> str_len = struc2 -> str_len;

    return 1;
}

int int_cmp(const void *aptr, const void *bptr) {

    assert(aptr && bptr);

    int *a = (int *)aptr;

    int *b = (int *)bptr;

    return *a - *b;

}

int int_cpy(void *a_ptr,const void *b_ptr, size_t left, size_t right) {

    assert( a_ptr && b_ptr);

    int *a = (int *)a_ptr;

    int *b = (int *)b_ptr;

    *(a + left) = *(b + right);

    return 1; 
    
}

int str_cpy(void *s1ptr,const void * s2ptr, size_t left, size_t right) {

    assert( s1ptr && s2ptr);
    //char *s1 = calloc(128, sizeof(char));
    //s1 = *((char **)s1ptr + left);
    
    //har s2 = calloc(128, sizeof(char));
    
    //s2 = *((char **)s2ptr + right);

    //printf("why are we still here\n"); it was debug

    //printf("%s, %s\n", s1, s2);

    *((char **)s1ptr + left) = strdup(*((char **)s2ptr + right));

    return 1;

}



void merge();

void merge_sort(void * array, size_t el_size, size_t left, size_t right, 

int (*comp)(const void * lv, const void * rv), int (*cpy)( void * array1,const void * array2, size_t left_index, size_t right_index)) {

    assert(array);

    if ( right > left){
        
        size_t mid = (left + right) / 2;

        merge_sort( array, el_size, left, mid, (*comp), (*cpy));
        
        merge_sort( array, el_size, mid + 1, right, (*comp), (*cpy));
    
    merge(array, el_size, left, right, mid, (*comp) , (*cpy));

    }

}

void merge(void * array, size_t el_size, size_t left, size_t right, size_t mid, int (*comp)(const void * lv,const void * rv),

 int (*cpy)(void* array1,const void* array2, size_t left_index, size_t right_index)) {

     assert( (mid - left + 1) && (right - mid));

    size_t num1 = mid - left + 1;

    size_t num2 = right - mid;

    size_t i = 0, j = 0, k = left;

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

       if ( comp ( (S_S_Pair *)right_arr + j * el_size, (S_S_Pair*)left_arr + i * el_size) >= 0) {

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
