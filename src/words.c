#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "words.h"
#include "structs/stack.h"
#include "structs/dict.h"

// Include error reporting for all of these
// def doesnt follow DRY very well, it just reimplements interpret
// FUCK STRTOK, use strdup and stuff instead?
error def(char *line, dict_t *dict)
{
    fwordseq_t *new_words; // The dict lookup sets this ptr, no init
    fwordseq_t *def = malloc(sizeof(fwordseq_t));
    char *initial_line = malloc(sizeof(char) * (strlen(line) + 1));
    strcpy(initial_line, line);
    line = initial_line;
    line = strtok(NULL, " ");
    char *name = malloc(strlen(line) * sizeof(char) + sizeof(char));
    strcpy(name, line);
    def->num_words = 0;
    def->fwords = calloc(1, sizeof(fword_t));
    line = strtok(NULL, " ");

    if (name == NULL || def == NULL || def->fwords == NULL) {
        return ER_MALLOC_FAIL;
    }

    do {
        // if we're out of tokens, without ';', wait for more lines
        // Note that this needs so many features added
        // fuck this block
        // this should be fixed in some sane way
        // i really fucking hate strtok(?)
        if (line == NULL) {
            // This reimplements main, fix later for DRY
            size_t size = 0;
            ssize_t len;
            char **line_ptr = malloc(sizeof(char*));
            *line_ptr = NULL;
            len = getline(line_ptr, &size, stdin); // fix to use any input
            printf("\n==> Newline: %s\n", *line_ptr);
            if (len == -1) {
            }
                return ER_IO_FAIL;
            free(initial_line);
            line = malloc((len + 1) * sizeof(char));
            initial_line = line;
            if (line == NULL) {
                return ER_MALLOC_FAIL;
            }
            strcpy(line, *line_ptr);
            free(*line_ptr);
            free(line_ptr);
            line = strtok(line, " ");
        }
        else if (!lookup(dict, line, &new_words)) {
            size_t initial_size = def->num_words;
            def->num_words += new_words->num_words;
            fword_t *tmp = realloc(def->fwords, def->num_words * sizeof(fword_t));
            if (tmp == NULL) {
                return ER_MALLOC_FAIL;
            }
            else {
                def->fwords = tmp;
            }
            memcpy(def->fwords + initial_size, new_words->fwords,
                    new_words->num_words * sizeof(fword_t));
        }
        else {
            char *end;
            int *val = malloc(sizeof(int));
            *val = (int) strtol(line, &end, 0);
            fword_t *tmp = realloc(def->fwords, (def->num_words + 1) * sizeof(fword_t));
            if (tmp == NULL || val == NULL) {
                return ER_MALLOC_FAIL;
            }
            else {
                def->fwords = tmp;
            }
            (def->fwords)[def->num_words].type = PSH;
            (def->fwords)[def->num_words].psh = &push;
            (def->fwords)[def->num_words].val = val;
            def->num_words++;
        }
        line = strtok(NULL, " ");
    } while (line == NULL || *line != ';'); // sort of messed up

    add_word(dict, name, def);
    free(name);
    free(initial_line);
    free(def->fwords);
    free(def);

    return ER_SUCCESS;
}

error show_stack(stack_t *stack)
{
    printf(" <%zu> ", stklen(stack));
    for (size_t i = 0; i < stklen(stack); i++) {
        printf("%d ", ((int*) stack->data)[i]);
    }
    putchar('\n');

    return ER_SUCCESS;
}

error emit(stack_t *stack)
{
    int top;
    if (pop(stack, &top) != ER_SUCCESS) {
        return ER_OUT_OF_BOUNDS;
    }
    printf("%c", top);

    return ER_SUCCESS;
}
