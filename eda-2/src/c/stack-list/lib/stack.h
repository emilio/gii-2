#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef STACK_VALUE_TYPE
#define STACK_VALUE_TYPE int
#endif

typedef STACK_VALUE_TYPE stack_value_t;

typedef struct stack_item {
	stack_value_t value;
	struct stack_item* prev;
} stack_item_t;

typedef struct stack {
	stack_item_t* last;
	size_t size;
} stack_t;

stack_t* stack_new();

#define stack_empty(stack) stack->size == 0

stack_value_t stack_pop(stack_t* stack);

void stack_push(stack_t* stack, stack_value_t val);

#define stack_top(stack) (stack->last->value);

void stack_destroy(stack_t* stack);

#endif
