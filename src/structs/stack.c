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
 * @section DESCRIPTION
 *
 * This implements functions for initializing, pushing and popping a
 * generic stack.
 */

#include <stdlib.h>
#include <string.h>
#include "stack.h"


/**
 * This method sets all stack attributes to their default values.
 * If memory allocation fails or the size of the stack is 0, the
 * function returns error code -1.
 */
error init_stack(stack_t *stack, size_t size, size_t element_size)
{
    stack->top = -1;
    stack->size = size;
    stack->element_size = element_size;
    stack->data = malloc(size * element_size);

    if (stack->data == NULL || stack->size == 0) // This is more readable than !size and !data
        return throw_er(ER_MALLOC_FAIL, "ER: Failed malloc in init_stack");
    else
        return throw_er(ER_SUCCESS, "ER: No error");
}

size_t stklen(const stack_t *stack)
{
    return stack->top + 1;
}

error push(stack_t *stack, void *elem)
{
    error e;
    if (stklen(stack) >= stack->size) {
        if ((e = resize_stack(stack, 2 * stack->size)).code != ER_SUCCESS) {
            return e;
        }
    }

    stack->top++;
    memcpy(((char *) stack->data) + stack->element_size * stack->top, elem, stack->element_size);

    return throw_er(ER_SUCCESS, "ER: No error");
}

error pop(stack_t *stack, void *elem)
{
    if (stklen(stack) == 0)
        return throw_er(ER_OUT_OF_BOUNDS, "ER: Pop size zero stack");

    memcpy(elem, ((char *) stack->data) + stack->element_size * stack->top, stack->element_size);
    stack->top--;

    return throw_er(ER_SUCCESS, "No error");
}

error top(const stack_t *stack, void *top)
{
    if (stklen(stack) <= 0)
        return throw_er(ER_OUT_OF_BOUNDS, "ER: Top of size zero stack");

    memcpy(top, ((char *) stack->data) + stack->element_size * stack->top, stack->element_size);

    return throw_er(ER_SUCCESS, "ER: No error");
}

error clear_stack(stack_t *stack)
{
    stack->top = -1;
    stack->data = realloc(stack->data, 1);

    return throw_er(ER_SUCCESS, "ER: No error");
}

error resize_stack(stack_t *stack, size_t new_size)
{
    if (new_size > stack->size)
        stack->top = new_size - 1;

    stack->size = new_size;
    stack->data = realloc(stack->data, new_size);

    if (stack->data == NULL || stack->size == 0)
        return throw_er(ER_INIT_ZERO_SZ, "ER: Resized stack to zero");
    else
        return throw_er(ER_SUCCESS, "ER: No error");
}

error free_stack(stack_t *stack)
{
    free(stack->data);
    stack->data = NULL;
    free(stack);
    stack = NULL;

    return throw_er(ER_SUCCESS, "ER: No error");
}
