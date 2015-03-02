#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STACK_INITIAL_SIZE 10

typedef struct stack {
	char* data;
	size_t size;
	size_t capacity;
	size_t item_size;
} stack_t;

#define stack_empty(s) (s->size == 0)

void stack_grow(stack_t* stack) {
	size_t dest_size = stack->capacity * 2;

	if ( dest_size == 0 )
		dest_size = STACK_INITIAL_SIZE;

	if ( stack->data == NULL )
		stack->data = (char*) malloc(dest_size * stack->item_size);
	else
		stack->data = (char*) realloc(stack->data, dest_size * stack->item_size);

	stack->capacity = dest_size;

	assert(stack->data != NULL);
}

/**
 * stack_t* stack = stack_new(int);
 *
 * int i = 4;
 * stack_push(stack, i);
 */
#define stack_new(T) stack_new_(sizeof(T))
stack_t* stack_new_(size_t item_size) {
	stack_t* ret = (stack_t*) malloc(sizeof(stack_t));

	assert(ret != NULL);

	ret->data = NULL;
	ret->size = ret->capacity = 0;
	ret->item_size = item_size;

	return ret;
}

/** Note we can't push a raw value... that's the cost of flexibility in C */
#define stack_push(s, val) stack_push_(s, (char*) &val)
void stack_push_(stack_t* stack, char* val) {

	assert(val != NULL);

	if ( stack->size == stack->capacity )
		stack_grow(stack);

	memcpy(stack->data + (stack->size++ * stack->item_size), val, stack->item_size);
}


/** We define a void pop() operation... it's perfectly reasonable using top + pop */
void stack_pop(stack_t* stack) {
	assert(stack->size != 0);
	stack->size--;
}

/** Our top function must take a pointer... */
void stack_top(stack_t* stack, void* val) {
	assert(val != NULL);
	assert(stack->size != 0);
	memcpy(val, stack->data + ((stack->size - 1) * stack->item_size), stack->item_size);
}

void stack_destroy(stack_t* stack) {
	if ( stack->data != NULL )
		free(stack->data);
	free(stack);
}

#endif
