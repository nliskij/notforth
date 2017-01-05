#ifndef WORDS_H
#define WORDS_H
#include "structs/stack.h"
#include "structs/dict.h"
#include "util/error.h"

error def(char *line, dict_t *dict);
error show_stack(stack_t *stack);
error emit(stack_t *stack);
error addw(stack_t *stack);
error subw(stack_t *stack);
error mulw(stack_t *stack);
error divw(stack_t *stack);
error eqw(stack_t *stack);
error gtw(stack_t *stack);
#endif
