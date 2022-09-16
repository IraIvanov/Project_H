#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<ctype.h>

#ifndef MY_SORT_H_INCLUDED
#define MY_SORT_H_INCLUDED

typedef struct _String_Size_Pair {

char * str;

size_t str_len;

} S_S_Pair;

int my_comp(const void *s1ptr, const void *s2ptr);

int my_comp_revers(const void *s1ptr, const void *s2ptr); 

int my_cpy ( void *struc1_ptr,const void *struc2_ptr, size_t left, size_t right);

void merge(void * array, size_t el_size, size_t left, size_t right, size_t mid, int (*comp)(const void * lv,const void * rv),

 int (*cpy)(void* array1,const void* array2, size_t left_index, size_t right_index));

void merge_sort(void * array, size_t el_size, size_t left, size_t right, 

int (*comp)(const void * lv, const void * rv), int (*cpy)( void * array1,const void * array2, size_t left_index, size_t right_index));

#endif