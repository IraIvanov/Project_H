#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED


    enum operations {
        
        ADD = 1,
        SUB = 2,
        MUL = 10,
        DIV = 20,
        DEG = 30,
        LG  = 31,
        SIN = 32,
        COS = 33,

    };

    int is_var( int c );
    int is_op( char* str );
    int tree_upload( char* str, node_t* node );
    tree_t* _Diff ( const tree_t* node );
    node_t* Diff ( const node_t* src );

#endif