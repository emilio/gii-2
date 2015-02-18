#include "queue.h"

/**
 * Allocate a new empty, dinamic memory queue
 */
queue_t* queue_new() {
	queue_t* ret = (queue_t*) malloc(sizeof(queue_t));

	assert(ret != NULL);

	ret->size = 0;
	ret->last = ret->first = NULL;

	return ret;
}

/**
 * Delete the top of the queue and
 * return the value
 */
queue_value_t queue_pop(queue_t* queue) {
	queue_item_t* item = queue->first;
	queue_value_t ret = item->value;

	queue->first = item->next;

	if ( queue->last == item )
		queue->last = NULL;

	queue->size -= 1;

	free(item);

	return ret;
}

/** Push a value onto the queue */
void queue_push(queue_t* queue, queue_value_t val) {
	queue_item_t* item = (queue_item_t*) malloc(sizeof(queue_item_t));

	assert(item != NULL);
	item->value = val;

	if ( queue->first == NULL )
		queue->first = item;

	if ( queue->last != NULL )
		queue->last->next = item;

	queue->last = item;
	queue->size += 1;
}

void queue_destroy(queue_t* queue) {

	while ( ! queue_empty(queue) )
		queue_pop(queue);

	free(queue);
}
