#include"my_sort.h"
#include<stdio.h>
#include<stdlib.h>

int main(){

    int a[10];

    for(int i = 0; i < 10 ; i++) {

        scanf("%d", a + i);

    }

    merge_sort( (void *)a, sizeof(int), 0, 9, int_cmp, int_cpy );

    for(int i = 0; i < 10 ; i++) {

        printf("%d ", a[i]);

    }

    return 0;
}