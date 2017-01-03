#ifndef FORTH_H
#define FORTH_H
#include "structs/stack.h"
#include "structs/dict.h"
#include "util/error.h"

error fill_dict(dict_t *dict);
error interpret(char *line, stack_t *the_stack, ssize_t len, dict_t *word_dict);
error add_primitive(dict_t *dict, fword_t fword, const char *name);
#endif
