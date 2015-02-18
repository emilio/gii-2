#include <stdio.h>
#include <stdlib.h>

#define QUEUE_ELEMENT_TYPE int
#include "lib/queue.h"

int main() {
	int i;
	queue_t* q = queue_new();

	assert(queue_empty(q));

	for ( i = 0; i < 10; ++i )
		queue_push(q, i);

	assert(!queue_empty(q));

	assert(queue_front(q) == 0);

	while ( ! queue_empty(q) ) {
		printf(" %d", queue_front(q));
		queue_pop(q);
	}

	printf("\n");

	queue_destroy(q);

	return 0;
}
