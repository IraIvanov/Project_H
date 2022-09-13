#include"my_string.hpp"
#include"my_sort.hpp"
#include<stdio.h>
#include<stdlib.h>

int main(){

    char  s2[128] = "Hello World!";
    char *s1 = (char *)calloc(128, sizeof(char));
    char *s3;

    my_puts(s2);
    s1 = my_strchr( s2, 'o');
    printf(" s1 len %lu and s2 len %lu\n", my_strlen(s2), my_strlen(s1));

    my_puts(s2);

    my_puts(s1);

    printf(" %c\n", s2[4]);
        //s1 n copy to s2 %s
    printf(" s2 copy to s1 %s,\n", /*my_strcpy(s2, s1),*/ my_strncpy(s1, s2, 5*sizeof(char)));

    my_puts(s1);

    my_puts(s2);

    printf(" s1 cat to s2 %s, s1 n cat to s2 %s\n", my_strcat(s2, s1), my_strncat(s2, s1, 5));
    
    s3 = strdup(s2);

    printf(" s2 dup to s3 %s", s3);
    return 0;
}