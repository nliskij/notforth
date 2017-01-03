#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "forth.h"
#include "word_type.h"
#include "words.h"
#include "structs/stack.h"
#include "structs/dict.h"

error fill_dict(dict_t *dict)
{
    fword_t *fwords = malloc(3 * sizeof(fword_t));
    if (fwords == NULL) {
        return throw_er(ER_MALLOC_FAIL, "ER: Failed dict malloc in fill_dict");
    }
    fwords[0] = (fword_t) {.stkdef = show_stack, .type = STK};
    fwords[1] = (fword_t) {.stkdef = emit, .type = STK};
    fwords[2] = (fword_t) {.defdef = def, .type = DEF};
    add_primitive(dict, fwords[0], ".s");
    add_primitive(dict, fwords[1], "emit");
    add_primitive(dict, fwords[2], ":");
    free(fwords);

    return throw_er(ER_SUCCESS, "ER: No error");
}

error add_primitive(dict_t *dict, fword_t primitive, const char *name)
{
    fword_t *fword = malloc(sizeof(fword_t));
    if (fword == NULL) {
        return throw_er(ER_MALLOC_FAIL,
                "ER: Failed dict malloc in add_primitive");
    }
    switch (primitive.type) {
        case STK: fword->stkdef = primitive.stkdef; break;
        case PSH: fword->psh = primitive.psh; break;
        case DEF: fword->defdef = primitive.defdef; break;
    }
    fwordseq_t *seq = malloc(sizeof(fwordseq_t));
    if (seq == NULL) {
        return throw_er(ER_MALLOC_FAIL,
                "ER: Failed dict malloc in add_primitive");
    }
    seq->num_words = 1;
    fword->type = primitive.type;
    fword->val = NULL;
    seq->fwords = fword;

    error dict_err = add_word(dict, name, seq);
    if (dict_err.code != ER_SUCCESS) {
        return throw_er(dict_err.code, "ER: Failed dict add in add_primitive");
    }
    free(seq);
    free(fword);

    return throw_er(ER_SUCCESS, "No error");
}

error interpret(char *line, stack_t *the_stack, ssize_t len, dict_t *word_dict)
{
    // Make enum for error type?
    fwordseq_t *words = NULL;
    char *end;
    char *token = strdup(line);
    char *initial_token_ptr = token;
    token = strtok(token, " ");

    if (token == NULL) {
        return throw_er(ER_MALLOC_FAIL, "ER: Failed malloc in interpret");
    }

    while (token) {
        int val = (int) strtol(token, &end, 0);
        if (*end == '\0') {
            error stack_error = push(the_stack, &val);
            if (stack_error.code) {
                return stack_error;
            }
        }
        else {
            lookup(word_dict, token, &words);
            if (words != NULL) {
                for (size_t i = 0; i < words->num_words; i++) {
                    error fail;
                    switch (words->fwords[i].type) {
                        case STK:
                            fail = (*(words->fwords[i]).stkdef)(the_stack);
                            break;
                        case DEF:
                            fail = (*(words->fwords[i]).defdef)(token, word_dict);
                            break;
                        case PSH:
                            fail = (*(words->fwords[i]).psh)(the_stack, words->fwords[i].val);
                            break;
                    }
                    if (fail.code != ER_SUCCESS) {
                        return fail;
                    }
                }
            }
            else {
                return throw_er(ER_LOOKUP_FAIL, "ER: Word not found");
            }
        }
    token = strtok(NULL, " ");
    }
    free(initial_token_ptr); // this shouldn't work?

    return throw_er(ER_SUCCESS, "ER: No error");
}
