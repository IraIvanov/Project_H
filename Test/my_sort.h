#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#ifndef MY_SORT_H_INCLUDED
#define MY_SORT_H_INCLUDED

typedef struct _String_Size_Pair {

char * str;

size_t str_len;

} S_S_Pair;

int my_comp(void *s1ptr, void *s2ptr);

int my_cpy ( void *struc1_ptr, void *struc2_ptr, int left, int right);

int int_cmp(void *aptr, void *bptr);

int int_cpy(void *a_ptr, void *b_ptr, int left, int right);

void merge( void * array, size_t el_size, int left, int right, int mid, int (*comp)(void * lv, void * rv),

 int (*cpy)(void * array1, void * array2, int left_index, int right_index));

void merge_sort( void * array, size_t el_size, int left, int right, 

int (*comp)(void * lv, void * rv), int (*cpy)( void * array1, void * array2, int left_index, int right_index));

int my_strcmp(void *s1ptr,void *s2ptr);

int str_cpy(void *s1ptr, void * str2ptr, int left, int right);

#endif