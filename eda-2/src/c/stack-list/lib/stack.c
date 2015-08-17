#include "stack.h"

/**
 * Allocate a new empty, dinamic memory stack
 */
stack_t* stack_new() {
    stack_t* ret = (stack_t*)malloc(sizeof(stack_t));

    assert(ret != NULL);

    ret->size = 0;
    ret->last = NULL;

    return ret;
}

/**
 * Delete the top of the stack and
 * return the value
 */
stack_value_t stack_pop(stack_t* stack) {
    stack_item_t* item = stack->last;
    stack_value_t ret = item->value;

    stack->last = item->prev;
    stack->size -= 1;

    free(item);

    return ret;
}

/** Push a value onto the stack */
void stack_push(stack_t* stack, stack_value_t val) {
    stack_item_t* item = (stack_item_t*)malloc(sizeof(stack_item_t));

    assert(item != NULL);

    item->prev = stack->last;
    item->value = val;

    stack->last = item;
    stack->size += 1;
}

void stack_destroy(stack_t* stack) {
    while (!stack_empty(stack))
        stack_pop(stack);

    free(stack);
}
