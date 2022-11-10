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

int game ( node_t* node/*, tree_t* tree*/, list_t* seek_list );

int show_def( node_t* node, list_t* seek_list );

int show_tree( node_t* node );

int compare( node_t* node, list_t* seek_list1, list_t* seek_list2 );

#endif