#include"my_sort.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

#define SIZE_TEXT 1024


int main(){



    /*char *hamlet[5];

    for ( int i = 0; i < 5; i++){

        char s1[128];
        scanf("%s", s1);
        hamlet[i] = strdup(s1);
    }

    printf("Sorting...\n");

    merge_sort((void *)hamlet, sizeof(char *), 0, 4, my_strcmp, str_cpy);

     for ( int i = 0; i < 5; i++){

        printf("%s\n", hamlet[i]);

    }*/
    char *buff;

    S_S_Pair *text;

    FILE* stream, *output;

    stream = fopen("hamlet.txt", "r");

    output = fopen("result.txt", "w");

    struct stat size_buff;

    fstat ( fileno (stream), &size_buff);

    size_t file_size = size_buff.st_size;

    buff = (char *)calloc(file_size + 1, sizeof(char));

    text = (S_S_Pair *) calloc(SIZE_TEXT, sizeof(S_S_Pair));

    fread(buff, sizeof(char) , file_size, stream);

    buff[file_size] = '\n';

    size_t str_plc = 0, j = 0, real_num = 2;

    for(size_t i = 0; i <= file_size; i++) {

        if ( buff[i] == '\n') {

            if ( j == ( sizeof(text)/ sizeof(S_S_Pair *) ) ){

                printf("Realoccating\n");

                text = realloc(text, SIZE_TEXT*sizeof(S_S_Pair) * real_num ); // realloc if text has no place to put new string

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

    for(size_t i = 0; i < j; i ++) {

        fprintf(output ,"%s", text[i].str);
    }

    //printf("Sorting...\n");

    merge_sort((void *) text, sizeof(S_S_Pair), 0, j - 1, my_comp, my_cpy);


    for(size_t i = 0; i < j; i ++) {

        fprintf(output ,"%s", text[i].str);
    }
    
    fprintf(output, "\nOriginal text: %s \n", buff); //---> last symbol is D

    fclose(output);

    fclose(stream);

    return 0;
}