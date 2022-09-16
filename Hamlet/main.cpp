#include"my_sort.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include"io.hpp"

#define SIZE_TEXT 1024


int main(){


    char *buff;

    S_S_Pair *text;

    FILE* stream, *output;

    stream = fopen("hamlet.txt", "r");

    output = fopen("result.txt", "w");

    struct stat size_buff = { };

    fstat ( fileno (stream), &size_buff);

    size_t file_size = (size_t) size_buff.st_size + 1;

    //printf("allocating buff\n");

    buff = (char *)calloc(file_size + 1, sizeof(char));

    //printf("allocating text\n");
    
    text = (S_S_Pair *) calloc(SIZE_TEXT, sizeof(S_S_Pair));

    fread(buff, sizeof(char) , file_size, stream);

    size_t j = 0;

    input(file_size, buff, SIZE_TEXT, &text, &j);

    printf("Sorting...\n");

    merge_sort((void *) text, sizeof(S_S_Pair), 0, j - 1, my_comp, my_cpy);

    my_output( output, text, j);

    qsort(text, j - 1, sizeof(S_S_Pair), my_comp_revers);

    free_output(output, text, j);
    
    fprintf(output, "\nOriginal text: %s \n", buff); //---> last symbol is D

    free(buff);

    free(text);

    fclose(stream);

    fclose(output);

    return 0;
}