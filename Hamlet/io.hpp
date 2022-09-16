#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include"my_sort.hpp"

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

void input(size_t file_size, char *buff, size_t size_sgstd, S_S_Pair** text, size_t *text_size);

void my_output(FILE *output, S_S_Pair* text, size_t text_size);

void free_output(FILE *output, S_S_Pair* text, size_t text_size);

#endif