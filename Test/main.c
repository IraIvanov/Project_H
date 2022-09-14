#include"my_sort.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){

    char *hamlet[10];

    for ( int i = 0; i < 10; i++){

        char s1[128];
        scanf("%s", s1);
        hamlet[i] = strdup(s1);
    }

    printf("Sorting...\n");

    merge_sort((void *)hamlet, sizeof(char *), 0, 9, my_strcmp, str_cpy);

     for ( int i = 0; i < 10; i++){

        printf("%s\n", hamlet[i]);

    }

    return 0;
}