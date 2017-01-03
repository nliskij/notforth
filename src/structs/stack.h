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
#include "../util/error.h"
#ifndef STACK_H
#define STACK_H

/**
 * A structure for a generic stack.
 */
typedef struct stack_t {
    /*@{*/
    size_t top;          /**< The index of the top of the stack. */
    size_t size;         /**< The size of the data pointer. */
    size_t element_size; /**< The size of an element on the stack. */
    void *data;          /**< The data on the stack. */
    /*@}*/
} stack_t;

/**
 * @brief  initializes a stack
 * @param[out] stack        The stack that will be initialized.
 * @param[in]  size         The size of the data pointer in the stack.
 * @param[in]  element_size The size of an element in the stack.
 * @return Returns 0 for success and -1 for failure.
 */
error init_stack(stack_t *stack, size_t size, size_t element_size);
/**
 * @brief  Pushes a value on the stack.
 * @param[in,out] stack The stack that the value will be pushed to.
 * @param[in]     elem  The element that will be pushed onto the stack.
 * @return Returns 0 for success and -1 for failure.
 */
error push(stack_t *stack, void *elem);
/**
 * @brief  Pops the top value off the stack.
 * @param[in,out] stack The stack.
 * @param[out]    elem  The value popped off the stack is stored here
 * @return Returns 0 for success and -1 for failure.
 */
error pop(stack_t *stack, void *elem);

/**
 * @brief Places the value on top of the stack in top without changing the stack.
 * @param[in]  stack The stack.
 * @param[out] top   The top value of the stack.
 * @returns Returns 0 for success and -1 for failure.
 */
error top(const stack_t *stack, void *top);

/**
 * @brief  Returns the number of elements on the stack.
 * @param[in] stack The stack whose elements are counted.
 * @return The number of elements on the stack.
 */
size_t stklen(const stack_t *stack);

/**
 * @brief Resets a stack.
 * @param[in,out] stack The stack that is cleared.
 * @return Returns 0 for success and -1 for failure.
 */
error clear_stack(stack_t *stack);

/**
 * @brief Resizes a stack.
 * @param[in,out] stack    The stack that is resized.
 * @param[in]     new_size The new size of the stack.
 * @returns Returns 0 for succesa and -1 for failure.
 */
error resize_stack(stack_t *stack, size_t new_size);

/**
 * @brief Frees memory on a stack.
 * @param[in] stack The stack that is freed.
 * @return Returns 0 for success and -1 for failure.
 */
error free_stack(stack_t *stack);

#endif
