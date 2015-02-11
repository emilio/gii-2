#include <stdlib.h>
#include <stdio.h>
#include "lib/stack.h"

int main() {
	stack_t* stack = stack_new(int);
	int i;

	printf("Testing stack...\n");

	for ( i = 0; i < 10; ++i )
		stack_push(stack, i);

	printf("Stack filled...\n");

	while ( ! stack_empty(stack) ) {
		stack_top(stack, &i);
		printf(" %d", i);
		stack_pop(stack);
	}

	printf("\n");

	return 0;
}
