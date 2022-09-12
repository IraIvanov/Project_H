#include"my_string.h"
#include<stdio.h>
#include<stdlib.hpp>

int main(){

    char s1[128] = "";
    char s2[128] = "Hello World!";
    char* s3;
    //(char*)s1 = my_strchr("Hello World!", 'o');
    my_puts(s1);
    printf("%d and %d\n", my_strlen("Hello World!"), my_strlen(s1));
    printf("%s and %s\n", my_strcpy(s1, s2), my_strncpy(s1, s2, 5));
    printf("%s and %s\n", my_strcat(s1, "Hello World!"), my_strncat(s1, s2, 5));
    s3 = my_strdup( s1 );
    int i = 0;
    s1[0] = '0';

    printf("%c\n", s3[3]);

    return 0;
}