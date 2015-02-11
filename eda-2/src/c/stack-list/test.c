#include <stdlib.h>
#include <stdio.h>
#include "lib/stack.h"

int main() {
	stack_t* stack = stack_new();
	int i;

	printf("Testing stack...\n");

	for ( i = 0; i < 10; ++i )
		stack_push(stack, i);

	printf("Stack filled...\n");

	while ( ! stack_empty(stack) )
		printf(" %d", stack_pop(stack));

	printf("\n");

	return 0;
}
