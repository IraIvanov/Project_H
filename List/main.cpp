#include <stdio.h>
#include <stdlib.h>
#include "list.hpp"

int main() {

    list_t list = {};

    list_ctor(&list, 64);

    char c = 0;
    size_t i = 0;
    while ( (scanf("%c", &c) && c != 'q') ) {

        printf("%c\n", c);
        list_add(&list, i, c);
        //list_dump(&list);
        i++;
    }
    printf("%lu\n", list_find(&list, 2));
    list_dump(&list);
    return 0;
}