#include<stdio.h>
#include<assert.h>
#include<stdlib.h>

#ifndef MY_STRING_H_INCLUDED
#define MY_STRING_H_INCLUDED

int my_puts(const char* str);

char *my_strchr(const char* str, int ch);

size_t my_strlen(const char* str);

char *my_strcpy(char *str1, const char *str2);

char *my_strncpy(char *str1, const char *str2, size_t count);

char *my_strcat (char *str1, const char *str2);

char *my_strncat (char *str1, const char *str2, size_t n);

char *my_fgets(char *str, int num, FILE *stream);

char *my_strdup (const char *str);

#endif