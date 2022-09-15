#include"my_sort.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

#define SIZE_TEXT 1024


int main(){


    char *buff;

    S_S_Pair *text;

    FILE* stream, *output;

    stream = fopen("hamlet.txt", "r");

    output = fopen("result.txt", "w");

    struct stat size_buff;

    fstat ( fileno (stream), &size_buff);

    size_t file_size = (size_t) size_buff.st_size;

    printf("allocating buff\n");

    buff = (char *)calloc(file_size + 1, sizeof(char));

    printf("allocating text\n");
    
    text = (S_S_Pair *) calloc(SIZE_TEXT, sizeof(S_S_Pair));

    fread(buff, sizeof(char) , file_size, stream);

    fclose(stream);

    //printf("buff is %s\n", buff);

    buff[file_size] = '\n';

    //printf("buff is %s\n", buff);

    size_t str_plc = 0, j = 0, real_num = 2;

    for(size_t i = 0; i <= file_size; i++) {

        if ( buff[i] == '\n') {

            if ( j == ( SIZE_TEXT * (real_num - 1) ) ){

                printf("Realoccating text\n");

                text = (S_S_Pair*)realloc(text, SIZE_TEXT*sizeof(S_S_Pair) * real_num ); // realloc if text has no place to put new string

                real_num++;

            }

            text[j].str = (char*)calloc(i - str_plc + 2, sizeof(char));

            strncpy(text[j].str, buff + str_plc, i - str_plc + 1);

            text[j].str[i - str_plc + 1] = '\0';

            text[j].str_len = i - str_plc + 2;
            
            j++;

            str_plc = i + 1;
        }

    }


    printf("Sorting...\n");

    merge_sort((void *) text, sizeof(S_S_Pair), 0, j - 1, my_comp, my_cpy);

    qsort(text, j - 1, sizeof(S_S_Pair), my_comp_revers);

    for(size_t i = 0; i < j; i ++) {

        fprintf(output ,"%s", text[i].str);
    }
    
    fprintf(output, "\nOriginal text: %s \n", buff); //---> last symbol is D

    free(buff);

    for(size_t i = 0; i < j; i ++) {

        free_text(text + i);
    }

    fclose(output);

    return 0;
}