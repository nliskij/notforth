#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forth.h"
#include "structs/stack.h"
#include "structs/dict.h"
#include "util/error.h"
#define DEFAULT_DICT_SIZE 144
#define DEFAULT_STACK_SIZE 64

int main(void)
{
    stack_t *the_stack = malloc(sizeof(stack_t));
    dict_t *dict = malloc(sizeof(dict_t));
    size_t size = 0;
    ssize_t len;
    char **line_ptr = malloc(sizeof(char*));
    *line_ptr = NULL;
    if (init_stack(the_stack, DEFAULT_STACK_SIZE, sizeof(int)).code ||
        init_dict(dict, DEFAULT_DICT_SIZE).code ||
        fill_dict(dict).code) {
        printf("ERROR: Failed to allocate memory.\n");
        return -1;
    }

    while ((len = getline(line_ptr, &size, stdin)) != -1) {
        (*line_ptr)[strcspn(*line_ptr, "\n")] = '\0';
        printf("\n==> Interpret result : %s\n",
                print_error(interpret(*line_ptr, the_stack, len, dict)));
        free(*line_ptr);
        *line_ptr = NULL;
    }
    free(*line_ptr);
    free(line_ptr);
    free_stack(the_stack);
    free_dict(dict);

    return 0;
}
