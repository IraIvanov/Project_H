#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include"my_sort.hpp"
#include<assert.h>
#include"io.hpp"

void input(size_t file_size, char *buff, size_t size_sgstd, S_S_Pair** text, size_t *text_size){

    assert(text);

    assert(buff);

    assert(text_size);

    size_t real_num = 2, str_plc = 0;

    buff[file_size - 1] = '\n';

    buff[file_size] = '\0';

    for(size_t i = 0; i <= file_size; i++) {

        if ( buff[i] == '\n') {

            if ( (*text_size) == ( size_sgstd * (real_num - 1) ) ){

                printf("Realoccating text\n");

                *text = (S_S_Pair*)realloc(*text, size_sgstd*sizeof(S_S_Pair) * real_num ); // realloc if text has no place to put new string

                real_num++;

            }

            (*text)[*text_size].str = (char*)calloc(i - str_plc + 2, sizeof(char));

            

            strncpy((*text)[*text_size].str, buff + str_plc, i - str_plc + 1);

            //fprintf(output, "%s", text[j].str);

            (*text)[*text_size].str[i - str_plc + 1] = '\0';

            (*text)[*text_size].str_len = i - str_plc + 2;
            
            (*text_size)++;

            str_plc = i + 1;
        }

    }

}

void my_output(FILE *output, S_S_Pair* text, size_t text_size) {

    assert( output );

    assert( text );

    for(size_t i = 0; i < text_size; i ++) {

        fprintf(output ,"%s", text[i].str);

    }

}

void free_output(FILE *output, S_S_Pair* text, size_t text_size) {

    assert( output );

    assert( text );

    for(size_t i = 0; i < text_size; i ++) {

        fprintf(output ,"%s", text[i].str);

        free(text[i].str);
    }

}