#include <stdlib.h>
#include <stdio.h>
#include "lib/queue.h"

int main() {
	queue_t* queue = queue_new(int);
	int i;

	printf("Testing queue...\n");

	for ( i = 0; i < 100; ++i )
		queue_push(queue, i);

	printf("Queue filled...\n");

	while ( ! queue_empty(queue) ) {
		queue_front(queue, &i);
		printf(" %d", i);
		queue_pop(queue);
	}

	printf("\n");

	queue_destroy(queue);

	return 0;
}
