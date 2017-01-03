#ifndef WORDS_H
#define WORDS_H
#include "structs/stack.h"
#include "structs/dict.h"
#include "util/error.h"

error def(char *line, dict_t *dict);
error show_stack(stack_t *stack);
error emit(stack_t *stack);
#endif
