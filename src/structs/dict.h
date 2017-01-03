/**
 * @file
 *
 * @section LICENSE
 *
 * This file is part of yaForth.
 *
 * yaForth is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef DICT_H
#define DICT_H
#include "../util/error.h"
#include "../word_type.h"

typedef struct elem_t {
    struct elem_t *next;
    char *key;
    fwordseq_t *words;
} elem_t;

typedef struct dict_t {
    size_t size;
    size_t num_defs;
    elem_t **defs;
} dict_t;

unsigned long hash(const char *str);
error init_dict(dict_t *dict, size_t size);
error free_dict(dict_t *dict);
error add_word(dict_t *dict, const char *key, fwordseq_t *words);
error grow_dict(dict_t *dict);
error lookup(dict_t *dict, const char *key, fwordseq_t **result);
#endif
