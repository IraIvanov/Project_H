#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include <string.h>
#include "io.hpp"

#ifndef _AKINATOR_H_INCLUDED_
#define _AKINATOR_H_INCLUDED_

enum errors {

    NULL_FILE = -1,

};

int upload ( node_t* node, FILE* file );

#endif