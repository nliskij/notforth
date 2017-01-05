#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "../util/error.h"

#define MULT 97
#define DICT_GROWTH 2 // This could be 1.5, debatable

unsigned long hash(const char *str)
{
    unsigned const char *ustr;
    unsigned long result = 0;

    for (ustr = (unsigned const char*) str; *ustr; ustr++) {
        result = result * MULT + *ustr;
    }

    return result;
}

error init_dict(dict_t * const dict, size_t size)
{
    dict->size = size;
    dict->num_defs = 0;
    dict->defs = calloc(dict->size, sizeof(elem_t *));

    if (size == 0)
        return throw_er(ER_INIT_ZERO_SZ, "ER_INIT_ZERO_SZ");
    else if (dict->defs == NULL)
        return throw_er(ER_MALLOC_FAIL, "ER_MALLOC_FAIL");
    else
        return throw_er(ER_SUCCESS, "ER: No error");
}

error free_dict(dict_t *dict)
{
    elem_t *elem;
    elem_t *next;

    for (size_t i = 0; i < dict->size; i++) {
        for (elem = dict->defs[i]; elem != 0; elem = next) {
            next = elem->next;
            for (size_t j = 0; j < elem->words->num_words; j++) {
                free(elem->words->fwords[j].val);
            }
            free(elem->words->fwords);
            free(elem->words);
            free(elem->key);
            free(elem);
        }
    }

    free(dict->defs);
    free(dict);

    return throw_er(ER_SUCCESS, "ER: No error");
}

error add_word(dict_t *dict, const char *key, fwordseq_t *words)
{
    unsigned long h = hash(key);
    elem_t *elem;
    elem = calloc(1, sizeof(elem_t));
    elem->words = calloc(1, sizeof(fwordseq_t));
    elem->words->fwords = calloc(words->num_words, sizeof(fword_t));
    elem->words->num_words = words->num_words;

    if (key == NULL || words == NULL ||
            elem == NULL || elem->words == NULL)
        return throw_er(ER_MALLOC_FAIL, "ER_MALLOC_FAIL"); // sort of...
    else if (words->num_words == 0 || elem->words->fwords == NULL)
        return throw_er(ER_INIT_ZERO_SZ, "ER_INIT_ZERO_SZ"); // same

    elem->key = strdup(key);
    memcpy(elem->words->fwords, words->fwords, sizeof(fword_t) * words->num_words);

    h = h % dict->size;
    elem->next = dict->defs[h];
    dict->defs[h] = elem;

    dict->num_defs++;

    if (dict->num_defs == dict->size) {
        return grow_dict(dict);
    }

    return throw_er(ER_SUCCESS, "ER: No error");
}

error grow_dict(dict_t *dict)
{
    dict_t *new_dict = malloc(sizeof(dict_t));
    dict_t swap_dict;
    elem_t *elem;
    error e;

    if ((e = init_dict(new_dict, dict->size * DICT_GROWTH)).code)
        return e;

    for (size_t i = 0; i < dict->size; i++) {
        for (elem = dict->defs[i]; elem != 0; elem = elem->next) {
            if (!(e = add_word(new_dict, elem->key, elem->words)).code) {
                return e; // I might've misunderstood and put a bug here
            }
        }
    }

    swap_dict = *dict;
    *dict = *new_dict;
    *new_dict = swap_dict;

    free_dict(new_dict);

    return throw_er(ER_SUCCESS, "ER: No error");
}

error lookup(dict_t *dict, const char *key, fwordseq_t **result)
{
    elem_t *elem;

    for (elem = dict->defs[hash(key) % dict->size]; elem != 0; elem = elem->next) {
        if (!strcmp(elem->key, key)) {
            *result = elem->words;
            return throw_er(ER_SUCCESS, "ER: No error");
        }
    }

    result = NULL;
    return throw_er(ER_LOOKUP_FAIL, "ER: Lookup word failed");
}
