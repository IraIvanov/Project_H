#include"my_sort.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include"io.hpp"
#include<time.h>
#include<sys/times.h>

#define SIZE_TEXT 1024


int main( int argc, char *argv[]){

    if (argc < 2) printf("File name wasn't inputed\n");

    else {
        char *buff = nullptr;

        S_S_Pair *text = nullptr;

        FILE* stream = fopen(argv[1], "r"), *output = fopen("result.txt", "w");

        struct stat size_buff = { };

        if ( !fstat ( fileno (stream), &size_buff) ) {

            size_t file_size = (size_t) size_buff.st_size + 1;

            //printf("allocating buff\n");

            buff = (char *)calloc(file_size + 1, sizeof(char));

            //printf("allocating text\n");
            
            //text = (S_S_Pair *) calloc(SIZE_TEXT, sizeof(S_S_Pair));

            if ( (size_t)size_buff.st_size == fread(buff, sizeof(char) , file_size, stream) ) {

                size_t j = 0;

                //input(file_size, buff, SIZE_TEXT, &text, &j);

                new_input(file_size, buff, &text, &j);

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

            } else printf("ERROR WHILE READIGN FILE\n");

        } else printf("ERROR WHILE READIGN FILE\n");

    }

    return 0;
}