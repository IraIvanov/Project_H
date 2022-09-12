/*
puts, strchr, strlen, strcpy, strncpy, sttcat, strncat, fgets, strdup.
*/

#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include"my_string.hpp"

int my_puts(const char* str){
    
    if(!str){
    
        return EOF;
    
    } else {

        printf("%s\n", str);

        return (int)'\n';

    }

}

char *my_strchr(const char* str, int ch) {

    assert( str );


    for(int i = 0; str[i] != '\0'; i++){

        if ( (int) str[i] == ch ) {
            
            return (char *)str + i;
        }

    }

    return NULL;

}

size_t my_strlen(const char* str) {
    assert( str );

    size_t i = 0;

    while ( str[i] != '\0') i++;

    return i;

}

char *my_strcpy(char *str1, const char *str2) {

    assert( str1 || str2 );

    int i = 0;
    
    while( str2[i] != '\0' ){
        
        str1[i] = str2[i];

        i++;

    }

    str1[i] = '\0';

    return str1;

}

char *my_strncpy(char *str1, const char *str2, size_t count) {

    assert( str1 && str2 );

    size_t str2_len = my_strlen( str2 );

    if ( str2_len < count) {

        for( size_t i = 0; i < str2_len; i++) {
            
            str1[i] = str2[i];
        
        }

        for (size_t i = str2_len; i <= count; i++){

            str1[i] = '\0';
        }

    } else {

        for ( size_t i = 0; i <= count; i++){

            str1[i] = str2[i];

        }

    }

    return str1;

}

char *my_strcat (char *str1, const char *str2) {
    assert( str1 && str2);

    size_t str1_len = my_strlen( str1 );

    size_t i;

    for( i = 0; str2[i] != '\0'; i++) {

        str1[ str1_len + i ] = str2[i];

    }

    str1[ str1_len + i] = '\0';

    return str1;

}

char *my_strncat (char *str1, const char *str2, size_t n) {
    assert( str1 && str2);

    size_t str1_len = my_strlen( str1 );

    size_t i;

    for( i = 0; i < n && str2[i] != '\0'; i++) {

        str1[ str1_len + i ] = str2[i];

    }  

    str1[ str1_len + i] = '\0';

    return str1;

}

char *my_fgets(char *str, int num, FILE *stream) {
    
    assert( str );

    char ch;

    for(int i = 0; i < num && ( ch = (char)fgetc(stream) ) != '\n' ; i++) {

        str[i] = ch;

    } 

    str[num] = '\0';

    return str;

}

char *my_strdup (const char *str) {
    assert( str );
    
    size_t str_len = my_strlen(str);
    
    char *temp = (char*)malloc( sizeof(char) * str_len );
    if ( temp ) {

        for(size_t i = 0; i <= str_len ; i++){

            temp[i] = str[i];

        }

    }

    return temp;

}