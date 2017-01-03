#ifndef WORD_TYPE_H
#define WORD_TYPE_H
#include "structs/stack.h"
#include "structs/dict.h"
#include "util/error.h"

struct dict_t;
typedef struct dict_t dict_t;

typedef error (*def_word)(char*, dict_t*);
typedef error (*stk_word)(stack_t*);
typedef error (*psh_word)(stack_t*, void*);
typedef enum {STK, PSH, DEF} word_type_t;
typedef struct fword_t {
    union {
        stk_word stkdef;
        def_word defdef;
        psh_word psh;
    };
    void *val; // This is an attempt to curry psh
    word_type_t type;
} fword_t;

typedef struct fwordseq_t {
    size_t num_words;
    fword_t *fwords;
} fwordseq_t;
#endif
