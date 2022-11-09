#include <stdio.h>
#include <stdlib.h>
#include "list.hpp"

int main() {

    list_t list = {};
    list_t new_list = {};

    list_ctor(&list, 10);

    char c = 0;
    int i = 0;
    while ( (scanf("%c", &c) && c != 'q') ) {

        printf("%c\n", c);
        list_add(&list, i, c);
        //list_dump(&list);
        i++;
    }
    //printf("%d\n", list_find(&list, 2));
    list_dump(&list, "added_elements");
    list_remove(&list, 1);
    list_dump(&list, "removed_first_element");
    //list_dump(&new_list);
    //list_realloc(&list, 4);
    list_remove(&list, 2);
    list_dump(&list, "removed_second_element");
    list_dtor(&list);
    //list_dump(&new_list);
    //printf( "\n%d\n", list_dtor(&new_list));
    //list_dump(new_list);
    return 0;
}