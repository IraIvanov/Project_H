#include<stdio.h>
#include<sys\stat.h>
#include<stdlib.h>

typedef struct _String_Size_Pair {

    char * str;

    size_t str_len;

} S_S_Pair;

size_t input( FILE* stream) {

    struct stat size_buff;

    fstat ( fileno (stream), &size_buff);

    return size_buff.st_size;

}

