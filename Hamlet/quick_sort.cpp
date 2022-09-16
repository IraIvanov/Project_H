#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"my_sort.hpp"

int my_comp(const void *s1ptr, const void *s2ptr) {

    assert( s1ptr && s2ptr);

    //printf("compairing\n");

    char * s1 = ((S_S_Pair *)s1ptr) -> str;

    //printf("s1 is [%s]\n", s1);

//    char * s2 = ((S_S_Pair *)s2ptr) -> str;
    
//    printf("s2 is [%s]\n", s2);

    size_t s1_len = ((S_S_Pair *)s1ptr) -> str_len - 1;

    //printf("s1_len is [%lu]\n", s1_len);

    size_t s2_len = ((S_S_Pair *)s2ptr) -> str_len - 1;

    //printf("s2_len is [%lu]\n", s2_len);

    char * s2 = ((S_S_Pair *)s2ptr) -> str;

    //printf("s1 is [%s] and s2 is [%s], their length are : %lu and %lu \n", s1, s2, strlen(s1), strlen(s2));

    size_t i_s1 = 0, j_s2 = 0;

    //printf("skipping punctuation marks\n");

    //printf("s1_len [%lu] and s2_len[%lu] and ptrs are i [%lu], j [%lu]\n", s1_len, s2_len, i_s1, j_s2);

    while( i_s1 < (s1_len ) && j_s2 < (s2_len )) {
        
        //printf("why are we still here?\n");

        //printf(" i is %lu j is %lu\n", i_s1, j_s2);

        while( !isalpha( *(s1 + i_s1) ) && i_s1 < (s1_len - 1) ) i_s1 ++;

        while( !isalpha( *(s2 + j_s2) ) && j_s2 < (s2_len - 1)) j_s2 ++;
        
        if ( s1[i_s1] != s2[j_s2] ) {

            //printf("s1 is [%s] and s2 is [%s], their length are : %lu and %lu \n", s1, s2, strlen(s1), strlen(s2));
            //printf("returning result\n");

            return s1[i_s1] - s2[j_s2];
        
        }

        i_s1++;
        
        j_s2++;

        //printf(" i is %lu j is %lu\n", i_s1, j_s2);
    
    }
    //printf(" i is %lu j is %lu\n", i_s1, j_s2);
    //printf("returning result with s1[%s| and s2[%s]\n", s1, s2);

    //printf("returning result with s1[%lu| -> %c and s2[%lu] -> %c\n", i_s1, *(s1 + i_s1), j_s2, *(s2 + j_s2));

    //printf("returning %d\n", s1[i_s1]- s2[j_s2]);

    return (s1[i_s1] - s2[j_s2]);

}

int my_comp_revers(const void *s1ptr, const void *s2ptr) {

    assert( s1ptr && s2ptr);

    //printf("compairing\n");

    char * s1 = ((S_S_Pair *)s1ptr) -> str;

    char * s2 = ((S_S_Pair *)s2ptr) -> str;

    size_t s1_len = ((S_S_Pair *)s1ptr) -> str_len - 1;

    size_t s2_len = ((S_S_Pair *)s2ptr) -> str_len - 1;

    //printf("s1 is [%s] and s2 is [%s], their length are : %lu and %lu \n", s1, s2, strlen(s1), strlen(s2));

    size_t i_s1 = s1_len - 1, j_s2 = s2_len- 1;

    //printf("skipping punctuation marks\n");

    while( i_s1 > 0 && j_s2 > 0 ) {
        
        //printf("why are we still here?\n");

        //printf("i [%lu], j [%lu]\n",i_s1, j_s2);

        while( !isalpha( s1[i_s1] ) && i_s1 > 1 ) i_s1 --;

        while( !isalpha( s2[j_s2] ) && j_s2 > 1) j_s2 --;
        
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

    //printf("string orig [%s]\n", struc2->str);

    //printf("string orig len [%lu]\n", struc2->str_len);

    if(struc1 -> str) free(struc1->str);

    struc1 -> str = strdup(struc2 -> str);

    //printf("string copied [%s]\n", struc1->str);

    struc1 -> str_len = struc2 -> str_len;

    //printf("string copied len [%lu]\n", struc1->str_len);

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

    //printf("why are we still here ?\n");

    while ( i < num1 && j < num2 ) {

        //printf("just to suffer\n" );

       if ( comp ( (S_S_Pair *) /*(int *)*/right_arr + j, (S_S_Pair*)/*(int *)*/left_arr + i) >= 0) {

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
    
    for(size_t num = 0; num < num1; num ++ ){
        free(((S_S_Pair*)left_arr + num) -> str);
    }

    for(size_t num = 0; num < num2; num ++ ){
        free(((S_S_Pair*)right_arr + num)->str);
    }

    //my_free((S_S_Pair*)left_arr, num1);
    free(left_arr);

    //my_free((S_S_Pair*)right_arr, num2);
    free(right_arr); 

}
