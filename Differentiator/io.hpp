#include <stdio.h>

#ifndef MY_IO_H_INCLUDED
#define MY_IO_H_INCLUDED

enum IO_ERRORS{
    BUFF_NULL_PTR  = 0b00000001,
    FILE_NULL_PTR  = 0b00000010,
    STATS_ERR      = 0b00000100,
    READ_ERR       = 0b00001000,
    CMD_NULL_PTR   = 0b00010000,
    CMD_ALLOC_ERR  = 0b00100000,
};

int buf_read( char** code, FILE* input, size_t* size);
int stricmp(const char* str1, const char* str2);
int cmd_get(char *code, char***cmd, size_t* size, const size_t code_size);
int cmd_print(const char** cmd, const size_t cmd_size);
int skip_spaces( const char *str);

#endif